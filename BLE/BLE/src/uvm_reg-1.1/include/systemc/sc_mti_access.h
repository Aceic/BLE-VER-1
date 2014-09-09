/*****************************************************************************
 *
 * SystemC MTI debug interface class.
 *
 * Copyright 1991-2011 Mentor Graphics Corporation
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
 * MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
 * $Revision: #1 $
 * $Id: //dvt/mti/rel/10.0b/src/systemc/sc_mti_access.h#1 $
 *
 *****************************************************************************/

#ifndef SC_MTI_ACCESS_H
#define SC_MTI_ACCESS_H

#include "scmti.h"


class sc_mti_access
{
public:

	virtual void mti_get_value_size(
		int* has_construction_param,
		int* elem_num_bits,
		int* elem_num_integer_bits) {

		*has_construction_param = 0;
		*elem_num_bits = 0;
		*elem_num_integer_bits = 0;
	}

	virtual void* mti_get_value() = 0;

	virtual void mti_set_value(void* value) = 0;

	virtual void mti_get_dpi_value(void* value) const {}

	virtual void mti_set_dpi_value(void* value) {}

	virtual mtiTypeIdT mti_get_value_type() { return NULL; }

protected:

    // constructor
    sc_mti_access()
	{}

	virtual ~sc_mti_access()
	{}
};


#endif
