/* The LibVMI Library is an introspection library that simplifies access to
 * memory in a target virtual machine or in a file containing a dump of
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government
 * retains certain rights in this software.
 *
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

#include <string.h>
#include <wchar.h>
#include <iconv.h>  // conversion between character sets
#include <errno.h>

#include "private.h"

unicode_string_t *
windows_read_unicode_struct(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid)
{
    unicode_string_t *us = 0;   // return val
    size_t struct_size = 0;
    size_t read = 0;
    addr_t buffer_va = 0;
    uint16_t buffer_len = 0;

    if (VMI_PM_IA32E == vmi_get_page_mode(vmi)) {   // 64 bit guest
        win64_unicode_string_t us64 = { 0 };
        struct_size = sizeof(us64);
        // read the UNICODE_STRING struct
        read = vmi_read_va(vmi, vaddr, pid, &us64, struct_size);
        if (read != struct_size) {
            dbprint(VMI_DEBUG_READ, "--%s: failed to read UNICODE_STRING\n",__FUNCTION__);
            goto out_error;
        }   // if
        buffer_va = (vaddr & 0xFFFFFFFF00000000) + (us64.pBuffer >> 32);
        buffer_len = us64.length;
    }
    else {
        win32_unicode_string_t us32 = { 0 };
        struct_size = sizeof(us32);
        // read the UNICODE_STRING struct
        read = vmi_read_va(vmi, vaddr, pid, &us32, struct_size);
        if (read != struct_size) {
            dbprint(VMI_DEBUG_READ, "--%s: failed to read UNICODE_STRING\n",__FUNCTION__);
            goto out_error;
        }   // if
        buffer_va = us32.pBuffer;
        buffer_len = us32.length;
    }   // if-else

    // allocate the return value
    us = safe_malloc(sizeof(unicode_string_t));

    us->length = buffer_len;
    us->contents = safe_malloc(sizeof(uint8_t) * (buffer_len + 2));

    read = vmi_read_va(vmi, buffer_va, pid, us->contents, us->length);
    if (read != us->length) {
        dbprint
            (VMI_DEBUG_READ, "--%s: failed to read UNICODE_STRING buffer\n",__FUNCTION__);
        goto out_error;
    }   // if

    // end with NULL (needed?)
    us->contents[buffer_len] = 0;
    us->contents[buffer_len + 1] = 0;

    us->encoding = "UTF-16";

    return us;

out_error:
    if (us) {
        if (us->contents) {
            free(us->contents);
        }
        free(us);
    }
    return 0;
}

