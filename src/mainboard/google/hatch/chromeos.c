/* SPDX-License-Identifier: GPL-2.0-only */

#include <baseboard/variants.h>
#include <bootmode.h>
#include <boot/coreboot_tables.h>
#include <ec/google/chromeec/ec.h>
#include <gpio.h>
#include <variant/gpio.h>
#include <types.h>
#include <vendorcode/google/chromeos/chromeos.h>

void fill_lb_gpios(struct lb_gpios *gpios)
{
	struct lb_gpio chromeos_gpios[] = {
		{-1, ACTIVE_HIGH, get_lid_switch(), "lid"},
		{-1, ACTIVE_HIGH, 0, "power"},
		{-1, ACTIVE_HIGH, gfx_get_init_done(), "oprom"},
		{GPIO_EC_IN_RW, ACTIVE_HIGH, gpio_get(GPIO_EC_IN_RW),
		 "EC in RW"},
	};
	lb_add_gpios(gpios, chromeos_gpios, ARRAY_SIZE(chromeos_gpios));
}

int get_write_protect_state(void)
{
	return gpio_get(GPIO_PCH_WP);
}

void mainboard_chromeos_acpi_generate(void)
{
	const struct cros_gpio *cros_gpios;
	size_t num_gpios = 0;

	cros_gpios = variant_cros_gpios(&num_gpios);

	chromeos_acpi_gpio_generate(cros_gpios, num_gpios);
}

int get_ec_is_trusted(void)
{
	/* EC is trusted if not in RW. */
	return !gpio_get(GPIO_EC_IN_RW);
}
