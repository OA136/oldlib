/* The LibVMI Library is an introspection library that simplifies access to
 * memory in a target virtual machine or in a file containing a dump of
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Copyright (C) 2011 Sandia National Laboratories
 * Author: Bryan D. Payne (bdpayne@acm.org)
 *
 * This file is part of LibVMI.
 *
 * LibVMI is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * LibVMI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with LibVMI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "private.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "driver/driver_wrapper.h"

status_t
windows_kernel_symbol_to_address(
    vmi_instance_t vmi,
    const char *symbol,
    addr_t *kernel_base_address,
    addr_t *address)
{
    status_t ret = VMI_FAILURE;
    addr_t rva = 0;
    windows_instance_t windows = vmi->os_data;

    if (windows == NULL || !windows->ntoskrnl_va) {
        goto exit;
    }

    dbprint(VMI_DEBUG_MISC, "--windows symbol lookup (%s)\n", symbol);

    if (windows->rekall_profile) {
        dbprint(VMI_DEBUG_MISC, "--trying Rekall profile\n");

        if (VMI_SUCCESS == rekall_profile_symbol_to_rva(windows->rekall_profile, symbol, NULL, &rva)) {
            *address = windows->ntoskrnl_va + rva;
            dbprint(VMI_DEBUG_MISC, "--got symbol from kernel sysmap (%s --> 0x%.16"PRIx64").\n",
                 symbol, *address);
            ret = VMI_SUCCESS;
            goto success;
        }

        dbprint(VMI_DEBUG_MISC, "--kernel sysmap lookup failed\n");
    }

    if (VMI_SUCCESS == windows_kdbg_lookup(vmi, symbol, address)) {
        dbprint(VMI_DEBUG_MISC, "--got symbol from kdbg (%s --> 0x%"PRIx64").\n", symbol, *address);
        ret = VMI_SUCCESS;
        goto success;
    }

    dbprint(VMI_DEBUG_MISC, "--kdbg lookup failed\n");
    dbprint(VMI_DEBUG_MISC, "--trying kernel PE export table\n");

    /* check exports */
    if (VMI_SUCCESS == windows_export_to_rva(vmi, windows->ntoskrnl_va, 0, symbol, &rva)) {
        *address = windows->ntoskrnl_va + rva;
        dbprint(VMI_DEBUG_MISC, "--got symbol from PE export table (%s --> 0x%.16"PRIx64").\n",
             symbol, *address);
        ret = VMI_SUCCESS;
        goto success;
    }

    dbprint(VMI_DEBUG_MISC, "--kernel PE export table failed\n");

    goto exit;

success:
    if (kernel_base_address) {
        *kernel_base_address = windows->ntoskrnl_va;
    }

exit:
    return ret;
}

/* finds the address of the page global directory for a given pid */
addr_t
windows_pid_to_pgd(
    vmi_instance_t vmi,
    vmi_pid_t pid)
{
    addr_t pgd = 0;
    addr_t eprocess = 0;
    int pid_offset = 0;
    int tasks_offset = 0;
    int pdbase_offset = 0;
    windows_instance_t windows = vmi->os_data;

    if (vmi->os_data == NULL) {
        return VMI_FAILURE;
    }

    pid_offset = windows->pid_offset;
    tasks_offset = windows->tasks_offset;
    pdbase_offset = windows->pdbase_offset;

    /* first we need a pointer to this pid's EPROCESS struct */
    eprocess = windows_find_eprocess_list_pid(vmi, pid);
    if (!eprocess) {
        errprint("Could not find EPROCESS struct for pid = %d.\n", pid);
        goto error_exit;
    }

    /* now follow the pointer to the memory descriptor and grab the pgd value */
    vmi_read_addr_va(vmi, eprocess + pdbase_offset - tasks_offset, 0,
                     &pgd);

error_exit:
    return pgd;
}

vmi_pid_t
windows_pgd_to_pid(
    vmi_instance_t vmi,
    addr_t pgd)
{
    vmi_pid_t pid = -1;
    addr_t eprocess = 0;
    int tasks_offset = 0;
    int pid_offset = 0;
    windows_instance_t windows = vmi->os_data;

    if (vmi->os_data == NULL) {
        return VMI_FAILURE;
    }

    tasks_offset = windows->tasks_offset;
    pid_offset = windows->pid_offset;

    /* first we need a pointer to this pgd's EPROCESS struct */
    eprocess = windows_find_eprocess_list_pgd(vmi, pgd);
    if (!eprocess) {
        errprint("Could not find EPROCESS struct for pgd = 0x%"PRIx64".\n", pgd);
        goto error_exit;
    }

    /* now follow the pointer to the memory descriptor and grab the pgd value */
    vmi_read_32_va(vmi, eprocess + pid_offset - tasks_offset, 0,
                   (uint32_t*)&pid);

error_exit:
    return pid;
}
