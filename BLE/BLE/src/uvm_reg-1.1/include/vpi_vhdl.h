/*****************************************************************************
 *
 * vpi_vhdl.h
 *
 * VHDL extensions to:
 *
 * IEEE Std 1364-1995 Verilog Hardware Description Language (HDL)
 * Programming Language Interface (PLI).
 *
 * This file contains the constant definitions, structure definitions,
 * and routine declarations used by the Verilog PLI procedural
 * interface (VPI) for VHDL.
 *
 * This file should be included with all C routines that use the VPI
 * routines to reference VHDL objects.
 *
 ****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/vsim/vpi_vhdl.h#1 $ */

#ifndef VPI_VHDL_H
#define VPI_VHDL_H

/* ----------------------------- Object Types ------------------------------ */

#define vpiVHDLAlias              1010  /* alias                             */
#define vpiVHDLArchitecture       1020  /* architecture                      */
#define vpiVHDLBlock              1030  /* block statement                   */
#define vpiVHDLConfiguration      1040  /* configuration                     */
#define vpiVHDLForeign            1050  /* foreign region                    */
#define vpiVHDLForLoop            1060  /* for loop                          */
#define vpiVHDLGenerate           1070  /* generate statement                */
#define vpiVHDLGeneric            1080  /* generic                           */
#define vpiVHDLPackage            1090  /* package                           */
#define vpiVHDLProcess            1100  /* process statement                 */
#define vpiVHDLSignal             1110  /* signal                            */
#define vpiVHDLSubprogram         1120  /* subprogram (function or procedure)*/
#define vpiVHDLVariable           1130  /* variable                          */
#define vpiVHDLWhileLoop          1140  /* while loop                        */

/* ------------------------------ Properties ------------------------------- */

#define vpiVHDLEntityVitalLevel0  1011  /* entity is marked Vital Level 0    */
#define vpiVHDLArchVitalLevel0    1012  /* arch is marked Vital Level 0      */
#define vpiVHDLArchVitalLevel1    1013  /* arch is marked Vital Level 1      */
#define vpiVHDLForeignArch        1014  /* arch has foreign attribute and no */
                                        /* VHDL statements or objects other  */
                                        /* than ports or generics            */
#define vpiVHDLForeignArchMixed   1015  /* arch has foreign attribute and    */
                                        /* contains VHDL statements or       */
                                        /* objects other than ports or       */
                                        /* generics                          */

#endif /* VPI_VHDL_H */

/* ***************************** END OF HEADER ***************************** */
