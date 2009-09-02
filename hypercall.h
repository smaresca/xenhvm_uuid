/******************************************************************************
 * hypercall.h
 * 
 * Linux-specific hypervisor handling.
 *
 * Copyright (c) 2009, Steve Maresca/Zentific LLC 
 * Copyright (c) 2002-2004, K A Fraser
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __HYPERCALL_H__
#define __HYPERCALL_H__

//#include <linux/string.h> /* memcpy() */
//#include <linux/stringify.h>

/*
 * HYPERCALLS: Copied directly from 
 * linux-2.6.18-xen/include/xen/interface/xen.h
 */

#define __HYPERVISOR_set_trap_table        0
#define __HYPERVISOR_mmu_update            1
#define __HYPERVISOR_set_gdt               2
#define __HYPERVISOR_stack_switch          3
#define __HYPERVISOR_set_callbacks         4
#define __HYPERVISOR_fpu_taskswitch        5
#define __HYPERVISOR_sched_op_compat       6 /* compat since 0x00030101 */
#define __HYPERVISOR_platform_op           7
#define __HYPERVISOR_set_debugreg          8
#define __HYPERVISOR_get_debugreg          9
#define __HYPERVISOR_update_descriptor    10
#define __HYPERVISOR_memory_op            12
#define __HYPERVISOR_multicall            13
#define __HYPERVISOR_update_va_mapping    14
#define __HYPERVISOR_set_timer_op         15
#define __HYPERVISOR_event_channel_op_compat 16 /* compat since 0x00030202 */
#define __HYPERVISOR_xen_version          17
#define __HYPERVISOR_console_io           18
#define __HYPERVISOR_physdev_op_compat    19 /* compat since 0x00030202 */
#define __HYPERVISOR_grant_table_op       20
#define __HYPERVISOR_vm_assist            21
#define __HYPERVISOR_update_va_mapping_otherdomain 22
#define __HYPERVISOR_iret                 23 /* x86 only */
#define __HYPERVISOR_vcpu_op              24
#define __HYPERVISOR_set_segment_base     25 /* x86/64 only */
#define __HYPERVISOR_mmuext_op            26
#define __HYPERVISOR_xsm_op               27
#define __HYPERVISOR_nmi_op               28
#define __HYPERVISOR_sched_op             29
#define __HYPERVISOR_callback_op          30
#define __HYPERVISOR_xenoprof_op          31
#define __HYPERVISOR_event_channel_op     32
#define __HYPERVISOR_physdev_op           33
#define __HYPERVISOR_hvm_op               34
#define __HYPERVISOR_sysctl               35
#define __HYPERVISOR_domctl               36
#define __HYPERVISOR_kexec_op             37

/* Architecture-specific hypercall definitions. */
#define __HYPERVISOR_arch_0               48
#define __HYPERVISOR_arch_1               49
#define __HYPERVISOR_arch_2               50
#define __HYPERVISOR_arch_3               51
#define __HYPERVISOR_arch_4               52
#define __HYPERVISOR_arch_5               53
#define __HYPERVISOR_arch_6               54
#define __HYPERVISOR_arch_7               55


/*
 * The following macros were copied from 
 * linux-2.6.18-xen/include/asm-x86_64/mach-xen/asm/hypercall.h.
 * Normally including xen/hypercall.h should suffice, however,
 * linux-2.6.26 does not seem to support 64 bit architectures for
 * xen purposes.
 */

#if defined(__LP64__)

/* 64-bit hypercalls */

#define _hypercall0(type, name)                 \
({                                              \
        type __res;                             \
        asm volatile (                          \
                HYPERCALL_STR(name)             \
                : "=a" (__res)                  \
                :                               \
                : "memory" );                   \
        __res;                                  \
})

