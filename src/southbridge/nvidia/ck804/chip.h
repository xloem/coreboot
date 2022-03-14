/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef SOUTHBRIDGE_NVIDIA_CK804_CHIP_H
#define SOUTHBRIDGE_NVIDIA_CK804_CHIP_H

struct southbridge_nvidia_ck804_config {
	unsigned int usb1_hc_reset : 1;
	unsigned int ide0_enable : 1;
	unsigned int ide1_enable : 1;
	unsigned int sata0_enable : 1;
	unsigned int sata1_enable : 1;
	unsigned int mac_eeprom_smbus;
	unsigned int mac_eeprom_addr;
};

/*
 * Porting.h included an open #pragma pack(1) that broke things in hard to debug
 * ways. This struct is one of the examples, and while it won't catch all
* problems caused by that #pragma, hopefully it can reduce time wasted on
 * debugging in wrong places.
 */
_Static_assert(sizeof(struct southbridge_nvidia_ck804_config) == 12,
               "check for bad #pragmas");


extern struct pci_operations ck804_pci_ops;

#endif
