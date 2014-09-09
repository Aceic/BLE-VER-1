
/*****************************************************************************
 *
 * acc_vhdl.h
 *
 * List of all predefined type and fulltype constants for VHDL objects.
 *
 * Type Constant       Fulltype Constant       Description
 * ===========================================================================
 *
 * accAlias            accAlias                Object is a VHDL alias.
 * ---------------------------------------------------------------------------
 * accArchitecture     accArchitecture         Object is a VHDL architecture.
 *
 *                     accEntityVitalLevel0    Object is an architecture
 *                                             whose entity is marked with
 *                                             the attribute VITAL_Level0.
 *
 *                     accArchVitalLevel0      Object is an architecture
 *                                             which is marked with the
 *                                             attribute VITAL_Level0.
 *
 *                     accArchVitalLevel1      Object is an architecture
 *                                             which is marked with the
 *                                             attribute VITAL_Level1.
 *
 *                     accForeignArch          Object is an architecture
 *                                             which is marked with the
 *                                             attribute FOREIGN and which
 *                                             does not contain any VHDL
 *                                             statements or objects other
 *                                             than ports and generics.
 *
 *                     accForeignArchMixed     Object is an architecture
 *                                             which is marked with the
 *                                             attribute FOREIGN and which
 *                                             contains some VHDL statements
 *                                             or objects besides ports
 *                                             and generics.
 *
 *                     accForeignArchContext   Object is an architecture
 *                                             inserted into the context
 *                                             tree by a 3rd party
 *                                             which is marked with the
 *                                             attribute FOREIGN and which
 *                                             does not contain any VHDL
 *                                             statements or objects other
 *                                             than ports and generics.
 *
 *                     accForeignArchContextMixed  Object is an architecture
 *                                             inserted into the context
 *                                             tree by a 3rd party
 *                                             which is marked with the
 *                                             attribute FOREIGN and which
 *                                             contains some VHDL statements
 *                                             or objects besides ports
 *                                             and generics.
 * ---------------------------------------------------------------------------
 * accBlock            accBlock                Object is a VHDL block.
 * ---------------------------------------------------------------------------
 * accConfiguration    accConfiguration        Object is a VHDL configuration.
 * ---------------------------------------------------------------------------
 * accForeign          accShadow               Object is a region created
 *                                             with the FLI function
 *                                             mti_CreateRegion().
 * ---------------------------------------------------------------------------
 * accForeignObject    accForeignObject        Object is a 3rd party object.
 * ---------------------------------------------------------------------------
 * accForeignScope     accForeignScope         Object is a 3rd party scope.
 * ---------------------------------------------------------------------------
 * accForLoop          accForLoop              Object is a VHDL for loop.
 * ---------------------------------------------------------------------------
 * accGenerate         accGenerate             Object is a VHDL generate
 *                                             statement.
 * ---------------------------------------------------------------------------
 * accGeneric          accGeneric              Object is a VHDL generic.
 *
 *                     accGenericConstant      Object is a VHDL generic that
 *                                             cannot be modified after design
 *                                             elaboration.
 * ---------------------------------------------------------------------------
 * accPackage          accPackage              Object is a VHDL package.
 * ---------------------------------------------------------------------------
 * accProcess          accProcess              Object is a VHDL process.
 * ---------------------------------------------------------------------------
 * accSignal           accSignal               Object is a VHDL signal.
 * ---------------------------------------------------------------------------
 * accSubprogram       accSubprogram           Object is a VHDL subprogram.
 * ---------------------------------------------------------------------------
 * accVariable         accVariable             Object is a VHDL variable.
 * ---------------------------------------------------------------------------
 * accVHDLConstant     accVHDLConstant         Object is a VHDL constant.
 *
 *****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/vsim/acc_vhdl.h#1 $ */

#ifndef ACC_VHDL_H
#define ACC_VHDL_H

/* ATTENTION: Do not define values here below 1010 or above 1499. */

#define    accArchitecture            1010
#define    VHDL_FIRST_ACC_ID		   accArchitecture

#define    accEntityVitalLevel0       1011
#define    accArchVitalLevel0         1012
#define    accArchVitalLevel1         1013
#define    accForeignArch             1014
#define    accForeignArchMixed        1015
#define    accArchArray               1016
#define    accEntity                  1017
#define    accForeignArchContext      1018
#define    accForeignArchContextMixed 1019
#define    accBlock                   1020
#define    accCompInst                1021
#define    accDirectInst              1022
#define    accinlinedBlock            1023
#define    accinlinedinnerBlock       1024
#define    accGenerate                1030
#define    accIfGenerate              1031
#define    accForGenerate             1032
#define    accPackage                 1040
#define    accConfiguration           1050
#define    accSubprogram              1060
#define    accProcess                 1070
#define    accForLoop                 1080
#define    accForeign                 1090
#define    accShadow                  1091
#define    accSignal                  1100
#define    accSignalBit               1101
#define    accSignalSubComposite      1102
#define    accVariable                1110
#define    accGeneric                 1120
#define    accGenericConstant         1121
#define    accAlias                   1130
#define    accAliasSignal             1131
#define    accAliasConstant           1132
#define    accAliasGeneric            1133
#define    accAliasVariable           1134
#define    accVHDLConstant            1140
#define    accForeignObject           1150

#define    accForeignScope            1160
#define    VHDL_LAST_ACC_ID		   accForeignObject


#define	 VS_TYPE_IS_VHDL(a) (((a) <= VHDL_LAST_ACC_ID) && (a) >= VHDL_FIRST_ACC_ID)

#endif /* ACC_VHDL_H */
