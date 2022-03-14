/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * ACPI - create the Fixed ACPI Description Tables (FADT)
 */

#include <acpi/acpi.h>
#include <console/console.h>
#include <cpu/x86/smm.h>
#include <cpu/amd/common/common.h>

extern unsigned int pm_base;	/* pm_base should be set in sb acpi */

void acpi_fill_fadt(acpi_fadt_t * fadt)
{
	// 3=Workstation,4=Enterprise Server, 7=Performance Server
	fadt->preferred_pm_profile = 0;
	fadt->sci_int = 9;
	// disable system management mode by setting to 0:
	if (!CONFIG(HAVE_SMI_HANDLER))
		fadt->smi_cmd = 0;
	else
		fadt->smi_cmd = APM_CNT;
	fadt->acpi_enable = APM_CNT_ACPI_ENABLE;
	fadt->acpi_disable = APM_CNT_ACPI_DISABLE;
	fadt->s4bios_req = 0;
	fadt->pstate_cnt = 0;

	fadt->pm1a_evt_blk = pm_base;
	fadt->pm1b_evt_blk = 0x0000;
	fadt->pm1a_cnt_blk = pm_base + 0x04;
	fadt->pm1b_cnt_blk = 0x0000;
	fadt->pm2_cnt_blk = pm_base + 0x1c;
	fadt->pm_tmr_blk = pm_base + 0x08;
	fadt->gpe0_blk = pm_base + 0x20;
	fadt->gpe1_blk = 0x0000;

	fadt->pm1_evt_len = 4;
	fadt->pm1_cnt_len = 2;
	fadt->pm2_cnt_len = 1;
	fadt->pm_tmr_len = 4;
	fadt->gpe0_blk_len = 8;
	fadt->gpe1_blk_len = 0;
	fadt->gpe1_base = 0;
	fadt->cst_cnt = 0;
	fadt->p_lvl2_lat = 0xffff;
	fadt->p_lvl3_lat = 0xffff;
	fadt->flush_size = 0;
	fadt->flush_stride = 0;
	fadt->duty_offset = 1;
	fadt->duty_width = 3;
	fadt->day_alrm = 0x7d;
	fadt->mon_alrm = 0x7e;
	fadt->century = 0x32;
	fadt->iapc_boot_arch = ACPI_FADT_LEGACY_FREE;
	fadt->flags = 0xa5; // kgpe-d16 changed these to compiler defines. is this ACPI_FADT_WBINVD, ACPI_FADT_C1_SUPPORTED, and (1<<32) ?

#ifdef LONG_FADT
	fadt->x_pm1a_evt_blk.space_id = 1;
	fadt->x_pm1a_evt_blk.bit_width = 32;
	fadt->x_pm1a_evt_blk.bit_offset = 0;
	fadt->x_pm1a_evt_blk.access_size = 0;
	fadt->x_pm1a_evt_blk.addrl = pm_base;
	fadt->x_pm1a_evt_blk.addrh = 0x0;

	fadt->x_pm1b_evt_blk.space_id = 1;
	fadt->x_pm1b_evt_blk.bit_width = 4;
	fadt->x_pm1b_evt_blk.bit_offset = 0;
	fadt->x_pm1b_evt_blk.access_size = 0;
	fadt->x_pm1b_evt_blk.addrl = 0x0;
	fadt->x_pm1b_evt_blk.addrh = 0x0;

	fadt->x_pm1a_cnt_blk.space_id = 1;
	fadt->x_pm1a_cnt_blk.bit_width = 16;
	fadt->x_pm1a_cnt_blk.bit_offset = 0;
	fadt->x_pm1a_cnt_blk.access_size = 0;
	fadt->x_pm1a_cnt_blk.addrl = pm_base + 4;
	fadt->x_pm1a_cnt_blk.addrh = 0x0;

	fadt->x_pm1b_cnt_blk.space_id = 1;
	fadt->x_pm1b_cnt_blk.bit_width = 2;
	fadt->x_pm1b_cnt_blk.bit_offset = 0;
	fadt->x_pm1b_cnt_blk.access_size = 0;
	fadt->x_pm1b_cnt_blk.addrl = 0x0;
	fadt->x_pm1b_cnt_blk.addrh = 0x0;

	fadt->x_pm2_cnt_blk.space_id = 1;
	fadt->x_pm2_cnt_blk.bit_width = 0;
	fadt->x_pm2_cnt_blk.bit_offset = 0;
	fadt->x_pm2_cnt_blk.access_size = 0;
	fadt->x_pm2_cnt_blk.addrl = 0x0;
	fadt->x_pm2_cnt_blk.addrh = 0x0;

	fadt->x_pm_tmr_blk.space_id = 1;
	fadt->x_pm_tmr_blk.bit_width = 32;
	fadt->x_pm_tmr_blk.bit_offset = 0;
	fadt->x_pm_tmr_blk.access_size = 0;
	fadt->x_pm_tmr_blk.addrl = pm_base + 0x08;
	fadt->x_pm_tmr_blk.addrh = 0x0;

	fadt->x_gpe0_blk.space_id = 1;
	fadt->x_gpe0_blk.bit_width = 32;
	fadt->x_gpe0_blk.bit_offset = 0;
	fadt->x_gpe0_blk.access_size = 0;
	fadt->x_gpe0_blk.addrl = pm_base + 0x20;
	fadt->x_gpe0_blk.addrh = 0x0;

	fadt->x_gpe1_blk.space_id = 1;
	fadt->x_gpe1_blk.bit_width = 64;
	fadt->x_gpe1_blk.bit_offset = 16;
	fadt->x_gpe1_blk.access_size = 0;
	fadt->x_gpe1_blk.addrl = pm_base + 0xb0;
	fadt->x_gpe1_blk.addrh = 0x0;
#endif
}

#if CONFIG(HAVE_ACPI_TABLES)

unsigned long acpi_fill_mcfg(unsigned long current)
{
	struct device *dev;
	// i believe the updated sb700 code from kgpe-d16 uses a compile-time define rather than mcfg_base here
	unsigned long mcfg_base;

	dev = pcidev_on_root(0x0, 0);
	if (!dev)
		return current;

	mcfg_base = pci_read_config16(dev, 0x90);
	if ((mcfg_base & 0x1000) == 0)
		return current;

	mcfg_base = (mcfg_base & 0xf) << 28;

	printk(BIOS_INFO, "mcfg_base %lx.\n", mcfg_base);

	current += acpi_create_mcfg_mmconfig((acpi_mcfg_mmconfig_t *)
			current, mcfg_base, 0x0, 0x0, 0xff);
	return current;
}

#endif
