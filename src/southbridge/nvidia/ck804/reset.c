/* SPDX-License-Identifier: GPL-2.0-only */

#define __SIMPLE_DEVICE__

#include <arch/io.h>
#include <reset.h>

#include "../../../northbridge/amd/amdfam10/reset_test.c"

void do_board_reset(void)
{
	set_bios_reset();
	/* Try rebooting through port 0xcf9. */
	outb((0 << 3) | (0 << 2) | (1 << 1), 0xcf9);
	outb((0 << 3) | (1 << 2) | (1 << 1), 0xcf9);
}
