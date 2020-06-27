/** @file
 *
 * This file is automatically generated.
 *
 */

#ifndef __FSPSUPD_H__
#define __FSPSUPD_H__

#include <FspUpd.h>

#pragma pack(1)


typedef struct {
	/** Offset 0x0020**/	uint32_t                    emmc0_mode;
	/** Offset 0x0024**/	uint8_t                     unused0[12];
	/** Offset 0x0030**/	uint8_t                     dxio_descriptor0[16];
	/** Offset 0x0040**/	uint8_t                     dxio_descriptor1[16];
	/** Offset 0x0050**/	uint8_t                     dxio_descriptor2[16];
	/** Offset 0x0060**/	uint8_t                     dxio_descriptor3[16];
	/** Offset 0x0070**/	uint8_t                     dxio_descriptor4[16];
	/** Offset 0x0080**/	uint8_t                     dxio_descriptor5[16];
	/** Offset 0x0090**/	uint32_t                    ddi_descriptor0;
	/** Offset 0x0094**/	uint32_t                    ddi_descriptor1;
	/** Offset 0x0098**/	uint32_t                    ddi_descriptor2;
	/** Offset 0x009C**/	uint32_t                    ddi_descriptor3;
	/** Offset 0x00A0**/	uint32_t                    unused1;
	/** Offset 0x00A4**/	uint32_t                    unused2;
	/** Offset 0x00A8**/	uint32_t                    unused3;
	/** Offset 0x00AC**/	uint32_t                    unused4;
	/** Offset 0x00B0**/	uint8_t                     fch_usb_version_major;
	/** Offset 0x00B1**/	uint8_t                     fch_usb_version_minor;
	/** Offset 0x00B2**/	uint8_t                     fch_usb_2_port0_phy_tune[9];
	/** Offset 0x00BB**/	uint8_t                     fch_usb_2_port1_phy_tune[9];
	/** Offset 0x00C4**/	uint8_t                     fch_usb_2_port2_phy_tune[9];
	/** Offset 0x00CD**/	uint8_t                     fch_usb_2_port3_phy_tune[9];
	/** Offset 0x00D6**/	uint8_t                     fch_usb_2_port4_phy_tune[9];
	/** Offset 0x00DF**/	uint8_t                     fch_usb_2_port5_phy_tune[9];
	/** Offset 0x00E8**/	uint8_t                     fch_usb_device_removable;
	/** Offset 0x00E9**/	uint8_t                     fch_usb_3_port_force_gen1;
	/** Offset 0x00EA**/	uint8_t                     fch_usb_u3_rx_det_wa_enable;
	/** Offset 0x00EB**/	uint8_t                     fch_usb_u3_rx_det_wa_portmap;
	/** Offset 0x00EC**/	uint8_t                     fch_usb_early_debug_select_enable;
	/** Offset 0x00ED**/	uint8_t                     unused8;
	/** Offset 0x00EE**/	uint32_t                    xhci_oc_pin_select;
	/** Offset 0x00F2**/	uint8_t                     xhci0_force_gen1;
	/** Offset 0x00F3**/	uint8_t                     UnusedUpdSpace0[45];
	/** Offset 0x0120**/	uint16_t                    UpdTerminator;
} FSP_S_CONFIG;

/** Fsp S UPD Configuration
**/
typedef struct {
	/** Offset 0x0000**/	FSP_UPD_HEADER              FspUpdHeader;
	/** Offset 0x0020**/	FSP_S_CONFIG                FspsConfig;
} FSPS_UPD;

#pragma pack()

#endif
