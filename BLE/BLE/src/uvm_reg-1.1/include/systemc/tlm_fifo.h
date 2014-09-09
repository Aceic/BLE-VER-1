/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/systemc/tlm_fifo.h#1 $ */

#ifndef __TLM_FIFO_H__
#define __TLM_FIFO_H__

//
// This implements put, get and peek
//
// It also implements 0 and infinite size fifos - but the size
// zero fifos aren't rendezvous like zero length fifos, they simply are both
// full and empty at the same time.
//
// The size can be dynamically changed using the resize interface
//
// To get an infinite fifo use a -ve size in the constructor.
// The absolute value of the size is taken as the starting size of the
// actual physical buffer.
//

// BEGIN MODELSIM MODIFICATIONS
#include <systemc.h>
// END MODELSIM MODIFICATIONS

#include "tlm_fifo_ifs.h"
#include "circular_buffer.h"

namespace tlm {

// BEGIN MODELSIM MODIFICATIONS
template <class T>
class tlm_fifo :
  public sc_prim_channel,	// Need to be the first to ensure the address
							// of the class is in the registery for name
							// binding to work correctly.
  public virtual tlm_fifo_get_if<T>,
  public virtual tlm_fifo_put_if<T>
// END MODELSIM MODIFICATIONS
{
public:

    // constructors

    explicit tlm_fifo( int size_ = 1 )
      : sc_core::sc_prim_channel( sc_core::sc_gen_unique_name( "fifo" ) ) {
     
      init( size_ );

      // BEGIN MODELSIM MODIFICATIONS
      sc_object::mti_set_use_default_name(true);
#ifdef SC_MTI_DELAY_DEBUG_FIFO_MEM
		if (m_mti_value == NULL) {

			size_t int_size = sizeof(int);
			size_t type_size = sizeof(T);
			int num_extra_elem_required = int_size/type_size;
			int* num_used_data = NULL;

			num_extra_elem_required += int_size%type_size ? 1 : 0;

			m_mti_value = new T[m_mti_initial_fifo_size + num_extra_elem_required];
			m_mti_fifo_data = &((T*)m_mti_value)[num_extra_elem_required];
			m_mti_is_uchar_val = false;

			//
			// Save the 4 bytes of data we would be using for storing the
			// number of used fifo elements.  These 4 bytes are part of a
			// user defined type and we need to restore the 4 bytes before
			// deleting the class.  If we try to delete the class without
			// restoring the bytes, the destructor of the class might crash.
			//
			m_mti_saved_bytes = new int();
			num_used_data = (int*)m_mti_fifo_data;
			num_used_data--;
			*m_mti_saved_bytes = *num_used_data;
		}
#endif
      // END MODELSIM MODIFICATIONS
    }

    explicit tlm_fifo( const char* name_, int size_ = 1 )
      : sc_core::sc_prim_channel( name_ ) {
    
      init( size_ );
      // BEGIN MODELSIM MODIFICATIONS
#ifdef SC_MTI_DELAY_DEBUG_FIFO_MEM
		if (m_mti_value == NULL) {

			size_t int_size = sizeof(int);
			size_t type_size = sizeof(T);
			int num_extra_elem_required = int_size/type_size;
			int* num_used_data = NULL;

			num_extra_elem_required += int_size%type_size ? 1 : 0;

			m_mti_value = new T[m_mti_initial_fifo_size + num_extra_elem_required];
			m_mti_fifo_data = &((T*)m_mti_value)[num_extra_elem_required];
			m_mti_is_uchar_val = false;

			//
			// Save the 4 bytes of data we would be using for storing the
			// number of used fifo elements.  These 4 bytes are part of a
			// user defined type and we need to restore the 4 bytes before
			// deleting the class.  If we try to delete the class without
			// restoring the bytes, the destructor of the class might crash.
			//
			m_mti_saved_bytes = new int();
			num_used_data = (int*)m_mti_fifo_data;
			num_used_data--;
			*m_mti_saved_bytes = *num_used_data;
		}
#endif
      // END MODELSIM MODIFICATIONS
    }

    // destructor

    virtual ~tlm_fifo() {

      if( buffer != 0 ) delete buffer;
	  // BEGIN MODELSIM ADDITIONS
	  if (m_mti_value != 0) {
	  	if (m_mti_is_uchar_val) 
			delete[] (unsigned char*)m_mti_value;
		else {
			//
			// Restore the saved data bytes to the memory location we used
			// for the number of used fifo elements.  This is essential
			// to avoid the destructor of the type from crashing.  Since
			// we write to the memory location of the class we might end
			// up curroupting the class fields.
			//
			int* num_used_data = (int*)m_mti_fifo_data;
			num_used_data--;
			*num_used_data = *m_mti_saved_bytes;
			delete[] (T*)m_mti_value;
			delete m_mti_saved_bytes;
		}
	  }
	  // END MODELSIM ADDITIONS
    }

    // tlm get interface

    T get( tlm_tag<T> *t = 0 );

    bool nb_get( T& );
    bool nb_can_get( tlm_tag<T> *t = 0 ) const;
    const sc_core::sc_event &ok_to_get( tlm_tag<T> *t = 0 ) const {
      return m_data_written_event;
    }

    // tlm peek interface

    T peek( tlm_tag<T> *t = 0 ) const;

    bool nb_peek( T& ) const;
    bool nb_can_peek( tlm_tag<T> *t = 0 ) const;
    const sc_core::sc_event &ok_to_peek( tlm_tag<T> *t = 0 ) const {
      return m_data_written_event;
    }

    // tlm put interface 

    void put( const T& );

    bool nb_put( const T& );
    bool nb_can_put( tlm_tag<T> *t = 0 ) const;

    const sc_core::sc_event& ok_to_put( tlm_tag<T> *t = 0 ) const {
      return m_data_read_event;
    }

    // resize if

    void nb_expand( unsigned int n = 1 );
    void nb_unbound( unsigned int n = 16 );

    bool nb_reduce( unsigned int n = 1 );
    bool nb_bound( unsigned int n );

    // debug interface

    bool nb_peek( T & , int n ) const;
    bool nb_poke( const T & , int n = 0 );

    int used() const {
      return m_num_readable - m_num_read;
    }

    int size() const {
      return m_size;
    }

    void debug() const {

      if( is_empty() ) std::cout << "empty" << std::endl;
      if( is_full() ) std::cout << "full" << std::endl;

      std::cout << "size " << size() << " - " << used() << " used "
                << std::endl;
      std::cout << "readable " << m_num_readable
                << std::endl;
      std::cout << "written/read " << m_num_written << "/" << m_num_read
                << std::endl;

    }

    // support functions

    static const char* const kind_string;

    const char* kind() const
        { return kind_string; }


protected:
    sc_core::sc_event &read_event( tlm_tag<T> *t = 0 ) {
      return m_data_read_event;
    }

protected:

    void update();

    // support methods

    void init( int );

protected:

    circular_buffer<T> *buffer;

    int m_size;                  // logical size of fifo

    int m_num_readable;	         // #samples readable
    int m_num_read;	         // #samples read during this delta cycle
    int m_num_written;	         // #samples written during this delta cycle
    bool m_expand;               // has an expand occurred during this delta cycle ? 
    int m_num_read_no_notify;    // #samples read without notify during this delta cycle

    sc_core::sc_event m_data_read_event;
    sc_core::sc_event m_data_written_event;

private:

    // disabled
    tlm_fifo( const tlm_fifo<T>& );
    tlm_fifo& operator = ( const tlm_fifo<T>& );

    //
    // use nb_can_get() and nb_can_put() rather than the following two
    // private functions
    //

    bool is_empty() const { 
      return used() == 0;
    }

    bool is_full() const {
      return size() == m_num_readable + m_num_written;
    }

	// BEGIN MODELSIM ADDITIONS

public:
    virtual void* mti_get_channel_value(int is_driving_value);

    virtual void mti_get_construction_params(
        int* elem_num_bits,
        int* elem_num_integer_bits,
        int* fifo_num_elem,
        int* hasConstructionParams,
        int* hasElementConstructionParams);

    virtual mtiObjectHandleT mti_get_channel_id( sc_port_base* port_ );

private:
    void* m_mti_value;  // the data passed to vsim side
    void* m_mti_fifo_data;  // pointer into the FIFO portion of the m_mti_value buffer
    int   m_mti_initial_fifo_size; // MTI fifo's are static and the fifo size is fixed at construction
    int   m_mti_is_resize_warning_not_given; // MTI issues a warning if the fifo size is resized
	int   m_mti_is_uchar_val; // Flag for MTI value type
	int* m_mti_saved_bytes; // Some saved bytes of data

    // END MODELSIM ADDITIONS
};

template <typename T>
const char* const tlm_fifo<T>::kind_string = "tlm_fifo";


/******************************************************************
//
// init and update
//
******************************************************************/

template< typename T >
inline 
void
tlm_fifo<T>::init( int size_ ) {

  if( size_ > 0 ) {
    buffer = new circular_buffer<T>( size_);
  }
  
  else if( size_ < 0 ) {
    buffer = new circular_buffer<T>( -size_ );
  }

  else {
    buffer = new circular_buffer<T>( 16 );
  }

  m_size = size_;
  m_num_readable = 0;
  m_num_read = 0;
  m_num_written = 0;
  m_expand = false;
  m_num_read_no_notify = false;

  // BEGIN MODELSIM ADDITIONS

  m_mti_value = 0;
  m_mti_initial_fifo_size = m_size;
  m_mti_is_resize_warning_not_given = 1;
  m_mti_is_uchar_val = true;

  m_data_read_event.mti_set_is_prim_channel_event(true);
  m_data_written_event.mti_set_is_prim_channel_event(true);

  // END MODELSIM ADDITIONS
}

template < typename T>
inline
void
tlm_fifo<T>::update()
{
    if( m_num_read > m_num_read_no_notify || m_expand ) {
	m_data_read_event.notify( sc_core::SC_ZERO_TIME );
    }

    if( m_num_written > 0 ) {
	m_data_written_event.notify( sc_core::SC_ZERO_TIME );
    }

    m_expand = false;
    m_num_read = 0;
    m_num_written = 0;
    m_num_readable = buffer->used();
    m_num_read_no_notify = 0;

}

// BEGIN MODELSIM ADDITIONS
#include "tlm_fifo_mti.h"
// END MODELSIM ADDITIONS

} // namespace tlm

#include "tlm_fifo_put_get.h"
#include "tlm_fifo_peek.h"
#include "tlm_fifo_resize.h"

#endif

