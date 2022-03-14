/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <device/pci_ops.h>
#include <northbridge/amd/amdfam10/amdfam10.h>
#include "chip.h"

static void ht_init(struct device *dev)
{
	u32 htmsi;

	/* Enable HT MSI Mapping in capability register */
	htmsi = pci_read_config32(dev, 0xe0);
	htmsi |= (1 << 16);
	pci_write_config32(dev, 0xe0, htmsi);
}

static struct device_operations ht_ops = {
	.read_resources   = pci_dev_read_resources,
	.set_resources    = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init             = ht_init,
	.scan_bus         = 0,
	.ops_pci          = &ck804_pci_ops,
};

static const struct pci_driver ht_driver __pci_driver = {
	.ops    = &ht_ops,
	.vendor = PCI_VENDOR_ID_NVIDIA,
	.device = PCI_DEVICE_ID_NVIDIA_CK804_HT,
};
