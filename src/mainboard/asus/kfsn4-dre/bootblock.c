/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <bootblock_common.h>
#include <console/console.h>
#include <device/pci_ops.h>
#include <superio/winbond/common/winbond.h>
#include <superio/winbond/w83627thg/w83627thg.h>

#define SERIAL_DEV PNP_DEV(0x2e, W83627THG_SP1)

void bootblock_mainboard_early_init(void)
{
	/* Initialize early serial */
	winbond_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}

void bootblock_mainboard_init(void)
{
	printk(BIOS_DEBUG, "CPU INIT detected %08x\n",
	       pci_read_config32(PCI_DEV(0,0,0), 0x80));
}
