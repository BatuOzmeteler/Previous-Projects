/*
*****************************************************************************
*
*  Project :        OpenECU
*  Version :        $Id : gen_memmap.py 51645 2016-05-12 19 :55 :43Z sborders $
*  File :           $URL : https ://aetius.ps.local/svn/OpenECU/development/platform/tags/release-2.6.0-r2016-1/tools/gen_memmap/gen_memmap.py $
*  Description :    Memory map file for the strategy application.
*
*  Copyright (C) 2016 Pi Innovo
*  This document (program) contains proprietary information
*  which is the property of Pi Innovo
*  The contents of this document (program) must not be copied
*  or disclosed to a third party without the prior agreement
*  of an authorised officer of Pi Innovo
*
*  Notes :          None.
*
*****************************************************************************
*/

MEMORY
{

    /*  Configuration A : */

    /*** Define RAM memory areas ****************************************
     *  Internal RAM
     *  Physical address == virtual address
     *  _________________________________
     *  0x4000FFFF  |                   |
     *              |                   |
     *              |                   |
     *              |                   |
     *              |                   |
     *  (not fixed) |___________________|  Variable data
     *  0x40000000  |___________________|  Stack area
     */
    data_ram :               ORIGIN = 0x40000000, LENGTH = 0x010000

    /* Note, eTPU code may not be robust to arrays in RAM
     * straddling a 24-bit address boundary. Not a problem so
     * long as the RAM starts on a 24-bit address boundary and
     * occupies less than 0x1000000 bytes, as here.
     */

     /* External RAM -- may not be present on the PCB
     *  Physical address == virtual address
     * __________________________________
     *  0x200FFFFF  |                   |
     *              |                   |
     *              |                   |
     *              |                   |
     *  0x20040000  |___________________|  unused
     *              |                   |
     *              |                   |
     *              |                   |
     *  0x20000400  |___________________|  copy    Calibration data
     *  0x20000000  |___________________|  copy    Calibration header
     */
    cal_ram :                ORIGIN = 0x20000000, LENGTH = 0x100000

    /*** Define Flash memory areas **************************************
     *  Physical address == virtual address
     *  _________________________________
     *  0xFFFFF     |                   |
     *  0xFFFC0     |___________________| Reserved
     *  0xFFFBF     |                   |
     *              |                   |
     *              |                   |
     *              |                   |
     *              |                   |
     *  0x83A30     |                   |  Application code
     *  0x83030     |                   |  Application PEM code
     *  0x82830     | checksum region 1 |  Application RAM code
     *  0x82530     |___________________|  Application PFL code
     *  0x82130     |                   |  Application header
     *  0x82000     | checksum region 0 |  Application exceptions
     *  0x80000     |___________________|  Application ISRs
     *              |                   |
     *  0x40400     |___________________|  Calibration data
     *  0x40000     |___________________|  Calibration header
     *              |                   |
     *  0x30000     |___________________|  unused
     */
    cal_flash :              ORIGIN = 0x00040000, LENGTH = 0x040000
    app_code_flash :         ORIGIN = 0x00083a30, LENGTH = 0x07c58f

    /* Note that the last section doesn't range up to the end of full
     * internal Flash size due to errata note 1651 (don't use the last
     * 64 bytes of internal Flash).
     */

    /* This tiny region is used to generate labels where the linker
     * doesn't like the use of the SIZEOF command (not clear why).
     */
    app_code_flash_end :     ORIGIN = 0x000fffbf, LENGTH = 0x000001


    /*** Define Flash memory areas **************************************
     *  Physical address == virtual address
     *  _________________________________
     *  0xFFFFF     |                   |
     *  0xFFFC0     |___________________| Reserved
     *              -        ...        -
                    -        ...        -
                    -        ...        -
        0x30000      ___________________
     *  0x10000     |___________________|  PFS flash
     *              |                   |
     *  0x4830      |___________________|  PRG code
     *  0x4530      |___________________|  PRG RAM code
     *  0x4400      |___________________|  PRG exception
     *  0x4000      |___________________|  PRG header
     *              |                   |
     *              |                   |
     *  0x0540      |___________________|  Boot code
     *  0x0410      |___________________|  Boot exceptions
     *  0x0008      |___________________|  Boot header
     *  0x0002      |___________________|  Reset vector
     *  0x0000      |___________________|  RCW
     */
    boot_rchw_flash :        ORIGIN = 0x00000000, LENGTH = 0x000002
    boot_reset_vec_flash :   ORIGIN = 0x00000004, LENGTH = 0x000004
    boot_header_flash :      ORIGIN = 0x00000008, LENGTH = 0x000400
    boot_excep_flash :       ORIGIN = 0x00000410, LENGTH = 0x000130
    boot_flash :             ORIGIN = 0x00000540, LENGTH = 0x003ac0

    prg_header_flash :       ORIGIN = 0x00004000, LENGTH = 0x000400
    prg_excep_flash :        ORIGIN = 0x00004400, LENGTH = 0x000130
    prg_pfl_i_flash :        ORIGIN = 0x00004530, LENGTH = 0x000100
    prg_pfl_e_flash :        ORIGIN = 0x00004630, LENGTH = 0x000100
    prg_pfl_p_flash :        ORIGIN = 0x00004730, LENGTH = 0x000100
    prg_flash :              ORIGIN = 0x00004830, LENGTH = 0x00b7d0

    app_isr_flash :          ORIGIN = 0x00080000, LENGTH = 0x002000
    app_excep_flash :        ORIGIN = 0x00082000, LENGTH = 0x000130
    app_header_flash :       ORIGIN = 0x00082130, LENGTH = 0x000400
    app_pfl_i_flash :        ORIGIN = 0x00082530, LENGTH = 0x000100
    app_pfl_e_flash :        ORIGIN = 0x00082630, LENGTH = 0x000100
    app_pfl_p_flash :        ORIGIN = 0x00082730, LENGTH = 0x000100
    app_ram_flash :          ORIGIN = 0x00082830, LENGTH = 0x000800
    app_pem_flash :          ORIGIN = 0x00083030, LENGTH = 0x000a00

    /* app_code_flash comes next, but it has already been defined */

    pfs_flash_0 :            ORIGIN = 0x00010000, LENGTH = 0x010000
    pfs_flash_1 :            ORIGIN = 0x00020000, LENGTH = 0x010000

    /* cal_flash comes next, but it has already been defined */

    pfl_shadow_flash :       ORIGIN = 0x00fffc00, LENGTH = 0x000400

    /*** Define processor memory mapped register areas ******************
     * Here, the sections are defined in order of memory address.
     */
    pbridge_a_mmreg :        ORIGIN = 0xc3f00000, LENGTH = 0x000100
    fmpll_mmreg :            ORIGIN = 0xc3f80000, LENGTH = 0x000100
    ebi_mmreg :              ORIGIN = 0xc3f84000, LENGTH = 0x000100
    flash_mmreg :            ORIGIN = 0xc3f88000, LENGTH = 0x000100
    siu_mmreg :              ORIGIN = 0xc3f90000, LENGTH = 0x001000
    emios_mmreg :            ORIGIN = 0xc3fa0000, LENGTH = 0x001000
    etpu_a_mmreg :           ORIGIN = 0xc3fc0000, LENGTH = 0x001000
    etpu_scm_mmreg :         ORIGIN = 0xc3fcd000, LENGTH = 0x003000
    etpu_sdm_mmreg :         ORIGIN = 0xc3fc8000, LENGTH = 0x000a00
    pbridge_b_mmreg :        ORIGIN = 0xfff00000, LENGTH = 0x000100
    xbar_mmreg :             ORIGIN = 0xfff04000, LENGTH = 0x001000
    ecsm_mmreg :             ORIGIN = 0xfff40000, LENGTH = 0x000100
    edma_mmreg :             ORIGIN = 0xfff44000, LENGTH = 0x002000
    intc_mmreg :             ORIGIN = 0xfff48000, LENGTH = 0x000200
    eqadc_mmreg :            ORIGIN = 0xfff80000, LENGTH = 0x000800
    dspi_b_mmreg :           ORIGIN = 0xfff94000, LENGTH = 0x000100
    dspi_c_mmreg :           ORIGIN = 0xfff98000, LENGTH = 0x000100
    dspi_d_mmreg :           ORIGIN = 0xfff9c000, LENGTH = 0x000100
    esci_a_mmreg :           ORIGIN = 0xfffb0000, LENGTH = 0x000100
    esci_b_mmreg :           ORIGIN = 0xfffb4000, LENGTH = 0x000100
    flexcan_a_mmreg :        ORIGIN = 0xfffc0000, LENGTH = 0x000800
    flexcan_c_mmreg :        ORIGIN = 0xfffc8000, LENGTH = 0x000800

}

    /* Define linker symbols to identify the memory configuration regions to include in the
     * application checksum and the translated address that will be used while generating
     * the final application image
     */
    psy_start_app_region_0     = 0x80000;
    psy_end_app_region_0       = 0x82130;
    psy_translate_app_region_0 = 0x80000;

    psy_start_app_region_1     = 0x82530;
    psy_end_app_region_1       = 0xFFFBF;
    psy_translate_app_region_1 = 0x82530;


    /**** Linked placed variables ***********************************************/

    /* Place various variables at specific memory locations. There are
     * no C definitions for these variables, only C declarations.
     */

    /* Processor memory mapped registers
     * Here, the variables are defined in alphabetical order.
     */
    psy_dspi_b                  = ORIGIN(dspi_b_mmreg);
    psy_dspi_c                  = ORIGIN(dspi_c_mmreg);
    psy_dspi_d                  = ORIGIN(dspi_d_mmreg);
    psy_ebi                     = ORIGIN(ebi_mmreg);
    psy_ecsm                    = ORIGIN(ecsm_mmreg);
    psy_edma                    = ORIGIN(edma_mmreg);
    psy_esci_a                  = ORIGIN(esci_a_mmreg);
    psy_esci_b                  = ORIGIN(esci_b_mmreg);
    psy_emios                   = ORIGIN(emios_mmreg);
    psy_eqadc                   = ORIGIN(eqadc_mmreg);
    psy_etpu_a                  = ORIGIN(etpu_a_mmreg);
    psy_etpu_scm                = ORIGIN(etpu_scm_mmreg);
    psy_etpu_sdm                = ORIGIN(etpu_sdm_mmreg);
    psy_flash                   = ORIGIN(flash_mmreg);
    psy_flexcan_a               = ORIGIN(flexcan_a_mmreg);
    psy_flexcan_c               = ORIGIN(flexcan_c_mmreg);
    psy_fmpll                   = ORIGIN(fmpll_mmreg);
    psy_intc                    = ORIGIN(intc_mmreg);
    psy_pbridge_a               = ORIGIN(pbridge_a_mmreg);
    psy_pbridge_b               = ORIGIN(pbridge_b_mmreg);
    psy_siu                     = ORIGIN(siu_mmreg);
    psy_xbar                    = ORIGIN(xbar_mmreg);

    /* Define the internal Flash memory addressing */
    pfl_h7f_l0_partition_start  = ORIGIN(boot_rchw_flash);
    pfl_h7f_l1_partition_start  = ORIGIN(boot_rchw_flash) + 0x00004000;
    pfl_h7f_l2_partition_start  = ORIGIN(boot_rchw_flash) + 0x00010000;
    pfl_h7f_l3_partition_start  = ORIGIN(boot_rchw_flash) + 0x0001C000;
    pfl_h7f_l4_partition_start  = ORIGIN(boot_rchw_flash) + 0x00020000;
    pfl_h7f_l5_partition_start  = ORIGIN(boot_rchw_flash) + 0x00030000;
    pfl_h7f_m0_partition_start  = ORIGIN(boot_rchw_flash) + 0x00040000;
    pfl_h7f_m1_partition_start  = ORIGIN(boot_rchw_flash) + 0x00060000;
    pfl_h7f_h0_partition_start  = ORIGIN(boot_rchw_flash) + 0x00080000;
    pfl_h7f_h1_partition_start  = ORIGIN(boot_rchw_flash) + 0x000A0000;
    pfl_h7f_h2_partition_start  = ORIGIN(boot_rchw_flash) + 0x000C0000;
    pfl_h7f_h3_partition_start  = ORIGIN(boot_rchw_flash) + 0x000E0000;

