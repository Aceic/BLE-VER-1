#include "sc_prim_channel.h"
#include "sc_fifo_ifs.h"
#include "sc_event.h"
#include "sc_simcontext.h"
#include "sc_logic.h"
#include "sc_signed.h"
#include "sc_unsigned.h"
#include "sc_lv.h"
#include "sc_bv.h"
#include "sc_bigint.h"
#include "sc_trace.h"
#include <typeinfo>

#ifdef SC_INCLUDE_FX
#include "sc_fix.h"
#include "sc_fixed.h"
#include "sc_ufix.h"
#include "sc_ufixed.h"
#endif

/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_fifo_mti.h#1 $ */

namespace sc_core {

#define MTI_DECLARE_SCTYPE_FIFO_FUNCTIONS(TYPE) \
	template <> \
    inline sc_fifo<TYPE>::sc_fifo(int size_) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
	{ \
		init( size_ ); \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
		sc_object::mti_set_use_default_name(true); \
	} \
\
	template <> \
    inline sc_fifo<TYPE>::sc_fifo( const char* name_, int size_ ) \
		: sc_prim_channel( name_ ) \
	{ \
		init( size_ );  \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <> \
	inline void* sc_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		int numOfBits  = m_buf[0].length(); \
\
		if (m_mti_value == NULL) { \
			int d_size = sizeof(int) + (sizeof(unsigned char) * numOfBits) * m_size; \
			m_mti_value = new unsigned char[d_size]; \
			memset(m_mti_value, 0, d_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, numOfBits); \
			valp += numOfBits; \
		} \
		return (void *) bufp; \
	} \
\
	template <> \
	inline void sc_fifo<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	}


#define MTI_DECLARE_SCTYPE_FIFO_BIT_LOGIC_FUNCTIONS(TYPE) \
	template <> \
    inline sc_fifo<TYPE>::sc_fifo( int size_) \
	: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
	{  \
		init( size_ );  \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
		sc_object::mti_set_use_default_name(true); \
	} \
