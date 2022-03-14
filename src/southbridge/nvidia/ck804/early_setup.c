/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <console/console.h>
#include <reset.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <northbridge/amd/amdfam10/amdfam10.h>
#include <southbridge/amd/common/reset.h>

#include "ck804.h"
#include "early_setup_ss.h"

static const unsigned int ctrl_conf_fix_pci_numbering[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0x44), ~(0x00010000), 0x00000000,	/* Force CK804 to start its internal device numbering (Base Unit ID) at 0 instead of the power-on default of 1 */
};

static const unsigned int ctrl_conf_enable_msi_mapping[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0xe0), ~(0x00000000), 0x00010000,	/* Enable MSI mapping on host bridge -- without this Linux cannot use the network device MSI interrupts! */
};

/* Someone messed up and snuck in some K8-specific code */
static int  set_ht_link_buffer_counts_chain(uint8_t ht_c_num, unsigned int vendorid, unsigned int val) { return 0; /* stub */};

static int set_ht_link_ck804(u8 ht_c_num)
{
	unsigned int vendorid = 0x10de;
	unsigned int val = 0x01610169;
	return set_ht_link_buffer_counts_chain(ht_c_num, vendorid, val);
}

static void setup_ss_table(unsigned int index, unsigned int where, unsigned int control,
			   const unsigned int *register_values, int max)
{
	int i;
	unsigned int val;

	val = inl(control);
	val &= 0xfffffffe;
	outl(val, control);

	outl(0, index);

	for (i = 0; i < max; i++) {
		unsigned long reg;
		reg = register_values[i];
		outl(reg, where);
	}
	val = inl(control);
	val |= 1;
	outl(val, control);
}

/*
 * Values for CONFIG_CK804_PCI_E_X and CONFIG_CK804B_PCI_E_X.
 * Apparently some sort of lane configuration.
 *
 *	16 1 1 2 :0
 *	 8 8 2 2 :1
 *	 8 8 4   :2
 *	 8 4 4 4 :3
 *	16 4     :4
 */

/* There will be implicit offsets applied, the writes below do not
 * really happen at the PCI_ADDR() this expands to.
 */
#define CK804_DEV(d, f, r) PCI_ADDR(0, d, f, r)

static void ck804_early_set_port(unsigned int ck804_num, unsigned int *busn,
				 unsigned int *io_base)
{
	static const unsigned int ctrl_devport_conf[] = {
		CK804_DEV(0x1, 0, ANACTRL_REG_POS), ~(0x0000ff00), ANACTRL_IO_BASE,
		CK804_DEV(0x1, 0, SYSCTRL_REG_POS), ~(0x0000ff00), SYSCTRL_IO_BASE,
	};

	int j;
	for (j = 0; j < ck804_num; j++) {
		u32 dev;
		if (busn[j] == 0) /* SB chain */
			dev = PCI_DEV(busn[j], CK804_DEVN_BASE, 0);
		else
			dev = PCI_DEV(busn[j], CK804B_DEVN_BASE, 0);
		setup_resource_map_offset(ctrl_devport_conf,
			ARRAY_SIZE(ctrl_devport_conf), dev, io_base[j]);
	}
}

static void ck804_early_clear_port(unsigned int ck804_num, unsigned int *busn,
				   unsigned int *io_base)
{
	static const unsigned int ctrl_devport_conf_clear[] = {
		CK804_DEV(0x1, 0, ANACTRL_REG_POS), ~(0x0000ff01), 0,
		CK804_DEV(0x1, 0, SYSCTRL_REG_POS), ~(0x0000ff01), 0,
	};

	int j;
	for (j = 0; j < ck804_num; j++) {
		u32 dev;
		if (busn[j] == 0) /* SB chain */
			dev = PCI_DEV(busn[j], CK804_DEVN_BASE, 0);
		else
			dev = PCI_DEV(busn[j], CK804B_DEVN_BASE, 0);
		setup_resource_map_offset(ctrl_devport_conf_clear,
			ARRAY_SIZE(ctrl_devport_conf_clear), dev, io_base[j]);
	}
}

