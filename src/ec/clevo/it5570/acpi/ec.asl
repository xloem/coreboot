/* SPDX-License-Identifier: GPL-2.0-only */

Device (EC0)
{
	Name (_HID, EisaId ("PNP0C09") /* Embedded Controller Device */)  // _HID: Hardware ID
	Name (_GPE, EC_GPE_SCI)  // _GPE: General Purpose Events
	Name (_CRS, ResourceTemplate ()  // _CRS: Current Resource Settings
	{
		IO (Decode16,
			0x0062,             // Range Minimum
			0x0062,             // Range Maximum
			0x00,               // Alignment
			0x01,               // Length
			)
		IO (Decode16,
			0x0066,             // Range Minimum
			0x0066,             // Range Maximum
			0x00,               // Alignment
			0x01,               // Length
			)
		IO (Decode16,
			0x002e,             // Range Minimum
			0x002e,             // Range Maximum
			0x00,               // Alignment
			0x02,               // Length
			)
	})

	#include "ec_ram.asl"

	Name (ECOK, Zero)
	Method (_REG, 2, Serialized)  // _REG: Region Availability
	{
		Debug = Concatenate("EC: _REG", Concatenate(ToHexString(Arg0), Concatenate(" ", ToHexString(Arg1))))
		If (((Arg0 == 0x03) && (Arg1 == One))) {
			// Enable hardware touchpad lock and keyboard backlight keys
			// Enable software airplane mode key
			ECOS = 2

			// Enable software display brightness keys
			WINF = 1

			// Enable camera toggle hotkey
			OEM3 = OEM3 | 4

			// Set current AC state
			\_SB.AC.ACFG = ADP
			// Update battery information and status
			\_SB.BAT0.UPBI ()
			\_SB.BAT0.UPBS ()

			PNOT ()

			// Initialize UCSI
			^UCSI.INIT ()

			// Apply custom fan curve
			SFCV ()

			// EC is now available
			ECOK = Arg1
		}
	}

	Method (UPB, 0, Serialized) {
		Debug = "EC: UPB"
		// Set current AC state
		\_SB.AC.ACFG = ADP

		// Update battery information and status
		\_SB.BAT0.UPBI ()
		\_SB.BAT0.UPBS ()

		// Notify of changes
		Notify(\_SB.AC, Zero)
		Notify(\_SB.BAT0, Zero)

		Sleep (1000)
	}

	Method (PTS, 1, Serialized) {
		Debug = Concatenate("EC: PTS: ", ToHexString(Arg0))
		If (ECOK) {
			// Clear wake cause
			WFNO = Zero
		}
	}

	Method (WAK, 1, Serialized) {
		Debug = Concatenate("EC: WAK: ", ToHexString(Arg0))
		If (ECOK) {
			UPB ()
			SFCV ()
		}
	}

	Method (I2ER, 1, NotSerialized)
	{
		SIOI = 0x2e
		SIOD = 0x11
		SIOI = 0x2f
		SIOD = ((Arg0 >> 8) & 0xff)
		SIOI = 0x2e
		SIOD = 0x10
		SIOI = 0x2f
		SIOD = (Arg0 & 0xff)
		SIOI = 0x2e
		SIOD = 0x12
		SIOI = 0x2f
		Local0 = SIOD

		Return (Local0)
	}

	Method (I2EW, 2, NotSerialized)
	{
		SIOI = 0x2e
		SIOD = 0x11
		SIOI = 0x2f
		SIOD = ((Arg0 >> 8) & 0xff)
		SIOI = 0x2e
		SIOD = 0x10
		SIOI = 0x2f
		SIOD = (Arg0 & 0xff)
		SIOI = 0x2e
		SIOD = 0x12
		SIOI = 0x2f

		SIOD = Arg1;
	}

	Method (GKBL, 0, Serialized) // Get Keyboard LED
	{
		Local0 = 0
		If (ECOK) {
			FDAT = One
			FCMD = 0xCA
			Local0 = FBUF
			FCMD = Zero
		}
		Return (Local0)
	}

	Method (SKBL, 1, Serialized) // Set Keyboard LED
	{
		If (ECOK) {
			FDAT = Zero
			FBUF = Arg0
			FCMD = 0xCA
		}
	}

	Method (S0IX, 1, Serialized) // S0IX Entry/Exit
	{
		If (Arg0) {
			Debug = "EC: S0IX Enter"
		} Else {
			Debug = "EC: S0IX Exit"
			UPB ()
			SFCV ()
		}

		FDAT = 0xC2
		FBUF = Arg0
		FCMD = 0xD2
		Local0 = 0xFF
		Local1 = 0xFF
		While (Local0 != Arg0 && Local1 > Zero)
		{
			Sleep(50)
			FDAT = 0xC3
			FCMD = 0xD2
			Local3 = FBUF
			If (Local3 == Zero || Local3 == One)
			{
				Local0 = Local3
			}
			Local1 = Local1 - 1
		}
	}

	Method (DGPM, 1, Serialized) // Handle dGPU power state change
	{
		If (ECOK) {
			If (Arg0 == 0x0) {
				AIRP &= 0x7F
				Debug = "EC: DGPU Off"
			} ElseIf (Arg0 == 0x1) {
				AIRP |= 0x80
				Debug = "EC: DGPU On"
			}
		}
	}

	Method (_Q0A, 0, NotSerialized) // Touchpad Toggle
	{
		Debug = "EC: Touchpad Toggle"
	}

	Method (_Q0B, 0, NotSerialized) // Screen Toggle
	{
		Debug = "EC: Screen Toggle"
	}

	Method (_Q0C, 0, NotSerialized)  // Mute
	{
		Debug = "EC: Mute"
	}

	Method (_Q0D, 0, NotSerialized) // Keyboard Backlight
	{
		Debug = "EC: Keyboard Backlight"
	}

	Method (_Q0E, 0, NotSerialized) // Volume Down
	{
		Debug = "EC: Volume Down"
	}

	Method (_Q0F, 0, NotSerialized) // Volume Up
	{
		Debug = "EC: Volume Up"
	}

	Method (_Q10, 0, NotSerialized) // Switch Video Mode
	{
		Debug = "EC: Switch Video Mode"
		If (CondRefOf (\_SB.PCI0.GFX0)) {
			Notify (\_SB.PCI0.GFX0, 0x80)
		}
	}

	Method (_Q11, 0, NotSerialized) // Brightness Down
	{
		Debug = "EC: Brightness Down"

		If (CondRefOf (\_SB.PCI0.GFX0.LCD0)) {
			Notify (\_SB.PCI0.GFX0.LCD0, 0x87)
		}
	}

	Method (_Q12, 0, NotSerialized) // Brightness Up
	{
		Debug = "EC: Brightness Up"

		If (CondRefOf (\_SB.PCI0.GFX0.LCD0)) {
			Notify (\_SB.PCI0.GFX0.LCD0, 0x86)
		}
	}

	Method (_Q13, 0, NotSerialized) // Camera Toggle
	{
		Debug = "EC: Camera Toggle"
	}

	Method (_Q14, 0, NotSerialized) // Airplane Mode
	{
		Debug = "EC: Airplane Mode"

		If (CondRefOf (^HIDD.HPEM))
		{
			^HIDD.HPEM (8)
		}
	}

	Method (_Q15, 0, NotSerialized) // Suspend Button
	{
		Debug = "EC: Suspend Button"
		Notify (SLPB, 0x80)
	}

	Method (_Q16, 0, NotSerialized) // AC Detect
	{
		Debug = "EC: AC Detect"

		If (MSFG)
		{
			FDAT = Zero
			FBUF = 0xC1
			FCMD = 0xD2
			If ((FBUF & 0x80))
			{
				FBUF = Zero
				Return (Zero)
			}
		}

		\_SB.AC.ACFG = ADP
		Notify (AC, 0x80) // Status Change
		Sleep (0x01F4)
		If (BAT0)
		{
			Notify (\_SB.BAT0, 0x81) // Information Change
			Sleep (0x32)
			Notify (\_SB.BAT0, 0x80) // Status Change
			Sleep (0x32)
		}

		Return (Zero)
	}

	Method (_Q17, 0, NotSerialized)  // BAT0 Update
	{
		Debug = "EC: BAT0 Update (17)"
		Notify (\_SB.BAT0, 0x81) // Information Change
	}

	Method (_Q19, 0, NotSerialized)  // BAT0 Update
	{
		Debug = "EC: BAT0 Update (19)"
		Notify (\_SB.BAT0, 0x81) // Information Change
	}

	Method (_Q1B, 0, NotSerialized) // Lid Close
	{
		Debug = "EC: Lid Close"
		Notify (LID0, 0x80)
	}

	Method (_Q1C, 0, NotSerialized) // Thermal Trip
	{
		Debug = "EC: Thermal Trip"
		/* TODO
		Notify (\_TZ.TZ0, 0x81) // Thermal Trip Point Change
		Notify (\_TZ.TZ0, 0x80) // Thermal Status Change
		*/
	}

	Method (_Q1D, 0, NotSerialized) // Power Button
	{
		Debug = "EC: Power Button"
		Notify (PWRB, 0x80)
	}

	Method (_Q50, 0, NotSerialized) // Other Events
	{
		Local0 = OEM4
		If (Local0 == 0x8A) {
			Debug = "EC: White Keyboard Backlight"
		} ElseIf (Local0 == 0x9F) {
			Debug = "EC: Color Keyboard Toggle"
		} ElseIf (Local0 == 0x81) {
			Debug = "EC: Color Keyboard Down"
		} ElseIf (Local0 == 0x82) {
			Debug = "EC: Color Keyboard Up"
		} ElseIf (Local0 == 0x80) {
			Debug = "EC: Color Keyboard Color Change"
		} Else {
			Debug = Concatenate("EC: Other: ", ToHexString(Local0))
		}
	}

	Method (_Q62, 0, NotSerialized)  // UCSI event
	{
		Debug = "EC: UCSI Event"
		^UCSI.MGI0 = MGI0
		^UCSI.MGI1 = MGI1
		^UCSI.MGI2 = MGI2
		^UCSI.MGI3 = MGI3
		^UCSI.MGI4 = MGI4
		^UCSI.MGI5 = MGI5
		^UCSI.MGI6 = MGI6
		^UCSI.MGI7 = MGI7
		^UCSI.MGI8 = MGI8
		^UCSI.MGI9 = MGI9
		^UCSI.MGIA = MGIA
		^UCSI.MGIB = MGIB
		^UCSI.MGIC = MGIC
		^UCSI.MGID = MGID
		^UCSI.MGIE = MGIE
		^UCSI.MGIF = MGIF
		^UCSI.CCI0 = CCI0
		^UCSI.CCI1 = CCI1
		^UCSI.CCI2 = CCI2
		^UCSI.CCI3 = CCI3
		CCI0 = Zero
		CCI3 = Zero
		Notify (^UCSI, 0x80)
	}

	Method (SFCV, 0, NotSerialized) // Set custom fan curve
	{
		// Fan curve: Temperature in deg C, PWM duty cycle 0-255
		// Fan ramps: (duty2 - duty1) / (temp2 - temp1) * 2.55 * 16.0

		// 40 degrees / 16% - The fan starts spinning at this temperature
		// 60 degrees / 25%
		// 70 degrees / 50%
		// 80 degrees / 100%

		// Fan 1 curve
		P1F1 = 0x28
		P1D1 = 0x28
		P2F1 = 0x3c
		P2D1 = 0x3f
		P3F1 = 0x46
		P3D1 = 0x7f
		P4F1 = 0x50
		P4D1 = 0xff

		// Fan 1 ramps
		SH11 = 0x0
		SL11 = 0x12
		SH12 = 0x0
		SL12 = 0x66
		SH13 = 0x0
		SL13 = 0xcc

		// Fan 2 curve
		P1F2 = 0x28
		P1D2 = 0x28
		P2F2 = 0x3c
		P2D2 = 0x3f
		P3F2 = 0x46
		P3D2 = 0x7f
		P4F2 = 0x50
		P4D2 = 0xff

		// Fan 2 ramps
		SH21 = 0x0
		SL21 = 0x12
		SH22 = 0x0
		SL22 = 0x66
		SH23 = 0x0
		SL23 = 0xcc

		// Apply custom fan mode
		FDAT = 0x04
		FCMD = 0xD7
	}

	// Get temperature
	Method (GTMP, 1, Serialized) {
		FDAT = One
		FCMD = 0xC0
		Debug = FBF1
		Debug = FBUF
		Debug = FDAT
	}

	#include "hid.asl"
	#include "ucsi.asl"
}
