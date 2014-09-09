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

  sc_mutex.h -- The sc_mutex primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_mutex.h#1 $

 *****************************************************************************/

#ifndef SC_MUTEX_H
#define SC_MUTEX_H

#include "sc_event.h"
#include "sc_prim_channel.h"
#include "sc_mutex_if.h"

namespace sc_core {


// ----------------------------------------------------------------------------
//  CLASS : sc_mutex
//
//  The sc_mutex primitive channel class.
// ----------------------------------------------------------------------------

// BEGIN MODELSIM MODIFICATIONS

class sc_mutex
: public sc_prim_channel,  // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_mutex_if

// END MODELSIM MODIFICATIONS
{
public:

    // constructors

    sc_mutex();
    explicit sc_mutex( const char* name_ );

	virtual ~sc_mutex();

    // interface methods

    // blocks until mutex could be locked
    virtual int lock();

    // returns -1 if mutex could not be locked
    virtual int trylock();

    // returns -1 if mutex was not locked by caller
    virtual int unlock();

    virtual const char* kind() const
        { return "sc_mutex"; }

protected:

    // support methods

    bool in_use() const
	{ return ( m_owner != 0 ); }

protected:

    sc_process_b* m_owner;
    sc_event      m_free;

private:

    // disabled
    sc_mutex( const sc_mutex& );
    sc_mutex& operator = ( const sc_mutex& );

	// BEGIN MODELSIM ADDITIONS
protected:

	virtual mtiObjectHandleT mti_get_channel_id( sc_object* );
	virtual void end_of_elaboration();

private:
	unsigned char m_mti_value;

	// END MODELSIM ADDITIONS
};

} // namespace sc_core

#endif

// Taf!
