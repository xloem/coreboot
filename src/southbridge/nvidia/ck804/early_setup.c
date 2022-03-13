/* SPDX-License-Identifier: GPL-2.0-or-later */

static const unsigned int ctrl_conf_fix_pci_numbering[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0x44), ~(0x00010000), 0x00000000,	/* Force CK804 to start its internal device numbering (Base Unit ID) at 0 instead of the power-on default of 1 */
};

static const unsigned int ctrl_conf_enable_msi_mapping[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0xe0), ~(0x00000000), 0x00010000,	/* Enable MSI mapping on host bridge -- without this Linux cannot use the network device MSI interrupts! */
};

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
	printk(BIOS_DEBUG, "set_ck804_base_unit_id()\n");
	ck804_control(ctrl_conf_fix_pci_numbering, ARRAY_SIZE(ctrl_conf_fix_pci_numbering), CK804_BOARD_BOOT_BASE_UNIT_UID);
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
