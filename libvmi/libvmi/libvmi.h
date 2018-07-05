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

/**
 * @file libvmi.h
 * @brief The LibVMI API is defined here.
 *
 * More detailed description can go here.
 */
#ifndef LIBVMI_H
#define LIBVMI_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

typedef uint32_t vmi_mode_t;

/* These will be used in conjuction with vmi_mode_t variables */

#define VMI_AUTO (1 << 0)  /**< libvmi should detect what to monitor or view */

#define VMI_XEN  (1 << 1)  /**< libvmi is monitoring a Xen VM */

#define VMI_KVM  (1 << 2)  /**< libvmi is monitoring a KVM VM */

#define VMI_FILE (1 << 3)  /**< libvmi is viewing a file on disk */

#define VMI_INIT_PARTIAL  (1 << 16) /**< init enough to view physical addresses */

#define VMI_INIT_COMPLETE (1 << 17) /**< full initialization */

#define VMI_INIT_EVENTS (1 << 18) /**< init support for VM events */

#define VMI_INIT_SHM_SNAPSHOT (1 << 19) /**< setup shm-snapshot in vmi_init() if the feature is activated */

#define VMI_CONFIG_NONE (1 << 24) /**< no config provided */

#define VMI_CONFIG_GLOBAL_FILE_ENTRY (1 << 25) /**< config in file provided */

#define VMI_CONFIG_STRING (1 << 26) /**< config string provided */

#define VMI_CONFIG_GHASHTABLE (1 << 27) /**< config GHashTable provided */

#define VMI_INVALID_DOMID ~0ULL /**< invalid domain id */

typedef enum status {

    VMI_SUCCESS,  /**< return value indicating success */

    VMI_FAILURE   /**< return value indicating failure */
} status_t;

typedef enum os {

    VMI_OS_UNKNOWN,  /**< OS type is unknown */

    VMI_OS_LINUX,    /**< OS type is Linux */

    VMI_OS_WINDOWS   /**< OS type is Windows */
} os_t;

typedef enum win_ver {

    VMI_OS_WINDOWS_NONE,    /**< Not Windows */
    VMI_OS_WINDOWS_UNKNOWN, /**< Is Windows, not sure which */

    VMI_OS_WINDOWS_2000     = 0x0208U, /**< Magic value for Windows 2000 */
    VMI_OS_WINDOWS_XP       = 0x0290U, /**< Magic value for Windows XP */
    VMI_OS_WINDOWS_2003     = 0x0318U, /**< Magic value for Windows 2003 */
    VMI_OS_WINDOWS_VISTA    = 0x0328U, /**< Magic value for Windows Vista */
    VMI_OS_WINDOWS_2008     = 0x0330U, /**< Magic value for Windows 2008 */
    VMI_OS_WINDOWS_7        = 0x0340U, /**< Magic value for Windows 7 */
    VMI_OS_WINDOWS_8        = 0x0360U  /**< Magic value for Windows 8 */
} win_ver_t;

typedef enum page_mode {

    VMI_PM_UNKNOWN, /**< page mode unknown */

    VMI_PM_LEGACY,  /**< x86 32-bit paging */

    VMI_PM_PAE,     /**< x86 PAE paging */

    VMI_PM_IA32E,   /**< x86 IA-32e paging */

    VMI_PM_AARCH32  /**< ARM 32-bit paging */
} page_mode_t;

typedef enum page_size {

    VMI_PS_UNKNOWN  = 0ULL,         /**< page size unknown */

    VMI_PS_1KB      = 0x400ULL,     /**< 1KB */

    VMI_PS_4KB      = 0x1000ULL,    /**< 4KB */

    VMI_PS_64KB     = 0x10000ULL,   /**< 64KB */

    VMI_PS_1MB      = 0x100000ULL,  /**< 1MB */

    VMI_PS_2MB      = 0x200000ULL,  /**< 2MB */

    VMI_PS_4MB      = 0x400000ULL,  /**< 4MB */

    VMI_PS_16MB     = 0x1000000ULL, /**< 16MB */

    VMI_PS_1GB      = 0x4000000ULL  /**< 1GB */

} page_size_t;

