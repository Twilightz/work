/*
    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!
*/

#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( heapSTRUCT_SIZE * 2 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const uint16_t heapSTRUCT_SIZE	= ( ( sizeof ( BlockLink_t ) + ( portBYTE_ALIGNMENT - 1 ) ) & ~portBYTE_ALIGNMENT_MASK );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart, *pxEnd = NULL;

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t xFreeBytesRemaining;
static size_t xMinimumEverFreeBytesRemaining;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;

/*-----------------------------------------------------------*/

void *pvPortMalloc( size_t xWantedSize )
{
BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
void *pvReturn = NULL;

	/* Check the requested block size is not so large that the top bit is
	set.  The top bit of the block size member of the BlockLink_t structure
	is used to determine who owns the block - the application or the
	kernel, so it must be free. */
	if( ( xWantedSize & xBlockAllocatedBit ) == 0 )
	{
		/* The wanted size is increased so it can contain a BlockLink_t
		structure in addition to the requested amount of bytes. */
		if( xWantedSize > 0 )
		{
			xWantedSize += heapSTRUCT_SIZE;

			/* Ensure that blocks are always aligned to the required number
			of bytes. */
			if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
			{
				/* Byte alignment required. */
				xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		vTaskSuspendAll();
		{
			/* malloc only available after the heap is initialised. */
			if( pxEnd == NULL )
			{
				mtCOVERAGE_TEST_MARKER();
			}
			else
			{
				if( ( xWantedSize > 0 ) && ( xWantedSize <= xFreeBytesRemaining ) )
				{
					/* Traverse the list from the start	(lowest address) block until
					one	of adequate size is found. */
					pxPreviousBlock = &xStart;
					pxBlock = xStart.pxNextFreeBlock;
					while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
					{
						pxPreviousBlock = pxBlock;
						pxBlock = pxBlock->pxNextFreeBlock;
					}

					/* If the end marker was reached then a block of adequate size
					was	not found. */
					if( pxBlock != pxEnd )
					{
						/* Return the memory space pointed to - jumping over the
						BlockLink_t structure at its start. */
						pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + heapSTRUCT_SIZE );

						/* This block is being returned for use so must be taken out
						of the list of free blocks. */
						pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

						/* If the block is larger than required it can be split into
						two. */
						if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
						{
							/* This block is to be split into two.  Create a new
							block following the number of bytes requested. The void
							cast is used to prevent byte alignment warnings from the
							compiler. */
							pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );

							/* Calculate the sizes of two blocks split from the
							single block. */
							pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
							pxBlock->xBlockSize = xWantedSize;

							/* Insert the new block into the list of free blocks. */
							//prvInsertBlockIntoFreeList( ( pxNewBlockLink ) );
							pxNewBlockLink->pxNextFreeBlock = pxBlock->pxNextFreeBlock;
							pxPreviousBlock->pxNextFreeBlock = pxNewBlockLink;
						}
						else
						{
							mtCOVERAGE_TEST_MARKER();
						}

						xFreeBytesRemaining -= pxBlock->xBlockSize;

						if( xFreeBytesRemaining < xMinimumEverFreeBytesRemaining )
						{
							xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
						}
						else
						{
							mtCOVERAGE_TEST_MARKER();
						}

						/* The block is being returned - it is allocated and owned
						by the application and has no "next" block. */
						pxBlock->xBlockSize |= xBlockAllocatedBit;
						pxBlock->pxNextFreeBlock = NULL;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
		}
		( void ) xTaskResumeAll();
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
	traceMALLOC( pvReturn, xWantedSize );

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif

	return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree( void *pv )
{
uint8_t *puc = ( uint8_t * ) pv;
BlockLink_t *pxLink;

	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= heapSTRUCT_SIZE;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		configASSERT( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 );
		configASSERT( pxLink->pxNextFreeBlock == NULL );

		if( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 )
		{
			if( pxLink->pxNextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->xBlockSize &= ~xBlockAllocatedBit;

				vTaskSuspendAll();
				{
					/* Add this block to the list of free blocks. */
					xFreeBytesRemaining += pxLink->xBlockSize;
					traceFREE( pv, pxLink->xBlockSize );
					prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
				}
				( void ) xTaskResumeAll();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize( void )
{
	return xFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize( void )
{
	return xMinimumEverFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

void vHeapInit( void *start_addr, size_t size )
{
BlockLink_t *pxFirstFreeBlock;
uint8_t *pucHeapEnd, *pucAlignedHeap;

	xFreeBytesRemaining = ( ( size_t ) (size - portBYTE_ALIGNMENT) ) & ( ( size_t ) ~portBYTE_ALIGNMENT_MASK );

	/* Ensure the heap starts on a correctly aligned boundary. */
	pucAlignedHeap = ( uint8_t * ) ( ( ( portPOINTER_SIZE_TYPE ) ( (unsigned long)start_addr + portBYTE_ALIGNMENT ) ) & ( ( portPOINTER_SIZE_TYPE ) ~portBYTE_ALIGNMENT_MASK ) );

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
	xStart.xBlockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	pucHeapEnd = pucAlignedHeap + xFreeBytesRemaining;
	pucHeapEnd -= heapSTRUCT_SIZE;
	pxEnd = ( void * ) pucHeapEnd;
	configASSERT( ( ( ( uint32_t ) pxEnd ) & ( ( uint32_t ) portBYTE_ALIGNMENT_MASK ) ) == 0UL );
	pxEnd->xBlockSize = 0;
	pxEnd->pxNextFreeBlock = NULL;

	/* The heap now contains pxEnd. */
	xFreeBytesRemaining -= heapSTRUCT_SIZE;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = xFreeBytesRemaining;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

	xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;

	/* Work out the position of the top bit in a size_t variable. */
	xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert )
{
BlockLink_t *pxIterator;
uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}