/*
 *****************************************************************************
 *
 *  Project:        OpenECU
 *  Version:        $Id: mk_model_gcc_4_7_3.ld 39643 2014-07-01 15:33:49Z cforhart $
 *  File:           $URL: https://apple.ps.local/svn/OpenECU/development/platform/trunk/targets/mpc5534_common/mk_model_gcc_4_7_3.ld $
 *  Description:    Linker file for the strategy application.
 *
 *  Copyright (C) 2016 Pi Innovo
 *  This document (program) contains proprietary information
 *  which is the property of Pi Innovo
 *  The contents of this document (program) must not be copied
 *  or disclosed to a third party without the prior agreement
 *  of an authorised officer of Pi Innovo
 *
 *  Notes:          None.
 *
 *****************************************************************************
 */

SECTIONS
{

/*--- BOOT ---*/
        .boot :
        {
            /* Allocate the first address of the internal Flash */
            pfl_h7f_addr_start = .;
            pccp_flash_addr_start_ro = .;
        } > boot_rchw_flash = 0xFFFFFFFF

        .boot_header_flash :
        {
            /* Provide a link to the boot header */
            pbt_header = . ;
        } > boot_header_flash = 0xFFFFFFFF

/*--- PRG ---*/

        .prg_header_flash :
        {
            /* This is required by the boot module. */
            prg_header = . ;
        } > prg_header_flash = 0xFFFFFFFF

/*--- PFS ---*/

        .pfs_flash_0 (NOLOAD) :
        {
        /* Declare first byte of internal Flash that can be reprogrammed */
              pccp_flash_addr_start_rw = ABSOLUTE(ORIGIN(pfs_flash_0));
              pfs_start_flash_0 = ABSOLUTE(ORIGIN(pfs_flash_0));
              . = ABSOLUTE(ORIGIN(pfs_flash_0) + LENGTH(pfs_flash_0) - 1);
              pfs_end_flash_0   = ABSOLUTE(.);
        } > pfs_flash_0 = 0xFFFFFFFF

        .pfs_flash_1 (NOLOAD) :
        {
            pfs_start_flash_1 = ABSOLUTE(.);
            . = ABSOLUTE(ORIGIN(pfs_flash_1) + LENGTH(pfs_flash_1) - 1);
            pfs_end_flash_1   = ABSOLUTE(.);
        } > pfs_flash_1 = 0xFFFFFFFF

/*
 * calibration flash area. Note that while diab can collect .cal_sec
 * definitions from multiple groups, GNU ld cannot, so both cal header
 * and cal section proper must be declared in the same output section.
 * GCC mapping is slightly different than Diab to ensure contiguous
 * output sections and maintain names for possible relocation issues.
 */

        .cal_sec :
        {
            psy_start_used_cal = ABSOLUTE(.);
            pccp_cal_addr_start = ABSOLUTE(.);

            *(.cal_header_sec)
            *(.cal_sec)
            *(.other_sec)

            /* GCC has a rodata section for non-small initialized constants. */

            *(.rodata*)

            pnv_start_used_cal_adap = ABSOLUTE(.);
            *(.cal_adap)
            pnv_end_used_cal_adap = ABSOLUTE(.);

            /* Diab automatically creates _SDA2_BASE_, GCC does not so put this here */
            PROVIDE(_SDA2_BASE_ = ABSOLUTE(.));
        } > cal_flash=0xFFFFFFFF

        /* sdata2 named because of relocation stuff */

        .sdata2 _SDA2_BASE_ :
        {
            *(.sdata2)


            psy_end_used_cal = ABSOLUTE(.);
            psy_start_free_cal = ABSOLUTE(.);

        } > cal_flash = 0xFFFFFFFF

        /* outside the .sdata2 section to make sure it is small */

        psy_end_free_cal = ABSOLUTE(ORIGIN(cal_flash)) + LENGTH(cal_flash)-1;
        pccp_cal_addr_end = ABSOLUTE(ORIGIN(cal_flash)) + LENGTH(cal_flash)-1;

/*
 * application flash area. Note that the diab script uses a mechanism not
 * supported by GNU ld; namely it spreads multiple items in the .text_app
 * section among several GROUP declarations. GNU ld cannot do this; so
 * everything in the .text_app section must go at once.
 */
        .text_app_isr :
        {
            psy_start_used_code = ABSOLUTE(.);

            /* interrupt handlers */
                pkn_start_isr_code = ABSOLUTE(.);
                *platform_no_vle_diab_5_9.a:pkn_interrupt_traps_*.o(.text)
                psc_app_isr_end_addr = ABSOLUTE(.);

        } > app_isr_flash = 0xFFFFFFFF

        .text_app_excep :
        {
            /* interrupt handlers */
                psc_app_excep_start_addr = ABSOLUTE(.);
                *platform_no_vle_diab_5_9.a:pkn_exception_traps.o(.text)
                psc_app_excep_end_addr = ABSOLUTE(.);

        } > app_excep_flash = 0xFFFFFFFF

        .text_app_header :
        {
            /* Application header */
                psc_app_header_start_addr = ABSOLUTE(.);
                *platform_no_vle_diab_5_9.a:psc_header_*.o(.text)
                psc_app_header_end_addr = ABSOLUTE(.);

        } > app_header_flash = 0xFFFFFFFF

        .text_app_pfl_i (NOLOAD) :
        {
            /* pfl_init */
                pfl_start_ram_code_i = ABSOLUTE(.);
                . += LENGTH(app_pfl_i_flash);
                pfl_end_ram_code_i = ABSOLUTE(.);

        } > app_pfl_i_flash

        .text_app_pfl_e (NOLOAD) :
        {
            /* pfl_erase */
                pfl_start_ram_code_e = ABSOLUTE(.);
                . += LENGTH(app_pfl_e_flash);
                pfl_end_ram_code_e = ABSOLUTE(.);

        } > app_pfl_e_flash

        .text_app_pfl_p (NOLOAD) :
        {
            /* pfl_program */
                pfl_start_ram_code_p = ABSOLUTE(.);
                . += LENGTH(app_pfl_p_flash);
                pfl_end_ram_code_p = ABSOLUTE(.);

        } > app_pfl_p_flash

        .text_app_pem :
        {
            /* pem does some memory remapping, so we have to make sure its own
             * code and data is located somewhere it doesn't attempt to remap
             */
                *platform_no_vle_diab_5_9.a:pem_cal_ram.o(.text)

        } > app_pem_flash = 0xFFFFFFFF

        .text_app :
        {
            /* app code proper */

            psc_app_code_start_addr = ABSOLUTE(.);

            /* Some init functions must be called before memory configuration,
             * and they must be within the MMU configuration set up in the header.
             * Allocate platform text preferentially to avoid a TLB miss before
             * memory configuration.
             */
            *platform_no_vle_diab_5_9.a:*.o(EXCLUDE_FILE (*pfl_h7f_ram_init.o *pfl_h7f_ram_erase.o *pfl_h7f_ram_prog.o) .text)

            /* memcpy et all from libc */
            *libc.a:*.o(.text)

            /* The text_data must fall within 'default' regions for signing */
            *(.text_data)

            /* The remaining text is not accessed until after MMU configuration */
            /* APP_CODE_FLASH_INSERT */
/*
 *****************************************************************************
 *
 *  Project:        OpenECU
 *  File:           mk_mpc_v2_specific_linkage.ld
 *  Description:    (none)
 *
 *  (none)
 *
 *  Notes:          Automatically generated as part of the build process.
 *                  Do not modify.
 *                  Created on: Sun May 31 18:33:58 2020 
 *
 *****************************************************************************
 */
/* This auto-generated file must be inserted in the LD file in the app_code_flash
 * group definition, before the ".text_app : { *(.text_vle) }" line.
 * This sets up addresses so that function sizes can be determined for those
 * functions that will be relocated, since C gives no "sizeof" operator for
 * finding the size of functions.
 */


            *(EXCLUDE_FILE (*pfl_h7f_ram_init.o *pfl_h7f_ram_erase.o *pfl_h7f_ram_prog.o) .text)
            psc_app_code_end_addr = ABSOLUTE(.);

            /* Create label to copy initialised variables from during
             * startup. Although not explicit here, the linker places
             * the initialisation data at this address so this must
             * be the last item in the group.
             */
            __diab_data_ro_start = ABSOLUTE(.);

        } > app_code_flash = 0xFFFFFFFF

        .text_app_end :
        {
                /* Move to the end of this flash region and label it as required.
                 * This is no longer included in the app_code_flash group itself,
                 * because the ADDR + SIZEOF trick fails when the data_ram group
                 * is put in front of the app_code_flash group.
                 */
                pfl_h7f_addr_end     = .;
                pccp_flash_addr_end_ro = .;
                pccp_flash_addr_end_rw = .;
                psy_end_free_code    = .;

        } > app_code_flash_end = 0xFFFFFFFF

/*--- DATA RAM ---*/
    /*
     * Shared stack space at bottom of RAM. If stack overflows,
     * it will overflow into a non-mapped memory area,
     * and the processor will raise a checkstop.
     */
        .bss_stack ORIGIN(data_ram):
        {
            psc_ram_addr_start  = ABSOLUTE(.);
            psc_last_sp         = ABSOLUTE(.);
            psy_start_used_ram  = ABSOLUTE(.);
            pccp_ram_addr_start = ABSOLUTE(.);

            *(.bss_stack)

            psc_initial_sp = ABSOLUTE(.);
        } > data_ram

        .text_app_pfl_i_ram (ORIGIN(data_ram) + SIZEOF(.bss_stack)) : AT (pfl_start_ram_code_i)
        {
            pfl_ram_code_i_location = ABSOLUTE(.);
            *platform_no_vle_diab_5_9.a:pfl_h7f_ram_init.o(.text)
        } > data_ram

        .text_app_pfl_e_ram : AT (pfl_start_ram_code_e)
        {
            pfl_ram_code_e_location = ABSOLUTE(.);
            *platform_no_vle_diab_5_9.a:pfl_h7f_ram_erase.o(.text)
        } > data_ram

        .text_app_pfl_p_ram : AT (pfl_start_ram_code_p)
        {
            pfl_ram_code_p_location = ABSOLUTE(.);
            *platform_no_vle_diab_5_9.a:pfl_h7f_ram_prog.o(.text)
        } > data_ram

        .data : AT (__diab_data_ro_start)
        {
            __diab_data_rw_start = ABSOLUTE(.);
            *(.data)
            __sda_base = ABSOLUTE(.);
            PROVIDE(_SDA_BASE_ = ABSOLUTE(.));
        } > data_ram

        .sdata __sda_base : AT (__diab_data_ro_start + SIZEOF(.data))
        {
            *(.sdata)
            __diab_data_rw_end = ABSOLUTE(.);
            __diab_bss_start = ABSOLUTE(.);
        } > data_ram

/*
 * SBSS moved here because it must be close enough to .sdata to fit in the
 * SDA!
 */
        .sbss __diab_bss_start :
        {
            *(.sbss)
        } > data_ram

        .bss (ADDR(.sbss)+SIZEOF(.sbss)) :
        {
            /* Allocate platform variables preferentially to increase the
             * chance of their being located in internal RAM, by using
             * a wildcard search on the standard platform file prefixes.
             */
            *platform_no_vle_diab_5_9.a:etpuc*.o(.bss)
            *platform_no_vle_diab_5_9.a:ns*.o(.bss)
            *platform_no_vle_diab_5_9.a:pax*.o(.bss)
            *platform_no_vle_diab_5_9.a:pcan*.o(.bss)
            *platform_no_vle_diab_5_9.a:pccp*.o(.bss)
            *platform_no_vle_diab_5_9.a:pcfg*.o(.bss)
            *platform_no_vle_diab_5_9.a:pcp*.o(.bss)
            *platform_no_vle_diab_5_9.a:pcx*.o(.bss)
            *platform_no_vle_diab_5_9.a:pdb*.o(.bss)
            *platform_no_vle_diab_5_9.a:pdg*.o(.bss)
            *platform_no_vle_diab_5_9.a:pdma*.o(.bss)
            *platform_no_vle_diab_5_9.a:pdtc*.o(.bss)
            *platform_no_vle_diab_5_9.a:pdx*.o(.bss)
            *platform_no_vle_diab_5_9.a:pem*.o(.bss)
            *platform_no_vle_diab_5_9.a:pfc*.o(.bss)
            *platform_no_vle_diab_5_9.a:pff*.o(.bss)
            *platform_no_vle_diab_5_9.a:pfl*.o(.bss)
            *platform_no_vle_diab_5_9.a:pfs*.o(.bss)
            *platform_no_vle_diab_5_9.a:pgpio*.o(.bss)
            *platform_no_vle_diab_5_9.a:piso*.o(.bss)
            *platform_no_vle_diab_5_9.a:pj1939*.o(.bss)
            *platform_no_vle_diab_5_9.a:pkn*.o(.bss)
            *platform_no_vle_diab_5_9.a:pmios*.o(.bss)
            *platform_no_vle_diab_5_9.a:pnv*.o(.bss)
            *platform_no_vle_diab_5_9.a:ppid*.o(.bss)
            *platform_no_vle_diab_5_9.a:ppr*.o(.bss)
            *platform_no_vle_diab_5_9.a:pqadc*.o(.bss)
            *platform_no_vle_diab_5_9.a:preg*.o(.bss)
            *platform_no_vle_diab_5_9.a:psc*.o(.bss)
            *platform_no_vle_diab_5_9.a:psp*.o(.bss)
            *platform_no_vle_diab_5_9.a:pspi*.o(.bss)
            *platform_no_vle_diab_5_9.a:pss*.o(.bss)
            *platform_no_vle_diab_5_9.a:psy*.o(.bss)
            *platform_no_vle_diab_5_9.a:ptm*.o(.bss)
            *platform_no_vle_diab_5_9.a:ptpu*.o(.bss)
            *platform_no_vle_diab_5_9.a:put*.o(.bss)

            /* And then all the application stuff
             * including model output and user data.
             */
            *(.bss)
            __diab_bss_end = ABSOLUTE(.);
        } > data_ram

        /* Import non-volatile adaptive variables.
         *
         * For this target, set the copy address to be the same as the
         * actual RAM address, because on this target the copy does not occur
         * but the symbol needs to be resolved.
         */
        .adap __diab_bss_end :
        {
            pnv_start_copy_fl_adap = ABSOLUTE(.);
            pnv_start_used_adap = ABSOLUTE(.);
            . += pnv_end_used_cal_adap - pnv_start_used_cal_adap;
            pnv_end_used_adap   = ABSOLUTE(.);
        } > data_ram

        /* Import non-volatile diagnostic trouble code variables.
         *
         * For this target, set the copy address to be the same as the
         * actual RAM address, because on this target the copy does not occur
         * but the symbol needs to be resolved.
         */
        .dtcs pnv_end_used_adap (NOLOAD) :
        {
            pdtc_start_copy_fl_dtcs = ABSOLUTE(.);
            pdtc_start_used_dtcs = ABSOLUTE(.);
            *(.dtcs)
            pdtc_end_used_dtcs   = ABSOLUTE(.);

/*
 * End of RAM moved to this section now, since it's the last one
 */

            /* required by some GCC libraries: */
            __end = ABSOLUTE(.);
            _end  = ABSOLUTE(.);
            PROVIDE(end = ABSOLUTE(.));

            psy_end_used_ram = ABSOLUTE(.);
            psy_start_free_ram = ABSOLUTE(.);

            . = ABSOLUTE(ORIGIN(data_ram) + LENGTH(data_ram) - 1);
            psy_end_free_ram   = ABSOLUTE(.);

            /* psc_ram_addr_end represents the end of the internal RAM.
             * We can no longer define it by location at end of the RAM
             * group if extended RAM is used.
             */
            psc_ram_addr_end   = ABSOLUTE(psc_ram_addr_start + 0xFFFF);
            pccp_ram_addr_end  = ABSOLUTE(.);

        } > data_ram

/* --- SHADOW FLASH --- */
        pfl_h7f_shadow_addr_start = ABSOLUTE(ORIGIN(pfl_shadow_flash));
        pccp_shadow_addr_start_rw = ABSOLUTE(ORIGIN(pfl_shadow_flash));
        preg_config_addr_start = ABSOLUTE(ORIGIN(pfl_shadow_flash));
        pfl_h7f_shadow_addr_end = ABSOLUTE(ORIGIN(pfl_shadow_flash) + LENGTH(pfl_shadow_flash) - 1);
        pccp_shadow_addr_end_rw = ABSOLUTE(ORIGIN(pfl_shadow_flash) + LENGTH(pfl_shadow_flash) - 1);
        preg_config_addr_end = ABSOLUTE(ORIGIN(pfl_shadow_flash) + LENGTH(pfl_shadow_flash) - 1);

/*--- CAL RAM --- */
        /* Define the start of calibration RAM (may not be present on PCB). */
        pem_start_cal_ram = ABSOLUTE(ORIGIN(cal_ram));

}

/* Define the size of SRAM to initialise, divided by the size of each write
 * (written as 4 bytes * 32 registers at a time).
 */
psc_ram_mword_size  = (psc_ram_addr_end - psc_ram_addr_start + 1) / 4 / 32;

/* Can't do the necessary arithmetic in the section groups
 * above, so unfortunately bring the calculation out here.
 */
psy_end_used_code   = __diab_data_ro_start + (__diab_data_rw_end - __diab_data_rw_start);
psy_start_free_code = __diab_data_ro_start + (__diab_data_rw_end - __diab_data_rw_start);
psy_len_used_code   = psy_end_used_code - psy_start_used_code;
psy_len_used_cal    = psy_end_used_cal  - psy_start_used_cal;
psy_len_total_code  = psy_end_free_code - psy_start_used_code + 1;
psy_len_total_cal   = psy_end_free_cal  - psy_start_used_cal  + 1;