typedef uint64_t reg_t;
typedef enum registers {
    /**
     * x86* registers
     */
    RAX,
    RBX,
    RCX,
    RDX,
    RBP,
    RSI,
    RDI,
    RSP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,

    RIP,
    RFLAGS,

    CR0,
    CR2,
    CR3,
    CR4,
    XCR0,

    DR0,
    DR1,
    DR2,
    DR3,
    DR6,
    DR7,

    CS_SEL,
    DS_SEL,
    ES_SEL,
    FS_SEL,
    GS_SEL,
    SS_SEL,
    TR_SEL,
    LDTR_SEL,

    CS_LIMIT,
    DS_LIMIT,
    ES_LIMIT,
    FS_LIMIT,
    GS_LIMIT,
    SS_LIMIT,
    TR_LIMIT,
    LDTR_LIMIT,
    IDTR_LIMIT,
    GDTR_LIMIT,

    CS_BASE,
    DS_BASE,
    ES_BASE,
    FS_BASE,
    GS_BASE,
    SS_BASE,
    TR_BASE,
    LDTR_BASE,
    IDTR_BASE,
    GDTR_BASE,

    CS_ARBYTES,
    DS_ARBYTES,
    ES_ARBYTES,
    FS_ARBYTES,
    GS_ARBYTES,
    SS_ARBYTES,
    TR_ARBYTES,
    LDTR_ARBYTES,

    SYSENTER_CS,
    SYSENTER_ESP,
    SYSENTER_EIP,

    SHADOW_GS,

    MSR_FLAGS,
    MSR_LSTAR,
    MSR_CSTAR,
    MSR_SYSCALL_MASK,
    MSR_EFER,
    MSR_TSC_AUX,

    /**
     * Special generic case for handling MSRs, given their understandably
     * generic treatment for events in Xen and elsewhere. Not relevant for
     * vCPU get/set of register data.
     */
    MSR_ALL,

    TSC,

    /**
     * ARM32 Registers
     */
    SCTLR,

    TTBCR,
    TTBR0,
    TTBR1,

    R0_USR,
    R1_USR,
    R2_USR,
    R3_USR,
    R4_USR,
    R5_USR,
    R6_USR,
    R7_USR,
    R8_USR,
    R9_USR,
    R10_USR,
    R11_USR,
    R12_USR,

    SP_USR,
    LR_USR,

    LR_IRQ,
    SP_IRQ,

    LR_SVC,
    SP_SVC,

    LR_ABT,
    SP_ABT,

    LR_UND,
    SP_UND,

    R8_FIQ,
    R9_FIQ,
    R10_FIQ,
    R11_FIQ,
    R12_FIQ,

    SP_FIQ,
    LR_FIQ,

    PC32,

    SPSR_SVC,

    SPSR_FIQ,
    SPSR_IRQ,
    SPSR_UND,
    SPSR_ABT
} registers_t;

/**
 * typedef for forward compatibility with 64-bit guests
 */
typedef uint64_t addr_t;

/**
 * type def for consistent pid_t usage
 */
typedef int32_t vmi_pid_t;

/**
 * Struct for holding page lookup information
 */
typedef struct page_info {
    addr_t vaddr;       /**< virtual address */
    addr_t dtb;         /**< dtb used for translation */
    addr_t paddr;       /**< physical address */
    page_size_t size;   /**< page size (VMI_PS_*) */

    union {
        struct {
            addr_t pte_location;
            addr_t pte_value;
            addr_t pgd_location;
            addr_t pgd_value;
        } x86_legacy;

        struct {
            addr_t pte_location;
            addr_t pte_value;
            addr_t pgd_location;
            addr_t pgd_value;
            addr_t pdpe_location;
            addr_t pdpe_value;
        } x86_pae;

        struct {
            addr_t pte_location;
            addr_t pte_value;
            addr_t pgd_location;
            addr_t pgd_value;
            addr_t pdpte_location;
            addr_t pdpte_value;
            addr_t pml4e_location;
            addr_t pml4e_value;
        } x86_ia32e;

        struct {
            uint32_t fld_location;
            uint32_t fld_value;
            uint32_t sld_location;
            uint32_t sld_value;
        } arm_aarch32;
    };
} page_info_t;

/**
 * Available translation mechanism for v2p conversion.
 */
typedef enum translation_mechanism {
    VMI_TM_INVALID,         /**< Invalid translation mechanism */
    VMI_TM_NONE,            /**< No translation is required, address is physical address */
    VMI_TM_PROCESS_DTB,     /**< Translate addr via specified directory table base. */
    VMI_TM_PROCESS_PID,     /**< Translate addr by finding process first to use its DTB. */
    VMI_TM_KERNEL_SYMBOL    /**< Find virtual address of kernel symbol and translate it via kernel DTB. */
} translation_mechanism_t;

/**
 * Structure to use as input to accessor functions
 * specifying how the access should be performed.
 */
typedef struct {
    translation_mechanism_t translate_mechanism;

    addr_t addr;    /**< specify iff using VMI_TM_NONE, VMI_TM_PROCESS_DTB or VMI_TM_PROCESS_PID */
    char *ksym;     /**< specify iff using VMI_TM_KERNEL_SYMBOL */
    addr_t dtb;     /**< specify iff using VMI_TM_PROCESS_DTB */
    vmi_pid_t pid;  /**< specify iff using VMI_TM_PROCESS_PID */
} access_context_t;