static void ck804_early_setup(unsigned int ck804_num, unsigned int *busn,
			      unsigned int *io_base)
{
	static const unsigned int ctrl_conf_master[] = {
		RES_PCI_IO, CK804_DEV(1, 2, 0x8c), 0xffff0000, 0x00009880,
		RES_PCI_IO, CK804_DEV(1, 2, 0x90), 0xffff000f, 0x000074a0,
		RES_PCI_IO, CK804_DEV(1, 2, 0xa0), 0xfffff0ff, 0x00000a00,
		RES_PCI_IO, CK804_DEV(1, 2, 0xac), 0xffffff00, 0x00000000,

		RES_PCI_IO, CK804_DEV(0, 0, 0x48), 0xfffffffd, 0x00000002,
		RES_PCI_IO, CK804_DEV(0, 0, 0x74), 0xfffff00f, 0x000009d0,
		RES_PCI_IO, CK804_DEV(0, 0, 0x8c), 0xffff0000, 0x0000007f,
		RES_PCI_IO, CK804_DEV(0, 0, 0xcc), 0xfffffff8, 0x00000003,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd0), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd4), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd8), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xdc), 0x7f000000, 0x00000000,

		RES_PCI_IO, CK804_DEV(1, 0, 0xf0), 0xfffffffd, 0x00000002,
		RES_PCI_IO, CK804_DEV(1, 0, 0xf8), 0xffffffcf, 0x00000010,

		RES_PCI_IO, CK804_DEV(9, 0, 0x40), 0xfff8ffff, 0x00030000,
		RES_PCI_IO, CK804_DEV(9, 0, 0x4c), 0xfe00ffff, 0x00440000,
		RES_PCI_IO, CK804_DEV(9, 0, 0x74), 0xffffffc0, 0x00000000,

#ifdef CK804_MB_SETUP
		CK804_MB_SETUP
#endif

#if CONFIG(NORTHBRIDGE_AMD_AMDFAM10)
		/*
		 * Avoid crash (complete with severe memory corruption!) during initial CAR boot
		 * in ck804_early_setup_x() on Fam10h systems by not touching 0x78.
		 * Interestingly once the system is fully booted into Linux this can be set, but
		 * not before!  Apparently something isn't initialized but the amount of effort
		 * required to fix this is non-negligible and of unknown real-world benefit
		 */
#else
		RES_PCI_IO, CK804_DEV(1, 0, 0x78), 0xc0ffffff, 0x19000000,
