/** @file

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.

  This file is automatically generated. Please do NOT modify !!!

**/

#ifndef __FSPSUPD_H__
#define __FSPSUPD_H__

#include <FspUpd.h>

#pragma pack(1)


///
/// Azalia Header structure
///
typedef struct {
  UINT16 VendorId;           ///< Codec Vendor ID
  UINT16 DeviceId;           ///< Codec Device ID
  UINT8  RevisionId;         ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8  SdiNum;             ///< SDI number, 0xFF matches any SDI.
  UINT16 DataDwords;         ///< Number of data DWORDs pointed by the codec data buffer.
  UINT32 Reserved;           ///< Reserved for future use. Must be set to 0.
} AZALIA_HEADER;

///
/// Audio Azalia Verb Table structure
///
typedef struct {
  AZALIA_HEADER Header;      ///< AZALIA PCH header
  UINT32        *Data;       ///< Pointer to the data buffer. Its length is specified in the header
} AUDIO_AZALIA_VERB_TABLE;

///
/// Refer to the definition of PCH_INT_PIN
///
typedef enum {
  SiPchNoInt,        ///< No Interrupt Pin
  SiPchIntA,
  SiPchIntB,
  SiPchIntC,
  SiPchIntD
} SI_PCH_INT_PIN;
///
/// The PCH_DEVICE_INTERRUPT_CONFIG block describes interrupt pin, IRQ and interrupt mode for PCH device.
///
typedef struct {
  UINT8        Device;                  ///< Device number
  UINT8        Function;                ///< Device function
  UINT8        IntX;                    ///< Interrupt pin: INTA-INTD (see SI_PCH_INT_PIN)
  UINT8        Irq;                     ///< IRQ to be set for device.
} SI_PCH_DEVICE_INTERRUPT_CONFIG;

#define SI_PCH_MAX_DEVICE_INTERRUPT_CONFIG  64       ///< Number of all PCH devices


