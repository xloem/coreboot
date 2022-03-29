#include <arch/x86/memlayout.ld>

#if ENV_BOOTBLOCK
#include <southbridge/nvidia/ck804/romstrap.ld>
SECTIONS
{
	_ID_SECTION_END = _ROMSTRAP_SECTION;
	_ID_SECTION = _ID_SECTION_END - SIZEOF(.id);
}
#endif

