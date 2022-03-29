ifeq ($(CONFIG_SOUTHBRIDGE_NVIDIA_CK804),y)

bootblock-y += bootblock.c
bootblock-y += romstrap.ld
bootblock-y += romstrap.S

romstage-y += early_setup.c
romstage-y += early_smbus.c

postcar-y += reset.c

ramstage-y += acpi.c
ramstage-y += ck804.c
ramstage-y += ht.c
ramstage-y += lpc.c
ramstage-y += reset.c
ramstage-y += smbus.c

endif
