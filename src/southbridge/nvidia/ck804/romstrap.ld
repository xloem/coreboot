/* SPDX-License-Identifier: GPL-2.0-only */

SECTIONS {
	. = (0xffffffff - 0x10) - (__romstrap_end - __romstrap_start) + 1;
	.romstrap (.): {
		KEEP(*(.romstrap))
	}
}
