/*
 * This file is part of the coreboot project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * File taken from the Linux ast driver (v3.18.5)
 * coreboot-specific includes added at top and/or contents modified
 * as needed to function within the coreboot environment.
 */
#ifndef AST_DRAM_TABLES_H
#define AST_DRAM_TABLES_H

/* DRAM timing tables */
struct ast_dramstruct {
	u16 index;
	u32 data;
};

static const struct ast_dramstruct ast2000_dram_table_data[] = {
	{ 0x0108, 0x00000000 },
	{ 0x0120, 0x00004a21 },
	{ 0xFF00, 0x00000043 },
	{ 0x0000, 0xFFFFFFFF },
	{ 0x0004, 0x00000089 },
	{ 0x0008, 0x22331353 },
	{ 0x000C, 0x0d07000b },
	{ 0x0010, 0x11113333 },
	{ 0x0020, 0x00110350 },
	{ 0x0028, 0x1e0828f0 },
	{ 0x0024, 0x00000001 },
	{ 0x001C, 0x00000000 },
	{ 0x0014, 0x00000003 },
	{ 0xFF00, 0x00000043 },
	{ 0x0018, 0x00000131 },
	{ 0x0014, 0x00000001 },
	{ 0xFF00, 0x00000043 },
	{ 0x0018, 0x00000031 },
	{ 0x0014, 0x00000001 },
	{ 0xFF00, 0x00000043 },
	{ 0x0028, 0x1e0828f1 },
	{ 0x0024, 0x00000003 },
	{ 0x002C, 0x1f0f28fb },
	{ 0x0030, 0xFFFFFE01 },
	{ 0xFFFF, 0xFFFFFFFF }
};

static const struct ast_dramstruct ast1100_dram_table_data[] = {
	{ 0x2000, 0x1688a8a8 },
	{ 0x2020, 0x000041f0 },
	{ 0xFF00, 0x00000043 },
	{ 0x0000, 0xfc600309 },
	{ 0x006C, 0x00909090 },
	{ 0x0064, 0x00050000 },
	{ 0x0004, 0x00000585 },
	{ 0x0008, 0x0011030f },
	{ 0x0010, 0x22201724 },
	{ 0x0018, 0x1e29011a },
	{ 0x0020, 0x00c82222 },
	{ 0x0014, 0x01001523 },
	{ 0x001C, 0x1024010d },
	{ 0x0024, 0x00cb2522 },
	{ 0x0038, 0xffffff82 },
	{ 0x003C, 0x00000000 },
	{ 0x0040, 0x00000000 },
	{ 0x0044, 0x00000000 },
	{ 0x0048, 0x00000000 },
	{ 0x004C, 0x00000000 },
	{ 0x0050, 0x00000000 },
	{ 0x0054, 0x00000000 },
	{ 0x0058, 0x00000000 },
	{ 0x005C, 0x00000000 },
	{ 0x0060, 0x032aa02a },
	{ 0x0064, 0x002d3000 },
	{ 0x0068, 0x00000000 },
	{ 0x0070, 0x00000000 },
	{ 0x0074, 0x00000000 },
	{ 0x0078, 0x00000000 },
	{ 0x007C, 0x00000000 },
	{ 0x0034, 0x00000001 },
	{ 0xFF00, 0x00000043 },
	{ 0x002C, 0x00000732 },
	{ 0x0030, 0x00000040 },
	{ 0x0028, 0x00000005 },
	{ 0x0028, 0x00000007 },
	{ 0x0028, 0x00000003 },
	{ 0x0028, 0x00000001 },
	{ 0x000C, 0x00005a08 },
	{ 0x002C, 0x00000632 },
	{ 0x0028, 0x00000001 },
	{ 0x0030, 0x000003c0 },
	{ 0x0028, 0x00000003 },
	{ 0x0030, 0x00000040 },
	{ 0x0028, 0x00000003 },
	{ 0x000C, 0x00005a21 },
	{ 0x0034, 0x00007c03 },
	{ 0x0120, 0x00004c41 },
	{ 0xffff, 0xffffffff },
};

