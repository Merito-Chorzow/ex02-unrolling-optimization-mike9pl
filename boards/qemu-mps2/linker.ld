/* boards/qemu-mps2/linker.ld */
ENTRY(Reset_Handler)

/* QEMU mps2-an385: FLASH at 0x0000_0000, SRAM at 0x2000_0000 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 1M
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Top of stack (used by vector table) */
_estack = ORIGIN(RAM) + LENGTH(RAM);
PROVIDE(__StackTop = _estack);

SECTIONS
{
  /* Vector table in FLASH — KEEP to prevent gc-sections from discarding it */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))          /* <--- important */
    . = ALIGN(4);
  } > FLASH

  /* Text and read-only data in FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame*)
    . = ALIGN(4);
  } > FLASH

  /* Initialized data in RAM, with load image in FLASH */
  .data : AT(ADDR(.text) + SIZEOF(.text))
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM

  _sidata = LOADADDR(.data);

  /* Uninitialized data in RAM */
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  _end = .;
}