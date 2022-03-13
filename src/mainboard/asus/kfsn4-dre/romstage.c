/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <stdint.h>
#include <device/pci_ids.h>
#include <device/pnp_ops.h>
#include <arch/mmio.h>
#include <console/console.h>
#include <cpu/amd/common/common.h>
#include <southbridge/nvidia/ck804/early_smbus.h>
#include <northbridge/amd/amdfam10/amdfam10.h>
#include <northbridge/amd/amdfam10/raminit.h>

#include <delay.h>
#include <option.h>

/* DIMM SPD addresses */
#define DIMM0	0x50
#define DIMM1	0x51
#define DIMM2	0x52
#define DIMM3	0x53
#define DIMM4	0x54
#define DIMM5	0x55
#define DIMM6	0x56
#define DIMM7	0x57

#define CK804_MB_SETUP \
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+33, ~(0x0f),(0x04 | 0x01),	/* -ENOINFO Proprietary BIOS sets this register; "When in Rome..."*/

#include <southbridge/nvidia/ck804/early_setup_ss.h>
#include "southbridge/nvidia/ck804/early_setup_car.c"

#define GPIO3_DEV PNP_DEV(0x2e, W83627THG_GPIO3)

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

/*
 * ASUS KFSN4-DRE specific SPD enable/disable magic.
 *
 * Setting CK804 GPIO43 and GPIO44 to 0 and 0 respectively will make the
 * board DIMMs accessible at SMBus/SPD offsets 0x50-0x53. Per default the SPD
 * offsets 0x50-0x53 are _not_ readable (all SPD reads will return 0xff) which
 * will make RAM init fail.
 *
 * Disable SPD access after RAM init to allow access to standard SMBus/I2C offsets
 * which is required e.g. by lm-sensors.
 */

#define CK804_BOARD_BOOT_BASE_UNIT_UID 1

static const unsigned int ctrl_conf_enable_spd_node0[] = {
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+42, ~(0x0f),(0x04 | 0x00),/* W2,GPIO43, U6 input S0*/
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+43, ~(0x0f),(0x04 | 0x00),/* W3,GPIO44, U6 input S1*/
};

static const unsigned int ctrl_conf_enable_spd_node1[] = {
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+42, ~(0x0f),(0x04 | 0x00),/* W2,GPIO43, U6 input S0*/
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+43, ~(0x0f),(0x04 | 0x01),/* W3,GPIO44, U6 input S1*/
};

static const unsigned int ctrl_conf_disable_spd[] = {
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+42, ~(0x0f),(0x04 | 0x01),/* W2,GPIO43, U6 input S0*/
	RES_PORT_IO_8, SYSCTRL_IO_BASE + 0xc0+43, ~(0x0f),(0x04 | 0x00),/* W3,GPIO44, U6 input S1*/
};

static const unsigned int ctrl_conf_fix_pci_numbering[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0x44), ~(0x00010000), 0x00000000,	/* Force CK804 to start its internal device numbering (Base Unit ID) at 0 instead of the power-on default of 1 */
};

static const unsigned int ctrl_conf_enable_msi_mapping[] = {
	RES_PCI_IO, PCI_ADDR(0, 0, 0, 0xe0), ~(0x00000000), 0x00010000,	/* Enable MSI mapping on host bridge -- without this Linux cannot use the network device MSI interrupts! */
};

