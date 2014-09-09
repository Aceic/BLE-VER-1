#include "sc_logic.h"
#include "sc_signed.h"
#include "sc_unsigned.h"
#include "sc_lv.h"
#include "sc_bv.h"
#include "sc_bigint.h"
#include <typeinfo>

#ifdef SC_INCLUDE_FX
#include "sc_fix.h"
#include "sc_fixed.h"
#include "sc_ufix.h"
#include "sc_ufixed.h"
#endif

/* $Id: //dvt/mti/rel/10.0b/src/systemc/tlm_fifo_mti.h#1 $ */

#define MTI_DECLARE_SC_TLM_TYPE_FIFO_FUNCTIONS(TYPE) \
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( int size_) \
      : sc_core::sc_prim_channel( sc_core::sc_gen_unique_name( "fifo" ) ) { \
      init( size_ ); \
      sc_object::mti_set_use_default_name(true); \
    } \
\
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( const char* name_, int size_) \
      : sc_core::sc_prim_channel( name_ ) { \
      init( size_ ); \
    } \
\
	template<> \
	inline void* tlm_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		int numOfBits  = buffer->poke_data(0).length(); \
\
		if (m_mti_value == NULL) { \
			int d_size = sizeof(int) + (sizeof(unsigned char) * numOfBits) * m_mti_initial_fifo_size; \
			m_mti_value = new unsigned char[d_size]; \
			memset(m_mti_value, 0, d_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = used(); \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)buffer->poke_data(i).mti_get_value(); \
			memcpy(valp, temp, numOfBits); \
			valp += numOfBits; \
		} \
		return (void *) bufp; \
	} \
\
	template<> \
	inline void tlm_fifo<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		buffer->poke_data(0).mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_mti_initial_fifo_size; \
		*hasConstructionParams = 1; \
	}

#define MTI_DECLARE_SC_TLM_BIT_LOGIC_FIFO_FUNCTIONS(TYPE) \
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( int size_) \
      : sc_core::sc_prim_channel( sc_core::sc_gen_unique_name( "fifo" ) ) { \
      init( size_ ); \
      sc_object::mti_set_use_default_name(true); \
    } \
\
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( const char* name_, int size_) \
      : sc_core::sc_prim_channel( name_ ) { \
      init( size_ ); \
    } \
\
	template<> \
	inline void* tlm_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		if (m_mti_value == NULL) { \
			int d_size = sizeof(int) + (sizeof(unsigned char) * m_mti_initial_fifo_size); \
			m_mti_value = new unsigned char[d_size]; \
			memset(m_mti_value, 0, d_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = used(); \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
		for ( int i = 0; i < *sptr; i++) { \
			bufp[i] = *(unsigned char *)buffer->poke_data(i).mti_get_value(); \
		} \
\
		return (void *)bufp; \
	}

#define MTI_DECLARE_SC_TLM_FIX_TYPE_FIFO_FUNCTIONS(TYPE) \
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( int size_) \
      : sc_core::sc_prim_channel( sc_core::sc_gen_unique_name( "fifo" ) ) { \
      init( size_ ); \
      sc_object::mti_set_use_default_name(true); \
    } \
\
	template<> \
    inline tlm_fifo<TYPE>::tlm_fifo( const char* name_, int size_) \
      : sc_core::sc_prim_channel( name_ ) { \
      init( size_ ); \
    } \
\
	template <> \
	inline void* tlm_fifo<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		int wl  = buffer->poke_data(0).wl(); \
\
		if (m_mti_value == NULL) { \
			int d_size = sizeof(int) + (sizeof(unsigned char) * wl) * m_mti_initial_fifo_size; \
			m_mti_value = new unsigned char[d_size]; \
			memset(m_mti_value, 0, d_size); \
		} \
\
		int* sptr = (int *)m_mti_value; \
		*sptr = used(); \
\
		unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
		unsigned char* temp; \
		unsigned char* valp; \
\
		valp = bufp; \
		for (int i = 0; i < *sptr ; i++) { \
			temp = (unsigned char *)buffer->poke_data(i).mti_get_value(); \
			memcpy(valp, temp, wl); \
			valp += wl; \
		} \
		return (void *) bufp; \
	} \
\
	template <> \
	inline void tlm_fifo<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		buffer->poke_data(0).mti_get_value_size( \
					hasElementConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = m_mti_initial_fifo_size; \
		*hasConstructionParams = 1; \
	}

