/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_master_slave_utils.h -- utilities functions

  Original Author: Dirk Vermeersch, Coware, Inc.
                   Vijay Kumar, Coware, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/scv/scms/sc_master_slave_utils.h#1 $

 *****************************************************************************/


#ifndef SC_MASTER_SLAVE_UTILS_H
#define SC_MASTER_SLAVE_UTILS_H

// iterate through a container of pointers and delete each of the pointer.
// Note:- the container itself is not deleted only the pointer the container
// holds
template< class C>
void delete_container_pointers( C& c )
{
  for ( typename C::iterator it( c ); !it.empty( ) ; it++ ) {
    delete it.contents( );
    it.set_contents( 0 );
  }
}

#endif // SC_MASTER_SLAVE_UTILS_H
