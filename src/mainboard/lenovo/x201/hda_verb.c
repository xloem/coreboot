/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 Vladimir Serbinenko.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <device/azalia_device.h>

const u32 cim_verb_data[] = {
	0x14f15069,	/* Codec Vendor / Device ID: Conexant CX20585 */
	0x17aa2155,	/* Subsystem ID  */
	11,		/* Number of 4 dword sets */
	AZALIA_SUBVENDOR(0x0, 0x17aa2155),
	AZALIA_PIN_CFG(0x0, 0x19, 0x042140f0),	/* Headphone jack */
	AZALIA_PIN_CFG(0x0, 0x1a, 0x61a190f0),	/* Dock mic jack */
	AZALIA_PIN_CFG(0x0, 0x1b, 0x04a190f0),	/* Mic jack */
	AZALIA_PIN_CFG(0x0, 0x1c, 0x612140f0),	/* Dock headphone jack */
	AZALIA_PIN_CFG(0x0, 0x1d, 0x601700f0),	/* EAPD detect */
	AZALIA_PIN_CFG(0x0, 0x1e, 0x40f001f0),
	AZALIA_PIN_CFG(0x0, 0x1f, 0x901701f0),
	AZALIA_PIN_CFG(0x0, 0x20, 0x40f001f0),
	AZALIA_PIN_CFG(0x0, 0x22, 0x40f001f0),
	AZALIA_PIN_CFG(0x0, 0x23, 0x90a601f0),	/* Internal mic boost volume */

	0x80862804,	/* Codec Vendor / Device ID: Intel Ibexpeak HDMI.  */
	0x17aa21b5,	/* Subsystem ID  */
	4,		/* Number of 4 dword sets */
	AZALIA_SUBVENDOR(0x3, 0x17aa21b5),
	AZALIA_PIN_CFG(0x3, 0x04, 0x58560010),
	AZALIA_PIN_CFG(0x3, 0x05, 0x18560020),
	AZALIA_PIN_CFG(0x3, 0x06, 0x58560030),
};

const u32 pc_beep_verbs[0] = {};

AZALIA_ARRAY_SIZES;
