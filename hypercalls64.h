/******************************************************************************
 * hypercall64.h
 * 
 * Linux-specific hypervisor handling.
 * 
 * Copyright (c) 2009-2013, Steve Maresca/Zentific LLC 
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

#ifndef __HYPERCALL_64_H__
#define __HYPERCALL_64_H__

//#include <linux/string.h> /* memcpy() */
//#include <linux/stringify.h>

/*
 * The following macros were copied from 
 * linux-2.6.18-xen/include/asm-x86_64/mach-xen/asm/hypercall.h.
 * Normally including xen/hypercall.h should suffice, however,
 * linux-2.6.26 does not seem to support 64 bit architectures for
 * xen purposes.
 */

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


#endif /* __HYPERCALL_64_H__ */
