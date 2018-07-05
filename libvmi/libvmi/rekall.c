/* The LibVMI Library is an introspection library that simplifies access to
 * memory in a target virtual machine or in a file containing a dump of
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Author: Tamas K Lengyel (tamas@tklengyel.com)
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
#include <stdio.h>
#include <json-c/json.h>

status_t
rekall_profile_symbol_to_rva(
    const char *rekall_profile,
    const char *symbol,
    const char *subsymbol,
    addr_t *rva)
{
    status_t ret = VMI_FAILURE;
    addr_t mask = 0;
    if(!rekall_profile || !symbol) {
        return ret;
    }

    json_object *root = json_object_from_file(rekall_profile);
    if(!root) {
        errprint("Rekall profile couldn't be opened!\n");
        return ret;
    }

    if(!subsymbol) {
        json_object *constants = NULL, *jsymbol = NULL;
        if (!json_object_object_get_ex(root, "$CONSTANTS", &constants)) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: no $CONSTANTS section found\n", symbol);
            goto exit;
        }

        if (!json_object_object_get_ex(constants, symbol, &jsymbol)){
            dbprint(VMI_DEBUG_MISC, "Rekall profile: symbol '%s' not found\n", symbol);
            json_object_put(constants);
            goto exit;
        }

        *rva = json_object_get_int64(jsymbol);

        ret = VMI_SUCCESS;

        json_object_put(jsymbol);
        json_object_put(constants);
    } else {
        json_object *structs = NULL, *jstruct = NULL, *jstruct2 = NULL, *jmember = NULL, *jvalue = NULL;
        if (!json_object_object_get_ex(root, "$STRUCTS", &structs)) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: no $STRUCTS section found\n", symbol);
            goto exit;
        }
        if (!json_object_object_get_ex(structs, symbol, &jstruct)) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: no %s found\n", symbol);
            json_object_put(structs);
            goto exit;
        }

        jstruct2 = json_object_array_get_idx(jstruct, 1);
        if (!jstruct2) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: struct %s has no second element\n", symbol);
            json_object_put(jstruct);
            json_object_put(structs);
            goto exit;
        }

        if (!json_object_object_get_ex(jstruct2, subsymbol, &jmember)) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: %s has no %s member\n", symbol, subsymbol);
            json_object_put(jstruct2);
            json_object_put(jstruct);
            json_object_put(structs);
            goto exit;
        }

        jvalue = json_object_array_get_idx(jmember, 0);
        if (!jvalue) {
            dbprint(VMI_DEBUG_MISC, "Rekall profile: %s.%s has no RVA defined\n", symbol, subsymbol);
            json_object_put(jmember);
            json_object_put(jstruct2);
            json_object_put(jstruct);
            json_object_put(structs);
            goto exit;
        }

        *rva = json_object_get_int64(jvalue);
        ret = VMI_SUCCESS;

        json_object_put(jmember);
        json_object_put(jstruct2);
        json_object_put(jstruct);
        json_object_put(structs);
    }

exit:
    json_object_put(root);
    return ret;
}