static const struct ast_dramstruct ast2100_dram_table_data[] = {
	{ 0x2000, 0x1688a8a8 },
	{ 0x2020, 0x00004120 },
	{ 0xFF00, 0x00000043 },
	{ 0x0000, 0xfc600309 },
	{ 0x006C, 0x00909090 },
	{ 0x0064, 0x00070000 },
	{ 0x0004, 0x00000489 },
	{ 0x0008, 0x0011030f },
	{ 0x0010, 0x32302926 },
	{ 0x0018, 0x274c0122 },
	{ 0x0020, 0x00ce2222 },
	{ 0x0014, 0x01001523 },
	{ 0x001C, 0x1024010d },
	{ 0x0024, 0x00cb2522 },
	{ 0x0038, 0xffffff82 },
	{ 0x003C, 0x00000000 },
	{ 0x0040, 0x00000000 },
	{ 0x0044, 0x00000000 },
	{ 0x0048, 0x00000000 },
	{ 0x004C, 0x00000000 },
	{ 0x0050, 0x00000000 },
	{ 0x0054, 0x00000000 },
	{ 0x0058, 0x00000000 },
	{ 0x005C, 0x00000000 },
	{ 0x0060, 0x0f2aa02a },
	{ 0x0064, 0x003f3005 },
	{ 0x0068, 0x02020202 },
	{ 0x0070, 0x00000000 },
	{ 0x0074, 0x00000000 },
	{ 0x0078, 0x00000000 },
	{ 0x007C, 0x00000000 },
	{ 0x0034, 0x00000001 },
	{ 0xFF00, 0x00000043 },
	{ 0x002C, 0x00000942 },
	{ 0x0030, 0x00000040 },
	{ 0x0028, 0x00000005 },
	{ 0x0028, 0x00000007 },
	{ 0x0028, 0x00000003 },
	{ 0x0028, 0x00000001 },
	{ 0x000C, 0x00005a08 },
	{ 0x002C, 0x00000842 },
	{ 0x0028, 0x00000001 },
	{ 0x0030, 0x000003c0 },
	{ 0x0028, 0x00000003 },
	{ 0x0030, 0x00000040 },
	{ 0x0028, 0x00000003 },
	{ 0x000C, 0x00005a21 },
	{ 0x0034, 0x00007c03 },
	{ 0x0120, 0x00005061 },
	{ 0xffff, 0xffffffff },
};

/*
 * AST2500 DRAM settings modules
 */
#define REGTBL_NUM		17
#define REGIDX_010		0
#define REGIDX_014		1
#define REGIDX_018		2
#define REGIDX_020		3
#define REGIDX_024		4
#define REGIDX_02C		5
#define REGIDX_030		6
#define REGIDX_214		7
#define REGIDX_2E0		8
#define REGIDX_2E4		9
#define REGIDX_2E8		10
#define REGIDX_2EC		11
#define REGIDX_2F0		12
#define REGIDX_2F4		13
#define REGIDX_2F8		14
#define REGIDX_RFC		15
#define REGIDX_PLL		16

static const u32 ast2500_ddr3_1600_timing_table[REGTBL_NUM] = {
	0x64604D38,			/* 0x010 */
	0x29690599,			/* 0x014 */
	0x00000300,			/* 0x018 */
	0x00000000,			/* 0x020 */
	0x00000000,			/* 0x024 */
	0x02181E70,			/* 0x02C */
	0x00000040,			/* 0x030 */
	0x00000024,			/* 0x214 */
	0x02001300,			/* 0x2E0 */
	0x0E0000A0,			/* 0x2E4 */
	0x000E001B,			/* 0x2E8 */
	0x35B8C105,			/* 0x2EC */
	0x08090408,			/* 0x2F0 */
	0x9B000800,			/* 0x2F4 */
	0x0E400A00,			/* 0x2F8 */
	0x9971452F,			/* tRFC  */
	0x000071C1			/* PLL   */
	};

	static const u32 ast2500_ddr4_1600_timing_table[REGTBL_NUM] = {
	0x63604E37,			/* 0x010 */
	0xE97AFA99,			/* 0x014 */
	0x00019000,			/* 0x018 */
	0x08000000,			/* 0x020 */
	0x00000400,			/* 0x024 */
	0x00000410,			/* 0x02C */
	0x00000101,			/* 0x030 */
	0x00000024,			/* 0x214 */
	0x03002900,			/* 0x2E0 */
	0x0E0000A0,			/* 0x2E4 */
	0x000E001C,			/* 0x2E8 */
	0x35B8C106,			/* 0x2EC */
	0x08080607,			/* 0x2F0 */
	0x9B000900,			/* 0x2F4 */
	0x0E400A00,			/* 0x2F8 */
	0x99714545,			/* tRFC  */
	0x000071C1			/* PLL   */
	};

#endif