/**
 * Macro to test bitfield values (up to 64-bits)
 */
#define VMI_GET_BIT(reg, bit) (!!(reg & (1ULL<<bit)))

/**
 * Macro to compute bitfield masks (up to 64-bits)
 */
#define VMI_BIT_MASK(a, b) (((unsigned long long) -1 >> (63 - (b))) & ~((1ULL << (a)) - 1))

/**
 * Generic representation of Unicode string to be used within libvmi
 */
typedef struct _ustring {

    size_t length;         /**< byte count of contents */

    uint8_t *contents;     /**< pointer to byte array holding string */

    const char *encoding;  /**< holds iconv-compatible encoding of contents; do not free */
} unicode_string_t;

/**
 * Custom config input source
 */
typedef void* vmi_config_t;

/**
 * @brief LibVMI Instance.
 *
 * This struct holds all of the relavent information for an instance of
 * LibVMI.  Each time a new domain is accessed, a new instance must
 * be created using the vmi_init function.  When you are done with an instance,
 * its resources can be freed using the vmi_destroy function.
 */
typedef struct vmi_instance *vmi_instance_t;

/*---------------------------------------------------------
 * Initialization and Destruction functions from core.c
 */

/**
 * Initializes access to a specific VM or file given a name.  All
 * calls to vmi_init must eventually call vmi_destroy.
 *
 * This is a costly funtion in terms of the time needed to execute.
 * You should call this function only once per VM or file, and then use the
 * resulting instance when calling any of the other library functions.
 *
 * @param[out] vmi Struct that holds instance information
 * @param[in] flags VMI_AUTO, VMI_XEN, VMI_KVM, or VMI_FILE plus
 *  VMI_INIT_PARTIAL or VMI_INIT_COMPLETE
 * @param[in] name Unique name specifying the VM or file to view
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_init(
    vmi_instance_t *vmi,
    uint32_t flags,
    const char *name);

/**
 * Initializes access to a specific VM with a custom configuration source.  All
 * calls to vmi_init_custom must eventually call vmi_destroy.
 *
 * This is a costly funtion in terms of the time needed to execute.
 * You should call this function only once per VM or file, and then use the
 * resulting instance when calling any of the other library functions.
 *
 * @param[out] vmi Struct that holds instance information
 * @param[in] flags VMI_AUTO, VMI_XEN, VMI_KVM, or VMI_FILE plus
 *  VMI_INIT_PARTIAL or VMI_INIT_COMPLETE plus
 *  VMI_CONFIG_FILE/STRING/GHASHTABLE
 * @param[in] config Pointer to the specified configuration structure
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_init_custom(
    vmi_instance_t *vmi,
    uint32_t flags,
    vmi_config_t config);

/**
 * Completes initialization.  Call this after calling vmi_init with
 * VMI_INIT_PARTIAL.  Calling this at any other time results in undefined
 * behavior.  The partial init provides physical memory access only.  So
 * the purpose of this function is to allow for a staged init of LibVMI.
 * You can gain physical memory access, run some heuristics to obtain
 * the necessary offsets, and then complete the init.
 *
 * @param[in,out] vmi Struct that holds the instance information and was
 *  passed to vmi_init with a VMI_INIT_PARTIAL flag
 * @param[in] config Pointer to a string containing the config entries for
 *  this domain.  Entries should be specified as in the config file
 *  (e.g., '{ostype = "Windows"; win_tasks = 0x88; win_pdbase = 0x18; ...}').
 *  If this is NULL, then the config is pulled from /etc/libvmi.conf.
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_init_complete(
    vmi_instance_t *vmi,
    const char *config);

/**
 * Completes initialization.  Call this after calling vmi_init or vmi_init_custom
 * with VMI_INIT_PARTIAL.  Calling this at any other time results in undefined
 * behavior.  The partial init provides physical memory access only.  So
 * the purpose of this function is to allow for a staged init of LibVMI.
 * You can gain physical memory access, run some heuristics to obtain
 * the necessary offsets, and then complete the init.
 *
 * @param[in,out] vmi Struct that holds the instance information and was
 *  passed to vmi_init with a VMI_INIT_PARTIAL flag
 * @param[in] flags VMI_CONFIG_FILE/STRING/GHASHTABLE
 * @param[in] config Pointer to a structure containing the config entries for
 *  this domain.
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_init_complete_custom(
    vmi_instance_t *vmi,
    uint32_t flags,
    vmi_config_t config);

/**
 * Initialize or reinitialize the paging specific functionality of LibVMI.
 * This function is most useful when dynamically monitoring the booting of
 * an OS in VMI_INIT_PARTIAL mode.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] force_reinit Force the reinitialization of the paging mode
 *  even if one was already setup previously.
 * @return The current page mode of the architecture or VMI_PM_UNKNOWN.
 */