#endif
		RES_PCI_IO, CK804_DEV(1, 0, 0xe0), 0xfffffeff, 0x00000100,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x20, 0xe00fffff, 0x11000000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x24, 0xc3f0ffff, 0x24040000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x80, 0x8c3f04df, 0x51407120,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x84, 0xffffff8f, 0x00000010,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x94, 0xff00ffff, 0x00c00000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0xcc, 0xf7ffffff, 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x74, ~(0xffff), 0x0f008,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x78, ~((0xff) | (0xff << 16)), (0x41 << 16) | (0x32),
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x7c, ~(0xff << 16), (0xa0 << 16),

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x24, 0xfcffff0f, 0x020000b0,

		/* Activate master port on primary SATA controller. */
		RES_PCI_IO, CK804_DEV(7, 0, 0x50), ~(0x1f000013), 0x15000013,
		RES_PCI_IO, CK804_DEV(7, 0, 0x64), ~(0x00000001), 0x00000001,
		RES_PCI_IO, CK804_DEV(7, 0, 0x68), ~(0x02000000), 0x02000000,
		RES_PCI_IO, CK804_DEV(7, 0, 0x70), ~(0x000f0000), 0x00040000,
		RES_PCI_IO, CK804_DEV(7, 0, 0xa0), ~(0x000001ff), 0x00000150,
		RES_PCI_IO, CK804_DEV(7, 0, 0xac), ~(0xffff8f00), 0x02aa8b00,
		RES_PCI_IO, CK804_DEV(7, 0, 0x7c), ~(0x00000010), 0x00000000,
		RES_PCI_IO, CK804_DEV(7, 0, 0xc8), ~(0x0fff0fff), 0x000a000a,
		RES_PCI_IO, CK804_DEV(7, 0, 0xd0), ~(0xf0000000), 0x00000000,
		RES_PCI_IO, CK804_DEV(7, 0, 0xe0), ~(0xf0000000), 0x00000000,

		RES_PCI_IO, CK804_DEV(8, 0, 0x50), ~(0x1f000013), 0x15000013,
		RES_PCI_IO, CK804_DEV(8, 0, 0x64), ~(0x00000001), 0x00000001,
		RES_PCI_IO, CK804_DEV(8, 0, 0x68), ~(0x02000000), 0x02000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0x70), ~(0x000f0000), 0x00040000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xa0), ~(0x000001ff), 0x00000150,
		RES_PCI_IO, CK804_DEV(8, 0, 0xac), ~(0xffff8f00), 0x02aa8b00,
		RES_PCI_IO, CK804_DEV(8, 0, 0x7c), ~(0x00000010), 0x00000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xc8), ~(0x0fff0fff), 0x000a000a,
		RES_PCI_IO, CK804_DEV(8, 0, 0xd0), ~(0xf0000000), 0x00000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xe0), ~(0xf0000000), 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x04, ~((0x3ff << 0) | (0x3ff << 10)), (0x21 << 0) | (0x22 << 10),

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x08, ~(0xfffff), (0x1c << 10) | 0x1b,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x80, ~(1 << 3), 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0xcc, ~((7 << 4) | (1 << 8)), (CONFIG_CK804_PCI_E_X << 4) | (1 << 8),

		/* SYSCTRL */
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 8, ~(0xff), ((0 << 4) | (0 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 9, ~(0xff), ((0 << 4) | (1 << 2) | (1 << 0)),
#if CONFIG(CK804_USE_NIC)
		RES_PCI_IO, CK804_DEV(0xa, 0, 0xf8), 0xffffffbf, 0x00000040,
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 19, ~(0xff), ((0 << 4) | (1 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 3, ~(0xff), ((0 << 4) | (1 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 3, ~(0xff), ((0 << 4) | (1 << 2) | (1 << 0)),
		RES_PCI_IO, CK804_DEV(1, 0, 0xe4), ~(1 << 23), (1 << 23),
#endif

#if CONFIG(CK804_USE_ACI)
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 0x0d, ~(0xff), ((0 << 4) | (2 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 0x1a, ~(0xff), ((0 << 4) | (2 << 2) | (0 << 0)),
#endif

#if CONFIG(CK804_PCIE_PME_WAKE)
		RES_PCI_IO, CK804_DEV(1, 0, 0xe4), 0xffffffff, 0x00400000,
#else
		RES_PCI_IO, CK804_DEV(1, 0, 0xe4), 0xffbfffff, 0x00000000,
#endif
	};

	static const unsigned int ctrl_conf_multiple[] = {
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 0, ~(3 << 2), (0 << 2),
	};

	static const unsigned int ctrl_conf_slave[] = {
		RES_PCI_IO, CK804_DEV(1, 2, 0x8c), 0xffff0000, 0x00009880,
		RES_PCI_IO, CK804_DEV(1, 2, 0x90), 0xffff000f, 0x000074a0,
		RES_PCI_IO, CK804_DEV(1, 2, 0xa0), 0xfffff0ff, 0x00000a00,

		RES_PCI_IO, CK804_DEV(0, 0, 0x48), 0xfffffffd, 0x00000002,
		RES_PCI_IO, CK804_DEV(0, 0, 0x74), 0xfffff00f, 0x000009d0,
		RES_PCI_IO, CK804_DEV(0, 0, 0x8c), 0xffff0000, 0x0000007f,
		RES_PCI_IO, CK804_DEV(0, 0, 0xcc), 0xfffffff8, 0x00000003,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd0), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd4), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xd8), 0xff000000, 0x00000000,
		RES_PCI_IO, CK804_DEV(0, 0, 0xdc), 0x7f000000, 0x00000000,

		RES_PCI_IO, CK804_DEV(1, 0, 0xf0), 0xfffffffd, 0x00000002,
		RES_PCI_IO, CK804_DEV(1, 0, 0xf8), 0xffffffcf, 0x00000010,

		RES_PCI_IO, CK804_DEV(9, 0, 0x40), 0xfff8ffff, 0x00030000,
		RES_PCI_IO, CK804_DEV(9, 0, 0x4c), 0xfe00ffff, 0x00440000,
		RES_PCI_IO, CK804_DEV(9, 0, 0x74), 0xffffffc0, 0x00000000,

		/*
		 * Avoid touching 0x78 for CONFIG_NORTHBRIDGE_AMD_AMDFAM10 for
		 * non-primary chains too???
		 */
		RES_PCI_IO, CK804_DEV(1, 0, 0x78), 0xc0ffffff, 0x20000000,
		RES_PCI_IO, CK804_DEV(1, 0, 0xe0), 0xfffffeff, 0x00000000,
		RES_PCI_IO, CK804_DEV(1, 0, 0xe8), 0xffffff00, 0x000000ff,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x20, 0xe00fffff, 0x11000000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x24, 0xc3f0ffff, 0x24040000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x80, 0x8c3f04df, 0x51407120,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x84, 0xffffff8f, 0x00000010,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x94, 0xff00ffff, 0x00c00000,
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0xcc, 0xf7ffffff, 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x24, 0xfcffff0f, 0x020000b0,

		RES_PCI_IO, CK804_DEV(8, 0, 0x50), ~(0x1f000013), 0x15000013,
		RES_PCI_IO, CK804_DEV(8, 0, 0x64), ~(0x00000001), 0x00000001,
		RES_PCI_IO, CK804_DEV(8, 0, 0x68), ~(0x02000000), 0x02000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0x70), ~(0x000f0000), 0x00040000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xa0), ~(0x000001ff), 0x00000150,
		RES_PCI_IO, CK804_DEV(8, 0, 0xac), ~(0xffff8f00), 0x02aa8b00,
		RES_PCI_IO, CK804_DEV(8, 0, 0x7c), ~(0x00000010), 0x00000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xc8), ~(0x0fff0fff), 0x000a000a,
		RES_PCI_IO, CK804_DEV(8, 0, 0xd0), ~(0xf0000000), 0x00000000,
		RES_PCI_IO, CK804_DEV(8, 0, 0xe0), ~(0xf0000000), 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x04, ~((0x3ff << 0) | (0x3ff << 10)), (0x21 << 0) | (0x22 << 10),

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x08, ~(0xfffff), (0x1c << 10) | 0x1b,

		/* This line doesn't exist in the non-CAR version. */
		RES_PORT_IO_32, ANACTRL_IO_BASE + 0x80, ~(1 << 3), 0x00000000,

		RES_PORT_IO_32, ANACTRL_IO_BASE + 0xcc, ~((7 << 4) | (1 << 8)), (CONFIG_CK804B_PCI_E_X << 4) | (1 << 8),

#if CONFIG(CK804_USE_NIC)
		RES_PCI_IO, CK804_DEV(0xa, 0, 0xf8), 0xffffffbf, 0x00000040,
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 19, ~(0xff), ((0 << 4) | (1 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 3, ~(0xff), ((0 << 4) | (1 << 2) | (0 << 0)),
		RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0 + 3, ~(0xff), ((0 << 4) | (1 << 2) | (1 << 0)),
		RES_PCI_IO, CK804_DEV(1, 0, 0xe4), ~(1 << 23), (1 << 23),
#endif
	};

	int j;
	for (j = 0; j < ck804_num; j++) {
		if (busn[j] == 0) {
			setup_resource_map_x_offset(ctrl_conf_master,
				ARRAY_SIZE(ctrl_conf_master),
				PCI_DEV(0, CK804_DEVN_BASE, 0), io_base[0]);
			if (ck804_num > 1)
				setup_resource_map_x_offset(ctrl_conf_multiple,
					ARRAY_SIZE(ctrl_conf_multiple),
					PCI_DEV(0, CK804_DEVN_BASE, 0), 0);

			continue;
		}

		setup_resource_map_x_offset(ctrl_conf_slave,
			ARRAY_SIZE(ctrl_conf_slave),
			PCI_DEV(busn[j], CK804B_DEVN_BASE, 0), io_base[j]);
	}

	for (j = 0; j < ck804_num; j++) {
		/* PCI-E (XSPLL) SS table 0x40, x044, 0x48 */
		/* SATA  (SPPLL) SS table 0xb0, 0xb4, 0xb8 */
		/* CPU   (PPLL)  SS table 0xc0, 0xc4, 0xc8 */
		setup_ss_table(io_base[j] + ANACTRL_IO_BASE + 0x40,
			       io_base[j] + ANACTRL_IO_BASE + 0x44,
			       io_base[j] + ANACTRL_IO_BASE + 0x48,
			       pcie_ss_tbl, 64);
		setup_ss_table(io_base[j] + ANACTRL_IO_BASE + 0xb0,
			       io_base[j] + ANACTRL_IO_BASE + 0xb4,
			       io_base[j] + ANACTRL_IO_BASE + 0xb8,
			       sata_ss_tbl, 64);
		setup_ss_table(io_base[j] + ANACTRL_IO_BASE + 0xc0,
			       io_base[j] + ANACTRL_IO_BASE + 0xc4,
			       io_base[j] + ANACTRL_IO_BASE + 0xc8,
			       cpu_ss_tbl, 64);
	}
}

static int ck804_early_setup_x(void)
{
	unsigned int busn[4], io_base[4];
	int i, ck804_num = 0;

	for (i = 0; i < 4; i++) {
		u32 id;
		pci_devfn_t dev;
		if (i == 0) /* SB chain */
			dev = PCI_DEV(i * 0x40, CK804_DEVN_BASE, 0);
		else
			dev = PCI_DEV(i * 0x40, CK804B_DEVN_BASE, 0);
		id = pci_read_config32(dev, PCI_VENDOR_ID);
		if (id == 0x005e10de) {
			busn[ck804_num] = i * 0x40;
			io_base[ck804_num] = i * 0x4000;
			ck804_num++;
		}
	}

	ck804_early_set_port(ck804_num, busn, io_base);
	ck804_early_setup(ck804_num, busn, io_base);
	ck804_early_clear_port(ck804_num, busn, io_base);

	return set_ht_link_ck804(4);
}

void do_board_reset(void)
{
	set_bios_reset();

	/* full reset */
	outb(0x0a, 0x0cf9);
	outb(0x0e, 0x0cf9);
}

void do_soft_reset(void)
{
	set_bios_reset();

	/* link reset */
	outb(0x02, 0x0cf9);
	outb(0x06, 0x0cf9);
}

void enable_fid_change_on_sb(unsigned int sbbusn, unsigned int sbdn)
{
	/* The default value for CK804 is good. */
	/* Set VFSMAF (VID/FID System Management Action Field) to 2. */
}

void ck804_control(const unsigned int *values, u32 size,
			  uint8_t bus_unit_id)
{
	unsigned int busn[4], io_base[4];
	int i, ck804_num = 0;

	for (i = 0; i < 4; i++) {
		u32 id;
		pci_devfn_t dev;
		if (i == 0) /* SB chain */
			dev = PCI_DEV(i * 0x40, bus_unit_id, 0);
		else
			dev = 0;
		id = pci_read_config32(dev, PCI_VENDOR_ID);
		if (id == 0x005e10de) {
			busn[ck804_num] = i * 0x40;
			io_base[ck804_num] = i * 0x4000;
			ck804_num++;
		}
	}

	if (ck804_num < 1) {
		printk(BIOS_WARNING, "CK804 not found at device base unit id %02x!\n", bus_unit_id);
		return;
	}

	ck804_early_set_port(ck804_num, busn, io_base);

	setup_resource_map_x_offset(values,
		size,
		PCI_DEV(0, bus_unit_id, 0), io_base[0]);

	ck804_early_clear_port(ck804_num, busn, io_base);
}

/**
 * @brief Get SouthBridge device number
 * @param[in] bus target bus number
 * @return southbridge device number
 */
unsigned int get_sbdn(unsigned int bus)
{
	pci_devfn_t dev;

	dev = pci_locate_device_on_bus(PCI_ID(PCI_VENDOR_ID_NVIDIA,
					PCI_DEVICE_ID_NVIDIA_CK804_PRO), bus);
	return (dev >> 15) & 0x1f;
}

void southbridge_early_setup(void)
{
	u32 wants_reset;

	printk(BIOS_DEBUG, "set_ck804_base_unit_id()\n");
	ck804_control(ctrl_conf_fix_pci_numbering, ARRAY_SIZE(ctrl_conf_fix_pci_numbering), CK804_BOARD_BOOT_BASE_UNIT_UID);

	wants_reset = ck804_early_setup_x();

	if (wants_reset) {
		printk(BIOS_DEBUG, "ck804_early_setup_x wanted additional reset!\n");
	}

}

void southbridge_ht_init(void)
{
	int wants_reset = ck804_early_setup_x();

	/* Reset for HT, FIDVID, PLL and errata changes to take affect. */
	if (!warm_reset_detect(0)) {
		printk(BIOS_INFO, "...WARM RESET...\n\n\n");
		soft_reset();
		die("After soft_reset - shouldn't see this message!!!\n");
	}

	if (wants_reset) {
		printk(BIOS_DEBUG, "ck804_early_setup_x wanted additional reset!\n");
	}
}

void southbridge_before_pci_init(void)
{
	printk(BIOS_DEBUG, "enable_msi_mapping()\n");
	ck804_control(ctrl_conf_enable_msi_mapping, ARRAY_SIZE(ctrl_conf_enable_msi_mapping), CK804_DEVN_BASE);
}
