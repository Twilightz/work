#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "stdafx.h"
#include "uniapi.h"
#include "Config.h"
#include "ProcessCenter.h"
#include "Messenger.h"
#include "Utils.h"
#include "HAPacket.h"
#include "VanException.h"
#include "global.h"

int main(int argc, char *argv[])
{
    char id[64];
    memset(id, 0, 64);
    if (read_id(id, 64) <= 0)
    {
        printf("Can not read device id \n");
        return 1;
    }

    g_device_id = id;
    printf("device id : %s\n", id);

    ProcessCenter::Instance()->Start();

    ////////////////////////////////////////////////////////
   
    //Utils::PrintHexBuf(pDst, packet.GetLength());

    usleep(3000000);

    while (1)
    {
        usleep(50000);
    }

    return 0;
}
