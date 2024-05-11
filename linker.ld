OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
    ram (wxa) : ORIGIN = 0x80000000, LENGTH = 128M
}

PHDRS 
{
    text PT_LOAD;
    data PT_LOAD;
    bss  PT_LOAD;
}

SECTIONS
{
    .text : {
        PROVIDE(_kernel_start = .);
        PROVIDE(_text_boot_start = .);
        *(.text.boot)
        PROVIDE(_text_boot_end = .);
        PROVIDE(_text_start = .);
        *(.text)
        PROVIDE(_text_end = .);     

        . = ALIGN(0x1000);
        PROVIDE(_trampoline_start = .);
        *(.trampoline)
        PROVIDE(_trampoline_end = .);
        . += 0x1000;
        . = ALIGN(0x1000);

    } >ram AT>ram :text

    PROVIDE(_global_pointer = .);

    .rodata : {
        PROVIDE(_rodata_start = .);
        *(.rodata)
        PROVIDE(_rodata_end = .);
    } >ram AT>ram :text

    .data : {
        . = ALIGN(0x1000);
        PROVIDE(_mstack_bot = .);
        . += 0x4000;
        PROVIDE(_mstack_top = .);
        
        *(.data.boot)
        
        . = ALIGN(0x1000);
        PROVIDE(_data_start = .);
        *(.data)
        PROVIDE(_data_end = .);
    } >ram AT>ram :data

    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss)
        PROVIDE(_bss_end = .);
    } >ram AT>ram :bss

    . = ALIGN(0x1000);
    PROVIDE(_kernel_end = .);
}