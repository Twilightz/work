
#include "cmd.h"

int set_gpio23(int on)
{
	REG_BIT_T reg[] = {{SCR(0x050), GPIO23(clr), GPIO23(1)},
						{SCR(0x058), GPOE23(clr), GPOE23(1)},
						{SCR(0x060), gpo23_u_cis_mode(clr)|gpo23_shrg(clr), 0}};
	if(on == 0)
		reg[0].set = 0;
	return set_group_regbit(reg, sizeof(reg));
}

int set_gpio40(int on)
{
	REG_BIT_T reg[] = {{SCR(0x054), GPIO40(clr), GPIO40(1)},
						{SCR(0x05c), GPOE40(clr), GPOE40(1)},
						{SCR(0x064), gpo40_mtg(clr)|gpo40_mtrl_0(clr), 0}};
	reg[0].set = (on? GPIO40(1): 0);
	return set_group_regbit(reg, sizeof(reg));
}

int set_gpio71_82(U32 io, in on)
{
	REG_BIT_T reg[] = {{TCR(0x058), 0, 0},
						{TCR(0x05c), 0, 0},
						{TCR(0x068), LCM_SEL(clr), 0}};
	reg[0].clear = reg[1].clear = reg[1].set = io;
	reg[0].set = (on? io: 0);
	return set_group_regbit(reg, sizeof(reg));
}

int set_gpio83(int on)
{
	REG_BIT_T reg[] = {{SCR(0x054), GPIO83(clr), GPIO83(1)},
						{SCR(0x05c), GPOE83(clr), GPOE83(1)}};
					//{SCR(0x064), SPI_SEL(clr)|BSPI_SEL(clr), 0}};
	reg[0].set = (on? GPIO83(1): 0);
	return set_group_regbit(reg, sizeof(reg));
}