/** Fsp S Configuration
**/
typedef struct {

/** Offset 0x0020 - Reserved
**/
  UINT8                      Reserved0[16];

/** Offset 0x0030 - Graphics Configuration Ptr
  Points to VBT
**/
  UINT32                      GraphicsConfigPtr;

/** Offset 0x0034 - Enable Device 4
  Enable/disable Device 4
  $EN_DIS
**/
  UINT8                       Device4Enable;

/** Offset 0x0035 - Reserved
**/
  UINT8                       Reserved1[12];

/** Offset 0x0041 - Enable SATA SALP Support
  Enable/disable SATA Aggressive Link Power Management.
  $EN_DIS
**/
  UINT8                       SataSalpSupport;

/** Offset 0x0042 - Enable SATA ports
  Enable/disable SATA ports. One byte for each port, byte0 for port0, byte1 for port1,
  and so on.
**/
  UINT8                       SataPortsEnable[8];

/** Offset 0x004A - Enable SATA DEVSLP Feature
  Enable/disable SATA DEVSLP per port. 0 is disable, 1 is enable. One byte for each
  port, byte0 for port0, byte1 for port1, and so on.
**/
  UINT8                       SataPortsDevSlp[8];

/** Offset 0x0052 - Enable USB2 ports
  Enable/disable per USB2 ports. One byte for each port, byte0 for port0, byte1 for
  port1, and so on.
**/
  UINT8                       PortUsb20Enable[16];

/** Offset 0x0062 - Enable USB3 ports
  Enable/disable per USB3 ports. One byte for each port, byte0 for port0, byte1 for
  port1, and so on.
**/
  UINT8                       PortUsb30Enable[10];

/** Offset 0x006C - Enable xDCI controller
  Enable/disable to xDCI controller.
  $EN_DIS
**/
  UINT8                       XdciEnable;

/** Offset 0x006D - Reserved
**/
  UINT8                       Reserved2[28];

/** Offset 0x0089 - Enable SATA
  Enable/disable SATA controller.
  $EN_DIS
**/
  UINT8                       SataEnable;

/** Offset 0x008A - SATA Mode
  Select SATA controller working mode.
  0:AHCI, 1:RAID
**/
  UINT8                       SataMode;

/** Offset 0x008B - SPIn Device Mode
  Selects SPI operation mode. N represents controller index: SPI0, SPI1, ... Available
  modes: 0:SerialIoSpiDisabled, 1:SerialIoSpiPci, 2:SerialIoSpiHidden
**/
  UINT8                       SerialIoSpiMode[7];

/** Offset 0x0092 - Reserved
**/
  UINT8                       Reserved3[35];

/** Offset 0x00B5 - SPIn Default Chip Select Mode HW/SW
  Sets Default CS Mode Hardware or Software. N represents controller index: SPI0,
  SPI1, ... Available options: 0:HW, 1:SW
**/
  UINT8                       SerialIoSpiCsMode[7];

/** Offset 0x00BC - SPIn Default Chip Select State Low/High
  Sets Default CS State Low or High. N represents controller index: SPI0, SPI1, ...
  Available options: 0:Low, 1:High
**/
  UINT8                       SerialIoSpiCsState[7];

/** Offset 0x00C3 - UARTn Device Mode
  Selects Uart operation mode. N represents controller index: Uart0, Uart1, ... Available
  modes: 0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom,
  4:SerialIoUartSkipInit
**/
  UINT8                       SerialIoUartMode[7];

/** Offset 0x00CA - Reserved
**/
  UINT8                       Reserved4[65];

/** Offset 0x010B - Enables UART hardware flow control, CTS and RTS lines
  Enables UART hardware flow control, CTS and RTS lines.
**/
  UINT8                       SerialIoUartAutoFlow[7];

/** Offset 0x0112 - Reserved
**/
  UINT8                       Reserved5[2];

/** Offset 0x0114 - SerialIoUartRtsPinMuxPolicy
  Select SerialIo Uart Rts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS*
  for possible values.
**/
  UINT32                      SerialIoUartRtsPinMuxPolicy[7];

/** Offset 0x0130 - SerialIoUartCtsPinMuxPolicy
  Select SerialIo Uart Cts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_CTS*
  for possible values.
**/
  UINT32                      SerialIoUartCtsPinMuxPolicy[7];

/** Offset 0x014C - SerialIoUartRxPinMuxPolicy
  Select SerialIo Uart Rx pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RX* for
  possible values.
**/
  UINT32                      SerialIoUartRxPinMuxPolicy[7];

/** Offset 0x0168 - SerialIoUartTxPinMuxPolicy
  Select SerialIo Uart Tx pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_TX* for
  possible values.
**/
  UINT32                      SerialIoUartTxPinMuxPolicy[7];

/** Offset 0x0184 - UART Number For Debug Purpose
  UART number for debug purpose. 0:UART0, 1:UART1, 2:UART2, 3:UART3, 4:UART4, 5:UART5,
  6:UART6. Note: If UART0 is selected as CNVi BT Core interface, it cannot be used
  for debug purpose.
   0:UART0, 1:UART1, 2:UART2, 3:UART3, 4:UART4, 5:UART5, 6:UART6
**/
  UINT8                       SerialIoDebugUartNumber;

/** Offset 0x0185 - Reserved
**/
  UINT8                       Reserved6[7];

/** Offset 0x018C - I2Cn Device Mode
  Selects I2c operation mode. N represents controller index: I2c0, I2c1, ... Available
  modes: 0:SerialIoI2cDisabled, 1:SerialIoI2cPci, 2:SerialIoI2cHidden
**/
  UINT8                       SerialIoI2cMode[8];

/** Offset 0x0194 - Serial IO I2C SDA Pin Muxing
  Select SerialIo I2c Sda pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SDA* for
  possible values.
**/
  UINT32                      PchSerialIoI2cSdaPinMux[8];

/** Offset 0x01B4 - Serial IO I2C SCL Pin Muxing
  Select SerialIo I2c Scl pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SCL* for
  possible values.
**/
  UINT32                      PchSerialIoI2cSclPinMux[8];

/** Offset 0x01D4 - Reserved
**/
  UINT8                       Reserved7[192];

/** Offset 0x0294 - USB Per Port HS Preemphasis Bias
  USB Per Port HS Preemphasis Bias. 000b-0mV, 001b-11.25mV, 010b-16.9mV, 011b-28.15mV,
  100b-28.15mV, 101b-39.35mV, 110b-45mV, 111b-56.3mV. One byte for each port.
**/
  UINT8                       Usb2PhyPetxiset[16];

/** Offset 0x02A4 - USB Per Port HS Transmitter Bias
  USB Per Port HS Transmitter Bias. 000b-0mV, 001b-11.25mV, 010b-16.9mV, 011b-28.15mV,
  100b-28.15mV, 101b-39.35mV, 110b-45mV, 111b-56.3mV, One byte for each port.
**/
  UINT8                       Usb2PhyTxiset[16];

/** Offset 0x02B4 - USB Per Port HS Transmitter Emphasis
  USB Per Port HS Transmitter Emphasis. 00b - Emphasis OFF, 01b - De-emphasis ON,
  10b - Pre-emphasis ON, 11b - Pre-emphasis & De-emphasis ON. One byte for each port.
**/
  UINT8                       Usb2PhyPredeemp[16];

/** Offset 0x02C4 - USB Per Port Half Bit Pre-emphasis
  USB Per Port Half Bit Pre-emphasis. 1b - half-bit pre-emphasis, 0b - full-bit pre-emphasis.
  One byte for each port.
**/
  UINT8                       Usb2PhyPehalfbit[16];

/** Offset 0x02D4 - Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment
  Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment. Each value
  in arrary can be between 0-1. One byte for each port.
**/
  UINT8                       Usb3HsioTxDeEmphEnable[10];

/** Offset 0x02DE - USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting
  USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting, HSIO_TX_DWORD5[21:16],
  <b>Default = 29h</b> (approximately -3.5dB De-Emphasis). One byte for each port.
**/
  UINT8                       Usb3HsioTxDeEmph[10];

/** Offset 0x02E8 - Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment
  Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment, Each value
  in arrary can be between 0-1. One byte for each port.
**/
  UINT8                       Usb3HsioTxDownscaleAmpEnable[10];

/** Offset 0x02F2 - USB 3.0 TX Output Downscale Amplitude Adjustment
  USB 3.0 TX Output Downscale Amplitude Adjustment, HSIO_TX_DWORD8[21:16], <b>Default
  = 00h</b>. One byte for each port.
**/
  UINT8                       Usb3HsioTxDownscaleAmp[10];

/** Offset 0x02FC - Reserved
**/
  UINT8                       Reserved8[80];

/** Offset 0x034C - Enable LAN
  Enable/disable LAN controller.
  $EN_DIS
**/
  UINT8                       PchLanEnable;

/** Offset 0x034D - Reserved
**/
  UINT8                       Reserved9[11];

/** Offset 0x0358 - PCIe PTM enable/disable
  Enable/disable Precision Time Measurement for PCIE Root Ports.
**/
  UINT8                       PciePtm[24];

/** Offset 0x0370 - Reserved
**/
  UINT8                       Reserved10[73];

/** Offset 0x03B9 - Transition time in microseconds from Low Current Mode Voltage to High Current Mode Voltage
  This field has 1us resolution. When value is 0 PCH will not transition VCCIN_AUX
  to low current mode voltage.
**/
  UINT8                       PchFivrVccinAuxLowToHighCurModeVolTranTime;

/** Offset 0x03BA - Transition time in microseconds from Retention Mode Voltage to High Current Mode Voltage
  This field has 1us resolution. When value is 0 PCH will not transition VCCIN_AUX
  to retention mode voltage.
**/
  UINT8                       PchFivrVccinAuxRetToHighCurModeVolTranTime;

/** Offset 0x03BB - Reserved
**/
  UINT8                       Reserved11;

/** Offset 0x03BC - Transition time in microseconds from Off (0V) to High Current Mode Voltage
  This field has 1us resolution. When value is 0 Transition to 0V is disabled.
**/
  UINT16                      PchFivrVccinAuxOffToHighCurModeVolTranTime;

/** Offset 0x03BE - Reserved
**/
  UINT8                       Reserved12[38];

/** Offset 0x03E4 - CNVi Configuration
  This option allows for automatic detection of Connectivity Solution. [Auto Detection]
  assumes that CNVi will be enabled when available, [Disable] allows for disabling CNVi.
  0:Disable, 1:Auto
**/
  UINT8                       CnviMode;

/** Offset 0x03E5 - CNVi BT Core
  Enable/Disable CNVi BT Core, Default is ENABLE. 0: DISABLE, 1: ENABLE
  $EN_DIS
**/
  UINT8                       CnviBtCore;

/** Offset 0x03E6 - CNVi BT Audio Offload
  Enable/Disable BT Audio Offload, Default is DISABLE. 0: DISABLE, 1: ENABLE
  $EN_DIS
**/
  UINT8                       CnviBtAudioOffload;

/** Offset 0x03E7 - Reserved
**/
  UINT8                       Reserved13;

/** Offset 0x03E8 - CNVi RF_RESET pin muxing
  Select CNVi RF_RESET# pin depending on board routing. TGP-LP: GPP_A8 = 0x2942E408(default)
  or GPP_F4 = 0x194CE404. TGP-H: 0. Refer to GPIO_*_MUXING_CNVI_RF_RESET_* in GpioPins*.h.
**/
  UINT32                      CnviRfResetPinMux;

/** Offset 0x03EC - CNVi CLKREQ pin muxing
  Select CNVi CLKREQ pin depending on board routing. TGP-LP: GPP_A9 = 0x3942E609(default)
  or GPP_F5 = 0x394CE605. TGP-H: 0. Refer to GPIO_*_MUXING_CNVI_MODEM_CLKREQ_* in
  GpioPins*.h.
**/
  UINT32                      CnviClkreqPinMux;

/** Offset 0x03F0 - Reserved
**/
  UINT8                       Reserved14[14];

/** Offset 0x03FE - HECI3 state
  The HECI3 state from Mbp for reference in S3 path or when MbpHob is not installed.
  0: disable, 1: enable
  $EN_DIS
**/
  UINT8                       Heci3Enabled;

/** Offset 0x03FF - Reserved
**/
  UINT8                       Reserved15[141];

/** Offset 0x048C - CdClock Frequency selection
  0 (Default) Auto (Max based on reference clock frequency),  0: 192, 1: 307.2, 2:
  312 Mhz, 3: 552 Mhz, 4: 556.8 Mhz, 5: 648 Mhz, 6: 652.8 Mhz
  0xFF: Auto (Max based on reference clock frequency), 0: 192, 1: 307.2, 2: 312 Mhz,
  3: 552 Mhz, 4: 556.8 Mhz, 5: 648 Mhz, 6: 652.8 Mhz
**/
  UINT8                       CdClock;

/** Offset 0x048D - Enable/Disable PeiGraphicsPeimInit
  Enable(Default): Enable PeiGraphicsPeimInit, Disable: Disable PeiGraphicsPeimInit
  $EN_DIS
**/
  UINT8                       PeiGraphicsPeimInit;

/** Offset 0x048E - Reserved
**/
  UINT8                       Reserved16[2];

/** Offset 0x0490 - TypeC port GPIO setting
  GPIO Ping number for Type C Aux Oritation setting, use the GpioPad that is defined
  in GpioPinsXXXH.h and GpioPinsXXXLp.h as argument.(XXX is platform name, Ex: Tgl
  = TigerLake)
**/
  UINT32                      IomTypeCPortPadCfg[8];

/** Offset 0x04B0 - Reserved
**/
  UINT8                       Reserved17[30];

/** Offset 0x04CE - TCSS Aux Orientation Override Enable
  Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides
**/
  UINT16                      TcssAuxOri;

/** Offset 0x04D0 - TCSS HSL Orientation Override Enable
  Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides
**/
  UINT16                      TcssHslOri;

/** Offset 0x04D2 - Reserved
**/
  UINT8                       Reserved18[2];

/** Offset 0x04D4 - ITBT Root Port Enable
  ITBT Root Port Enable, 0:Disable, 1:Enable
  0:Disable, 1:Enable
**/
  UINT8                       ITbtPcieRootPortEn[4];

/** Offset 0x04D8 - Reserved
**/
  UINT8                      Reserved19[11];

/** Offset 0x04E3 - Enable/Disable PTM
  This policy will enable/disable Precision Time Measurement for TCSS PCIe Root Ports
  $EN_DIS
**/
  UINT8                       PtmEnabled[4];

/** Offset 0x04E7 - Reserved
**/
  UINT8                       Reserved20[194];

/** Offset 0x05A9 - Skip Multi-Processor Initialization
  When this is skipped, boot loader must initialize processors before SilicionInit
  API. </b>0: Initialize; <b>1: Skip
  $EN_DIS
**/
  UINT8                       SkipMpInit;

/** Offset 0x05AA - Reserved
**/
  UINT8                      Reserved21[60];

/** Offset 0x05E6 - Enable Power Optimizer
  Enable DMI Power Optimizer on PCH side.
  $EN_DIS
**/
  UINT8                       PchPwrOptEnable;

/** Offset 0x05E7 - Reserved
**/
  UINT8                       Reserved22[36];

/** Offset 0x060B - Enable PCH ISH SPI Cs0 pins assigned
  Set if ISH SPI Cs0 pins are to be enabled by BIOS. 0: Disable; 1: Enable.
**/
  UINT8                       PchIshSpiCs0Enable[1];

/** Offset 0x060C - Reserved
**/
  UINT8                       Reserved23[2];

/** Offset 0x060E - Enable PCH ISH SPI pins assigned
  Set if ISH SPI native pins are to be enabled by BIOS. 0: Disable; 1: Enable.
**/
  UINT8                       PchIshSpiEnable[1];

/** Offset 0x060F - Enable PCH ISH UART pins assigned
  Set if ISH UART native pins are to be enabled by BIOS. 0: Disable; 1: Enable.
**/
  UINT8                       PchIshUartEnable[2];

/** Offset 0x0611 - Enable PCH ISH I2C pins assigned
  Set if ISH I2C native pins are to be enabled by BIOS. 0: Disable; 1: Enable.
**/
  UINT8                       PchIshI2cEnable[3];

/** Offset 0x0614 - Enable PCH ISH GP pins assigned
  Set if ISH GP native pins are to be enabled by BIOS. 0: Disable; 1: Enable.
**/
  UINT8                       PchIshGpEnable[8];

/** Offset 0x061C - Reserved
**/
  UINT8                       Reserved24[2];

/** Offset 0x061E - Enable LOCKDOWN BIOS LOCK
  Enable the BIOS Lock feature and set EISS bit (D31:F5:RegDCh[5]) for the BIOS region
  protection.
  $EN_DIS
**/
  UINT8                       PchLockDownBiosLock;

/** Offset 0x061F - Reserved
**/
  UINT8                       Reserved25[75];

/** Offset 0x066A - Enable PCIE RP Clk Req Detect
  Probe CLKREQ# signal before enabling CLKREQ# based power management.
**/
  UINT8                       PcieRpClkReqDetect[24];

/** Offset 0x0682 - PCIE RP Advanced Error Report
  Indicate whether the Advanced Error Reporting is enabled.
**/
  UINT8                       PcieRpAdvancedErrorReporting[24];

/** Offset 0x069A - Reserved
**/
  UINT8                       Reserved26[168];

/** Offset 0x0742 - PCIE RP Max Payload
  Max Payload Size supported, Default 128B, see enum PCH_PCIE_MAX_PAYLOAD.
**/
  UINT8                       PcieRpMaxPayload[24];

/** Offset 0x075A - Reserved
**/
  UINT8                       Reserved27[86];

/** Offset 0x07B0 - PCIE RP Aspm
  The ASPM configuration of the root port (see: PCH_PCIE_ASPM_CONTROL). Default is
  PchPcieAspmAutoConfig.
**/
  UINT8                       PcieRpAspm[24];

/** Offset 0x07C8 - PCIE RP L1 Substates
  The L1 Substates configuration of the root port (see: PCH_PCIE_L1SUBSTATES_CONTROL).
  Default is PchPcieL1SubstatesL1_1_2.
**/
  UINT8                       PcieRpL1Substates[24];

/** Offset 0x07E0 - PCIE RP Ltr Enable
  Latency Tolerance Reporting Mechanism.
**/
  UINT8                       PcieRpLtrEnable[24];

/** Offset 0x07F8 - Reserved
**/
  UINT8                       Reserved28[98];

/** Offset 0x085A - PCH Sata Pwr Opt Enable
  SATA Power Optimizer on PCH side.
  $EN_DIS
**/
  UINT8                       SataPwrOptEnable;

/** Offset 0x085B - Reserved
**/
  UINT8                       Reserved29[50];

/** Offset 0x088D - Enable SATA Port DmVal
  DITO multiplier. Default is 15.
**/
  UINT8                       SataPortsDmVal[8];

/** Offset 0x0895 - Reserved
**/
  UINT8                       Reserved30;

/** Offset 0x0896 - Enable SATA Port DmVal
  DEVSLP Idle Timeout (DITO), Default is 625.
**/
  UINT16                      SataPortsDitoVal[8];

/** Offset 0x08A6 - Reserved
**/
  UINT8                       Reserved31[72];

/** Offset 0x08EE - USB2 Port Over Current Pin
  Describe the specific over current pin number of USB 2.0 Port N.
**/
  UINT8                       Usb2OverCurrentPin[16];

/** Offset 0x08FE - USB3 Port Over Current Pin
  Describe the specific over current pin number of USB 3.0 Port N.
**/
  UINT8                       Usb3OverCurrentPin[10];

/** Offset 0x0908 - Reserved
**/
  UINT8                       Reserved32[16];

/** Offset 0x0918 - Enable 8254 Static Clock Gating
  Set 8254CGE=1 is required for SLP_S0 support. However, set 8254CGE=1 in POST time
  might fail to boot legacy OS using 8254 timer. Make sure it is disabled to support
  legacy OS using 8254 timer. Also enable this while S0ix is enabled.
  $EN_DIS
**/
  UINT8                       Enable8254ClockGating;

/** Offset 0x0919 - Enable 8254 Static Clock Gating On S3
  This is only applicable when Enable8254ClockGating is disabled. FSP will do the
  8254 CGE programming on S3 resume when Enable8254ClockGatingOnS3 is enabled. This
  avoids the SMI requirement for the programming.
  $EN_DIS
**/
  UINT8                       Enable8254ClockGatingOnS3;

/** Offset 0x091A - Reserved
**/
  UINT8                       Reserved33[3];

/** Offset 0x091D - Hybrid Storage Detection and Configuration Mode
  Enables support for Hybrid storage devices. 0: Disabled; 1: Dynamic Configuration.
  Default is 0: Disabled
  0: Disabled, 1: Dynamic Configuration
**/
  UINT8                       HybridStorageMode;

/** Offset 0x091E - Reserved
**/
  UINT8                       Reserved34[434];

/** Offset 0x0AD0 - RpPtmBytes
**/
  UINT8                       RpPtmBytes[4];

/** Offset 0x0AD4 - Reserved
**/
  UINT8                      Reserved35[101];

/** Offset 0x0B39 - GT Frequency Limit
  0xFF: Auto(Default), 2: 100 Mhz, 3: 150 Mhz, 4: 200 Mhz, 5: 250 Mhz, 6: 300 Mhz,
  7: 350 Mhz, 8: 400 Mhz, 9: 450 Mhz, 0xA: 500 Mhz, 0xB: 550 Mhz, 0xC: 600 Mhz, 0xD:
  650 Mhz, 0xE: 700 Mhz, 0xF: 750 Mhz, 0x10: 800 Mhz, 0x11: 850 Mhz, 0x12:900 Mhz,
  0x13: 950 Mhz, 0x14: 1000 Mhz, 0x15: 1050 Mhz, 0x16: 1100 Mhz, 0x17: 1150 Mhz,
  0x18: 1200 Mhz
  0xFF: Auto(Default), 2: 100 Mhz, 3: 150 Mhz, 4: 200 Mhz, 5: 250 Mhz, 6: 300 Mhz,
  7: 350 Mhz, 8: 400 Mhz, 9: 450 Mhz, 0xA: 500 Mhz, 0xB: 550 Mhz, 0xC: 600 Mhz, 0xD:
  650 Mhz, 0xE: 700 Mhz, 0xF: 750 Mhz, 0x10: 800 Mhz, 0x11: 850 Mhz, 0x12:900 Mhz,
  0x13: 950 Mhz, 0x14: 1000 Mhz, 0x15: 1050 Mhz, 0x16: 1100 Mhz, 0x17: 1150 Mhz,
  0x18: 1200 Mhz
**/
  UINT8                       GtFreqMax;

/** Offset 0x0B3A - Reserved
**/
  UINT8                       Reserved36[264];

/** Offset 0x0C42 - PCIE RP Ltr Max Snoop Latency
  Latency Tolerance Reporting, Max Snoop Latency.
**/
  UINT16                      PcieRpLtrMaxSnoopLatency[24];

/** Offset 0x0C72 - PCIE RP Ltr Max No Snoop Latency
  Latency Tolerance Reporting, Max Non-Snoop Latency.
**/
  UINT16                      PcieRpLtrMaxNoSnoopLatency[24];

/** Offset 0x0CA2 - Reserved
**/
  UINT8                       Reserved37[269];

/** Offset 0x0DAF - LpmStateEnableMask
**/
  UINT8                       LpmStateEnableMask;

/** Offset 0x0DB0 - Reserved
**/
  UINT8                       Reserved38[80];
} FSP_S_CONFIG;

/** Fsp S UPD Configuration
**/
typedef struct {

/** Offset 0x0000
**/
  FSP_UPD_HEADER              FspUpdHeader;

/** Offset 0x0020
**/
  FSP_S_CONFIG                FspsConfig;

/** Offset 0x0E00
**/
  UINT8                       UnusedUpdSpace34[6];

/** Offset 0x0E06
**/
  UINT16                      UpdTerminator;
} FSPS_UPD;

#pragma pack()

#endif
