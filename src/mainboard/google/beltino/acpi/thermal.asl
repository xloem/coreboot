/*
 * This file is part of the coreboot project.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

External (\PPKG, MethodObj)

#include <variant/thermal.h>

// Thermal Zone

#define HAVE_THERMALZONE
Scope (\_TZ)
{
	ThermalZone (THRM)
	{
		Name (_TC1, 0x02)
		Name (_TC2, 0x05)

		// Thermal zone polling frequency: 10 seconds
		Name (_TZP, 100)

		// Thermal sampling period for passive cooling: 2 seconds
		Name (_TSP, 20)

		// Convert from Degrees C to 1/10 Kelvin for ACPI
		Method (CTOK, 1) {
			// 10th of Degrees C
			Multiply (Arg0, 10, Local0)

			// Convert to Kelvin
			Add (Local0, 2732, Local0)

			Return (Local0)
		}

		// Threshold for OS to shutdown
		Method (_CRT, 0, Serialized)
		{
			Return (CTOK (\TCRT))
		}

		// Threshold for passive cooling
		Method (_PSV, 0, Serialized)
		{
			Return (CTOK (\TPSV))
		}

		// Processors used for passive cooling
		Method (_PSL, 0, Serialized)
		{
			Return (\PPKG ())
		}

		// Start fan at state 4 = lowest temp state
		Method (_INI)
		{
			Store (4, \FLVL)
			Store (FAN4_PWM, \_SB.PCI0.LPCB.SIO.ENVC.F2PS)
			Notify (\_TZ.THRM, 0x81)
		}

		Method (TCHK, 0, Serialized)
		{
			// Get CPU Temperature from PECI via SuperIO TMPIN3
			Store (\_SB.PCI0.LPCB.SIO.ENVC.TIN3, Local0)

			// Check for "no reading available"
			If (LEqual (Local0, 0x80)) {
				Return (CTOK (FAN0_THRESHOLD_ON))
			}

			// Check for invalid readings
			If (LOr (LEqual (Local0, 255), LEqual (Local0, 0))) {
				Return (CTOK (FAN0_THRESHOLD_ON))
			}

			// PECI raw value is an offset from Tj_max
			Subtract (255, Local0, Local1)

			// Handle values greater than Tj_max
			If (LGreaterEqual (Local1, \TMAX)) {
				Return (CTOK (\TMAX))
			}

			// Subtract from Tj_max to get temperature
			Subtract (\TMAX, Local1, Local0)
			Return (CTOK (Local0))
		}

		Method (_TMP, 0, Serialized)
		{
			// Get temperature from SuperIO in deci-kelvin
			Store (TCHK (), Local0)

			// Critical temperature in deci-kelvin
			Store (CTOK (\TMAX), Local1)

			If (LGreaterEqual (Local0, Local1)) {
				Store ("CRITICAL TEMPERATURE", Debug)
				Store (Local0, Debug)

				// Wait 1 second for SuperIO to re-poll
				Sleep (1000)

				// Re-read temperature from SuperIO
				Store (TCHK (), Local0)

				Store ("RE-READ TEMPERATURE", Debug)
				Store (Local0, Debug)
			}

			Return (Local0)
		}

		Method (_AC0) {
			If (LLessEqual (\FLVL, 0)) {
				Return (CTOK (FAN0_THRESHOLD_OFF))
			} Else {
				Return (CTOK (FAN0_THRESHOLD_ON))
			}
		}

		Method (_AC1) {
			If (LLessEqual (\FLVL, 1)) {
				Return (CTOK (FAN1_THRESHOLD_OFF))
			} Else {
				Return (CTOK (FAN1_THRESHOLD_ON))
			}
		}

		Method (_AC2) {
			If (LLessEqual (\FLVL, 2)) {
				Return (CTOK (FAN2_THRESHOLD_OFF))
			} Else {
				Return (CTOK (FAN2_THRESHOLD_ON))
			}
		}

		Method (_AC3) {
			If (LLessEqual (\FLVL, 3)) {
				Return (CTOK (FAN3_THRESHOLD_OFF))
			} Else {
				Return (CTOK (FAN3_THRESHOLD_ON))
			}
		}

		Method (_AC4) {
			If (LLessEqual (\FLVL, 4)) {
				Return (CTOK (FAN4_THRESHOLD_OFF))
			} Else {
				Return (CTOK (FAN4_THRESHOLD_ON))
			}
		}

		Name (_AL0, Package () { FAN0 })
		Name (_AL1, Package () { FAN1 })
		Name (_AL2, Package () { FAN2 })
		Name (_AL3, Package () { FAN3 })
		Name (_AL4, Package () { FAN4 })

		PowerResource (FNP0, 0, 0)
		{
			Method (_STA) {
				If (LLessEqual (\FLVL, 0)) {
					Return (One)
				} Else {
					Return (Zero)
				}
			}
			Method (_ON)  {
				If (LNot (_STA ())) {
					Store (0, \FLVL)
					Store (FAN0_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
			Method (_OFF) {
				If (_STA ()) {
					Store (1, \FLVL)
					Store (FAN1_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
		}

		PowerResource (FNP1, 0, 0)
		{
			Method (_STA) {
				If (LLessEqual (\FLVL, 1)) {
					Return (One)
				} Else {
					Return (Zero)
				}
			}
			Method (_ON)  {
				If (LNot (_STA ())) {
					Store (1, \FLVL)
					Store (FAN1_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
			Method (_OFF) {
				If (_STA ()) {
					Store (2, \FLVL)
					Store (FAN2_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
		}

		PowerResource (FNP2, 0, 0)
		{
			Method (_STA) {
				If (LLessEqual (\FLVL, 2)) {
					Return (One)
				} Else {
					Return (Zero)
				}
			}
			Method (_ON)  {
				If (LNot (_STA ())) {
					Store (2, \FLVL)
					Store (FAN2_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
			Method (_OFF) {
				If (_STA ()) {
					Store (3, \FLVL)
					Store (FAN3_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
		}

		PowerResource (FNP3, 0, 0)
		{
			Method (_STA) {
				If (LLessEqual (\FLVL, 3)) {
					Return (One)
				} Else {
					Return (Zero)
				}
			}
			Method (_ON)  {
				If (LNot (_STA ())) {
					Store (3, \FLVL)
					Store (FAN3_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
			Method (_OFF) {
				If (_STA ()) {
					Store (4, \FLVL)
					Store (FAN4_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
		}

		PowerResource (FNP4, 0, 0)
		{
			Method (_STA) {
				If (LLessEqual (\FLVL, 4)) {
					Return (One)
				} Else {
					Return (Zero)
				}
			}
			Method (_ON)  {
				If (LNot (_STA ())) {
					Store (4, \FLVL)
					Store (FAN4_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
			Method (_OFF) {
				If (_STA ()) {
					Store (4, \FLVL)
					Store (FAN4_PWM,
						\_SB.PCI0.LPCB.SIO.ENVC.F2PS)
					Notify (\_TZ.THRM, 0x81)
				}
			}
		}

		Device (FAN0)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_UID, 0)
			Name (_PR0, Package () { FNP0 })
		}

		Device (FAN1)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_UID, 1)
			Name (_PR0, Package () { FNP1 })
		}

		Device (FAN2)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_UID, 2)
			Name (_PR0, Package () { FNP2 })
		}

		Device (FAN3)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_UID, 3)
			Name (_PR0, Package () { FNP3 })
		}

		Device (FAN4)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_UID, 4)
			Name (_PR0, Package () { FNP4 })
		}
	}
}
