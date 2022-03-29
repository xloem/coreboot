/* SPDX-License-Identifier: GPL-2.0-only */

SECTIONS {
	_ROMSTRAP_SECTION = (0xffffffff - 0x10) - (__romstrap_end - __romstrap_start) + 1;
	. = _ROMSTRAP_SECTION;
	.romstrap (.): {
		KEEP(*(.romstrap))
	}
}
