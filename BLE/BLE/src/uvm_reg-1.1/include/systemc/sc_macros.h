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

  sc_macros.h -- Miscellaneous definitions that are needed by the headers.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_macros.h#1 $

 *****************************************************************************/

#ifndef SC_MACROS_H
#define SC_MACROS_H

namespace sc_dt {

template <class T>
inline
const T
sc_min( const T& a, const T& b )
{
    return ( ( a <= b ) ? a : b );
}

template <class T>
inline
const T
sc_max( const T& a, const T& b )
{
    return ( ( a >= b ) ? a : b );
}

template <class T>
inline
const T
sc_abs( const T& a )
{
    // return ( a >= 0 ? a : -a );
    // the code below is functionaly the same as the code above; the
    // difference is that the code below works for all arithmetic
    // SystemC datatypes.
    T z( a );
    z = 0;
    if( a >= z ) {
	return a;
    } else {
	T c( a );
	c = -a;
	return c;
    }
}

} // namespace sc_dt

namespace sc_core {

#if defined(__GNUC__) && defined(USE_RTTI)
#define HAVE_CAST_OPERATORS
#endif


#if defined(__GNUC__)
// 10.3.5 - Some compilers (e.g. K&A C++) do not support the
// construct in which a virtual function defined in a subclass returns
// a pointer or reference to a class D whereas the declaration of the
// same virtual function in the base class returns a pointer or
// reference to a base class B of D.
#define ANSI_VIRTUAL_RETURN_INHERITED_TYPE
#endif

// BEGIN MODELSIM MODIFICATION

// END MODELSIM MODIFICATION

/*
 *  These help debugging --
 *  -- user can find out where each process is stopped at.
 */

#define WAIT()                                                                \
    sc_set_location( __FILE__, __LINE__ );                                    \
    wait()

#define WAITN(n)                                                              \
    sc_set_location( __FILE__, __LINE__ );                                    \
    wait(n)

#define WAIT_UNTIL(expr)                                                    \
    sc_set_location( __FILE__, __LINE__ );                                    \
    do { wait(); } while( !(expr) )

} // namespace sc_core


#endif
