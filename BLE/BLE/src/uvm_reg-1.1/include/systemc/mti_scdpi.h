/* $Id: //dvt/mti/rel/10.0b/src/systemc/mti_scdpi.h#1 $ */

#ifndef _MTI_SCDPI_H_
#define _MTI_SCDPI_H_

/* Routines to support SystemC datatype and SystemVerilog DPI datatype conversion */

#include "svdpi.h"
#include "systemc.h"

/* conversion from SystemVerilog DPI data type to SystemC data type */
/* These interace returns 0 to indicate success. nonzero if there is a failure.*/

extern int  mti_scdpi_create_sc_bit(sc_bit*, svBit*);
extern int  mti_scdpi_create_sc_bit(sc_bit&, svBit*);
extern int  mti_scdpi_create_sc_bit(sc_bit*, svBit);
extern int  mti_scdpi_create_sc_bit(sc_bit&, svBit);

extern int  mti_scdpi_create_sc_logic(sc_logic*, svLogic*);
extern int  mti_scdpi_create_sc_logic(sc_logic&, svLogic*);
extern int  mti_scdpi_create_sc_logic(sc_logic*, svLogic);
extern int  mti_scdpi_create_sc_logic(sc_logic&, svLogic);

extern int  mti_scdpi_create_sc_bv(sc_mti_access* scb, const svBitVecVal*);
extern int  mti_scdpi_create_sc_bv(sc_mti_access& scb, const svBitVecVal*);
extern int  mti_scdpi_create_sc_lv(sc_mti_access* scl, const svLogicVecVal*);
extern int  mti_scdpi_create_sc_lv(sc_mti_access& scl, const svLogicVecVal*);


/* conversion from SystemC data type to SystemVerilog DPI data type.
 *
 * These interaces assumes that enough memory has pre-allocated for the DPI data.
 * These interace returns 0 to indicate success. nonzero if there is a failure.
 */

extern int mti_scdpi_create_svBit(const sc_bit*, svBit*);
extern int mti_scdpi_create_svBit(const sc_bit&, svBit*);
extern int mti_scdpi_create_svBit(const sc_bit*, svBit&);
extern int mti_scdpi_create_svBit(const sc_bit&, svBit&);

extern int mti_scdpi_create_svLogic(const sc_logic*, svLogic*);
extern int mti_scdpi_create_svLogic(const sc_logic&, svLogic*);
extern int mti_scdpi_create_svLogic(const sc_logic*, svLogic&);
extern int mti_scdpi_create_svLogic(const sc_logic&, svLogic&);

extern int mti_scdpi_create_svBitVecVal(const sc_mti_access*, svBitVecVal*);
extern int mti_scdpi_create_svBitVecVal(const sc_mti_access&, svBitVecVal*);
extern int mti_scdpi_create_svLogicVecVal(const sc_mti_access*,svLogicVecVal*);
extern int mti_scdpi_create_svLogicVecVal(const sc_mti_access&,svLogicVecVal*);

#endif
