bootblock-y += gpio.c

romstage-y += gpio.c
romstage-y += memory.c

ramstage-y += gpio.c
ramstage-$(CONFIG_FW_CONFIG) += fw_config.c