#define MTI_DECLARE_SC_TLM_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	class tlm_fifo<TYPE<W,I,Q,O,N> > : \
	  public sc_prim_channel, \
	  public virtual tlm_fifo_get_if<TYPE<W,I,Q,O,N> >, \
	  public virtual tlm_fifo_put_if<TYPE<W,I,Q,O,N> > \
	{ \
	public: \
\
		explicit tlm_fifo( int size_ = 1 ) \
		  : sc_prim_channel( sc_gen_unique_name( "fifo" ) ) { \
\
		  init( size_ ); \
		  sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit tlm_fifo( const char* name_, int size_ = 1 ) \
		  : sc_prim_channel( name_ ) { \
\
		  init( size_ ); \
\
		} \
\
		virtual ~tlm_fifo() { \
\
		  if( buffer != 0 ) delete buffer; \
		  if (m_mti_value != 0) delete[] (unsigned char*)m_mti_value; \
		} \
\
		TYPE<W,I,Q,O,N> get( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ); \
\
		bool nb_get( TYPE<W,I,Q,O,N>& ); \
		bool nb_can_get( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const; \
		const sc_event &ok_to_get( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const { \
		  return m_data_written_event; \
		} \
\
		TYPE<W,I,Q,O,N> peek( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const; \
\
		bool nb_peek( TYPE<W,I,Q,O,N>& ) const; \
		bool nb_can_peek( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const; \
		const sc_event &ok_to_peek( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const { \
		  return m_data_written_event; \
		} \
\
		void put( const TYPE<W,I,Q,O,N>& ); \
\
		bool nb_put( const TYPE<W,I,Q,O,N>& ); \
		bool nb_can_put( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const; \
\
		const sc_event& ok_to_put( tlm_tag<TYPE<W,I,Q,O,N> > *t = 0 ) const { \
		  return m_data_read_event; \
		} \
\
	public: \
		void nb_expand( unsigned int n = 1 ); \
		void nb_unbound( unsigned int n = 16 ); \
\
		bool nb_reduce( unsigned int n = 1 ); \
		bool nb_bound( unsigned int n ); \
\
		bool nb_peek( TYPE<W,I,Q,O,N> & , int n ) const; \
		bool nb_poke( const TYPE<W,I,Q,O,N> & , int n = 0 ); \
\
		int used() const { \
		  return m_num_readable - m_num_read; \
		} \
\
		int size() const { \
		  return m_size; \
		} \
\
		void debug() const { \
\
		  if( is_empty() ) cout << "empty" << endl; \
		  if( is_full() ) cout << "full" << endl; \
\
		  cout << "size " << size() << " - " << used() << " used " << endl; \
		  cout << "readable " << m_num_readable << endl; \
		  cout << "written/read " << m_num_written << "/" << m_num_read << endl; \
		} \
\
		static const char* const kind_string; \
\
		const char* kind() const \
			{ return kind_string; } \
\
	protected: \
\
		void update(); \
\
		void init( int ); \
\
	protected: \
\
		circular_buffer<TYPE<W,I,Q,O,N> > *buffer; \
\
		int m_size; \
\
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
		bool m_expand; \
		bool m_edit; \
		TYPE<W,I,Q,O,N> m_overwrite_data; \
		int m_num_shrink; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
		sc_event m_pop_event; \
		sc_event m_unshrink_event; \
\
	private: \
\
		tlm_fifo( const tlm_fifo<TYPE<W,I,Q,O,N> >& ); \
		tlm_fifo& operator = ( const tlm_fifo<TYPE<W,I,Q,O,N> >& ); \
\
		bool is_empty() const {  \
		  return used() == 0; \
		} \
\
		bool is_full() const { \
		  return size() == m_num_readable + m_num_written; \
		} \
\
	public: \
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiObjectHandleT mti_get_channel_id( sc_port_base* port_ ); \
\
	private: \
		void* m_mti_value; \
		void* m_mti_fifo_data; \
		int   m_mti_initial_fifo_size; \
		int   m_mti_is_resize_warning_not_given; \
		bool  m_mti_is_uchar_val; \
\
	}; \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	const char* const tlm_fifo<TYPE<W,I,Q,O,N> >::kind_string = "tlm_fifo"; \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline  \
	void \
	tlm_fifo<TYPE<W,I,Q,O,N> >::init( int size_ ) { \
\
	  if( size_ > 0 ) { \
		buffer = new circular_buffer<TYPE<W,I,Q,O,N> >( size_); \
	  } \
\
	  else if( size_ < 0 ) { \
		buffer = new circular_buffer<TYPE<W,I,Q,O,N> >( -size_ ); \
	  } \
\
	  else { \
		buffer = new circular_buffer<TYPE<W,I,Q,O,N> >( 16 ); \
	  } \
\
	  m_size = size_; \
	  m_num_readable = 0; \
	  m_num_read = 0; \
	  m_num_written = 0; \
	  m_expand = false; \
	  m_edit = false; \
\
	  m_mti_value = 0; \
	  m_mti_initial_fifo_size = m_size; \
	  m_mti_is_resize_warning_not_given = 1; \
	  m_mti_is_uchar_val = true; \
\
	  m_data_read_event.mti_set_is_prim_channel_event(true); \
	  m_data_written_event.mti_set_is_prim_channel_event(true); \
\
	} \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	void \
	tlm_fifo<TYPE<W,I,Q,O,N> >::update() \
	{ \
		if( m_num_read > 0 || m_expand ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
	  \
		if( m_edit ) { \
\
		  m_edit = false; \
		  m_data_written_event.notify_delayed(); \
\
		  if( used() >= 1 ) { \
		buffer->poke_data( 0 ) = m_overwrite_data; \
		  } \
\
		  else { \
\
		m_num_written ++; \
		buffer->write( m_overwrite_data ); \
\
		  } \
\
		} \
\
		m_expand = false; \
		m_num_read = 0; \
		m_num_written = 0; \
		m_num_readable = buffer->used(); \
\
	} \
\
    template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
    inline \
    void \
    tlm_fifo<TYPE<W,I,Q,O,N> >::mti_get_construction_params( \
        int* elem_num_bits, \
        int* elem_num_integer_bits, \
        int* fifo_num_elem, \
        int* hasConstructionParams, \
        int* hasElementConstructionParams) \
    { \
        buffer->poke_data(0).mti_get_value_size( \
                    hasElementConstructionParams, \
                    elem_num_bits, \
                    elem_num_integer_bits); \
        *fifo_num_elem = m_mti_initial_fifo_size; \
        *hasConstructionParams = 1; \
    } \
\
    template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
    inline \
    mtiObjectHandleT \
    tlm_fifo<TYPE<W,I,Q,O,N> >::mti_get_channel_id( sc_port_base* port_ ) \
    { \
		return mti_get_prim_channel(); \
    } \
\
    template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
    inline \
    void* \
    tlm_fifo<TYPE<W,I,Q,O,N> >::mti_get_channel_value(int is_driving_value) \
    { \
        int wl = buffer->poke_data(0).wl(); \
\
        if (m_mti_value == NULL) { \
            int v_size = sizeof(int) + (sizeof(unsigned char) * wl) * m_mti_initial_fifo_size; \
            m_mti_value = new unsigned char[v_size]; \
            memset(m_mti_value, 0, v_size); \
        } \
\
        int* sptr = (int *)m_mti_value; \
        *sptr = used(); \
\
        unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
        unsigned char* temp; \
        unsigned char* valp; \
\
        valp = bufp; \
        for (int i = 0; i < *sptr ; i++) { \
            temp = (unsigned char *) buffer->poke_data(i).mti_get_value(); \
            memcpy(valp, temp, wl); \
            valp += wl; \
        } \
\
        return (void *) bufp; \
    }

#define MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W> \
	class tlm_fifo<TYPE<W> > : \
	  public sc_prim_channel, \
	  public virtual tlm_fifo_get_if<TYPE<W> >, \
	  public virtual tlm_fifo_put_if<TYPE<W> > \
	{ \
	public: \
\
		explicit tlm_fifo( int size_ = 1 ) \
		  : sc_prim_channel( sc_gen_unique_name( "fifo" ) ) { \
		  \
		  init( size_ ); \
		  sc_object::mti_set_use_default_name(true); \
\
		} \
\
		explicit tlm_fifo( const char* name_, int size_ = 1 ) \
		  : sc_prim_channel( name_ ) { \
\
		  init( size_ ); \
\
		} \
\
		virtual ~tlm_fifo() { \
\
		  if( buffer != 0 ) delete buffer; \
		  if (m_mti_value != 0) delete[] (unsigned char*)m_mti_value; \
		} \
\
		TYPE<W> get( tlm_tag<TYPE<W> > *t = 0 ); \
\
		bool nb_get( TYPE<W>& ); \
		bool nb_can_get( tlm_tag<TYPE<W> > *t = 0 ) const; \
		const sc_event &ok_to_get( tlm_tag<TYPE<W> > *t = 0 ) const { \
		  return m_data_written_event; \
		} \
\
		TYPE<W> peek( tlm_tag<TYPE<W> > *t = 0 ) const; \
\
		bool nb_peek( TYPE<W>& ) const; \
		bool nb_can_peek( tlm_tag<TYPE<W> > *t = 0 ) const; \
		const sc_event &ok_to_peek( tlm_tag<TYPE<W> > *t = 0 ) const { \
		  return m_data_written_event; \
		} \
\
		void put( const TYPE<W>& ); \
\
		bool nb_put( const TYPE<W>& ); \
		bool nb_can_put( tlm_tag<TYPE<W> > *t = 0 ) const; \
\
		const sc_event& ok_to_put( tlm_tag<TYPE<W> > *t = 0 ) const { \
		  return m_data_read_event; \
		} \
\
	public: \
\
		void nb_expand( unsigned int n = 1 ); \
		void nb_unbound( unsigned int n = 16 ); \
\
		bool nb_reduce( unsigned int n = 1 ); \
		bool nb_bound( unsigned int n ); \
\
		bool nb_peek( TYPE<W> & , int n ) const; \
		bool nb_poke( const TYPE<W> & , int n = 0 ); \
\
		int used() const { \
		  return m_num_readable - m_num_read; \
		} \
\
		int size() const { \
		  return m_size; \
		} \
\
		void debug() const { \
\
		  if( is_empty() ) cout << "empty" << endl; \
		  if( is_full() ) cout << "full" << endl; \
\
		  cout << "size " << size() << " - " << used() << " used " << endl; \
		  cout << "readable " << m_num_readable << endl; \
		  cout << "written/read " << m_num_written << "/" << m_num_read << endl; \
\
		} \
\
		static const char* const kind_string; \
\
		const char* kind() const \
			{ return kind_string; } \
\
	protected: \
\
		void update(); \
\
		void init( int ); \
\
	protected: \
\
		circular_buffer<TYPE<W> > *buffer; \
\
		int m_size; \
		int m_num_readable; \
		int m_num_read; \
		int m_num_written; \
		bool m_expand; \
		bool m_edit; \
		TYPE<W> m_overwrite_data; \
		int m_num_shrink; \
\
		sc_event m_data_read_event; \
		sc_event m_data_written_event; \
\
		sc_event m_pop_event; \
		sc_event m_unshrink_event; \
\
	private: \
\
		tlm_fifo( const tlm_fifo<TYPE<W> >& ); \
		tlm_fifo& operator = ( const tlm_fifo<TYPE<W> >& ); \
\
		bool is_empty() const {  \
		  return used() == 0; \
		} \
\
		bool is_full() const { \
		  return size() == m_num_readable + m_num_written; \
		} \
\
	public: \
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiObjectHandleT mti_get_channel_id( sc_port_base* port_ ); \
\
	private: \
		void* m_mti_value; \
		void* m_mti_fifo_data; \
		int   m_mti_initial_fifo_size; \
		int   m_mti_is_resize_warning_not_given; \
		bool  m_mti_is_uchar_val; \
\
	}; \
\
	template <int W> \
	const char* const tlm_fifo<TYPE<W> >::kind_string = "tlm_fifo"; \
\
	template< int W > \
	inline  \
	void \
	tlm_fifo<TYPE<W> >::init( int size_ ) { \
\
	  if( size_ > 0 ) { \
		buffer = new circular_buffer<TYPE<W> >( size_); \
	  } \
	   \
	  else if( size_ < 0 ) { \
		buffer = new circular_buffer<TYPE<W> >( -size_ ); \
	  } \
\
	  else { \
		buffer = new circular_buffer<TYPE<W> >( 16 ); \
	  } \
\
	  m_size = size_; \
	  m_num_readable = 0; \
	  m_num_read = 0; \
	  m_num_written = 0; \
	  m_expand = false; \
	  m_edit = false; \
\
	  m_mti_value = 0; \
	  m_mti_initial_fifo_size = m_size; \
	  m_mti_is_resize_warning_not_given = 1; \
\
	  m_data_read_event.mti_set_is_prim_channel_event(true); \
	  m_data_written_event.mti_set_is_prim_channel_event(true); \
\
	} \
\
	template < int W> \
	inline \
	void \
	tlm_fifo<TYPE<W> >::update() \
	{ \
		if( m_num_read > 0 || m_expand ) { \
		m_data_read_event.notify_delayed(); \
		} \
\
		if( m_num_written > 0 ) { \
		m_data_written_event.notify_delayed(); \
		} \
	  \
		if( m_edit ) { \
\
		  m_edit = false; \
		  m_data_written_event.notify_delayed(); \
\
		  if( used() >= 1 ) { \
		buffer->poke_data( 0 ) = m_overwrite_data; \
		  } \
\
		  else { \
\
		m_num_written ++; \
		buffer->write( m_overwrite_data ); \
		  } \
		} \
\
		m_expand = false; \
		m_num_read = 0; \
		m_num_written = 0; \
		m_num_readable = buffer->used(); \
	} \
\
	template <int W> \
    inline \
    void \
    tlm_fifo<TYPE<W> >::mti_get_construction_params( \
        int* elem_num_bits, \
        int* elem_num_integer_bits, \
        int* fifo_num_elem, \
        int* hasConstructionParams, \
        int* hasElementConstructionParams) \
    { \
        buffer->poke_data(0).mti_get_value_size( \
                    hasElementConstructionParams, \
                    elem_num_bits, \
                    elem_num_integer_bits); \
        *fifo_num_elem = m_mti_initial_fifo_size; \
        *hasConstructionParams = 1; \
    } \
\
	template <int W> \
    inline \
    mtiObjectHandleT \
    tlm_fifo<TYPE<W> >::mti_get_channel_id( sc_port_base* port_ ) \
    { \
		return mti_get_prim_channel(); \
    } \
\
	template <int W> \
    inline \
    void* \
    tlm_fifo<TYPE<W> >::mti_get_channel_value(int is_driving_value) \
    { \
\
        if (m_mti_value == NULL) { \
            int v_size = sizeof(int) + (sizeof(unsigned char) * W) * m_mti_initial_fifo_size; \
            m_mti_value = new unsigned char[v_size]; \
            memset(m_mti_value, 0, v_size); \
        } \
\
        int* sptr = (int *)m_mti_value; \
        *sptr = used(); \
\
        unsigned char* bufp = (unsigned char*)((int *)m_mti_value + 1); \
\
        unsigned char* temp; \
        unsigned char* valp; \
\
        valp = bufp; \
        for (int i = 0; i < *sptr ; i++) { \
            temp = (unsigned char *) buffer->poke_data(i).mti_get_value(); \
            memcpy(valp, temp, W); \
            valp += W; \
        } \
\
        return (void *) bufp; \
    }

template <class T>
inline
mtiObjectHandleT
tlm_fifo<T>::mti_get_channel_id( sc_port_base* port_ )
{
	return mti_get_prim_channel();
}

template <class T>
inline
void*
tlm_fifo<T>::mti_get_channel_value(int is_driving_value)
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
     * correct initialization for the subsequent calls to the assignment
     * operator.
     */
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
    *sptr = used();

    //
    // Normalize the buffer here. With a circular fifo m_buf, we want to
    // have the first readable element be the first of the retuend value
    // buffer.
    //
    for ( int i = 0; i < *sptr; i++) {
        bufp[i] = buffer->poke_data(i);
    }

    return (void *)bufp;
}

template <class T>
inline
void
tlm_fifo<T>::mti_get_construction_params(
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
    *fifo_num_elem = m_mti_initial_fifo_size;
    *hasConstructionParams = 1;
    *hasElementConstructionParams = 0;
}

MTI_DECLARE_SC_TLM_TYPE_FIFO_FUNCTIONS(sc_signed)
MTI_DECLARE_SC_TLM_TYPE_FIFO_FUNCTIONS(sc_unsigned)
MTI_DECLARE_SC_TLM_BIT_LOGIC_FIFO_FUNCTIONS(sc_bit)
MTI_DECLARE_SC_TLM_BIT_LOGIC_FIFO_FUNCTIONS(sc_logic)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bigint)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_biguint)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bv)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_lv)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_int)
MTI_DECLARE_SC_TLM_VECTOR_TYPE_FIFO_FUNCTIONS(sc_uint)

#ifdef SC_INCLUDE_FX
	MTI_DECLARE_SC_TLM_FIX_TYPE_FIFO_FUNCTIONS(sc_fix)
	MTI_DECLARE_SC_TLM_FIX_TYPE_FIFO_FUNCTIONS(sc_fix_fast)
	MTI_DECLARE_SC_TLM_FIX_TYPE_FIFO_FUNCTIONS(sc_ufix)
	MTI_DECLARE_SC_TLM_FIX_TYPE_FIFO_FUNCTIONS(sc_ufix_fast)
	MTI_DECLARE_SC_TLM_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed)
	MTI_DECLARE_SC_TLM_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed_fast)
	MTI_DECLARE_SC_TLM_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed)
	MTI_DECLARE_SC_TLM_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed_fast)
#endif
