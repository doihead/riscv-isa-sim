// See LICENSE for license details.
#ifndef _RISCV_PLATFORM_H
#define _RISCV_PLATFORM_H

#define DEFAULT_KERNEL_BOOTARGS "console=ttyS0 console=tty0 earlycon irqpoll"
#define DEFAULT_RSTVEC     0x00001000
#define CLINT_BASE         0x02000000
#define CLINT_SIZE         0x000c0000
#define PLIC_BASE          0x0c000000
#define PLIC_SIZE          0x01000000
#define PLIC_NDEV          31
#define PLIC_PRIO_BITS     4
#define NS16550_BASE       0x10000000
#define NS16550_SIZE       0x100
#define NS16550_REG_SHIFT  0
#define NS16550_REG_IO_WIDTH 1
#define NS16550_INTERRUPT_ID 1
#define ALTPS2_BASE        0x10000100
#define ALTPS2_SIZE        8
#define ALTPS2_INTERRUPT_ID  2
#define FB_BASE            0x40000000
#define FB_SIZE            1920000
#define EXT_IO_BASE        0x40000000
#define DRAM_BASE          0x80000000

#endif
