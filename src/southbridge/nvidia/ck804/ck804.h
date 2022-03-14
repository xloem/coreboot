/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef SOUTHBRIDGE_NVIDIA_CK804_CK804_H
#define SOUTHBRIDGE_NVIDIA_CK804_CK804_H

#if CONFIG_HT_CHAIN_END_UNITID_BASE < CONFIG_HT_CHAIN_UNITID_BASE
#define CK804_DEVN_BASE CONFIG_HT_CHAIN_END_UNITID_BASE
#else
#define CK804_DEVN_BASE CONFIG_HT_CHAIN_UNITID_BASE
#endif

#define CK804B_BUSN 0x80
#define CK804B_DEVN_BASE (!CONFIG(SB_HT_CHAIN_UNITID_OFFSET_ONLY) ? CK804_DEVN_BASE : 1)

#define ANACTRL_IO_BASE 0x3000
#define ANACTRL_REG_POS 0x68

#define SYSCTRL_IO_BASE 0x2000
#define SYSCTRL_REG_POS 0x64

#define CK804_BOARD_BOOT_BASE_UNIT_UID 1

void enable_fid_change_on_sb(unsigned int sbbusn, unsigned int sbdn);

void ck804_control(const unsigned int *values, u32 size,
			  uint8_t bus_unit_id);

/**
 * @brief Get SouthBridge device number
 * @param[in] bus target bus number
 * @return southbridge device number
 */
unsigned int get_sbdn(unsigned int bus);

#endif
