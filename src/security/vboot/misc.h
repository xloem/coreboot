/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2014 The ChromiumOS Authors.  All rights reserved.
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

#ifndef __VBOOT_MISC_H__
#define __VBOOT_MISC_H__

#include <assert.h>
#include <security/vboot/vboot_common.h>

struct vb2_context;
struct vb2_shared_data;

/*
 * Source: security/vboot/common.c
 */
void *vboot_get_workbuf(void);
struct vb2_context *vboot_get_context(void);

/*
 * Returns 1 if firmware slot A is used, 0 if slot B is used.
 */
static inline int vboot_is_firmware_slot_a(const struct vb2_context *ctx)
{
	return !(ctx->flags & VB2_CONTEXT_FW_SLOT_B);
}

/*
 * Check if given flag is set in the flags field in GBB header.
 * Return value:
 * true: Flag is set.
 * false: Flag is not set.
 */
static inline bool vboot_is_gbb_flag_set(enum vb2_gbb_flag flag)
{
	return !!(vb2api_gbb_get_flags(vboot_get_context()) & flag);
}

/*
 * Locates firmware as a region device. Returns 0 on success, -1 on failure.
 */
int vboot_locate_firmware(const struct vb2_context *ctx,
			  struct region_device *fw);

/*
 * Source: security/vboot/vboot_handoff.c
 */
void vboot_fill_handoff(void);

/*
 * Source: security/vboot/bootmode.c
 */
void vboot_save_recovery_reason_vbnv(void);

/*
 * The stage loading code is compiled and entered from multiple stages. The
 * helper functions below attempt to provide more clarity on when certain
 * code should be called. They are implemented inline for better compile-time
 * code elimination.
 */

static inline int verification_should_run(void)
{
	if (CONFIG(VBOOT_SEPARATE_VERSTAGE))
		return ENV_VERSTAGE;
	else if (CONFIG(VBOOT_STARTS_IN_ROMSTAGE))
		return ENV_ROMSTAGE;
	else if (CONFIG(VBOOT_STARTS_IN_BOOTBLOCK))
		return ENV_BOOTBLOCK;
	else
		dead_code();
}

static inline int verstage_should_load(void)
{
	if (CONFIG(VBOOT_SEPARATE_VERSTAGE))
		return ENV_BOOTBLOCK;
	else
		return 0;
}

static inline int vboot_logic_executed(void)
{
	extern int vboot_executed;	/* should not be globally accessible */

	/* If we are in the stage that runs verification, or in the stage that
	   both loads the verstage and is returned to from it afterwards, we
	   need to check a global to see if verfication has run. */
	if (verification_should_run() ||
	    (verstage_should_load() && CONFIG(VBOOT_RETURN_FROM_VERSTAGE)))
		return vboot_executed;

	if (CONFIG(VBOOT_STARTS_IN_BOOTBLOCK)) {
		/* All other stages are "after the bootblock" */
		return !ENV_BOOTBLOCK;
	} else if (CONFIG(VBOOT_STARTS_IN_ROMSTAGE)) {
		/* Post-RAM stages are "after the romstage" */
		return !ENV_ROMSTAGE_OR_BEFORE;
	} else {
		dead_code();
	}
}


#endif /* __VBOOT_MISC_H__ */
