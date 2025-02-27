/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef AMD_SABRINA_GPIO_H
#define AMD_SABRINA_GPIO_H

#define GPIO_DEVICE_NAME	"AMD0030"
#define GPIO_DEVICE_DESC	"GPIO Controller"

#ifndef __ACPI__
#include <soc/iomap.h>
#include <amdblocks/gpio.h>
#endif /* !__ACPI__ */

#include <amdblocks/gpio_defs.h>

/* The following sections describe only the GPIOs defined for this SOC */

#define SOC_GPIO_TOTAL_PINS		158

/* Bank 0: GPIO_0 - GPIO_63 */
#define GPIO_0				0
#define GPIO_1				1
#define GPIO_2				2
#define GPIO_3				3
#define GPIO_4				4
#define GPIO_5				5
#define GPIO_6				6
#define GPIO_7				7
#define GPIO_8				8
#define GPIO_9				9
#define GPIO_10				10
#define GPIO_11				11
#define GPIO_12				12
#define GPIO_13				13
#define GPIO_14				14
#define GPIO_16				16
#define GPIO_17				17
#define GPIO_18				18
#define GPIO_19				19
#define GPIO_20				20
#define GPIO_21				21
#define GPIO_22				22
#define GPIO_23				23
#define GPIO_24				24
#define GPIO_26				26
#define GPIO_27				27
#define GPIO_29				29
#define GPIO_30				30
#define GPIO_31				31
#define GPIO_32				32
#define GPIO_38				38
#define GPIO_39				39
#define GPIO_40				40
#define GPIO_42				42

/* Bank 1: GPIO_64 - GPIO_127 */
#define GPIO_67				67
#define GPIO_68				68
#define GPIO_69				69
#define GPIO_70				70
#define GPIO_74				74
#define GPIO_75				75
#define GPIO_76				76
#define GPIO_77				77
#define GPIO_78				78
#define GPIO_79				79
#define GPIO_80				80
#define GPIO_81				81
#define GPIO_84				84
#define GPIO_85				85
#define GPIO_86				86
#define GPIO_89				89
#define GPIO_90				90
#define GPIO_91				91
#define GPIO_92				92
#define GPIO_104			104
#define GPIO_105			105
#define GPIO_106			106
#define GPIO_107			107
#define GPIO_113			113
#define GPIO_114			114
#define GPIO_115			115
#define GPIO_116			116

/* Bank 2: GPIO_128 - GPIO_191 */
#define GPIO_130			130
#define GPIO_131			131
#define GPIO_132			132
#define GPIO_135			135
#define GPIO_136			136
#define GPIO_137			137
#define GPIO_138			138
#define GPIO_139			139
#define GPIO_140			140
#define GPIO_141			141
#define GPIO_142			142
#define GPIO_143			143
#define GPIO_144			144
#define GPIO_145			145
#define GPIO_146			146
#define GPIO_147			147
#define GPIO_148			148
#define GPIO_153			153
#define GPIO_154			154
#define GPIO_155			155
#define GPIO_156			156
#define GPIO_157			157

