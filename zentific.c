/* Copyright (c) 2009-2013, Steve Maresca/Zentific LLC */

#include "zentific.h"

MODULE_AUTHOR("steve@zentific.com");
MODULE_DESCRIPTION("Xen hypercall skeleton for HVM guests. Offers information via /proc/zentific");
MODULE_LICENSE("GPL");

/* -------------------------------------------------------------------------*/

#define MODULE_NAME "zentific"
char *hypercall_stubs;
xen_domain_handle_t uuid;
static struct proc_dir_entry *proc_dir, *uuid_file, *xen_ver_file;
uint32_t xen_major, xen_minor;

int print_uuid(void){

    printk(KERN_INFO "Guest UUID %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);

    return 0;
}

static int proc_read_uuid(char *page, char **start,
                            off_t off, int count, 
                            int *eof, void *data) {
        int len;

        len = sprintf(page, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
        uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
        
        return len;
}

static int proc_read_xenver(char *page, char **start,
                            off_t off, int count, 
                            int *eof, void *data) {
    int len;

    len = sprintf(page, "%d.%d\n", xen_major, xen_minor);

    return len;
}

static int __init init_hypercall(void) {

    uint32_t eax, ebx, ecx, edx, pages, msr, i;
    char signature[13];

    cpuid(0x40000000, &eax, &ebx, &ecx, &edx);
    *(uint32_t*)(signature + 0) = ebx;
    *(uint32_t*)(signature + 4) = ecx;
    *(uint32_t*)(signature + 8) = edx;
    signature[12] = 0;

    if (strcmp("XenVMMXenVMM", signature) || (eax < 0x40000002)) {
        printk(KERN_WARNING
               "Detected Xen platform device but not Xen VMM?"
               " (sig %s, eax %x)\n",
               signature, eax);
        return -EINVAL;
    }

    cpuid(0x40000001, &eax, &ebx, &ecx, &edx);

    printk(KERN_INFO "Xen version %d.%d.\n", eax >> 16, eax & 0xffff);
    /* save version info */
    xen_major = eax >> 16;
    xen_minor = eax & 0xffff;

    /*
     * Find largest supported number of hypercall pages.
     * We'll create as many as possible up to this number.
     */
    cpuid(0x40000002, &pages, &msr, &ecx, &edx);

    /*
     * Use __vmalloc() because vmalloc_exec() is not an exported symbol.
     * PAGE_KERNEL_EXEC also is not exported, hence we use PAGE_KERNEL.
     * hypercall_stubs = vmalloc_exec(pages * PAGE_SIZE);
     */
    while (pages > 0) {
        hypercall_stubs = __vmalloc(
            pages * PAGE_SIZE,
            GFP_KERNEL, // | __GFP_HIGHMEM,
            __pgprot(__PAGE_KERNEL & ~_PAGE_NX));
        if (hypercall_stubs != NULL)
            break;
        pages--; /* vmalloc failed: try one fewer pages */
    }

    if (hypercall_stubs == NULL)
        return -ENOMEM;

    for (i = 0; i < pages; i++) {
        unsigned long pfn;
        pfn = vmalloc_to_pfn((char *)hypercall_stubs + i*PAGE_SIZE);
        wrmsrl(msr, ((u64)pfn << PAGE_SHIFT) + i);
    }

    printk(KERN_INFO "Hypercall area is %u pages.\n", pages);
    hypercall_xen_version(XENVER_guest_handle, uuid);
    print_uuid();

    return 0;
}

int proc_plumbing(void){

    /* create directory */
    proc_dir = proc_mkdir(MODULE_NAME, NULL);
    if(proc_dir == NULL) {
        return -ENOMEM;
    }

    /* create jiffies using convenience function */
    uuid_file = create_proc_read_entry("uuid", 
                                          0444, proc_dir, 
                                          proc_read_uuid,
                                          NULL);
    if( uuid_file == NULL) {
            return  -ENOMEM;
    }

    /* create jiffies using convenience function */
    xen_ver_file = create_proc_read_entry("xen_ver", 
                                          0444, proc_dir, 
                                          proc_read_xenver,
                                          NULL);
    if( xen_ver_file == NULL) {
        return  -ENOMEM;
    }

    return 0;
}

int init_module(void) {

    if ( init_hypercall() == 0)
        printk(KERN_INFO MODULE_NAME ": initialized.\n");

    if ( proc_plumbing() == 0 ) {
        printk(MODULE_NAME ": exported xen version and UUID via procfs.\n");
    } else {
        printk(MODULE_NAME ": Could not export xen version or UUID via procfs.\n");
    }

    return 0;
}

void cleanup_module(void) {
    remove_proc_entry("uuid", proc_dir);
    remove_proc_entry("xen_ver", proc_dir);
    remove_proc_entry(MODULE_NAME, NULL);
    printk(KERN_INFO MODULE_NAME ": uninitialized.\n");
}

//module_init(init_module);