page_mode_t vmi_init_paging(
    vmi_instance_t vmi,
    uint8_t force_reinit);

/**
 * Destroys an instance by freeing memory and closing any open handles.
 *
 * @param[in] vmi Instance to destroy
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_destroy(
    vmi_instance_t vmi);

/*---------------------------------------------------------
 * Memory translation functions from memory.c
 */

/**
 * Performs the translation from a kernel virtual address to a
 * physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Desired kernel virtual address to translate
 * @return Physical address, or zero on error
 */
addr_t vmi_translate_kv2p(
    vmi_instance_t vmi,
    addr_t vaddr);

/**
 * Performs the translation from a user virtual address to a
 * physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Desired kernel virtual address to translate
 * @param[in] pid Process id for desired user address space
 * @return Physical address, or zero on error
 */
addr_t vmi_translate_uv2p(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid);

/**
 * Performs the translation from a kernel symbol to a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] symbol Desired kernel symbol to translate
 * @return Virtual address, or zero on error
 */
addr_t vmi_translate_ksym2v(
    vmi_instance_t vmi,
    const char *symbol);

/**
 * Performs the translation from a symbol to a virtual address.
 * On Windows this function walks the PE export table.
 * Linux is unimplemented at this time.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] base_vaddr Base virtual address (beginning of PE header in Windows)
 * @param[in] pid PID
 * @param[in] symbol Desired symbol to translate
 * @return Virtual address, or zero on error
 */
addr_t vmi_translate_sym2v(
    vmi_instance_t vmi,
    addr_t base_vaddr,
    vmi_pid_t pid,
    const char *symbol);

/**
 * Performs the translation from an RVA to a symbol
 * On Windows this function walks the PE export table.
 * The Linux translation works currently only for kernel symbols (pid=0 & base_vaddr=0).
 * Only the first matching symbol of System.map is returned.
 * ELF Headers are not supported.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] base_vaddr Base virtual address (beginning of PE header in Windows)
 * @param[in] pid PID
 * @param[in] rva RVA to translate
 * @return Symbol, or NULL on error
 */
const char* vmi_translate_v2sym(
    vmi_instance_t vmi,
    addr_t base_vaddr,
    vmi_pid_t pid,
    addr_t rva);

/**
 * Given a pid, this function returns the virtual address of the
 * directory table base for this process' address space.  This value
 * is effectively what would be in the CR3 register while this process
 * is running.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] pid Desired process id to lookup
 * @return The directory table base virtual address for \a pid
 */
addr_t vmi_pid_to_dtb(
    vmi_instance_t vmi,
    vmi_pid_t pid);

/**
 * Given a dtb, this function returns the PID corresponding to the
 * virtual address of the directory table base.
 * This function does NOT implement caching as to avoid false mappings.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] dtb Desired dtb to lookup
 * @return The PID corresponding to the dtb
 */
vmi_pid_t vmi_dtb_to_pid(
    vmi_instance_t vmi,
    addr_t dtb);

/**
 * Translates a virtual address to a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] dtb address of the relevant page directory base
 * @param[in] vaddr virtual address to translate via dtb
 * @return Physical address, or zero on error
 */

addr_t vmi_pagetable_lookup (
    vmi_instance_t vmi,
    addr_t dtb,
    addr_t vaddr);

/**
 * Gets the physical address and page size of the VA
 * as well as the addresses of other paging related structures
 * depending on the page mode of the VM.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] dtb address of the relevant page directory base
 * @param[in] vaddr virtual address to translate via dtb
 * @param[in,out] info Pointer to the struct to store the lookup information in
 * @return VMI_SUCCESS or VMI_FAILURE of the VA is invalid
 */
status_t vmi_pagetable_lookup_extended(
    vmi_instance_t vmi,
    addr_t dtb,
    addr_t vaddr,
    page_info_t *info);

/*---------------------------------------------------------
 * Memory access functions
 */

/**
 * Reads \a count bytes from memory and stores the output in \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[out] buf The data read from memory
 * @param[in] count The number of bytes to read
 * @return The number of bytes read.
 */
size_t vmi_read(
    vmi_instance_t vmi,
    access_context_t *ctx,
    void *buf,
    size_t count);

/**
 * Reads 8 bits from memory.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_8(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint8_t * value);

/**
 * Reads 16 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_16(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint16_t * value);

/**
 * Reads 32 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_32(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint32_t * value);

/**
 * Reads 64 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_64(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint64_t * value);

/**
 * Reads an address from memory, given a virtual address.  The number of
 * bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_addr(
    vmi_instance_t vmi,
    access_context_t *ctx,
    addr_t *value);

/**
 * Reads a null terminated string from memory, starting at
 * the given virtual address.  The returned value must be
 * freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @return String read from memory or NULL on error
 */
