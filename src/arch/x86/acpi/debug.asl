/* SPDX-License-Identifier: GPL-2.0-only */

/*
* 0x80: POST_BASE
* 0x3F8: DEBCOM_BASE
* X80: POST_REGION
* P80: PORT80
*
* CREG: DEBCOM_REGION
* CUAR: DEBCOM_UART
* CDAT: DEBCOM_DATA
* CDLM: DEBCOM_DLM
* DLCR: DEBCOM_LCR
* CMCR: DEBCOM_MCR
* CLSR: DEBCOM_LSR
*
* DEBUG_INIT	DINI
*/

OperationRegion(X80, SystemIO, 0x80, 1)
	Field(X80, ByteAcc, NoLock, Preserve)
{
	P80, 8
}

OperationRegion(CREG, SystemIO, 0x3F8, 8)
	Field(CREG, ByteAcc, NoLock, Preserve)
{
	CDAT, 8,
	CDLM, 8,, 8, DLCR, 8, CMCR, 8, CLSR, 8
}

/*
* DINI
* Initialize the COM port to 115,200 8-N-1
*/
Method(DINI)
{
	store(0x83, DLCR)
	store(0x01, CDAT)	/* 115200 baud (low) */
	store(0x00, CDLM)	/* 115200 baud (high) */
	store(0x03, DLCR)	/* word=8 stop=1 parity=none */
	store(0x03, CMCR)	/* DTR=1 RTS=1 Out2=Off Loop=Off */
	store(0x00, CDLM)	/* turn off interrupts */
}

/*
* THRE
* Wait for COM port transmitter holding register to go empty
*/
Method(THRE)
{
	and(CLSR, 0x20, local0)
	while (local0 == 0) {
		and(CLSR, 0x20, local0)
	}
}

/*
* OUTX
* Send a single raw character
*/
Method(OUTX, 1)
{
	THRE()
	store(Arg0, CDAT)
}

/*
* OUTC
* Send a single character, expanding LF into CR/LF
*/
Method(OUTC, 1)
{
	if (Arg0 == 0x0a) {
		OUTX(0x0d)
	}
	OUTX(Arg0)
}

/*
* DBGN
* Send a single hex nibble
*/
Method(DBGN, 1)
{
	and(Arg0, 0x0f, Local0)
	if (LLess(Local0, 10)) {
		add(Local0, 0x30, Local0)
	} else {
		add(Local0, 0x37, Local0)
	}
	OUTC(Local0)
}

/*
* DBGB
* Send a hex byte
*/
Method(DBGB, 1)
{
	ShiftRight(Arg0, 4, Local0)
	DBGN(Local0)
	DBGN(Arg0)
}

/*
* DBGW
* Send a hex word
*/
Method(DBGW, 1)
{
	ShiftRight(Arg0, 8, Local0)
	DBGB(Local0)
	DBGB(Arg0)
}

/*
* DBGD
* Send a hex Dword
*/
Method(DBGD, 1)
{
	ShiftRight(Arg0, 16, Local0)
	DBGW(Local0)
	DBGW(Arg0)
}

/*
* DBGO
* Send either a string or an integer
*/
Method(DBGO, 1)
{
	/* DINI() */
	if (ObjectType(Arg0) == 1) {
		if (LGreater(Arg0, 0xffff)) {
			DBGD(Arg0)
		} else {
			if (LGreater(Arg0, 0xff)) {
				DBGW(Arg0)
			} else {
				DBGB(Arg0)
			}
		}
	} else {
		Name(BDBG, Buffer(80) {})
		store(Arg0, BDBG)
		store(0, Local1)
		while (One) {
			store(GETC(BDBG, Local1), Local0)
			if (Local0 == 0) {
				return (0)
			}
			OUTC(Local0)
			Local1++
		}
	}
	return (0)
}

/* Get a char from a string */
Method(GETC, 2)
{
	CreateByteField(Arg0, Arg1, DBGC)
	return (DBGC)
}
