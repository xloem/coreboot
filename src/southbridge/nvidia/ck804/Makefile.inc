ifeq ($(CONFIG_SOUTHBRIDGE_NVIDIA_CK804),y)

bootblock-y += bootblock.c

romstage-y += early_setup.c
romstage-y += early_smbus.c

ramstage-y += ht.c
ramstage-y += reset.c
ramstage-y += smbus.c

endif