char *vmi_read_str(
    vmi_instance_t vmi,
    access_context_t *ctx);

/**
 * Reads \a count bytes from memory located at the kernel symbol \a sym
 * and stores the output in \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] buf The data read from memory
 * @param[in] count The number of bytes to read
 * @return The number of bytes read.
 */
size_t vmi_read_ksym(
    vmi_instance_t vmi,
    char *sym,
    void *buf,
    size_t count);

/**
 * Reads \a count bytes from memory located at the virtual address \a vaddr
 * and stores the output in \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] buf The data read from memory
 * @param[in] count The number of bytes to read
 * @return The number of bytes read.
 */
size_t vmi_read_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    void *buf,
    size_t count);

/**
 * Reads \a count bytes from memory located at the physical address \a paddr
 * and stores the output in \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] buf The data read from memory
 * @param[in] count The number of bytes to read
 * @return The number of bytes read.
 */
size_t vmi_read_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    void *buf,
    size_t count);

/**
 * Reads 8 bits from memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_8_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint8_t * value);

/**
 * Reads 16 bits from memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_16_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint16_t * value);

/**
 * Reads 32 bits from memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_32_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint32_t * value);

/**
 * Reads 64 bits from memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_64_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint64_t * value);

/**
 * Reads an address from memory, given a kernel symbol.  The number of
 * bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_addr_ksym(
    vmi_instance_t vmi,
    char *sym,
    addr_t *value);

/**
 * Reads a null-terminated string from memory, starting at
 * the given kernel symbol.  The returned value must be
 * freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol for memory location where string starts
 * @return String read from memory or NULL on error
 */
char *vmi_read_str_ksym(
    vmi_instance_t vmi,
    char *sym);

/**
 * Reads 8 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_8_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint8_t * value);

/**
 * Reads 16 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_16_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint16_t * value);

/**
 * Reads 32 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_32_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint32_t * value);

/**
 * Reads 64 bits from memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_64_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint64_t * value);

/**
 * Reads an address from memory, given a virtual address.  The number of
 * bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to read from
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_addr_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    addr_t *value);

/**
 * Reads a null terminated string from memory, starting at
 * the given virtual address.  The returned value must be
 * freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address for start of string
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @return String read from memory or NULL on error
 */
char *vmi_read_str_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid);

/**
 * Reads a Unicode string from the given address. If the guest is running
 * Windows, a UNICODE_STRING struct is read. Linux is not yet
 * supported. The returned value must be freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address of the UNICODE_STRING structure
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @return String read from memory or NULL on error; this function
 *         will set the encoding field.
 */
unicode_string_t *vmi_read_unicode_str_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid);

/**
 * Converts character encoding from that in the input string to another
 * specified encoding. Two common ways to use this function are: (1) convert a
 * string to the "UTF-8" encoding and output with printf("%s"); (2) convert a
 * string to the "WCHAR_T" encoding and output with printf("%ls").
 *
 * @param[in] in  unicode_string_t to be converted; encoding field must be set
 * @param[in] out output unicode_string_t, allocated by caller (this function allocates the contents field)
 * @param[in] outencoding output encoding, must be compatible with the iconv function
 * @return status code
 */
status_t vmi_convert_str_encoding(
    const unicode_string_t *in,
    unicode_string_t *out,
    const char *outencoding);

/**
 * Convenience function to free a unicode_string_t struct.
 *
 * @param[in] p_us Pointer to a unicode_string_t struct
 */
void vmi_free_unicode_str(
    unicode_string_t *p_us);

/**
 * Reads 8 bits from memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_8_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint8_t * value);

/**
 * Reads 16 bits from memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_16_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint16_t * value);

/**
 * Reads 32 bits from memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_32_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint32_t * value);

/**
 * Reads 64 bits from memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_64_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint64_t * value);

/**
 * Reads an address from memory, given a physical address.  The number of
 * bytes read is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to read from
 * @param[out] value The value read from memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_read_addr_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    addr_t *value);

/**
 * Reads a nul terminated string from memory, starting at
 * the given physical address.  The returned value must be
 * freed by the caller.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address for start of string
 * @return String read from memory or NULL on error
 */
char *vmi_read_str_pa(
    vmi_instance_t vmi,
    addr_t paddr);


/**
 * Writes \a count bytes to memory
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] buf The data written to memory
 * @param[in] count The number of bytes to write
 * @return The number of bytes written.
 */
size_t vmi_write(
    vmi_instance_t vmi,
    access_context_t *ctx,
    void *buf,
    size_t count);

