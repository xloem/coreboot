/* Initially copied from System76: src/mainboard/system76/cml-u/acpi */
/* SPDX-License-Identifier: GPL-2.0-only */
/* This file is part of the coreboot project. */

Device (LID0)
{
	Name (_HID, EisaId ("PNP0C0D"))
	Name (_PRW, Package () { EC_GPE_SWI, 3 })

	Method (_LID, 0, NotSerialized) {
		DEBUG = "LID: _LID"
		If (^^PCI0.LPCB.EC0.ECOK) {
			Return (^^PCI0.LPCB.EC0.LSTE)
		} Else {
			Return (One)
		}
	}

	Method (_PSW, 1, NotSerialized) {
		DEBUG = Concatenate("LID: _PSW: ", ToHexString(Arg0))
		If (^^PCI0.LPCB.EC0.ECOK) {
			^^PCI0.LPCB.EC0.LWKE = Arg0
		}
	}
}
