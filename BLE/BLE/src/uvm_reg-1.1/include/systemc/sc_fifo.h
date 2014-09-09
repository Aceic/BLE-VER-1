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

  sc_fifo.h -- The sc_fifo<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_fifo.h#1 $

 *****************************************************************************/

#ifndef SC_FIFO_H
#define SC_FIFO_H


#include "sc_communication_ids.h"
#include "sc_prim_channel.h"
#include "sc_fifo_ifs.h"
#include "sc_event.h"
#include "sc_simcontext.h"
#include "sc_trace.h"
#include <typeinfo>

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_fifo<T>
//
//  The sc_fifo<T> primitive channel class.
// ----------------------------------------------------------------------------

// BEGIN MODELSIM MODIFICATIONS

template <class T>
class sc_fifo
: public sc_prim_channel,  // Need to be the first to ensure the address
						   // of the class is in the registery for name
						   // binding to work correctly.
  public sc_fifo_in_if<T>,
  public sc_fifo_out_if<T>

// END MODELSIM MODIFICATIONS
{
public:

    // constructors

    explicit sc_fifo( int size_ = 16 )
	: sc_prim_channel( sc_gen_unique_name( "fifo" ) )
	{ 
		init( size_ ); 
		// BEGIN MODELSIM MODIFICATIONS
		m_data_read_event.mti_set_is_prim_channel_event(true);
		m_data_written_event.mti_set_is_prim_channel_event(true);
		sc_object::mti_set_use_default_name(true);
#ifndef SC_MTI_DELAY_DEBUG_FIFO_MEM
		if (m_mti_value == NULL) {
			size_t int_size = sizeof(int);
			size_t type_size = sizeof(T);
			int num_extra_elem_required = int_size/type_size;
			int* num_used_data = NULL;
			num_extra_elem_required += int_size%type_size ? 1 : 0;

			m_mti_value = new T[m_size + num_extra_elem_required];
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

    explicit sc_fifo( const char* name_, int size_ = 16 )
	: sc_prim_channel( name_ )
	{
		init( size_ ); 
		// BEGIN MODELSIM MODIFICATIONS
		m_data_read_event.mti_set_is_prim_channel_event(true);
		m_data_written_event.mti_set_is_prim_channel_event(true);
#ifndef SC_MTI_DELAY_DEBUG_FIFO_MEM
		if (m_mti_value == NULL) {
			size_t int_size = sizeof(int);
			size_t type_size = sizeof(T);
			int num_extra_elem_required = int_size/type_size;
			int* num_used_data = NULL;
			num_extra_elem_required += int_size%type_size ? 1 : 0;

			m_mti_value = new T[m_size + num_extra_elem_required];
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

    virtual ~sc_fifo()
	{ 
		delete [] m_buf; 
		// BEGIN MODELSIM MODIFICATIONS
		if (m_mti_value != NULL) {
			if (m_mti_is_uchar_val) {
				delete[] (unsigned char*)m_mti_value;
			} else {
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
		// END MODELSIM MODIFICATIONS
	}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );

    // blocking read
    virtual void read( T& );
    virtual T read();

    // non-blocking read
    virtual bool nb_read( T& );


    // get the number of available samples

    virtual int num_available() const
	{ return ( m_num_readable - m_num_read ); }


    // get the data written event

    virtual const sc_event& data_written_event() const
	{ return m_data_written_event; }


    // blocking write
    virtual void write( const T& );

    // non-blocking write
    virtual bool nb_write( const T& );


    // get the number of free spaces

    virtual int num_free() const
	{ return ( m_size - m_num_readable - m_num_written ); }


    // get the data read event

    virtual const sc_event& data_read_event() const
	{ return m_data_read_event; }


    // other methods

    operator T ()
	{ return read(); }


    sc_fifo<T>& operator = ( const T& a )
        { write( a ); return *this; }


    void trace( sc_trace_file* tf ) const;


    virtual void print( ::std::ostream& = ::std::cout ) const;
    virtual void dump( ::std::ostream& = ::std::cout ) const;

    virtual const char* kind() const
        { return "sc_fifo"; }

protected:

    virtual void update();

    // support methods

    void init( int );

    void buf_init( int );
    bool buf_write( const T& );
    bool buf_read( T& );

protected:

    int m_size;			// size of the buffer
    T*  m_buf;			// the buffer
    int m_free;			// number of free spaces
    int m_ri;			// index of next read
    int m_wi;			// index of next write

    sc_port_base* m_reader;	// used for static design rule checking
    sc_port_base* m_writer;	// used for static design rule checking

    int m_num_readable;		// #samples readable
    int m_num_read;		// #samples read during this delta cycle
    int m_num_written;		// #samples written during this delta cycle

    sc_event m_data_read_event;
    sc_event m_data_written_event;

private:

    // disabled
    sc_fifo( const sc_fifo<T>& );
    sc_fifo& operator = ( const sc_fifo<T>& );

	// BEGIN MODELSIM ADDITIONS

protected:
	void* m_mti_value;  // the data passed to vsim side
	void* m_mti_fifo_data; // pointer into the FIFO portion of the m_mti_value buffer

	bool  m_mti_is_uchar_val; // Flag to determine mti value type

	int* m_mti_saved_bytes; // Some saved bytes of data

	virtual void* mti_get_channel_value(int is_driving_value);

	virtual void mti_get_construction_params(
		int* elem_num_bits,
		int* elem_num_integer_bits,
		int* fifo_num_elem,
		int* hasConstructionParams,
		int* hasElementConstructionParams);

	virtual mtiObjectHandleT mti_get_channel_id( sc_object* port_ );
	// END MODELSIM ADDITIONS
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
inline
void
sc_fifo<T>::register_port( sc_port_base& port_,
			    const char* if_typename_ )
{
	std::string nm( if_typename_ );
    if( nm == typeid( sc_fifo_in_if<T> ).name() ) {
	// only one reader can be connected
	if( m_reader != 0 ) {
	    SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 );
	}
	m_reader = &port_;
    } else {  // nm == typeid( sc_fifo_out_if<T> ).name()
	// only one writer can be connected
	if( m_writer != 0 ) {
	    SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 );
	}
	m_writer = &port_;
    }
}


// blocking read

template <class T>
inline
void
sc_fifo<T>::read( T& val_ )
{
    while( num_available() == 0 ) {
	sc_core::wait( m_data_written_event );
    }
    m_num_read ++;
    buf_read( val_ );
    request_update();
}

template <class T>
inline
T
sc_fifo<T>::read()
{
    T tmp;
    read( tmp );
    return tmp;
}

// non-blocking read

template <class T>
inline
bool
sc_fifo<T>::nb_read( T& val_ )
{
    if( num_available() == 0 ) {
	return false;
    }
    m_num_read ++;
    buf_read( val_ );
    request_update();
    return true;
}


// blocking write

template <class T>
inline
void
sc_fifo<T>::write( const T& val_ )
{
    while( num_free() == 0 ) {
	sc_core::wait( m_data_read_event );
    }
    m_num_written ++;
    buf_write( val_ );
    request_update();
}

// non-blocking write

template <class T>
inline
bool
sc_fifo<T>::nb_write( const T& val_ )
{
    if( num_free() == 0 ) {
	return false;
    }
    m_num_written ++;
    buf_write( val_ );
    request_update();
    return true;
}


template <class T>
inline
void
sc_fifo<T>::trace( sc_trace_file* tf ) const
{
#if defined(DEBUG_SYSTEMC)
    char buf[32];
	std::string nm = name();
    for( int i = 0; i < m_size; ++ i ) {
	std::sprintf( buf, "_%d", i );
	sc_trace( tf, m_buf[i], nm + buf );
    }
#endif
}


template <class T>
inline
void
sc_fifo<T>::print( ::std::ostream& os ) const
{
    if( m_free != m_size ) {
        int i = m_ri;
        do {
            os << m_buf[i] << ::std::endl;
            i = ( i + 1 ) % m_size;
        } while( i != m_wi );
    }
}

template <class T>
inline
void
sc_fifo<T>::dump( ::std::ostream& os ) const
{
    os << "name = " << name() << ::std::endl;
    if( m_free != m_size ) {
        int i = m_ri;
        int j = 0;
        do {
	    os << "value[" << i << "] = " << m_buf[i] << ::std::endl;
	    i = ( i + 1 ) % m_size;
	    j ++;
        } while( i != m_wi );
    }
}


template <class T>
inline
void
sc_fifo<T>::update()
{
    if( m_num_read > 0 ) {
	m_data_read_event.notify(SC_ZERO_TIME);
    }

    if( m_num_written > 0 ) {
	m_data_written_event.notify(SC_ZERO_TIME);
    }

    m_num_readable = m_size - m_free;
    m_num_read = 0;
    m_num_written = 0;
}


// support methods

template <class T>
inline
void
sc_fifo<T>::init( int size_ )
{
    buf_init( size_ );

    m_reader = 0;
    m_writer = 0;

    m_num_readable = 0;
    m_num_read = 0;
    m_num_written = 0;

	// BEGIN MODELSIM ADDITIONS
	m_data_read_event.mti_set_is_prim_channel_event(true);
	m_data_written_event.mti_set_is_prim_channel_event(true);
	// END MODELSIM ADDITIONS
}


template <class T>
inline
void
sc_fifo<T>::buf_init( int size_ )
{
    if( size_ <= 0 ) {
	SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 );
    }
    m_size = size_;
    m_buf = new T[m_size];
    m_free = m_size;
    m_ri = 0;
    m_wi = 0;
	// BEGIN MODELSIM ADDITIONS
	m_mti_value = NULL;
	m_mti_is_uchar_val = true;
	// END MODELSIM ADDITIONS
}

template <class T>
inline
bool
sc_fifo<T>::buf_write( const T& val_ )
{
    if( m_free == 0 ) {
	return false;
    }
    m_buf[m_wi] = val_;
    m_wi = ( m_wi + 1 ) % m_size;
    m_free --;
    return true;
}

template <class T>
inline
bool
sc_fifo<T>::buf_read( T& val_ )
{
    if( m_free == m_size ) {
	return false;
    }
    val_ = m_buf[m_ri];
    m_ri = ( m_ri + 1 ) % m_size;
    m_free ++;
    return true;
}


// ----------------------------------------------------------------------------

template <class T>
inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_fifo<T>& a )
{
    a.print( os );
    return os;
}

} // namespace sc_core

// BEGIN MODELSIM ADDITIONS

#include "sc_fifo_mti.h"

// END MODELSIM ADDITIONS



#endif

// Taf!
