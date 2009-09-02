/* Copyright (c) 2009, Steve Maresca/Zentific LLC */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <asm/pgtable.h>

#include <linux/proc_fs.h>

#include "xen_version.h"
#include "hypercalls.h"


#ifndef _Z_MOD_H
#define _Z_MOD_H
typedef uint8_t xen_domain_handle_t[16];
#define SCRATCH_PHYSICAL_ADDRESS      0x00010000
#define HYPERCALL_PHYSICAL_ADDRESS    0x00080000

#endif