static void ck804_control(const unsigned int *values, u32 size,
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

void activate_spd_rom(const struct mem_controller *ctrl)
{
	printk(BIOS_DEBUG, "activate_spd_rom() for node %02x\n", ctrl->node_id);
	if (ctrl->node_id == 0) {
		printk(BIOS_DEBUG, "enable_spd_node0()\n");
		ck804_control(ctrl_conf_enable_spd_node0, ARRAY_SIZE(ctrl_conf_enable_spd_node0), CK804_DEVN_BASE);
	}
	else if (ctrl->node_id == 1) {
		printk(BIOS_DEBUG, "enable_spd_node1()\n");
		ck804_control(ctrl_conf_enable_spd_node1, ARRAY_SIZE(ctrl_conf_enable_spd_node1), CK804_DEVN_BASE);
	}
}

void mainboard_sysinfo_hook(struct sys_info *sysinfo)
{
	// ht_speed_limit not set
}

void mainboard_early_init(int s3_resume)
{
	// no peripheral control lines

	// changed pci_write_config16 for SP5100 GPIOs to pnp_write_config for SuperIO gpios
	/* Initialize GPIO */
	/* Access SuperIO GPI03 logical device */
	pnp_enter_conf_state(GPIO3_DEV);
	pnp_set_logical_device(GPIO3_DEV);
	/* Set GP37 (power LED) to output */
	pnp_write_config(GPIO3_DEV, 0xf0, 0x7f);
	/* Set GP37 (power LED) on */
	pnp_write_config(GPIO3_DEV, 0xf1, 0x80);
	/* Set pin 64 multiplex to GP37 */
	uint8_t cr2c = pnp_read_config(GPIO3_DEV, 0x2c);
	pnp_write_config(GPIO3_DEV, 0x2c, (cr2c & 0xf3) | 0x04);
	/* Restore default SuperIO access */
	pnp_exit_conf_state(GPIO3_DEV);
}

static const uint8_t spd_addr[] = {
	// Node 0
	RC00, DIMM0, DIMM2, DIMM4, DIMM6, DIMM1, DIMM3, DIMM5, DIMM7,
	// Node 1
	RC01, DIMM0, DIMM2, DIMM4, DIMM6, DIMM1, DIMM3, DIMM5, DIMM7,
};

void mainboard_spd_info(struct sys_info *sysinfo)
{
	/* It's the time to set ctrl in sysinfo now; */
	printk(BIOS_DEBUG, "fill_mem_ctrl() detected %d nodes\n", sysinfo->nodes);
	fill_mem_ctrl(sysinfo->nodes, sysinfo->ctrl, spd_addr);
	post_code(0x3D);
}

void mainboard_after_raminit(struct sys_info *sysinfo)
{
	// no execute_memory_test

	printk(BIOS_DEBUG, "disable_spd()\n");
	// changed spd_mux stuff to ck804
	ck804_control(ctrl_conf_disable_spd, ARRAY_SIZE(ctrl_conf_disable_spd), CK804_DEVN_BASE);
}

//void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
//{
//	// esp not cached
//	
//	struct sys_info *sysinfo = get_sysinfo();
//
//	// ht_speed_limit not set
//
//	u32 bsp_apicid = 0, val, wants_reset; // wants_reset added
//	// byte, power_on_reset not used
//	msr_t msr;
//
//	// s3resume not used
//
//	// timestamps are outside cpu_init_detected instead of inside
//	timestamp_init(timestamp_get());
//	timestamp_add_now(TS_START_ROMSTAGE);
//
//	if (!cpu_init_detectedx && boot_cpu()) {
//		// no spinlocks inited
//
//		/* Nothing special needs to be done to find bus 0 */
//		/* Allow the HT devices to be found */
//		set_bsp_node_CHtExtNodeCfgEn();
//		enumerate_ht_chain();
//		// sio_setup added
//		sio_setup();
//
//		// no bridge initializations
//		// serial port initialization moved outside
//		// no LPC DMA support yet
//	}
//
//	// no esp spewing
//
//	post_code(0x30);
//
//	if (bist == 0)
//		bsp_apicid = init_cpus(cpu_init_detectedx, sysinfo);
//
//	post_code(0x32);
//
//	// serial init moved down here
//	winbond_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
//	console_init();
//
//	if (CONFIG_MAX_PHYSICAL_CPUS != 2)
//		printk(BIOS_WARNING, "CONFIG_MAX_PHYSICAL_CPUS is %d, but this is a dual socket board!\n", CONFIG_MAX_PHYSICAL_CPUS);
//
//	/* Halt if there was a built in self test failure */
//	report_bist_failure(bist);
//
//	val = cpuid_eax(1);
//	printk(BIOS_DEBUG, "BSP Family_Model: %08x\n", val);
//	printk(BIOS_DEBUG, "*sysinfo range: [%p,%p]\n",sysinfo,sysinfo+1);
//	printk(BIOS_DEBUG, "bsp_apicid = %02x\n", bsp_apicid);
//	printk(BIOS_DEBUG, "cpu_init_detectedx = %08lx\n", cpu_init_detectedx);
//
//	/* Setup sysinfo defaults */
//	set_sysinfo_in_ram(0);
//
//	// no sb7xx_51xx_decode_last_reset for power_on_reset
//
//	// no initialize_mca for power_on_reset
//	update_microcode(val);
//
//	post_code(0x33);
//
//	cpuSetAMDMSR(0);
//	post_code(0x34);
//
//	amd_ht_init(sysinfo);
//	// no amd_ht_fixup
//	post_code(0x35);
//
//	/* Setup nodes PCI space and start core 0 AP init. */
//	finalize_node_setup(sysinfo);
//
//	/* Setup any mainboard PCI settings etc. */
//	setup_mb_resource_map();
//	// no initialize_mca for power_on_reset
//	post_code(0x36);
//
//	// more comments here
//	/* wait for all the APs core0 started by finalize_node_setup. */
//	/* FIXME: A bunch of cores are going to start output to serial at once.
//	 * It would be nice to fix up prink spinlocks for ROM XIP mode.
//	 * I think it could be done by putting the spinlock flag in the cache
//	 * of the BSP located right after sysinfo.
//	 */
//	wait_all_core0_started();
//
//	// no _early_setup
//
//	// LOGICAL_CPUS are moved _after_ SET_FIDVID
//
//	if (CONFIG(SET_FIDVID)) {
//		msr = rdmsr(MSR_COFVID_STS);
//		printk(BIOS_DEBUG, "\nBegin FIDVID MSR 0xc0010071 0x%08x 0x%08x\n", msr.hi, msr.lo);
//
//		// no enable_fid_change_on_sb
//
//		post_code(0x39);
//
//		// no preprocessor guards around these condition blocks
//		if (!warm_reset_detect(0)) {			// BSP is node 0
//			init_fidvid_bsp(bsp_apicid, sysinfo->nodes);
//		} else {
//			init_fidvid_stage2(bsp_apicid, 0);	// BSP is node 0
//		}
//
//		post_code(0x3A);
//
//		/* show final fid and vid */
//		msr = rdmsr(MSR_COFVID_STS);
//		printk(BIOS_DEBUG, "End FIDVIDMSR 0xc0010071 0x%08x 0x%08x\n", msr.hi, msr.lo);
//	}
//
//	// LOGICAL_CPUS are moved _after_ SET_FIDVID
//
//	if (CONFIG(LOGICAL_CPUS)) {
//		/* Core0 on each node is configured. Now setup any additional cores. */
//		printk(BIOS_DEBUG, "start_other_cores()\n");
//		start_other_cores(bsp_apicid);
//		post_code(0x37);
//		wait_all_other_cores_started(bsp_apicid);
//	}
//
//	// added ck804 call here
//	printk(BIOS_DEBUG, "set_ck804_base_unit_id()\n");
//	ck804_control(ctrl_conf_fix_pci_numbering, ARRAY_SIZE(ctrl_conf_fix_pci_numbering), CK804_BOARD_BOOT_BASE_UNIT_UID);
//
//	post_code(0x38);
//
//	init_timer(); // Need to use TMICT to synconize FID/VID
//
//	// added setting wants_reset here from ck804
//	wants_reset = ck804_early_setup_x();
//
//	/* Reset for HT, FIDVID, PLL and errata changes to take affect. */
//	if (!warm_reset_detect(0)) {
//		printk(BIOS_INFO, "...WARM RESET...\n\n\n");
//		soft_reset();
//		die("After soft_reset - shouldn't see this message!!!\n");
//	}
//
//	// added debug output for wants_reset
//	if (wants_reset) {
//		printk(BIOS_DEBUG, "ck804_early_setup_x wanted additional reset!\n");
//	}
//
//	// no sr4540_htinit_decl_and_enable_isochronous_link
//	// no DDR default memory voltage, lpc_stick_ctl, s3resume ddr3_voltage
//
//	// no peripheral control lines
//
//	post_code(0x3B);
//
//	/* It's the time to set ctrl in sysinfo now; */
//	// does not report sysinfo->nodes as if fill_mem_ctrl returned it
//	printk(BIOS_DEBUG, "fill_mem_ctrl()\n");
//	fill_mem_ctrl(sysinfo->nodes, sysinfo->ctrl, spd_addr);
//	post_code(0x3D);
//
//	printk(BIOS_DEBUG, "enable_smbus()\n");
//	enable_smbus();
//
//#if 0
//	/* FIXME
//	 * After the AMD K10 code has been converted to use
//	 * CONFIG(DEBUG_SMBUS) uncomment this block
//	 */
//	if (CONFIG(DEBUG_SMBUS)) {
//		dump_spd_registers(&cpu[0]);
//		dump_smbus_registers();
//	}
//#endif
//
//	post_code(0x40);
//
//	raminit_amdmct(sysinfo);
//
//	// no execute_memory_test
//
//	// no s3resume handling
//	cbmem_initialize_empty();
//	post_code(0x41);
//
//	amdmct_cbmem_store_info(sysinfo);
//
//	printk(BIOS_DEBUG, "disable_spd()\n");
//	// changed spd_mux stuff to ck804
//	ck804_control(ctrl_conf_disable_spd, ARRAY_SIZE(ctrl_conf_disable_spd), CK804_DEVN_BASE);
//
//	// added msi_mapping instead of _before_pci_init
//	printk(BIOS_DEBUG, "enable_msi_mapping()\n");
//	ck804_control(ctrl_conf_enable_msi_mapping, ARRAY_SIZE(ctrl_conf_enable_msi_mapping), CK804_DEVN_BASE);
//
//	// changed pci_write_config16 for SP5100 GPIOs to pnp_write_config for SuperIO gpios
//	/* Initialize GPIO */
//	/* Access SuperIO GPI03 logical device */
//	pnp_enter_conf_state(GPIO3_DEV);
//	pnp_set_logical_device(GPIO3_DEV);
//	/* Set GP37 (power LED) to output */
//	pnp_write_config(GPIO3_DEV, 0xf0, 0x7f);
//	/* Set GP37 (power LED) on */
//	pnp_write_config(GPIO3_DEV, 0xf1, 0x80);
//	/* Set pin 64 multiplex to GP37 */
//	uint8_t cr2c = pnp_read_config(GPIO3_DEV, 0x2c);
//	pnp_write_config(GPIO3_DEV, 0x2c, (cr2c & 0xf3) | 0x04);
//	/* Restore default SuperIO access */
//	pnp_exit_conf_state(GPIO3_DEV);
//}

/**
 * BOOL AMD_CB_ManualBUIDSwapList(u8 Node, u8 Link, u8 **List)
 * Description:
 *	This routine is called every time a non-coherent chain is processed.
 *	BUID assignment may be controlled explicitly on a non-coherent chain. Provide a
 *	swap list. The first part of the list controls the BUID assignment and the
 *	second part of the list provides the device to device linking.  Device orientation
 *	can be detected automatically, or explicitly.  See documentation for more details.
 *
 *	Automatic non-coherent init assigns BUIDs starting at 1 and incrementing sequentially
 *	based on each device's unit count.
 *
 * Parameters:
 *	@param[in]  node   = The node on which this chain is located
 *	@param[in]  link   = The link on the host for this chain
 *	@param[out] List   = supply a pointer to a list
 */
BOOL AMD_CB_ManualBUIDSwapList (u8 node, u8 link, const u8 **List)
{
	return 0;
}
