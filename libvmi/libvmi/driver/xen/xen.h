/* The LibVMI Library is an introspection library that simplifies access to
 * memory in a target virtual machine or in a file containing a dump of
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government
 * retains certain rights in this software.
 *
 * Author: Bryan D. Payne (bdpayne@acm.org)
 * Author: Tamas K Lengyel (tamas.lengyel@zentific.com)
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

#ifndef XEN_DRIVER_H
#define XEN_DRIVER_H

#if ENABLE_XEN_EVENTS == 1
 #include "driver/xen/xen_events.h"
#endif

status_t xen_init(
    vmi_instance_t vmi);
status_t xen_init_vmi(
    vmi_instance_t vmi);
void xen_destroy(
    vmi_instance_t vmi);
uint64_t xen_get_domainid_from_name(
    vmi_instance_t vmi,
    const char *name);
status_t xen_get_name_from_domainid(
    vmi_instance_t vmi,
    uint64_t domainid,
    char **name);
uint64_t xen_get_domainid(
    vmi_instance_t vmi);
void xen_set_domainid(
    vmi_instance_t vmi,
    uint64_t domainid);
status_t xen_check_domainid(
    vmi_instance_t vmi,
    uint64_t domainid);
status_t xen_get_domainname(
    vmi_instance_t vmi,
    char **name);
void xen_set_domainname(
    vmi_instance_t vmi,
    const char *name);
status_t xen_get_memsize(
    vmi_instance_t vmi,
    uint64_t *allocated_ram_size,
    addr_t *maximum_physical_address);
status_t xen_get_vcpureg(
    vmi_instance_t vmi,
    reg_t *value,
    registers_t reg,
    unsigned long vcpu);
status_t
xen_set_vcpureg(
    vmi_instance_t vmi,
    reg_t value,
    registers_t reg,
    unsigned long vcpu);
status_t xen_get_address_width(
    vmi_instance_t vmi,
    uint8_t * width_in_bytes);
void *xen_read_page(
    vmi_instance_t vmi,
    addr_t page);
status_t xen_write(
    vmi_instance_t vmi,
    addr_t paddr,
    void *buf,
    uint32_t length);
int xen_is_pv(
    vmi_instance_t vmi);
status_t xen_test(
    uint64_t domainid,
    const char *name);
status_t xen_pause_vm(
    vmi_instance_t vmi);
status_t xen_resume_vm(
    vmi_instance_t vmi);
status_t xen_set_domain_debug_control(
    vmi_instance_t vmi,
    unsigned long vcpu,
    int enable);
status_t xen_create_shm_snapshot(
    vmi_instance_t vmi);
status_t xen_destroy_shm_snapshot(
    vmi_instance_t vmi);
size_t xen_get_dgpma(
    vmi_instance_t vmi,
    addr_t paddr,
    void** medial_addr_ptr,
    size_t count);

static inline status_t
driver_xen_setup(vmi_instance_t vmi)
{
    driver_interface_t driver = { 0 };
    driver.initialized = true;
    driver.init_ptr = &xen_init;
    driver.init_vmi_ptr = &xen_init_vmi;
    driver.destroy_ptr = &xen_destroy;
    driver.get_id_from_name_ptr = &xen_get_domainid_from_name;
    driver.get_name_from_id_ptr = &xen_get_name_from_domainid;
    driver.get_id_ptr = &xen_get_domainid;
    driver.set_id_ptr = &xen_set_domainid;
    driver.check_id_ptr = &xen_check_domainid;
    driver.get_name_ptr = &xen_get_domainname;
    driver.set_name_ptr = &xen_set_domainname;
    driver.get_memsize_ptr = &xen_get_memsize;
    driver.get_vcpureg_ptr = &xen_get_vcpureg;
    driver.set_vcpureg_ptr = &xen_set_vcpureg;
    driver.get_address_width_ptr = &xen_get_address_width;
    driver.read_page_ptr = &xen_read_page;
    driver.write_ptr = &xen_write;
    driver.is_pv_ptr = &xen_is_pv;
    driver.pause_vm_ptr = &xen_pause_vm;
    driver.resume_vm_ptr = &xen_resume_vm;
#if ENABLE_SHM_SNAPSHOT == 1
    driver.create_shm_snapshot_ptr = &xen_create_shm_snapshot;
    driver.destroy_shm_snapshot_ptr = &xen_destroy_shm_snapshot;
    driver.get_dgpma_ptr = &xen_get_dgpma;
#endif
#if ENABLE_XEN_EVENTS == 1
    driver.events_listen_ptr = &xen_events_listen;
    driver.are_events_pending_ptr = &xen_are_events_pending;
    driver.set_reg_access_ptr = &xen_set_reg_access;
    driver.set_intr_access_ptr = &xen_set_intr_access;
    driver.set_mem_access_ptr = &xen_set_mem_access;
    driver.start_single_step_ptr = &xen_start_single_step;
    driver.stop_single_step_ptr = &xen_stop_single_step;
    driver.shutdown_single_step_ptr = &xen_shutdown_single_step;
#endif
    vmi->driver = driver;
    return VMI_SUCCESS;
}

#endif /* XEN_DRIVER_H */
