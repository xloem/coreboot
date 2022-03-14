/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <assert.h>
#include <acpi/acpi.h>
#include <device/pci_def.h>
#include <device/pci_ops.h>
#include <arch/smp/mpspec.h>
#include <northbridge/amd/amdfam10/amdfam10.h>

/* APIC */
unsigned long acpi_fill_madt(unsigned long current)
{
	struct device *dev;
	struct resource *res;
	struct amdfam10_sysconf_t *sysconf = get_sysconf();

	/* create all subtables for processors */
	current = acpi_create_madt_lapics(current);

	/* Write NVIDIA CK804 IOAPIC. */
	dev = pcidev_on_root(sysconf->sbdn + 0x1, 0);
	ASSERT(dev != NULL);

	res = find_resource(dev, PCI_BASE_ADDRESS_1);
	ASSERT(res != NULL);

	current += acpi_create_madt_ioapic((acpi_madt_ioapic_t *)current,
		CONFIG_MAX_CPUS * CONFIG_MAX_PHYSICAL_CPUS, res->base, 0);

	/* Initialize interrupt mapping if mptable.c didn't. */
	if (!CONFIG(GENERATE_MP_TABLE)) {
		/* Copied from mptable.c */
		/* Enable interrupts for commonly used devices (USB, SATA, etc.) */
		pci_write_config32(dev, 0x7c, 0x0d800018);
		pci_write_config32(dev, 0x80, 0xd8002009);
		pci_write_config32(dev, 0x84, 0x00000001);
	}

	/* IRQ9 */
	current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
		current, 0, 9, 9, MP_IRQ_TRIGGER_LEVEL | MP_IRQ_POLARITY_HIGH);
	/* IRQ14 */
	current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
		current, 0, 14, 14, MP_IRQ_TRIGGER_EDGE | MP_IRQ_POLARITY_HIGH);
	/* IRQ15 */
	current += acpi_create_madt_irqoverride((acpi_madt_irqoverride_t *)
		current, 0, 15, 15, MP_IRQ_TRIGGER_EDGE | MP_IRQ_POLARITY_HIGH);

	/* create all subtables for processors */
	/* acpi_create_madt_lapic_nmis returns current, not size. */
	current = acpi_create_madt_lapic_nmis(current,
			MP_IRQ_TRIGGER_EDGE | MP_IRQ_POLARITY_HIGH, 1);

	return current;
}