/**
 * Writes \a count bytes to memory located at the kernel symbol \a sym
 * from \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] buf The data written to memory
 * @param[in] count The number of bytes to write
 * @return The number of bytes written.
 */
size_t vmi_write_ksym(
    vmi_instance_t vmi,
    char *sym,
    void *buf,
    size_t count);

/**
 * Writes \a count bytes to memory located at the virtual address \a vaddr
 * from \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] buf The data written to memory
 * @param[in] count The number of bytes to write
 * @return The number of bytes written.
 */
size_t vmi_write_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    void *buf,
    size_t count);

/**
 * Writes \a count bytes to memory located at the physical address \a paddr
 * from \a buf.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] buf The data written to memory
 * @param[in] count The number of bytes to write
 * @return The number of bytes written.
 */
size_t vmi_write_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    void *buf,
    size_t count);

/**
 * Writes 8 bits to memory
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_8(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint8_t * value);

/**
 * Writes 16 bits to memory
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_16(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint16_t * value);

/**
 * Writes 32 bits to memory
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_32(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint32_t * value);

/**
 * Writes 64 bits to memory
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_64(
    vmi_instance_t vmi,
    access_context_t *ctx,
    uint64_t * value);

/**
 * Writes the address to memory. The number of
 * bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] ctx Access context
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_addr(
    vmi_instance_t vmi,
    access_context_t *ctx,
    addr_t * value);

/**
 * Writes 8 bits to memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_8_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint8_t * value);

/**
 * Writes 16 bits to memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_16_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint16_t * value);

/**
 * Writes 32 bits to memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_32_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint32_t * value);

/**
 * Writes 64 bits to memory, given a kernel symbol.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_64_ksym(
    vmi_instance_t vmi,
    char *sym,
    uint64_t * value);

/**
 * Writes the address to memory. The number of
 * bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_addr_ksym(
    vmi_instance_t vmi,
    char *sym,
    addr_t * value);

/**
 * Writes 8 bits to memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_8_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint8_t * value);

/**
 * Writes 16 bits to memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_16_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint16_t * value);

/**
 * Writes 32 bits to memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_32_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint32_t * value);

/**
 * Writes 64 bits to memory, given a virtual address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_64_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    uint64_t * value);

/**
 * Writes the address to memory. The number of
 * bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to write to
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_addr_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    addr_t * value);

/**
 * Writes 8 bits to memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_8_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint8_t * value);

/**
 * Writes 16 bits to memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_16_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint16_t * value);

/**
 * Writes 32 bits to memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_32_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint32_t * value);

/**
 * Writes 64 bits from memory, given a physical address.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_64_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    uint64_t * value);

/**
 * Writes the address to memory. The number of
 * bytes written is 8 for 64-bit systems and 4 for 32-bit systems.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to write to
 * @param[in] value The value written to memory
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_write_addr_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    addr_t * value);

/*---------------------------------------------------------
 * Print util functions from pretty_print.c
 */

/**
 * Prints out the hex and ascii version of a chunk of bytes. The
 * output is similar to what you would get with 'od -h' with the
 * additional ascii information on the right side of the display.
 *
 * @param[in] data The bytes that will be printed to stdout
 * @param[in] length The length (in bytes) of data
 */
void vmi_print_hex(
    unsigned char *data,
    unsigned long length);

/**
 * Prints out the hex and ascii version of a chunk of bytes. The
 * output is similar to what you would get with 'od -h' with the
 * additional ascii information on the right side of the display.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] sym Kernel symbol to use as starting address
 * @param[in] length The length (in bytes) of data to print
 */
void vmi_print_hex_ksym(
    vmi_instance_t vmi,
    char *sym,
    size_t length);

/**
 * Prints out the hex and ascii version of a chunk of bytes. The
 * output is similar to what you would get with 'od -h' with the
 * additional ascii information on the right side of the display.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] vaddr Virtual address to use as starting address
 * @param[in] pid Pid of the virtual address space (0 for kernel)
 * @param[in] length The length (in bytes) of data to print
 */
void vmi_print_hex_va(
    vmi_instance_t vmi,
    addr_t vaddr,
    vmi_pid_t pid,
    size_t length);

/**
 * Prints out the hex and ascii version of a chunk of bytes. The
 * output is similar to what you would get with 'od -h' with the
 * additional ascii information on the right side of the display.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] paddr Physical address to use as starting address
 * @param[in] length The length (in bytes) of data to print
 */
void vmi_print_hex_pa(
    vmi_instance_t vmi,
    addr_t paddr,
    size_t length);

/*---------------------------------------------------------
 * Accessor functions from accessors.c
 */

/**
 * Gets the name of the VM (or file) that LibVMI is accessing.
 *
 * @param[in] vmi LibVMI instance
 * @return VM (or file) name, must be free'd by caller
 */
