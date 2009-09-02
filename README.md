xenhvm_uuid
===========

A simple kernel module for querying a VM's UUID via a hypercall skeleton. For
use with Linux guests, running in HVM mode (without PV drivers) 
atop the Xen hypervisor.

In essence, this is a Xen hypercall skeleton for HVM guests. Linux is rarely
run in this mode unless there is some operational requirement imposing
full hardware virtualization instead of paravirtualization. That said,
it is occasionally useful for management purposes to obtain a VM UUID from
within the guest rather than via the control plane.
 
Offers information via /proc/zentific including the uuid of the VM and
major/minor version information from the underlying hypervisor.

Yes, /proc is 'deprecated.' It was convenient. :)  I'll update this if
anyone ever actually cares.
 
 
Instructions
-------------
1. Build the module
 * make sure kernel headers are installed
 * make sure a full development toolchain is installed
 * run 'make'
2. /sbin/insmod zentific.ko
 * alternatively use modprobe if preference/requirement dictates.
3. run dmesg to see some status and initialization output
4. cat /proc/zentific/{uuid,xen_ver}
5. /sbin/rmmod zentific

Legal
------------
Copyright 2009 Zentific LLC Steve Maresca <steve@zentific.com>
