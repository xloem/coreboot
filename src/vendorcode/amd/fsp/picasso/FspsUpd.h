/** @file
 *
 * This file is automatically generated.
 *
 */

#ifndef __FSPSUPD_H__
#define __FSPSUPD_H__

#include <FspUpd.h>

#define FSPS_UPD_DXIO_DESCRIPTOR_COUNT	8
#define FSPS_UPD_DDI_DESCRIPTOR_COUNT	4

typedef struct __packed {
	/** Offset 0x0020**/	uint32_t                    emmc0_mode;
	/** Offset 0x0024**/	uint8_t                     unused0[12];
	/** Offset 0x0030**/	uint8_t                     dxio_descriptor[FSPS_UPD_DXIO_DESCRIPTOR_COUNT][16];
	/** Offset 0x00B0**/	uint8_t                     unused1[16];
	/** Offset 0x00C0**/	uint32_t                    ddi_descriptor[FSPS_UPD_DDI_DESCRIPTOR_COUNT];
	/** Offset 0x00D0**/	uint8_t                     unused2[16];
	/** Offset 0x00E0**/	uint8_t                     fch_usb_version_major;
	/** Offset 0x00E1**/	uint8_t                     fch_usb_version_minor;
	/** Offset 0x00E2**/	uint8_t                     fch_usb_2_port0_phy_tune[9];
	/** Offset 0x00EB**/	uint8_t                     fch_usb_2_port1_phy_tune[9];
	/** Offset 0x00F4**/	uint8_t                     fch_usb_2_port2_phy_tune[9];
	/** Offset 0x00FD**/	uint8_t                     fch_usb_2_port3_phy_tune[9];
	/** Offset 0x0106**/	uint8_t                     fch_usb_2_port4_phy_tune[9];
	/** Offset 0x010F**/	uint8_t                     fch_usb_2_port5_phy_tune[9];
	/** Offset 0x0118**/	uint8_t                     fch_usb_device_removable;
	/** Offset 0x0119**/	uint8_t                     fch_usb_3_port_force_gen1;
	/** Offset 0x011A**/	uint8_t                     fch_usb_u3_rx_det_wa_enable;
	/** Offset 0x011B**/	uint8_t                     fch_usb_u3_rx_det_wa_portmap;
	/** Offset 0x011C**/	uint8_t                     fch_usb_early_debug_select_enable;
	/** Offset 0x011D**/	uint8_t                     unused3;
	/** Offset 0x011E**/	uint32_t                    xhci_oc_pin_select;
	/** Offset 0x0122**/	uint8_t                     xhci0_force_gen1;
	/** Offset 0x0123**/	uint8_t                     UnusedUpdSpace0[45];
	/** Offset 0x0150**/	uint16_t                    UpdTerminator;
} FSP_S_CONFIG;

/** Fsp S UPD Configuration
**/
typedef struct __packed {
	/** Offset 0x0000**/	FSP_UPD_HEADER              FspUpdHeader;
	/** Offset 0x0020**/	FSP_S_CONFIG                FspsConfig;
} FSPS_UPD;

#endif