\
	template <> \
    inline sc_fifo<TYPE>::sc_fifo( const char* name_, int size_) \
	: sc_prim_channel( name_ ) \
	{ \
		init( size_ ); \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <> \
	inline void* sc_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
\
		if (m_mti_value == NULL) { \
			int n_size = sizeof(int) + sizeof(unsigned char) * m_size; \
			m_mti_value = new unsigned char[n_size]; \
			memset(m_mti_value, 0, n_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		for (int i = 0; i < *sptr ; i++) { \
			bufp[i] = *(unsigned char*)m_buf[(m_ri + i) % m_size].mti_get_value(); \
		} \
\
		return (void *)bufp; \
	}

#define MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W> \
	class sc_fifo<TYPE<W> > \
	: public sc_prim_channel, \
	  public sc_fifo_in_if<TYPE<W> >, \
	  public sc_fifo_out_if<TYPE<W> > \
	{ \
	public: \
		explicit sc_fifo( int size_ = 16 ) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
		{ \
			init( size_ ); \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_fifo( const char* name_, int size_ = 16 ) \
		: sc_prim_channel( name_ ) \
		{ \
			init( size_ );  \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
		} \
\
		virtual ~sc_fifo() \
		{ \
			delete [] m_buf; \
			if (m_mti_value != NULL) \
				delete [] (unsigned char*)m_mti_value; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
		virtual void read( TYPE<W>& ); \
		virtual TYPE<W> read(); \
\
		virtual bool nb_read( TYPE<W>& ); \
\
		virtual int num_available() const \
		{ return ( m_num_readable - m_num_read ); } \
\
		virtual const sc_event& data_written_event() const \
		{ return m_data_written_event; } \
\
		virtual void write( const TYPE<W>& ); \
\
		virtual bool nb_write( const TYPE<W>& ); \
\
		virtual int num_free() const \
		{ return ( m_size - m_num_readable - m_num_written ); } \
\
		virtual const sc_event& data_read_event() const \
		{ return m_data_read_event; } \
\
		operator TYPE<W> () \
		{ return read(); } \
\
		sc_fifo<TYPE<W> >& operator = ( const TYPE<W>& a ) \
			{ write( a ); return *this; } \
\
		void trace( sc_trace_file* tf ) const; \
\
		virtual void print( ::std::ostream&  = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_fifo"; } \
\
	protected: \
\
		virtual void update(); \
\
		void init( int ); \
\
		void buf_init( int ); \
		bool buf_write( const TYPE<W>& ); \
		bool buf_read( TYPE<W>& ); \
\
	protected: \
\
		int m_size; \
		TYPE<W>*  m_buf; \
		int m_free; \
		int m_ri; \
		int m_wi; \
\
		sc_port_base* m_reader; \
		sc_port_base* m_writer; \
\
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
	private: \
\
		sc_fifo( const sc_fifo<TYPE<W> >& ); \
		sc_fifo& operator = ( const sc_fifo<TYPE<W> >& ); \
\
	protected: \
		unsigned char* m_mti_value; \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		mtiObjectHandleT mti_get_channel_id( sc_object* bound_object ); \
	}; \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::read( TYPE<W>& val_ ) \
	{ \
		while( num_available() == 0 ) { \
		sc_core::wait( m_data_written_event ); \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
	} \
\
	template <int W> \
	inline \
	TYPE<W> \
	sc_fifo<TYPE<W> >::read() \
	{ \
		TYPE<W> tmp; \
		read( tmp ); \
		return tmp; \
	} \
\
	template <int W> \
	inline \
	bool \
	sc_fifo<TYPE<W> >::nb_read( TYPE<W>& val_ ) \
	{ \
		if( num_available() == 0 ) { \
		return false; \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
		return true; \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::write( const TYPE<W>& val_ ) \
	{ \
		while( num_free() == 0 ) { \
		sc_core::wait( m_data_read_event ); \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
	} \
\
	template <int W> \
	inline \
	bool \
	sc_fifo<TYPE<W> >::nb_write( const TYPE<W>& val_ ) \
	{ \
		if( num_free() == 0 ) { \
		return false; \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
		return true;  \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::trace( sc_trace_file* tf ) const \
	{} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::print( ::std::ostream& os ) const \
	{ \
		if( m_free != m_size ) { \
			int i = m_ri; \
			do { \
				os << m_buf[i] << ::std::endl; \
				i = ( i + 1 ) % m_size; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::dump( ::std::ostream& os ) const \
	{ \
		os << "name = " << name() << ::std::endl; \
		if( m_free != m_size ) { \
			int i = m_ri; \
			int j = 0; \
			do { \
			os << "value[" << i << "] = " << m_buf[i] << ::std::endl; \
			i = ( i + 1 ) % m_size; \
			j ++; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::update() \
	{ \
		if( m_num_read > 0 ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
\
		m_num_readable = m_size - m_free; \
		m_num_read = 0; \
		m_num_written = 0; \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::init( int size_ ) \
	{ \
		buf_init( size_ ); \
\
		m_reader = 0; \
		m_writer = 0; \
\
		m_num_readable = 0; \
		m_num_read = 0; \
		m_num_written = 0; \
\
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::buf_init( int size_ ) \
	{ \
		if( size_ <= 0 ) { \
		SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 ); \
		} \
		m_size = size_; \
		m_buf = new TYPE<W>[m_size]; \
		m_free = m_size; \
		m_ri = 0; \
		m_wi = 0; \
		m_mti_value = NULL; \
	} \
\
	template <int W> \
	inline \
	bool \
	sc_fifo<TYPE<W> >::buf_write( const TYPE<W>& val_ ) \
	{ \
		if( m_free == 0 ) { \
		return false; \
		} \
		m_buf[m_wi] = val_; \
		m_wi = ( m_wi + 1 ) % m_size; \
		m_free --; \
		return true; \
	} \
\
	template <int W> \
	inline \
	bool \
	sc_fifo<TYPE<W> >::buf_read( TYPE<W>& val_ ) \
	{ \
		if( m_free == m_size ) { \
		return false; \
		} \
		val_ = m_buf[m_ri]; \
		m_ri = ( m_ri + 1 ) % m_size; \
		m_free ++; \
		return true; \
	} \
\
	template <int W> \
	inline \
	::std::ostream& \
	operator << ( ::std::ostream& os, const sc_fifo<TYPE<W> >& a ) \
	{ \
		a.print( os ); \
		return os; \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	} \
\
	template <int W> \
	inline \
	mtiObjectHandleT \
	sc_fifo<TYPE<W> >::mti_get_channel_id( sc_object* bound_object ) \
	{ \
		if (bound_object) { \
			if (strcmp(bound_object->kind(), "sc_export") ==0) { \
				return mti_get_prim_channel(); \
			} else { \
				sc_port_base* port = (sc_port_base*)bound_object; \
				if (port->mti_is_foreign()) { \
					std::string msg = \
						std::string("port '") + \
						port->name() + \
						"' is connected to an sc_fifo object at the SystemC/HDL boundary."; \
					SC_REPORT_ERROR( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_, msg.c_str() ); \
					return NULL; \
				} else { \
					return mti_get_prim_channel(); \
				} \
			} \
		} \
		return NULL; \
	} \
\
	template <int W> \
	inline \
	void* \
	sc_fifo<TYPE<W> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (m_mti_value == NULL) { \
			int v_size = sizeof(int) + (sizeof(unsigned char) * W) * m_size; \
			m_mti_value = new unsigned char[v_size]; \
			memset(m_mti_value, 0, v_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, W); \
			valp += W; \
		} \
\
		return (void *) bufp; \
	} \
\
	template <int W> \
	inline \
	void \
	sc_fifo<TYPE<W> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		std::string nm( if_typename_ ); \
		if( nm == typeid( sc_fifo_in_if<TYPE<W> > ).name() ) { \
		if( m_reader != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 ); \
		} \
		m_reader = &port_; \
		} else { \
		if( m_writer != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 ); \
		} \
		m_writer = &port_; \
		} \
	}

#define MTI_DECLARE_SC_FIX_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <> \
    inline sc_fifo<TYPE>::sc_fifo(int size_) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
	{ \
		init( size_ ); \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
		sc_object::mti_set_use_default_name(true); \
	} \
\
	template <> \
    inline sc_fifo<TYPE>::sc_fifo( const char* name_, int size_ ) \
		: sc_prim_channel( name_ ) \
	{ \
		init( size_ );  \
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <> \
	inline \
	void* \
	sc_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		int wl = m_buf[0].wl(); \
\
		if (m_mti_value == NULL) { \
			int d_size = sizeof(int) + (sizeof(unsigned char) * wl) * m_size; \
			m_mti_value = new unsigned char[d_size]; \
			memset(m_mti_value, 0, d_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, wl); \
			valp += wl; \
		} \
		return (void *)bufp; \
	} \
\
	template <> \
	inline \
	void \
	sc_fifo<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	}

// Specialized template class for sc_fixed types.
#define MTI_DECLARE_SC_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	class sc_fifo<TYPE<W,I,Q,O,N> > \
	: public sc_prim_channel, \
	  public sc_fifo_in_if<TYPE<W,I,Q,O,N> >, \
	  public sc_fifo_out_if<TYPE<W,I,Q,O,N> > \
	{ \
	public: \
		explicit sc_fifo( int size_ = 16 ) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
		{ \
			init( size_ ); \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_fifo( const char* name_, int size_ = 16 ) \
		: sc_prim_channel( name_ ) \
		{ \
			init( size_ );  \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
		} \
\
		virtual ~sc_fifo() \
		{ \
			delete [] m_buf; \
			if (m_mti_value != NULL) \
				delete [] (unsigned char*)m_mti_value; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
		virtual void read( TYPE<W,I,Q,O,N>& ); \
		virtual TYPE<W,I,Q,O,N> read(); \
\
		virtual bool nb_read( TYPE<W,I,Q,O,N>& ); \
\
		virtual int num_available() const \
		{ return ( m_num_readable - m_num_read ); } \
\
		virtual const sc_event& data_written_event() const \
		{ return m_data_written_event; } \
\
		virtual void write( const TYPE<W,I,Q,O,N>& ); \
\
		virtual bool nb_write( const TYPE<W,I,Q,O,N>& ); \
\
		virtual int num_free() const \
		{ return ( m_size - m_num_readable - m_num_written ); } \
\
		virtual const sc_event& data_read_event() const \
		{ return m_data_read_event; } \
\
		operator TYPE<W,I,Q,O,N> () \
		{ return read(); } \
\
		sc_fifo<TYPE<W,I,Q,O,N> >& operator = ( const TYPE<W,I,Q,O,N>& a ) \
			{ write( a ); return *this; } \
\
		void trace( sc_trace_file* tf ) const; \
\
		virtual void print( ::std::ostream& = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_fifo"; } \
\
	protected: \
\
		virtual void update(); \
\
		void init( int ); \
\
		void buf_init( int ); \
		bool buf_write( const TYPE<W,I,Q,O,N>& ); \
		bool buf_read( TYPE<W,I,Q,O,N>& ); \
\
	protected: \
\
		int m_size; \
		TYPE<W,I,Q,O,N>*  m_buf; \
		int m_free; \
		int m_ri; \
		int m_wi; \
\
		sc_port_base* m_reader; \
		sc_port_base* m_writer; \
\
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
	private: \
\
		sc_fifo( const sc_fifo<TYPE<W,I,Q,O,N> >& ); \
		sc_fifo& operator = ( const sc_fifo<TYPE<W,I,Q,O,N> >& ); \
\
	protected: \
		unsigned char* m_mti_value; \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		mtiObjectHandleT mti_get_channel_id( sc_object* bound_object ); \
	}; \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::read( TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		while( num_available() == 0 ) { \
		sc_core::wait( m_data_written_event ); \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	TYPE<W,I,Q,O,N> \
	sc_fifo<TYPE<W,I,Q,O,N> >::read() \
	{ \
		TYPE<W,I,Q,O,N> tmp; \
		read( tmp ); \
		return tmp; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,Q,O,N> >::nb_read( TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		if( num_available() == 0 ) { \
		return false; \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
		return true; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::write( const TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		while( num_free() == 0 ) { \
		sc_core::wait( m_data_read_event ); \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,Q,O,N> >::nb_write( const TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		if( num_free() == 0 ) { \
		return false; \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
		return true;  \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::trace( sc_trace_file* tf ) const \
	{} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::print( ::std::ostream& os ) const \
	{ \
		if( m_free != m_size ) { \
			int i = m_ri; \
			do { \
				os << m_buf[i] << ::std::endl; \
				i = ( i + 1 ) % m_size; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::dump( ::std::ostream& os ) const \
	{ \
		os << "name = " << name() << ::std::endl; \
		if( m_free != m_size ) { \
			int i = m_ri; \
			int j = 0; \
			do { \
			os << "value[" << i << "] = " << m_buf[i] << ::std::endl; \
			i = ( i + 1 ) % m_size; \
			j ++; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::update() \
	{ \
		if( m_num_read > 0 ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
\
		m_num_readable = m_size - m_free; \
		m_num_read = 0; \
		m_num_written = 0; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::init( int size_ ) \
	{ \
		buf_init( size_ ); \
\
		m_reader = 0; \
		m_writer = 0; \
\
		m_num_readable = 0; \
		m_num_read = 0; \
		m_num_written = 0; \
\
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::buf_init( int size_ ) \
	{ \
		if( size_ <= 0 ) { \
		SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 ); \
		} \
		m_size = size_; \
		m_buf = new TYPE<W,I,Q,O,N>[m_size]; \
		m_free = m_size; \
		m_ri = 0; \
		m_wi = 0; \
		m_mti_value = NULL; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,Q,O,N> >::buf_write( const TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		if( m_free == 0 ) { \
		return false; \
		} \
		m_buf[m_wi] = val_; \
		m_wi = ( m_wi + 1 ) % m_size; \
		m_free --; \
		return true; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,Q,O,N> >::buf_read( TYPE<W,I,Q,O,N>& val_ ) \
	{ \
		if( m_free == m_size ) { \
		return false; \
		} \
		val_ = m_buf[m_ri]; \
		m_ri = ( m_ri + 1 ) % m_size; \
		m_free ++; \
		return true; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	::std::ostream& \
	operator << ( ::std::ostream& os, const sc_fifo<TYPE<W,I,Q,O,N> >& a ) \
	{ \
		a.print( os ); \
		return os; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	mtiObjectHandleT \
	sc_fifo<TYPE<W,I,Q,O,N> >::mti_get_channel_id( sc_object* bound_object ) \
	{ \
		if (bound_object != NULL) { \
			if (strcmp(bound_object->kind(), "sc_export") ==0) { \
				return mti_get_prim_channel(); \
			} else { \
				sc_port_base* port = (sc_port_base*)bound_object; \
				if (port->mti_is_foreign()) { \
					std::string msg = \
						std::string("port '") + \
						port->name() + \
						"' is connected to an sc_fifo object at the SystemC/HDL boundary."; \
					SC_REPORT_ERROR( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_, msg.c_str() ); \
					return NULL; \
				} else { \
					return mti_get_prim_channel(); \
				} \
			} \
		} \
		return NULL; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void* \
	sc_fifo<TYPE<W,I,Q,O,N> >::mti_get_channel_value(int is_driving_value) \
	{ \
		int wl = m_buf[0].wl(); \
\
		if (m_mti_value == NULL) { \
			int v_size = sizeof(int) + (sizeof(unsigned char) * W) * m_size; \
			m_mti_value = new unsigned char[v_size]; \
			memset(m_mti_value, 0, v_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, wl); \
			valp += wl; \
		} \
\
		return (void *) bufp; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_fifo<TYPE<W,I,Q,O,N> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		std::string nm( if_typename_ ); \
		if( nm == typeid( sc_fifo_in_if<TYPE<W,I,Q,O,N> > ).name() ) { \
		if( m_reader != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 ); \
		} \
		m_reader = &port_; \
		} else { \
		if( m_writer != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 ); \
		} \
		m_writer = &port_; \
		} \
	}

#define MTI_DECLARE_AC_INT_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, bool SIGN> \
	class sc_fifo<TYPE<W,SIGN> > \
	: public sc_prim_channel, \
	  public sc_fifo_in_if<TYPE<W,SIGN> >, \
	  public sc_fifo_out_if<TYPE<W,SIGN> > \
	{ \
	public: \
		explicit sc_fifo( int size_ = 16 ) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
		{ \
			init( size_ ); \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_fifo( const char* name_, int size_ = 16 ) \
		: sc_prim_channel( name_ ) \
		{ \
			init( size_ );  \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
		} \
\
		virtual ~sc_fifo() \
		{ \
			delete [] m_buf; \
			if (m_mti_value != NULL) \
				delete [] (unsigned char*)m_mti_value; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
		virtual void read( TYPE<W,SIGN>& ); \
		virtual TYPE<W,SIGN> read(); \
\
		virtual bool nb_read( TYPE<W,SIGN>& ); \
\
		virtual int num_available() const \
		{ return ( m_num_readable - m_num_read ); } \
\
		virtual const sc_event& data_written_event() const \
		{ return m_data_written_event; } \
\
		virtual void write( const TYPE<W,SIGN>& ); \
\
		virtual bool nb_write( const TYPE<W,SIGN>& ); \
\
		virtual int num_free() const \
		{ return ( m_size - m_num_readable - m_num_written ); } \
\
		virtual const sc_event& data_read_event() const \
		{ return m_data_read_event; } \
\
		operator TYPE<W,SIGN> () \
		{ return read(); } \
\
		sc_fifo<TYPE<W,SIGN> >& operator = ( const TYPE<W,SIGN>& a ) \
			{ write( a ); return *this; } \
\
		void trace( sc_trace_file* tf ) const; \
\
		virtual void print( ::std::ostream&  = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_fifo"; } \
\
	protected: \
\
		virtual void update(); \
\
		void init( int ); \
\
		void buf_init( int ); \
		bool buf_write( const TYPE<W,SIGN>& ); \
		bool buf_read( TYPE<W,SIGN>& ); \
\
	protected: \
\
		int m_size; \
		TYPE<W,SIGN>*  m_buf; \
		int m_free; \
		int m_ri; \
		int m_wi; \
\
		sc_port_base* m_reader; \
		sc_port_base* m_writer; \
\
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
	private: \
\
		sc_fifo( const sc_fifo<TYPE<W,SIGN> >& ); \
		sc_fifo& operator = ( const sc_fifo<TYPE<W,SIGN> >& ); \
\
	protected: \
		unsigned char* m_mti_value; \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		mtiObjectHandleT mti_get_channel_id( sc_object* bound_object ); \
	}; \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::read( TYPE<W,SIGN>& val_ ) \
	{ \
		while( num_available() == 0 ) { \
		sc_core::wait( m_data_written_event ); \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
	} \
\
	template <int W, bool SIGN> \
	inline \
	TYPE<W,SIGN> \
	sc_fifo<TYPE<W,SIGN> >::read() \
	{ \
		TYPE<W,SIGN> tmp; \
		read( tmp ); \
		return tmp; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	bool \
	sc_fifo<TYPE<W,SIGN> >::nb_read( TYPE<W,SIGN>& val_ ) \
	{ \
		if( num_available() == 0 ) { \
		return false; \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
		return true; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::write( const TYPE<W,SIGN>& val_ ) \
	{ \
		while( num_free() == 0 ) { \
		sc_core::wait( m_data_read_event ); \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
	} \
\
	template <int W, bool SIGN> \
	inline \
	bool \
	sc_fifo<TYPE<W,SIGN> >::nb_write( const TYPE<W,SIGN>& val_ ) \
	{ \
		if( num_free() == 0 ) { \
		return false; \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
		return true;  \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::trace( sc_trace_file* tf ) const \
	{} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::print( ::std::ostream& os ) const \
	{ \
		if( m_free != m_size ) { \
			int i = m_ri; \
			do { \
				os << m_buf[i] << ::std::endl; \
				i = ( i + 1 ) % m_size; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::dump( ::std::ostream& os ) const \
	{ \
		os << "name = " << name() << ::std::endl; \
		if( m_free != m_size ) { \
			int i = m_ri; \
			int j = 0; \
			do { \
			os << "value[" << i << "] = " << m_buf[i] << ::std::endl; \
			i = ( i + 1 ) % m_size; \
			j ++; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::update() \
	{ \
		if( m_num_read > 0 ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
\
		m_num_readable = m_size - m_free; \
		m_num_read = 0; \
		m_num_written = 0; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::init( int size_ ) \
	{ \
		buf_init( size_ ); \
\
		m_reader = 0; \
		m_writer = 0; \
\
		m_num_readable = 0; \
		m_num_read = 0; \
		m_num_written = 0; \
\
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::buf_init( int size_ ) \
	{ \
		if( size_ <= 0 ) { \
		SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 ); \
		} \
		m_size = size_; \
		m_buf = new TYPE<W,SIGN>[m_size]; \
		m_free = m_size; \
		m_ri = 0; \
		m_wi = 0; \
		m_mti_value = NULL; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	bool \
	sc_fifo<TYPE<W,SIGN> >::buf_write( const TYPE<W,SIGN>& val_ ) \
	{ \
		if( m_free == 0 ) { \
		return false; \
		} \
		m_buf[m_wi] = val_; \
		m_wi = ( m_wi + 1 ) % m_size; \
		m_free --; \
		return true; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	bool \
	sc_fifo<TYPE<W,SIGN> >::buf_read( TYPE<W,SIGN>& val_ ) \
	{ \
		if( m_free == m_size ) { \
		return false; \
		} \
		val_ = m_buf[m_ri]; \
		m_ri = ( m_ri + 1 ) % m_size; \
		m_free ++; \
		return true; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	::std::ostream& \
	operator << ( ::std::ostream& os, const sc_fifo<TYPE<W,SIGN> >& a ) \
	{ \
		a.print( os ); \
		return os; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	mtiObjectHandleT \
	sc_fifo<TYPE<W,SIGN> >::mti_get_channel_id( sc_object* bound_object ) \
	{ \
		if (bound_object) { \
			if (strcmp(bound_object->kind(), "sc_export") ==0) { \
				return mti_get_prim_channel(); \
			} else { \
				sc_port_base* port = (sc_port_base*)bound_object; \
				if (port->mti_is_foreign()) { \
					std::string msg = \
						std::string("port '") + \
						port->name() + \
						"' is connected to an sc_fifo object at the SystemC/HDL boundary."; \
					SC_REPORT_ERROR( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_, msg.c_str() ); \
					return NULL; \
				} else { \
					return mti_get_prim_channel(); \
				} \
			} \
		} \
		return NULL; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void* \
	sc_fifo<TYPE<W,SIGN> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (m_mti_value == NULL) { \
			int v_size = sizeof(int) + (sizeof(unsigned char) * W) * m_size; \
			m_mti_value = new unsigned char[v_size]; \
			memset(m_mti_value, 0, v_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, W); \
			valp += W; \
		} \
\
		return (void *) bufp; \
	} \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_fifo<TYPE<W,SIGN> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		std::string nm( if_typename_ ); \
		if( nm == typeid( sc_fifo_in_if<TYPE<W,SIGN> > ).name() ) { \
		if( m_reader != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 ); \
		} \
		m_reader = &port_; \
		} else { \
		if( m_writer != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 ); \
		} \
		m_writer = &port_; \
		} \
	}

// Specialized template class for ac_fixed types.
#define MTI_DECLARE_AC_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	class sc_fifo<TYPE<W,I,S,Q,O> > \
	: public sc_prim_channel, \
	  public sc_fifo_in_if<TYPE<W,I,S,Q,O> >, \
	  public sc_fifo_out_if<TYPE<W,I,S,Q,O> > \
	{ \
	public: \
		explicit sc_fifo( int size_ = 16 ) \
		: sc_prim_channel( sc_gen_unique_name( "fifo" ) ) \
		{ \
			init( size_ ); \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_fifo( const char* name_, int size_ = 16 ) \
		: sc_prim_channel( name_ ) \
		{ \
			init( size_ );  \
			m_data_read_event.mti_set_is_prim_channel_event(true); \
			m_data_written_event.mti_set_is_prim_channel_event(true); \
		} \
\
		virtual ~sc_fifo() \
		{ \
			delete [] m_buf; \
			if (m_mti_value != NULL) \
				delete [] (unsigned char*)m_mti_value; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
		virtual void read( TYPE<W,I,S,Q,O>& ); \
		virtual TYPE<W,I,S,Q,O> read(); \
\
		virtual bool nb_read( TYPE<W,I,S,Q,O>& ); \
\
		virtual int num_available() const \
		{ return ( m_num_readable - m_num_read ); } \
\
		virtual const sc_event& data_written_event() const \
		{ return m_data_written_event; } \
\
		virtual void write( const TYPE<W,I,S,Q,O>& ); \
\
		virtual bool nb_write( const TYPE<W,I,S,Q,O>& ); \
\
		virtual int num_free() const \
		{ return ( m_size - m_num_readable - m_num_written ); } \
\
		virtual const sc_event& data_read_event() const \
		{ return m_data_read_event; } \
\
		operator TYPE<W,I,S,Q,O> () \
		{ return read(); } \
\
		sc_fifo<TYPE<W,I,S,Q,O> >& operator = ( const TYPE<W,I,S,Q,O>& a ) \
			{ write( a ); return *this; } \
\
		void trace( sc_trace_file* tf ) const; \
\
		virtual void print( ::std::ostream& = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_fifo"; } \
\
	protected: \
\
		virtual void update(); \
\
		void init( int ); \
\
		void buf_init( int ); \
		bool buf_write( const TYPE<W,I,S,Q,O>& ); \
		bool buf_read( TYPE<W,I,S,Q,O>& ); \
\
	protected: \
\
		int m_size; \
		TYPE<W,I,S,Q,O>*  m_buf; \
		int m_free; \
		int m_ri; \
		int m_wi; \
\
		sc_port_base* m_reader; \
		sc_port_base* m_writer; \
\
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
	private: \
\
		sc_fifo( const sc_fifo<TYPE<W,I,S,Q,O> >& ); \
		sc_fifo& operator = ( const sc_fifo<TYPE<W,I,S,Q,O> >& ); \
\
	protected: \
		unsigned char* m_mti_value; \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		mtiObjectHandleT mti_get_channel_id( sc_object* bound_object ); \
	}; \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::read( TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		while( num_available() == 0 ) { \
		sc_core::wait( m_data_written_event ); \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	TYPE<W,I,S,Q,O> \
	sc_fifo<TYPE<W,I,S,Q,O> >::read() \
	{ \
		TYPE<W,I,S,Q,O> tmp; \
		read( tmp ); \
		return tmp; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,S,Q,O> >::nb_read( TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		if( num_available() == 0 ) { \
		return false; \
		} \
		m_num_read ++; \
		buf_read( val_ ); \
		request_update(); \
		return true; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::write( const TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		while( num_free() == 0 ) { \
		sc_core::wait( m_data_read_event ); \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,S,Q,O> >::nb_write( const TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		if( num_free() == 0 ) { \
		return false; \
		} \
		m_num_written ++; \
		buf_write( val_ ); \
		request_update(); \
		return true;  \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::trace( sc_trace_file* tf ) const \
	{} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::print( ::std::ostream& os ) const \
	{ \
		if( m_free != m_size ) { \
			int i = m_ri; \
			do { \
				os << m_buf[i] << ::std::endl; \
				i = ( i + 1 ) % m_size; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::dump( ::std::ostream& os ) const \
	{ \
		os << "name = " << name() << ::std::endl; \
		if( m_free != m_size ) { \
			int i = m_ri; \
			int j = 0; \
			do { \
			os << "value[" << i << "] = " << m_buf[i] << ::std::endl; \
			i = ( i + 1 ) % m_size; \
			j ++; \
			} while( i != m_wi ); \
		} \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::update() \
	{ \
		if( m_num_read > 0 ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
\
		m_num_readable = m_size - m_free; \
		m_num_read = 0; \
		m_num_written = 0; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::init( int size_ ) \
	{ \
		buf_init( size_ ); \
\
		m_reader = 0; \
		m_writer = 0; \
\
		m_num_readable = 0; \
		m_num_read = 0; \
		m_num_written = 0; \
\
		m_data_read_event.mti_set_is_prim_channel_event(true); \
		m_data_written_event.mti_set_is_prim_channel_event(true); \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::buf_init( int size_ ) \
	{ \
		if( size_ <= 0 ) { \
		SC_REPORT_ERROR( SC_ID_INVALID_FIFO_SIZE_, 0 ); \
		} \
		m_size = size_; \
		m_buf = new TYPE<W,I,S,Q,O>[m_size]; \
		m_free = m_size; \
		m_ri = 0; \
		m_wi = 0; \
		m_mti_value = NULL; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,S,Q,O> >::buf_write( const TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		if( m_free == 0 ) { \
		return false; \
		} \
		m_buf[m_wi] = val_; \
		m_wi = ( m_wi + 1 ) % m_size; \
		m_free --; \
		return true; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	bool \
	sc_fifo<TYPE<W,I,S,Q,O> >::buf_read( TYPE<W,I,S,Q,O>& val_ ) \
	{ \
		if( m_free == m_size ) { \
		return false; \
		} \
		val_ = m_buf[m_ri]; \
		m_ri = ( m_ri + 1 ) % m_size; \
		m_free ++; \
		return true; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	::std::ostream& \
	operator << ( ::std::ostream& os, const sc_fifo<TYPE<W,I,S,Q,O> >& a ) \
	{ \
		a.print( os ); \
		return os; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_buf[0].mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_size; \
		*hasConstructionParams = 1; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	mtiObjectHandleT \
	sc_fifo<TYPE<W,I,S,Q,O> >::mti_get_channel_id( sc_object* bound_object ) \
	{ \
		if (bound_object != NULL) { \
			if (strcmp(bound_object->kind(), "sc_export") ==0) { \
				return mti_get_prim_channel(); \
			} else { \
				sc_port_base* port = (sc_port_base*)bound_object; \
				if (port->mti_is_foreign()) { \
					std::string msg = \
						std::string("port '") + \
						port->name() + \
						"' is connected to an sc_fifo object at the SystemC/HDL boundary."; \
					SC_REPORT_ERROR( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_, msg.c_str() ); \
					return NULL; \
				} else { \
					return mti_get_prim_channel(); \
				} \
			} \
		} \
		return NULL; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void* \
	sc_fifo<TYPE<W,I,S,Q,O> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (m_mti_value == NULL) { \
			int v_size = sizeof(int) + (sizeof(unsigned char) * W) * m_size; \
			m_mti_value = new unsigned char[v_size]; \
			memset(m_mti_value, 0, v_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = m_num_readable - m_num_written; \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)m_buf[(m_ri + i) % m_size].mti_get_value(); \
			memcpy(valp, temp, W); \
			valp += W; \
		} \
\
		return (void *) bufp; \
	} \
\
	template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_fifo<TYPE<W,I,S,Q,O> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		std::string nm( if_typename_ ); \
		if( nm == typeid( sc_fifo_in_if<TYPE<W,I,S,Q,O> > ).name() ) { \
		if( m_reader != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_READER_, 0 ); \
		} \
		m_reader = &port_; \
		} else { \
		if( m_writer != 0 ) { \
			SC_REPORT_ERROR( SC_ID_MORE_THAN_ONE_FIFO_WRITER_, 0 ); \
		} \
		m_writer = &port_; \
		} \
	}

template <class T>
inline 
mtiObjectHandleT 
sc_fifo<T>::mti_get_channel_id( sc_object* bound_object )
{
	if (bound_object != NULL) {
		if (strcmp(bound_object->kind(), "sc_export") ==0) {
			return mti_get_prim_channel(); 
		} else {
			sc_port_base* port = (sc_port_base*)bound_object;
			if (port->mti_is_foreign()) {
				std::string msg =
					std::string("port '") +
					port->name() +
					"' is connected to an sc_fifo object at the SystemC/HDL boundary.";
				SC_REPORT_ERROR( SC_ID_UNSUPPORTED_BOUNDARY_PRIM_CHANNEL_, msg.c_str() );
				return NULL;
			} else {
				return mti_get_prim_channel();
			}
		}
	}
	return NULL;
}

template <class T>
inline 
void* 
sc_fifo<T>::mti_get_channel_value(int is_driving_value)
{
	/*
	 * On first access, create the value buffer that will be passed
	 * to the vsim kernel.  This buffer must be large enough to hold
	 * all FIFO elements plus one integer value.  The integer preceeds
	 * the FIFO data and care must be taken to align correctly both
	 * the integer and the FIFO data as required by the platform.
	 * m_mti_value is the entire allocated memory and must be released
	 * with a call to delete[] in the destructor.  m_mti_fifo_data is
	 * a pointer into this buffer to the first element of FIFO data.
	 * It is precomputed here because computing that location depends
	 * on how the original buffer is created.  It is important to create
	 * the MTI value buffer of any user defined type using new to guarantee
	 * correct initialization for the consecutive calls to the assignment
	 * operator.
	 */
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

	// bufp is the location of the first element of FIFO data in the
	// value buffer.  
	T* bufp = (T*) m_mti_fifo_data;

	//
	// Store the number of fifo elements in the begining of m_mti_value 
	// so later in vsim side can be retrieved when vsim needs it.  In
	// creating the value buffer (above), we insured that the first 
	// element of FIFO data is correctly aligned for the type and that
	// subracting four bytes from that address yields a correctly aligned
	// location to store an integer.
	//
	int* sptr = (int *)bufp;
	sptr -= 1;
	*sptr = m_num_readable - m_num_written;

	//
	// Normalize the buffer here. With a circular fifo m_buf, we want to
	// have the first readable element be the first of the retuend value
	// buffer.
	//
	for ( int i = 0; i < *sptr; i++) {
		bufp[i] = m_buf[(m_ri + i) % m_size];
	}

    return (void *)bufp;
}

template <class T>
inline 
void 
sc_fifo<T>::mti_get_construction_params(
	int* elem_num_bits,
	int* elem_num_integer_bits,
	int* fifo_num_elem,
	int* hasConstructionParams,
	int* hasElementConstructionParams)
{
	//
	// We are counting on the TD to determine if it's a fifo, so no
	// need to have a parameter to indicate it.  Only need a parameter
	// to pass the number of used fifo elements.
	//
	*elem_num_bits = 0;
	*elem_num_integer_bits = 0;
	*fifo_num_elem = m_size;
	*hasConstructionParams = 1;
	*hasElementConstructionParams = 0;
}

MTI_DECLARE_SCTYPE_FIFO_FUNCTIONS(sc_dt::sc_unsigned)
MTI_DECLARE_SCTYPE_FIFO_FUNCTIONS(sc_dt::sc_signed)
MTI_DECLARE_SCTYPE_FIFO_BIT_LOGIC_FUNCTIONS(sc_dt::sc_logic)
MTI_DECLARE_SCTYPE_FIFO_BIT_LOGIC_FUNCTIONS(sc_dt::sc_bit)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_bigint)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_biguint)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_bv)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_lv)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_int)
MTI_DECLARE_SC_VECTOR_TYPE_FIFO_FUNCTIONS(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
MTI_DECLARE_SC_FIX_TYPE_FIFO_FUNCTIONS(sc_dt::sc_fix)
MTI_DECLARE_SC_FIX_TYPE_FIFO_FUNCTIONS(sc_dt::sc_fix_fast)
MTI_DECLARE_SC_FIX_TYPE_FIFO_FUNCTIONS(sc_dt::sc_ufix)
MTI_DECLARE_SC_FIX_TYPE_FIFO_FUNCTIONS(sc_dt::sc_ufix_fast)
MTI_DECLARE_SC_FIXED_TYPE_FIFO_FUNCTIONS(sc_dt::sc_fixed)
MTI_DECLARE_SC_FIXED_TYPE_FIFO_FUNCTIONS(sc_dt::sc_fixed_fast)
MTI_DECLARE_SC_FIXED_TYPE_FIFO_FUNCTIONS(sc_dt::sc_ufixed)
MTI_DECLARE_SC_FIXED_TYPE_FIFO_FUNCTIONS(sc_dt::sc_ufixed_fast)
#endif

#if !defined(__HP_aCC) && defined(SC_INCLUDE_MTI_AC)
MTI_DECLARE_AC_INT_TYPE_FIFO_FUNCTIONS(ac_int)
MTI_DECLARE_AC_FIXED_TYPE_FIFO_FUNCTIONS(ac_fixed)
#endif // SC_INCLUDE_MTI_AC

} // namespace sc_core
