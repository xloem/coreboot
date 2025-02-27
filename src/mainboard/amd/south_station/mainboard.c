/* SPDX-License-Identifier: GPL-2.0-only */

#include <amdblocks/acpimmio.h>
#include <amdblocks/acpimmio_legacy_gpio100.h>
#include <delay.h>
#include <device/device.h>
#include <southbridge/amd/common/amd_pci_util.h>

static const u8 mainboard_intr_data[] = {
	[0x00] = 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,  /* INTA# - INTH# */
	[0x08] = 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F,  /* Misc-nil, 0, 1, 2,  INT from Serial irq */
	[0x10] = 0x09, 0x1F, 0x1F, 0x10, 0x1F, 0x12, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x12, 0x11, 0x12, 0x11, 0x12, 0x11, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x11, 0x12, 0x13
};

/* PIRQ Setup */
static void pirq_setup(void)
{
	intr_data_ptr = mainboard_intr_data;
}

/**
 * Southstation using SB GPIO 17/18 to control the Red/Green LED
 * These two LEDs can be used to show the OS booting status.
 */
static void southstation_led_init(void)
{
	/* multi-function pins switch to GPIO0-35 */
	pm_write8(0xea, (pm_read8(0xea) & 0xfe) | 1);

	/* select IOMux to function2, corresponds to GPIO */
	iomux_write8(0x11, (iomux_read8(0x11) & 0xfc) | 2);
	iomux_write8(0x12, (iomux_read8(0x12) & 0xfc) | 2);

	/* Lighting test */
	gpio_100_write8(0x11, 0x08); //output high
	gpio_100_write8(0x12, 0x08);
	mdelay(100);
	gpio_100_write8(0x11, 0x48); //output low
	gpio_100_write8(0x12, 0x48);
}

/**********************************************
 * Enable the dedicated functions of the board.
 **********************************************/
static void mainboard_enable(struct device *dev)
{
	pirq_setup();

	southstation_led_init();

	/*
	 * Initialize ASF registers to an arbitrary address because someone
	 * long ago set things up this way inside the SPD read code.  The
	 * SPD read code has been made generic and moved out of the board
	 * directory, so the ASF init is being done here.
	 */
	pm_write8(0x29, 0x80);
	pm_write8(0x28, 0x61);
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};
