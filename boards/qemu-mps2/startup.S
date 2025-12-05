/* boards/qemu-mps2/startup.S (fixed) */
.syntax unified
.cpu cortex-m3
.thumb

/* Linker symbols */
.global _estack
.global _sidata
.global _sdata
.global _edata
.global _sbss
.global _ebss
.extern main

/* ---------------- Vector table ---------------- */
.section .isr_vector, "a", %progbits
.align  2
g_pfnVectors:
  .word _estack           /* 0: Initial Stack Pointer */
  .word Reset_Handler     /* 1: Reset */
  .word NMI_Handler       /* 2: NMI */
  .word HardFault_Handler /* 3: HardFault */
  .word MemManage_Handler /* 4: MemManage */
  .word BusFault_Handler  /* 5: BusFault */
  .word UsageFault_Handler/* 6: UsageFault */
  .word 0                 /* 7: Reserved */
  .word 0                 /* 8: Reserved */
  .word 0                 /* 9: Reserved */
  .word 0                 /* 10: Reserved */
  .word SVC_Handler       /* 11: SVCall */
  .word DebugMon_Handler  /* 12: DebugMonitor */
  .word 0                 /* 13: Reserved */
  .word PendSV_Handler    /* 14: PendSV */
  .word SysTick_Handler   /* 15: SysTick */
  /* Fill remaining IRQs with a default handler */
  .rept 32
    .word Default_Handler
  .endr
.size g_pfnVectors, .-g_pfnVectors

/* ---------------- Reset_Handler ---------------- */
.section .text.Reset_Handler, "ax", %progbits
.thumb_func
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  /* Copy .data from FLASH (_sidata) to RAM (_sdata .. _edata) */
  ldr r0, =_sidata
  ldr r1, =_sdata
  ldr r2, =_edata
1:
  cmp r1, r2
  bcc 2f
  b   3f
2:
  ldr r3, [r0], #4
  str r3, [r1], #4
  b   1b

3:
  /* Zero .bss (_sbss .. _ebss) */
  ldr r0, =_sbss
  ldr r1, =_ebss
4:
  cmp r0, r1
  bcc 5f
  b   6f
5:
  movs r2, #0
  str r2, [r0], #4
  b   4b

6:
  /* Call main */
  bl  main

7:
  /* If main returns, loop forever */
  b   7b

/* ---------------- Default & weak handlers ---------------- */
.section .text.Default_Handler, "ax", %progbits
.thumb_func
.weak Default_Handler
.global Default_Handler
.type Default_Handler, %function
Default_Handler:
  b .

/* Provide weak aliases for common handlers so user can override them */
.weak NMI_Handler
.thumb_func
NMI_Handler:
  b Default_Handler

.weak HardFault_Handler
.thumb_func
HardFault_Handler:
  b Default_Handler

.weak MemManage_Handler
.thumb_func
MemManage_Handler:
  b Default_Handler

.weak BusFault_Handler
.thumb_func
BusFault_Handler:
  b Default_Handler

.weak UsageFault_Handler
.thumb_func
UsageFault_Handler:
  b Default_Handler

.weak SVC_Handler
.thumb_func
SVC_Handler:
  b Default_Handler

.weak DebugMon_Handler
.thumb_func
DebugMon_Handler:
  b Default_Handler

.weak PendSV_Handler
.thumb_func
PendSV_Handler:
  b Default_Handler

.weak SysTick_Handler
.thumb_func
SysTick_Handler:
  b Default_Handler
