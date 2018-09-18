
#include "cmd.h"
//-------------------------------------
int set_group_reg(int duplex, REG_T *reg, int size)
{
	int i;
	if(duplex & 1) {
		for(i = 0; i < size/sizeof(REG_T); i++)
			SET_REG(reg[i].addr, reg[i].data);
	}
	if(duplex & 2) {
		for(i = 0; i < size/sizeof(REG_T); i++)
			SET_REG(reg[i].addr+BENG, reg[i].data);
	}
	return TRUE;
}

int set_group_regbit(int duplex, REG_BIT_T *reg, int size)
{
	int i;
	if(duplex & 1) {
		for(i = 0; i < size/sizeof(REG_BIT_T); i++)
			SET_REGBIT(reg[i].addr, reg[i].clear, reg[i].set);
	}
	if(duplex & 2) {
		for(i = 0; i < size/sizeof(REG_BIT_T); i++)
			SET_REGBIT(reg[i].addr+BENG, reg[i].clear, reg[i].set);
	}
	return TRUE;
}

int set_group_afereg(int duplex, AFE_REG_T *reg, int size)
{
	int i, j;
	U32 data, status;
	if(duplex & 1) {
		for(i = 0; i < size/sizeof(AFE_REG_T); i++)
			SET_AFEREG(reg[i].addr, reg[i].data);
	}
	if(duplex & 2) {
		for(i = 0; i < size/sizeof(AFE_REG_T); i++)
			SET_b_AFEREG(reg[i].addr, reg[i].data);
	}
	return TRUE;
}