#define _hypercall1(type, name, a1)                             \
({                                                              \
        type __res;                                             \
        long __ign1;                                            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=D" (__ign1)                   \
                : "1" ((long)(a1))                              \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall2(type, name, a1, a2)                         \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2;                                    \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=D" (__ign1), "=S" (__ign2)    \
                : "1" ((long)(a1)), "2" ((long)(a2))            \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall3(type, name, a1, a2, a3)                     \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3;                            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=D" (__ign1), "=S" (__ign2),   \
                "=d" (__ign3)                                   \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                "3" ((long)(a3))                                \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall4(type, name, a1, a2, a3, a4)                 \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3;                            \
        register long __arg4 asm("r10") = (long)(a4);           \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=D" (__ign1), "=S" (__ign2),   \
                  "=d" (__ign3), "+r" (__arg4)                  \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                  "3" ((long)(a3))                              \
                : "memory" );                                   \
        __res;                                                  \
})
#define HYPERCALL_STR(name)                                     \
        "mov $("__stringify(__HYPERVISOR_##name)" * 32),%%eax; "\
        "add hypercall_stubs(%%rip),%%rax; "                    \
        "call *%%rax"

#define _hypercall5(type, name, a1, a2, a3, a4, a5)             \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3;                            \
        register long __arg4 asm("r10") = (long)(a4);           \
        register long __arg5 asm("r8") = (long)(a5);            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=D" (__ign1), "=S" (__ign2),   \
                  "=d" (__ign3), "+r" (__arg4), "+r" (__arg5)   \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                  "3" ((long)(a3))                              \
                : "memory" );                                   \
        __res;                                                  \
})

#else /* if defined(__LP64__) */

/* 32bit hypercalls */

#define _hypercall0(type, name)                 \
({                                              \
        type __res;                             \
        asm volatile (                          \
                HYPERCALL_STR(name)             \
                : "=a" (__res)                  \
                :                               \
                : "memory" );                   \
        __res;                                  \
})

#define _hypercall1(type, name, a1)                             \
({                                                              \
        type __res;                                             \
        long __ign1;                                            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=b" (__ign1)                   \
                : "1" ((long)(a1))                              \
                : "memory" );                                   \
        __res;                                                  \
})
 
#define _hypercall2(type, name, a1, a2)                         \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2;                                    \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=b" (__ign1), "=c" (__ign2)    \
                : "1" ((long)(a1)), "2" ((long)(a2))            \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall3(type, name, a1, a2, a3)                     \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3;                            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=b" (__ign1), "=c" (__ign2),   \
                "=d" (__ign3)                                   \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                "3" ((long)(a3))                                \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall4(type, name, a1, a2, a3, a4)                 \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3, __ign4;                    \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=b" (__ign1), "=c" (__ign2),   \
                "=d" (__ign3), "=S" (__ign4)                    \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                "3" ((long)(a3)), "4" ((long)(a4))              \
                : "memory" );                                   \
        __res;                                                  \
})

#define _hypercall5(type, name, a1, a2, a3, a4, a5)             \
({                                                              \
        type __res;                                             \
        long __ign1, __ign2, __ign3, __ign4, __ign5;            \
        asm volatile (                                          \
                HYPERCALL_STR(name)                             \
                : "=a" (__res), "=b" (__ign1), "=c" (__ign2),   \
                "=d" (__ign3), "=S" (__ign4), "=D" (__ign5)     \
                : "1" ((long)(a1)), "2" ((long)(a2)),           \
                "3" ((long)(a3)), "4" ((long)(a4)),             \
                "5" ((long)(a5))                                \
                : "memory" );                                   \
        __res;                                                  \
})


#endif/* if defined(__LP64__) */

static inline int
HYPERVISOR_arg_test(long a, long b, long c, long d, long e)
{
        return _hypercall5(int, grant_table_op, a, b, c, d, e);
}

static inline int
hypercall_xen_version(
        long cmd, void *arg) 
{   
        return _hypercall2(int, xen_version, cmd, arg);
}   

#endif /* __HYPERCALL_H__ */