char *vmi_get_name(
    vmi_instance_t vmi);

/**
 * Gets the id of the VM that LibVMI is accessing.
 *
 * @param[in] vmi LibVMI instance
 * @return VM id, or zero on error
 */
uint64_t vmi_get_vmid(
    vmi_instance_t vmi);

/**
 * Gets the current access mode for LibVMI, which tells what
 * resource is being using to access the memory (e.g., VMI_XEN,
 * VMI_KVM, or VMI_FILE).
 *
 * @param[in] vmi LibVMI instance
 * @return Access mode
 */
uint32_t vmi_get_access_mode(
    vmi_instance_t vmi);

/**
 * Gets the current page mode for LibVMI, which tells what
 * type of address translation is in use (e.g., VMI_PM_LEGACY,
 * VMI_PM_PAE, or VMI_PM_IA32E).
 *
 * If paging mode is altered after vmi_init, the information
 *  preserved in vmi_instance_t will have become stale and
 *  require re-initialization.
 *
 * @param[in] vmi LibVMI instance
 * @return Page mode
 */
page_mode_t vmi_get_page_mode(
    vmi_instance_t vmi);

/**
 * Gets the current address width for the given vmi_instance_t
 *
 * Note: relative to the OS mode, not that of a process.
 *       Also, if paging mode is altered after vmi_init,
 *       the information as recorded in vmi_instance_t will
 *       be stale and require re-initialization.
 *
 * @param[in] vmi LibVMI instance
 * @return address size in bytes
 */
uint8_t vmi_get_address_width(
    vmi_instance_t vmi);

/**
 * Get the OS type that LibVMI is currently accessing.  This is
 * simple windows or linux (no version information).
 *
 * @param[in] vmi LibVMI instance
 * @return OS type
 */
os_t vmi_get_ostype(
    vmi_instance_t vmi);

/**
 * Get the version of Windows that LibVMI is currently accessing.  This is the
 * simple Windows version - no service pack or patch level is provided.
 *
 * @param[in] vmi LibVMI instance
 * @return Windows version
 */
win_ver_t vmi_get_winver(
    vmi_instance_t vmi);

/**
 * Get string represenatation of the version of Windows that LibVMI is currently accessing.
 *
 * @param[in] vmi LibVMI instance
 * @return string description of Windows version (do not free)
 */
const char *vmi_get_winver_str(
    vmi_instance_t vmi);

/**
 * Get the version of Windows based on the provided KDVB address.  This is the
 * simple Windows version - no service pack or patch level is provided.
 *
 * This function is intended to be used during partial init as an aid to elevate
 * to full init.
 *
 * @param[in] vmi       LibVMI instance
 * @param[in] kdvb_pa   The physical address of the KDVB
 * @return Windows version
 */
win_ver_t vmi_get_winver_manual(
    vmi_instance_t vmi,
    addr_t kdvb_pa);

/**
 * Get the memory offset associated with the given offset_name.
 * Valid names include everything in the /etc/libvmi.conf file.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] offset_name String name for desired offset
 * @return The offset value
 */
uint64_t vmi_get_offset(
    vmi_instance_t vmi,
    char *offset_name);

/**
 * Gets the memory size of the guest or file that LibVMI is currently
 * accessing.  This is the amount of RAM allocated to the guest, but
 * does not necessarily indicate the highest addressable physical address;
 * get_max_physical_address() should be used.
 *
 * NOTE: if memory ballooning alters the allocation of memory to a
 *  VM after vmi_init, this information will have become stale
 *  and a re-initialization will be required.
 *
 * @param[in] vmi LibVMI instance
 * @return Memory size
 */
uint64_t vmi_get_memsize(
    vmi_instance_t vmi);

/**
 * Gets highest addressable physical memory address of the guest or file that
 * LibVMI is currently accessing plus one.  That is, any address less then the
 * returned value "may be" a valid physical memory address, but the layout of
 * the guest RAM is hypervisor specific, so there can and will be holes that
 * are not memory pages and can't be read by libvmi.
 *
 * NOTE: if memory ballooning alters the allocation of memory to a VM after
 *  vmi_init, this information will have become stale and a re-initialization
 *  will be required.
 *
 * @param[in] vmi LibVMI instance @return physical memory size
 */
addr_t vmi_get_max_physical_address(
    vmi_instance_t vmi);

/**
 * Gets the memory size of the guest that LibVMI is accessing.
 * This information is required for any interaction with of VCPU registers.
 *
 * @param[in] vmi LibVMI instance
 * @return Number of VCPUs
 */
unsigned int vmi_get_num_vcpus (
    vmi_instance_t vmi);

