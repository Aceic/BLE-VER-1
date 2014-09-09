/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_interface.h -- Abstract base class of all interface classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_interface.h#1 $

 *****************************************************************************/

#ifndef SC_INTERFACE_H
#define SC_INTERFACE_H

// BEGIN MODELSIM ADDITIONS
#include "scmti.h"
// END MODELSIM ADDITIONS

namespace sc_core {

// BEGIN MODELSIM ADDITIONS
class sc_object;

// Enable debug for user defined sc_port
#define MTI_SC_PORT_ENABLE_DEBUG \
	virtual mtiObjectHandleT mti_get_channel_id(sc_object*) \
	{ \
		return mti_get_prim_channel(); \
	}

// END MODELSIM ADDITIONS

class sc_event;
class sc_port_base;


// ----------------------------------------------------------------------------
//  CLASS : sc_interface
//
//  Abstract base class of all interface classes.
//  BEWARE: Direct inheritance from this class must be done virtual.
// ----------------------------------------------------------------------------

class sc_interface
{
public:

    // register a port with this interface (does nothing by default)
    virtual void register_port( sc_port_base& port_,
				const char*    if_typename_ );

    // get the default event
    virtual const sc_event& default_event() const;

    // destructor (does nothing)
    virtual ~sc_interface();

protected:

    // constructor (does nothing)
    sc_interface();

private:

    // disabled
    sc_interface( const sc_interface& );
    sc_interface& operator = ( const sc_interface& );

private:

    static sc_event m_never_notified;

#if __SUNPRO_CC == 0x520
    // Workaround for a bug in the Sun WorkShop 6 update 2 compiler.
    // An empty virtual base class can cause the optimizer to
    // generate wrong code.
    char dummy;
#endif

	// BEGIN MODELSIM ADDITIONS

public:

	// get a mti signal id of the channel
	virtual mtiObjectHandleT mti_get_channel_id( sc_object* bound_object );

	//
	// MTI support for a binding of a sub interface (signal)              //
	// 'mti_bind_sub_interface()' indicates whether binding to this       //
	//   interface should be to a sub-interface.                          //
	// 'mti_get_sub_interface()' returns the sub-interface (for a bind).  //
	// Note: This is used in sc_port where the sub-interface is used      //
	// instead of the interface for binding. This feature is specifically //
	// used to support the internal clock signal in sc_clock (MTI).       //
	// Note: For MTI internal use only. These methods may be supported in //
	// the future.                                                        //
	//
	virtual bool mti_bind_sub_interface();
	virtual sc_interface* mti_get_sub_interface();

	// END MODELSIM ADDITIONS
};

} // namespace sc_core

#endif

// Taf!