/* IOMUX function names and values */
#define GPIO_0_IOMUX_PWR_BTN_L 0
#define GPIO_0_IOMUX_GPIOxx 1
#define GPIO_1_IOMUX_SYS_RESET_L 0
#define GPIO_1_IOMUX_GPIOxx 1
#define GPIO_2_IOMUX_WAKE_L 0
#define GPIO_2_IOMUX_GPIOxx 1
#define GPIO_3_IOMUX_GPIOxx 0
#define GPIO_4_IOMUX_GPIOxx 0
#define GPIO_5_IOMUX_GPIOxx 0
#define GPIO_5_IOMUX_DEVSLP0 1
#define GPIO_6_IOMUX_GPIOxx 0
#define GPIO_6_IOMUX_DEVSLP1 1
#define GPIO_6_IOMUX_MDIO0_SCL 2
#define GPIO_7_IOMUX_GPIOxx 0
#define GPIO_7_IOMUX_SVI_RST_L 1
#define GPIO_8_IOMUX_GPIOxx 0
#define GPIO_8_IOMUX_TMU_CLK_OUT0 1
#define GPIO_8_IOMUX_TMU_CLK_OUT1 2
#define GPIO_9_IOMUX_GPIOxx 0
/* GPIO 9 IOMUX == 1 is also GPIOxx */
#define GPIO_9_IOMUX_MDIO2_SCL 2
#define GPIO_10_IOMUX_GPIOxx 0
#define GPIO_10_IOMUX_S0A3_GPIO 1
#define GPIO_11_IOMUX_GPIOxx 0
#define GPIO_11_IOMUX_BLINK 1
#define GPIO_11_IOMUX_MDIO3_SDA 2
#define GPIO_12_IOMUX_LLB_L 0
#define GPIO_12_IOMUX_GPIOxx 1
#define GPIO_12_IOMUX_LPC_PME_L 2
#define GPIO_13_IOMUX_USB_SBTX_0 0
#define GPIO_13_IOMUX_GPIOxx 1
#define GPIO_14_IOMUX_USB_SBTX_1 0
#define GPIO_14_IOMUX_GPIOxx 1
#define GPIO_16_IOMUX_USB_OC0_L 0
#define GPIO_16_IOMUX_GPIOxx 1
#define GPIO_17_IOMUX_USB_OC1_L 0
#define GPIO_17_IOMUX_GPIOxx 1
#define GPIO_18_IOMUX_USB_OC2_L 0
#define GPIO_18_IOMUX_GPIOxx 1
#define GPIO_19_IOMUX_SMBUS1_SCL 0
#define GPIO_19_IOMUX_I2C3_SCL 1
/* GPIO 19 IOMUX == 2 is also I2C3_SCL */
#define GPIO_19_IOMUX_GPIOxx 3
#define GPIO_20_IOMUX_SMBUS1_SDA 0
#define GPIO_20_IOMUX_I2C3_SDA 1
/* GPIO 19 IOMUX == 2 is also I2C3_SDA */
#define GPIO_20_IOMUX_GPIOxx 3
#define GPIO_21_IOMUX_ESPI_RESET_L 0
#define GPIO_21_IOMUX_KBRST_L 1
#define GPIO_21_IOMUX_GPIOxx 2
#define GPIO_22_IOMUX_LDRQ0_L 0
#define GPIO_22_IOMUX_ESPI_ALERT_D1 1
#define GPIO_22_IOMUX_GPIOxx 2
#define GPIO_22_IOMUX_SD0_CMD 3
#define GPIO_23_IOMUX_AC_PRES 0
#define GPIO_23_IOMUX_GPIOxx 1
#define GPIO_23_IOMUX_MDIO2_SDA 2
#define GPIO_24_IOMUX_USB_OC3_L 0
#define GPIO_24_IOMUX_GPIOxx 1
#define GPIO_26_IOMUX_PCIE_RST0_L 0
#define GPIO_26_IOMUX_GPIOxx 1
#define GPIO_27_IOMUX_GPIOxx 0
#define GPIO_27_IOMUX_PCIE_RST1_L 1
#define GPIO_29_IOMUX_SPI_TPM_CS_L 0
#define GPIO_29_IOMUX_GPIOxx 1
#define GPIO_30_IOMUX_SPI_CS2_L 0
#define GPIO_30_IOMUX_ESPI_CS_L 1
#define GPIO_30_IOMUX_GPIOxx 2
#define GPIO_31_IOMUX_SPI_CS3_L 0
#define GPIO_31_IOMUX_GPIOxx 1
#define GPIO_32_IOMUX_GPIOxx 0
#define GPIO_32_IOMUX_LPC_RST_L 1
#define GPIO_32_IOMUX_MDIO3_SCL 2
#define GPIO_38_IOMUX_CLK_REQ5_L 0
#define GPIO_38_IOMUX_GPIOxx 1
#define GPIO_38_IOMUX_MDIO1_SDA 2
#define GPIO_39_IOMUX_CLK_REQ6_L 0
#define GPIO_39_IOMUX_GPIOxx 1
#define GPIO_39_IOMUX_MDIO1_SCL 2
#define GPIO_40_IOMUX_GPIOxx 0
/* GPIO 40 IOMUX == 1 is also GPIOxx */
#define GPIO_40_IOMUX_MDIO0_SDA 2
#define GPIO_42_IOMUX_GPIOxx 0
#define GPIO_67_IOMUX_SPI_ROM_REQ 0
#define GPIO_67_IOMUX_GPIOxx 1
#define GPIO_68_IOMUX_SPI1_DAT2 0
#define GPIO_68_IOMUX_GPIOxx 1
#define GPIO_68_IOMUX_SERIRQ 2
#define GPIO_68_IOMUX_SD0_DATA3 3
#define GPIO_69_IOMUX_SPI1_DAT3 0
#define GPIO_69_IOMUX_GPIOxx 1
#define GPIO_69_IOMUX_SD0_CLK 2
#define GPIO_70_IOMUX_SPI2_CLK 0
#define GPIO_70_IOMUX_GPIOxx 1
#define GPIO_74_IOMUX_SPI1_CS1_L 0
#define GPIO_74_IOMUX_GPIOxx 1
#define GPIO_74_IOMUX_GFX10_CAC_IPIO0 2
#define GPIO_75_IOMUX_SPI2_CS1_L 0
#define GPIO_75_IOMUX_LPCCLK1 1
#define GPIO_75_IOMUX_GPIOxx 2
#define GPIO_76_IOMUX_SPI_ROM_GNT 0
#define GPIO_76_IOMUX_GPIOxx 1
#define GPIO_77_IOMUX_SPI1_CLK 0
#define GPIO_77_IOMUX_GPIOxx 1
/* GPIO 77 IOMUX == 2 is also GPIOxx */
#define GPIO_77_IOMUX_SD0_DATA0 3
#define GPIO_78_IOMUX_SPI1_CS2_L 0
#define GPIO_78_IOMUX_GPIOxx 1
#define GPIO_78_IOMUX_GFX10_CAC_IPIO1 2
#define GPIO_78_IOMUX_SD0_DATA1 3
#define GPIO_79_IOMUX_SPI1_CS3_L 0
#define GPIO_79_IOMUX_GPIOxx 1
#define GPIO_79_IOMUX_LPC_CLKRUN_L 2
#define GPIO_80_IOMUX_SPI1_DAT1 0
#define GPIO_80_IOMUX_GPIOxx 1
/* GPIO 80 IOMUX == 2 is also GPIOxx */
#define GPIO_80_IOMUX_SD0_DATA2 3
#define GPIO_81_IOMUX_SPI1_DAT0 0
#define GPIO_81_IOMUX_GPIOxx 1
#define GPIO_84_IOMUX_FANIN0 0
#define GPIO_84_IOMUX_GPIOxx 1
#define GPIO_85_IOMUX_FANOUT0 0
#define GPIO_85_IOMUX_GPIOxx 1
#define GPIO_86_IOMUX_GPIOxx 0
#define GPIO_86_IOMUX_LPC_SMI_L 1
#define GPIO_89_IOMUX_GENINT1_L 0
#define GPIO_89_IOMUX_PSP_INTR0 1
#define GPIO_89_IOMUX_GPIOxx 2
#define GPIO_90_IOMUX_GENINT2_L 0
#define GPIO_90_IOMUX_PSP_INTR1 1
#define GPIO_90_IOMUX_GPIOxx 2
#define GPIO_91_IOMUX_SPKR 0
#define GPIO_91_IOMUX_GPIOxx 1
#define GPIO_92_IOMUX_CLK_REQ0_L 0
#define GPIO_92_IOMUX_SATA_IS0_L 1
#define GPIO_92_IOMUX_SATA_ZP0_L 2
#define GPIO_92_IOMUX_GPIOxx 3
#define GPIO_104_IOMUX_SPI2_DAT0 0
#define GPIO_104_IOMUX_GPIOxx 1
#define GPIO_105_IOMUX_SPI2_DAT1 0
#define GPIO_105_IOMUX_GPIOxx 1
#define GPIO_106_IOMUX_SPI2_DAT2 0
#define GPIO_106_IOMUX_GPIOxx 1
#define GPIO_107_IOMUX_SPI2_DAT3 0
#define GPIO_107_IOMUX_GPIOxx 1
#define GPIO_113_IOMUX_SMBUS0_SCL 0
#define GPIO_113_IOMUX_I2C2_SCL 1
#define GPIO_113_IOMUX_I3C2_SCL 2
#define GPIO_113_IOMUX_GPIOxx 3
#define GPIO_114_IOMUX_SMBUS0_SDA 0
#define GPIO_114_IOMUX_I2C2_SDA 1
#define GPIO_114_IOMUX_I3C2_SDA 2
#define GPIO_114_IOMUX_GPIOxx 3
#define GPIO_115_IOMUX_CLK_REQ1_L 0
#define GPIO_115_IOMUX_GPIOxx 1
#define GPIO_116_IOMUX_CLK_REQ2_L 0
#define GPIO_116_IOMUX_GPIOxx 1
#define GPIO_130_IOMUX_SATA_ACT_L 0
#define GPIO_130_IOMUX_GPIOxx 1
#define GPIO_131_IOMUX_CLK_REQ3_L 0
#define GPIO_131_IOMUX_SATA_IS1_L 1
#define GPIO_131_IOMUX_SATA_ZP1_L 2
#define GPIO_131_IOMUX_GPIOxx 3
#define GPIO_132_IOMUX_CLK_REQ4_L 0
#define GPIO_132_IOMUX_OSCIN 1
#define GPIO_132_IOMUX_GPIOxx 2
#define GPIO_135_IOMUX_GPIOxx 0
#define GPIO_135_IOMUX_UART2_CTS_L 1
#define GPIO_135_IOMUX_UART3_TXD 2
#define GPIO_136_IOMUX_GPIOxx 0
#define GPIO_136_IOMUX_UART2_RXD 1
#define GPIO_137_IOMUX_GPIOxx 0
#define GPIO_137_IOMUX_UART2_RTS_L 1
#define GPIO_137_IOMUX_UART3_RXD 2
#define GPIO_138_IOMUX_GPIOxx 0
#define GPIO_138_IOMUX_UART2_TXD 1
#define GPIO_139_IOMUX_GPIOxx 0
#define GPIO_139_IOMUX_UART2_INTR 1
#define GPIO_140_IOMUX_GPIOxx 0
#define GPIO_140_IOMUX_UART0_CTS_L 1
#define GPIO_140_IOMUX_UART1_TXD 2
#define GPIO_141_IOMUX_GPIOxx 0
#define GPIO_141_IOMUX_UART0_RXD 1
#define GPIO_142_IOMUX_GPIOxx 0
#define GPIO_142_IOMUX_UART0_RTS_L 1
#define GPIO_142_IOMUX_UART1_RXD 2
#define GPIO_143_IOMUX_GPIOxx 0
#define GPIO_143_IOMUX_UART0_TXD 1
#define GPIO_144_IOMUX_GPIOxx 0
#define GPIO_144_IOMUX_SHUTDOWN_L 1
#define GPIO_144_IOMUX_UART0_INTR 2
#define GPIO_145_IOMUX_I2C0_SCL 0
#define GPIO_145_IOMUX_I3C0_SCL 1
#define GPIO_145_IOMUX_GPIOxx 2
#define GPIO_146_IOMUX_I2C0_SDA 0
#define GPIO_146_IOMUX_I3C0_SDA 1
#define GPIO_146_IOMUX_GPIOxx 2
#define GPIO_147_IOMUX_I2C1_SCL 0
#define GPIO_147_IOMUX_I3C1_SCL 1
#define GPIO_147_IOMUX_GPIOxx 2
#define GPIO_148_IOMUX_I2C1_SDA 0
#define GPIO_148_IOMUX_I3C1_SDA 1
#define GPIO_148_IOMUX_GPIOxx 2
#define GPIO_153_IOMUX_GPIOxx 0
#define GPIO_153_IOMUX_UART4_CTS_L 1
#define GPIO_154_IOMUX_GPIOxx 0
#define GPIO_154_IOMUX_UART4_RTS_L 1
#define GPIO_155_IOMUX_GPIOxx 0
#define GPIO_155_IOMUX_UART4_RXD 1
#define GPIO_156_IOMUX_GPIOxx 0
#define GPIO_156_IOMUX_UART4_TXD 1
#define GPIO_157_IOMUX_GPIOxx 0
#define GPIO_157_IOMUX_UART4_INTR 1

#endif /* AMD_SABRINA_GPIO_H */
