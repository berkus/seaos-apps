SCRIPT_NAME=elf
OUTPUT_FORMAT=elf32-i386
TEXT_START_ADDR=0x40000000
MAXPAGESIZE="CONSTANT (MAXPAGESIZE)"
COMMONPAGESIZE="CONSTANT (COMMONPAGESIZE)"
ARCH=i386
MACHINE=
NOP=0x90909090
TEMPLATE_NAME=elf32
GENERATE_SHLIB_SCRIPT=yes
GENERATE_PIE_SCRIPT=yes
NO_SMALL_DATA=yes
SEPERATE_GOTPLT=12
OTHER_READONLY_SECTIONS='
PROVIDE (___rodata = DEFINED (.rodata) ? .rodata : 0);
. = ALIGN (8);
'

# Make _edata and .bss aligned by smuggling in an alignment directive.
OTHER_GOT_SECTIONS='. = ALIGN (8);'
