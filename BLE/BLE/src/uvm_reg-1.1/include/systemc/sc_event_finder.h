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

  sc_event_finder.h --

  Original Author: Martin Janssen, Synopsys, Inc.
                   Stan Y. Liao, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_event_finder.h#1 $

 *****************************************************************************/

#ifndef SC_EVENT_FINDER
#define SC_EVENT_FINDER


#include "sc_port.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_event_finder
//
//  Event finder base class.
// ----------------------------------------------------------------------------

class sc_event_finder
{
  friend class sc_simcontext;

public:

    const sc_port_base& port() const
        { return m_port; }

    // destructor (does nothing)
    virtual ~sc_event_finder();

    virtual const sc_event& find_event( sc_interface* if_p = 0 ) const = 0;

protected:
    
    // constructor
    sc_event_finder( const sc_port_base& );

    // error reporting
    void report_error( const char* id, const char* add_msg = 0 ) const;


private:
    const  sc_port_base&    m_port;    // port providing the event.

private:

    // disabled
    sc_event_finder();
    sc_event_finder( const sc_event_finder& );
    sc_event_finder& operator = ( const sc_event_finder& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_event_finder_t<IF>
//
//  Interface specific event finder class.
// ----------------------------------------------------------------------------

template <class IF>
class sc_event_finder_t
: public sc_event_finder
{
public:

    // constructor

    sc_event_finder_t( const sc_port_base& port_,
		       const sc_event& (IF::*event_method_) () const )
        : sc_event_finder( port_ ), m_event_method( event_method_ )
        {}

    // destructor (does nothing)

    virtual ~sc_event_finder_t()
        {}

    virtual const sc_event& find_event( sc_interface* if_p = 0 ) const;

private:

    const sc_event& (IF::*m_event_method) () const;

private:

    // disabled
    sc_event_finder_t();
    sc_event_finder_t( const sc_event_finder_t<IF>& );
    sc_event_finder_t<IF>& operator = ( const sc_event_finder_t<IF>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class IF>
inline
const sc_event&
sc_event_finder_t<IF>::find_event( sc_interface* if_p ) const
{
    const IF* iface = ( if_p ) ? DCAST<const IF*>( if_p ) :
		                 DCAST<const IF*>( port().get_interface() );
    if( iface == 0 ) {
		report_error( SC_ID_FIND_EVENT_, "port is not bound" );
    }
    return (CCAST<IF*>( iface )->*m_event_method) ();
}

} // namespace sc_core

#endif

// Taf!
