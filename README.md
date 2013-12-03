xenhvm_uuid
===========

A simple kernel module for querying a VM's UUID via a hypercall skeleton. For
use with Linux guests, running in HVM mode (without PV drivers) 
atop the Xen hypervisor.

In essence, this is a Xen hypercall skeleton for HVM guests. This module
could be occasionally useful for management purposes to obtain a VM UUID from
within the guest rather than via the control plane. Examples: legacy systems
running in HVM and lacking PV driver support, hypercall fuzzing.
 
Offers information via /proc/zentific including the uuid of the VM and
major/minor version information from the underlying hypervisor.

Side comment: Yes, /proc is 'deprecated.' It was convenient. :)  I'll update
this if anyone ever actually cares.
 
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

FYI
------------
This code, while it still does work, has not been updated since 2009; since that time,
the march of progress in Xen has been swift, and many new hypercalls have been created.
Hypercall numbers are backwards compatible due to the stable interface, but nevertheless
some work could be done to bring this code up to a more modern state.

Legal
------------
Copyright 2009-2013 Zentific LLC Steve Maresca <steve@zentific.com>
