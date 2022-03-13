/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/bootblock.h>
#include <device/pci.h>
#include <device/pci_ops.h>
#include "ck804.h"

static void ck804_enable_rom(void)
{
	unsigned char byte;
	pci_devfn_t addr;

	/* Enable 4MB ROM access at 0xFFC00000 - 0xFFFFFFFF. */
	/* Locate the ck804 LPC. */
	addr = PCI_DEV(0, (CK804_DEVN_BASE + 1), 0);

	/* Set the 4MB enable bit. */
	byte = pci_read_config8(addr, 0x88);
	byte |= 0x80;
	pci_write_config8(addr, 0x88, byte);
}

static void sio_setup(void)
{
	u32 dword;
	u8 byte;

	/* Subject decoding */
	byte = pci_read_config8(PCI_DEV(0, CK804_BOARD_BOOT_BASE_UNIT_UID + 1, 0), 0x7b);
	byte |= 0x20;
	pci_write_config8(PCI_DEV(0, CK804_BOARD_BOOT_BASE_UNIT_UID + 1, 0), 0x7b, byte);

	/* LPC Positive Decode 0 */
	dword = pci_read_config32(PCI_DEV(0, CK804_BOARD_BOOT_BASE_UNIT_UID + 1, 0), 0xa0);
	/* Serial 0, Serial 1 */
	dword |= (1 << 0) | (1 << 1);
	pci_write_config32(PCI_DEV(0, CK804_BOARD_BOOT_BASE_UNIT_UID + 1, 0), 0xa0, dword);
}

void bootblock_early_southbridge_init(void)
{
	ck804_enable_rom();
	sio_setup();
}