/**
 * Gets the current value of a VCPU register.  This currently only
 * supports control registers.  When LibVMI is accessing a raw
 * memory file, this function will fail.
 *
 * NOTE: for some hypervisor drivers, it is important to understand
 *  the validity of the values that registers hold. For example,
 *  CR3 for Xen paravirtual VMs may hold a physical address higher than
 *  the maximum psuedophysical address of the given VM (this is an
 *  expected and correct idiosyncrasy of that platform).
 *  Similar scenarios exist for IDTR, etc.
 *
 * @param[in] vmi LibVMI instance
 * @param[out] value Returned value from the register, only valid on VMI_SUCCESS
 * @param[in] reg The register to access
 * @param[in] vcpu The index of the VCPU to access, use 0 for single VCPU systems
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_get_vcpureg(
    vmi_instance_t vmi,
    reg_t *value,
    registers_t reg,
    unsigned long vcpu);

/**
 * Sets the current value of a VCPU register.  This currently only
 * supports control registers.  When LibVMI is accessing a raw
 * memory file, this function will fail. Operating upon an unpaused
 * VM with this function is likely to have unexpected results.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] value Value to assign to the register
 * @param[in] reg The register to access
 * @param[in] vcpu The index of the VCPU to access, use 0 for single VCPU systems
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_set_vcpureg(
    vmi_instance_t vmi,
    reg_t value,
    registers_t reg,
    unsigned long vcpu);

/**
 * Pauses the VM.  Use vmi_resume_vm to resume the VM after pausing
 * it.  If accessing a memory file, this has no effect.
 *
 * @param[in] vmi LibVMI instance
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_pause_vm(
    vmi_instance_t vmi);

/**
 * Resumes the VM.  Use vmi_pause_vm to pause the VM before calling
 * this function.  If accessing a memory file, this has no effect.
 *
 * @param[in] vmi LibVMI instance
 * @return VMI_SUCCESS or VMI_FAILURE
 */
status_t vmi_resume_vm(
    vmi_instance_t vmi);

/**
 * Removes all entries from LibVMI's internal virtual to physical address
 * cache.  This is generally only useful if you believe that an entry in
 * the cache is incorrect, or out of date.
 *
 * @param[in] vmi LibVMI instance
 */
void vmi_v2pcache_flush(
    vmi_instance_t vmi);

/**
 * Adds one entry to LibVMI's internal virtual to physical address
 * cache.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] va Virtual address
 * @param[in] dtb Directory table base for \a va
 * @param[in] pa Physical address
 */
void vmi_v2pcache_add(
    vmi_instance_t vmi,
    addr_t va,
    addr_t dtb,
    addr_t pa);

/**
 * Removes all entries from LibVMI's internal kernel symbol to virtual address
 * cache.  This is generally only useful if you believe that an entry in
 * the cache is incorrect, or out of date.
 *
 * @param[in] vmi LibVMI instance
 */
void vmi_symcache_flush(
    vmi_instance_t vmi);

/**
 * Adds one entry to LibVMI's internal symbol to virtual address
 * cache.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] base_addr Base address
 * @param[in] pid PID
 * @param[in] sym Symbol
 * @param[in] va Virtual address
 */
void vmi_symcache_add(
    vmi_instance_t vmi,
    addr_t base_addr,
    vmi_pid_t pid,
    char *sym,
    addr_t va);

/**
 * Removes all entries from LibVMI's internal RVA to symbol
 * cache.  This is generally only useful if you believe that an entry in
 * the cache is incorrect, or out of date.
 *
 * @param[in] vmi LibVMI instance
 */
void vmi_rvacache_flush(
    vmi_instance_t vmi);

/**
 * Adds one entry to LibVMI's internal RVA to symbol
 * cache.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] base_addr Base address
 * @param[in] pid PID
 * @param[in] rva Relative virtual address
 * @param[in] sym Symbol
 */
void vmi_rvacache_add(
    vmi_instance_t vmi,
    addr_t base_addr,
    vmi_pid_t pid,
    addr_t rva,
    char *sym);

/**
 * Removes all entries from LibVMI's internal pid to directory table base
 * cache.  This is generally only useful if you believe that an entry in
 * the cache is incorrect, or out of date.
 *
 * @param[in] vmi LibVMI instance
 */
void vmi_pidcache_flush(
    vmi_instance_t vmi);

/**
 * Adds one entry to LibVMI's internal pid to directory table base
 * cache.
 *
 * @param[in] vmi LibVMI instance
 * @param[in] pid Process id
 * @param[in] dtb Directory table base
 */
void vmi_pidcache_add(
    vmi_instance_t vmi,
    vmi_pid_t pid,
    addr_t dtb);

#pragma GCC visibility pop

#ifdef __cplusplus
}
#endif

#endif /* LIBVMI_H */
