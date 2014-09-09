#include "sc_bv.h"
#include "sc_lv.h"
#include "sc_uint.h"
#include "sc_int.h"
#include "sc_biguint.h"
#include "sc_bigint.h"
#ifdef SC_INCLUDE_FX
#include "sc_fix.h"
#include "sc_fixed.h"
#include "sc_ufix.h"
#include "sc_ufixed.h"
#endif // #ifdef SC_INCLUDE_FX 

/* $Id: //dvt/mti/rel/10.0b/src/systemc/sc_signal_mti.h#1 $ */

namespace sc_core {

#define MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(TYPE, MTI_TYPE_ID) \
	template<> \
	inline mtiObjectHandleT sc_signal<TYPE>::mti_get_channel_id(  \
		sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template<> \
	inline void* sc_signal<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			m_mti_value = m_new_val; \
		else \
			m_mti_value = m_cur_val; \
		return &m_mti_value; \
	} \
\
	template<> \
	inline void sc_signal<TYPE>::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val = *(TYPE*)value; \
	} \
\
	template<> \
	inline void sc_signal<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		*elem_num_bits = 0; \
		*elem_num_integer_bits = 0; \
		*fifo_num_elem = 0; \
		*hasConstructionParams = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<> \
	inline mtiTypeIdT sc_signal<TYPE>::mti_get_channel_type() \
	{ \
		return mti_CreateNativeCType(MTI_TYPE_ID); \
	}


#define MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(TYPE) \
	template<> \
	inline mtiObjectHandleT sc_signal<TYPE>::mti_get_channel_id(  \
			sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template<> \
	inline void* sc_signal<TYPE>::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			return m_new_val.mti_get_value(); \
		else \
			return m_cur_val.mti_get_value(); \
	} \
\
	template<> \
	inline void sc_signal<TYPE>::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val.mti_set_value(value); \
	} \
\
	template<> \
	inline void sc_signal<TYPE>::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		m_cur_val.mti_get_value_size( \
					hasConstructionParams, \
					elem_num_bits, \
					elem_num_integer_bits); \
		*fifo_num_elem = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<> \
	inline mtiTypeIdT sc_signal<TYPE>::mti_get_channel_type() \
	{ \
		return m_cur_val.mti_get_value_type(); \
	}


#define MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(TYPE) \
	template <int W> \
	class sc_signal<TYPE<W> > \
	: public sc_prim_channel, \
	  public sc_signal_inout_if<TYPE<W> > \
	{ \
	public: \
\
		sc_signal() \
		: sc_prim_channel( sc_gen_unique_name( "signal" ) ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{ \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_signal( const char* name_ ) \
		: sc_prim_channel( name_ ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{} \
\
		virtual ~sc_signal() \
		{ \
			if ( !m_change_event_p ) delete m_change_event_p; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
\
		virtual const sc_event& default_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const sc_event& value_changed_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const TYPE<W>& read() const \
		{ return m_cur_val; } \
\
		virtual const TYPE<W>& get_data_ref() const \
			{ sc_deprecated_get_data_ref(); return m_cur_val; } \
\
		virtual bool event() const \
			{ return simcontext()->event_occurred(m_delta + 1); } \
\
		virtual void write( const TYPE<W>& ); \
\
		operator const TYPE<W>& () const \
		{ return read(); } \
\
		sc_signal<TYPE<W> >& operator = ( const TYPE<W>& a ) \
		{ write( a ); return *this; } \
\
		sc_signal<TYPE<W> >& operator = ( const sc_signal<TYPE<W> >& a ) \
		{ write( a.read() ); return *this; } \
\
		const TYPE<W>& get_new_value() const \
		{ return m_new_val; } \
\
		void trace( sc_trace_file* tf ) const \
		{ sc_deprecated_trace(); } \
\
		virtual void print( ::std::ostream& = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_signal"; } \
\
	protected: \
\
		virtual void update(); \
\
	protected: \
\
		mutable sc_event* m_change_event_p; \
		TYPE<W>           m_cur_val; \
		sc_dt::uint64     m_delta; \
		TYPE<W>           m_new_val; \
		sc_port_base*     m_output; \
		sc_object*        m_writer; \
\
	private: \
\
		sc_signal( const sc_signal<TYPE<W> >& ); \
\
	protected: \
\
		mtiObjectHandleT mti_get_channel_id(sc_object* ); \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
		virtual void mti_set_channel_value( \
			void* value, \
			int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiTypeIdT mti_get_channel_type(); \
\
	public: \
\
		bool control_foreign_signal(const char* hdl_name); \
\
		bool observe_foreign_signal(const char* hdl_name); \
	}; \
\
	template <int W> \
	inline \
	void \
	sc_signal<TYPE<W> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			std::string nm( if_typename_ ); \
			if( nm == typeid( sc_signal_inout_if<TYPE<W> > ).name() ) { \
				if( m_output != 0) \
					sc_signal_invalid_writer( this, m_output, &port_ ); \
				m_output = &port_; \
			} \
		} \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::write( const TYPE<W>& value_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			sc_object* writer = sc_get_curr_simcontext()->get_current_writer(); \
			if( m_writer == 0 ) { \
				m_writer = writer; \
			} else if( m_writer != writer ) { \
				sc_signal_invalid_writer( this, m_writer, writer ); \
			} \
		} \
 \
		m_new_val = value_; \
		if( !( m_new_val == m_cur_val ) ) { \
			request_update(); \
		} \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::print( ::std::ostream& os ) const \
	{ \
		os << m_cur_val; \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::dump( ::std::ostream& os ) const \
	{ \
		os << "     name = " << name() << ::std::endl; \
		os << "    value = " << m_cur_val << ::std::endl; \
		os << "new value = " << m_new_val << ::std::endl; \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::update() \
	{ \
		if( !( m_new_val == m_cur_val ) ) { \
		m_cur_val = m_new_val; \
		if ( m_change_event_p ) m_change_event_p->notify_next_delta(); \
		m_delta = sc_delta_count(); \
		} \
	} \
\
	template <int W> \
	inline mtiObjectHandleT sc_signal<TYPE<W> >::mti_get_channel_id(  \
			sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		*elem_num_bits = 0; \
		*elem_num_integer_bits = 0; \
		*fifo_num_elem = 0; \
		*hasConstructionParams = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<int W> \
	inline mtiTypeIdT sc_signal<TYPE<W> >::mti_get_channel_type() \
	{ \
		return m_cur_val.mti_get_value_type(); \
	} \
\
	template <int W> \
	inline void* sc_signal<TYPE<W> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			return m_new_val.mti_get_value(); \
		else \
			return m_cur_val.mti_get_value(); \
	} \
\
	template <int W> \
	inline void sc_signal<TYPE<W> >::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val.mti_set_value(value); \
	} \
\
	template <int W> \
	inline bool sc_signal<TYPE<W> >::control_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name); \
	} \
\
	template <int W> \
	inline bool sc_signal<TYPE<W> >::observe_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name); \
	}


#define MTI_DECLARE_SC_FIXED_TYPE_SIGNAL_FUNCTIONS(TYPE) \
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	class sc_signal<TYPE<W,I,Q,O,N> > \
	: public sc_prim_channel, \
	  public sc_signal_inout_if<TYPE<W,I,Q,O,N> > \
	{ \
	public: \
\
		sc_signal() \
		: sc_prim_channel( sc_gen_unique_name( "signal" ) ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,I,Q,O,N>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,I,Q,O,N>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{ \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_signal( const char* name_ ) \
		: sc_prim_channel( name_ ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,I,Q,O,N>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,I,Q,O,N>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{} \
\
		virtual ~sc_signal() \
		{ \
			if ( !m_change_event_p ) delete m_change_event_p; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
\
		virtual const sc_event& default_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const sc_event& value_changed_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const TYPE<W,I,Q,O,N>& read() const \
		{ return m_cur_val; } \
\
		virtual const TYPE<W,I,Q,O,N>& get_data_ref() const \
			{ sc_deprecated_get_data_ref(); return m_cur_val; } \
\
		virtual bool event() const \
			{ return simcontext()->event_occurred(m_delta + 1); } \
\
		virtual void write( const TYPE<W,I,Q,O,N>& ); \
\
		operator const TYPE<W,I,Q,O,N>& () const \
		{ return read(); } \
\
		sc_signal<TYPE<W,I,Q,O,N> >& operator = ( const TYPE<W,I,Q,O,N>& a ) \
		{ write( a ); return *this; } \
\
		sc_signal<TYPE<W,I,Q,O,N> >& operator = ( const sc_signal<TYPE<W,I,Q,O,N> >& a ) \
		{ write( a.read() ); return *this; } \
\
		const TYPE<W,I,Q,O,N>& get_new_value() const \
		{ return m_new_val; } \
\
		void trace( sc_trace_file* tf ) const \
		{ sc_deprecated_trace(); } \
\
		virtual void print( ::std::ostream&  = ::std::cout ) const; \
		virtual void dump( ::std::ostream&  = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_signal"; } \
\
	protected: \
\
		virtual void update(); \
\
	protected: \
\
		mutable sc_event* m_change_event_p; \
		TYPE<W,I,Q,O,N>   m_cur_val; \
		sc_dt::uint64     m_delta; \
		TYPE<W,I,Q,O,N>   m_new_val; \
		sc_port_base*     m_output; \
		sc_object*        m_writer; \
\
	private: \
\
		sc_signal( const sc_signal<TYPE<W,I,Q,O,N> >& ); \
\
	protected: \
\
		mtiObjectHandleT mti_get_channel_id(sc_object* ); \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_set_channel_value( \
			void* value, \
			int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiTypeIdT mti_get_channel_type(); \
\
	public: \
\
		bool control_foreign_signal(const char* hdl_name); \
\
		bool observe_foreign_signal(const char* hdl_name); \
	}; \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline \
	void \
	sc_signal<TYPE<W,I,Q,O,N> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			std::string nm( if_typename_ ); \
			if( nm == typeid( sc_signal_inout_if<TYPE<W,I,Q,O,N> > ).name() ) { \
				if( m_output != 0) \
					sc_signal_invalid_writer( this, m_output, &port_ ); \
				m_output = &port_; \
			} \
		} \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::write( const TYPE<W,I,Q,O,N>& value_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			sc_object* writer = sc_get_curr_simcontext()->get_current_writer(); \
			if( m_writer == 0 ) { \
				m_writer = writer; \
			} else if( m_writer != writer ) { \
				sc_signal_invalid_writer( this, m_writer, writer ); \
			} \
		} \
 \
		m_new_val = value_; \
		if( !( m_new_val == m_cur_val ) ) { \
			request_update(); \
		} \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::print( ::std::ostream& os ) const \
	{ \
		os << m_cur_val; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::dump( ::std::ostream& os ) const \
	{ \
		os << "     name = " << name() << ::std::endl; \
		os << "    value = " << m_cur_val << ::std::endl; \
		os << "new value = " << m_new_val << ::std::endl; \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::update() \
	{ \
		if( !( m_new_val == m_cur_val ) ) { \
		m_cur_val = m_new_val; \
		if ( m_change_event_p ) m_change_event_p->notify_next_delta(); \
		m_delta = sc_delta_count(); \
		} \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline mtiObjectHandleT sc_signal<TYPE<W,I,Q,O,N> >::mti_get_channel_id(  \
			sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		*elem_num_bits = 0; \
		*elem_num_integer_bits = 0; \
		*fifo_num_elem = 0; \
		*hasConstructionParams = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline mtiTypeIdT sc_signal<TYPE<W,I,Q,O,N> >::mti_get_channel_type() \
	{ \
		return m_cur_val.mti_get_value_type(); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void* sc_signal<TYPE<W,I,Q,O,N> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			return m_new_val.mti_get_value(); \
		else \
			return m_cur_val.mti_get_value(); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline void sc_signal<TYPE<W,I,Q,O,N> >::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val.mti_set_value(value); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline bool sc_signal<TYPE<W,I,Q,O,N> >::control_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name); \
	} \
\
	template <int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
	inline bool sc_signal<TYPE<W,I,Q,O,N> >::observe_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name); \
	}


#define MTI_DECLARE_AC_INT_TYPE_SIGNAL_FUNCTIONS(TYPE) \
	template <int W, bool SIGN> \
	class sc_signal<TYPE<W,SIGN> > \
	: public sc_prim_channel, \
	  public sc_signal_inout_if<TYPE<W,SIGN> > \
	{ \
	public: \
\
		sc_signal() \
		: sc_prim_channel( sc_gen_unique_name( "signal" ) ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,SIGN>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,SIGN>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{ \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_signal( const char* name_ ) \
		: sc_prim_channel( name_ ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,SIGN>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,SIGN>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{} \
\
		virtual ~sc_signal() \
		{ \
			if ( !m_change_event_p ) delete m_change_event_p; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
\
		virtual const sc_event& default_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const sc_event& value_changed_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const TYPE<W,SIGN>& read() const \
		{ return m_cur_val; } \
\
		virtual const TYPE<W,SIGN>& get_data_ref() const \
			{ sc_deprecated_get_data_ref(); return m_cur_val; } \
\
		virtual bool event() const \
			{ return simcontext()->event_occurred(m_delta + 1); } \
\
		virtual void write( const TYPE<W,SIGN>& ); \
\
		operator const TYPE<W,SIGN>& () const \
		{ return read(); } \
\
		sc_signal<TYPE<W,SIGN> >& operator = ( const TYPE<W,SIGN>& a ) \
		{ write( a ); return *this; } \
\
		sc_signal<TYPE<W,SIGN> >& operator = ( const sc_signal<TYPE<W,SIGN> >& a ) \
		{ write( a.read() ); return *this; } \
\
		const TYPE<W,SIGN>& get_new_value() const \
		{ return m_new_val; } \
\
		void trace( sc_trace_file* tf ) const \
		{ sc_deprecated_trace(); } \
\
		virtual void print( ::std::ostream& = ::std::cout ) const; \
		virtual void dump( ::std::ostream& = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_signal"; } \
\
	protected: \
\
		virtual void update(); \
\
	protected: \
\
		mutable sc_event* m_change_event_p; \
		TYPE<W,SIGN>           m_cur_val; \
		sc_dt::uint64     m_delta; \
		TYPE<W,SIGN>           m_new_val; \
		sc_port_base*     m_output; \
		sc_object*        m_writer; \
\
	private: \
\
		sc_signal( const sc_signal<TYPE<W,SIGN> >& ); \
\
	protected: \
\
		mtiObjectHandleT mti_get_channel_id(sc_object* ); \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
		virtual void mti_set_channel_value( \
			void* value, \
			int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiTypeIdT mti_get_channel_type(); \
\
	public: \
\
		bool control_foreign_signal(const char* hdl_name); \
\
		bool observe_foreign_signal(const char* hdl_name); \
	}; \
\
	template <int W, bool SIGN> \
	inline \
	void \
	sc_signal<TYPE<W,SIGN> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			std::string nm( if_typename_ ); \
			if( nm == typeid( sc_signal_inout_if<TYPE<W,SIGN> > ).name() ) { \
				if( m_output != 0) \
					sc_signal_invalid_writer( this, m_output, &port_ ); \
				m_output = &port_; \
			} \
		} \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::write( const TYPE<W,SIGN>& value_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			sc_object* writer = sc_get_curr_simcontext()->get_current_writer(); \
			if( m_writer == 0 ) { \
				m_writer = writer; \
			} else if( m_writer != writer ) { \
				sc_signal_invalid_writer( this, m_writer, writer ); \
			} \
		} \
 \
		m_new_val = value_; \
		if( !( m_new_val == m_cur_val ) ) { \
			request_update(); \
		} \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::print( ::std::ostream& os ) const \
	{ \
		os << m_cur_val; \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::dump( ::std::ostream& os ) const \
	{ \
		os << "     name = " << name() << ::std::endl; \
		os << "    value = " << m_cur_val << ::std::endl; \
		os << "new value = " << m_new_val << ::std::endl; \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::update() \
	{ \
		if( !( m_new_val == m_cur_val ) ) { \
		m_cur_val = m_new_val; \
		if ( m_change_event_p ) m_change_event_p->notify_next_delta(); \
		m_delta = sc_delta_count(); \
		} \
	} \
\
	template <int W, bool SIGN> \
	inline mtiObjectHandleT sc_signal<TYPE<W,SIGN> >::mti_get_channel_id(  \
			sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		*elem_num_bits = 0; \
		*elem_num_integer_bits = 0; \
		*fifo_num_elem = 0; \
		*hasConstructionParams = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<int W, bool SIGN> \
	inline mtiTypeIdT sc_signal<TYPE<W, SIGN> >::mti_get_channel_type() \
	{ \
		return NULL; \
	} \
\
	template <int W, bool SIGN> \
	inline void* sc_signal<TYPE<W,SIGN> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			return m_new_val.mti_get_value(); \
		else \
			return m_cur_val.mti_get_value(); \
	} \
\
	template <int W, bool SIGN> \
	inline void sc_signal<TYPE<W,SIGN> >::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val.mti_set_value(value); \
	} \
\
	template <int W, bool SIGN> \
	inline bool sc_signal<TYPE<W,SIGN> >::control_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name); \
	} \
\
	template <int W, bool SIGN> \
	inline bool sc_signal<TYPE<W,SIGN> >::observe_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name); \
	}

#define MTI_DECLARE_AC_FIXED_TYPE_SIGNAL_FUNCTIONS(TYPE) \
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	class sc_signal<TYPE<W,I,S,Q,O> > \
	: public sc_prim_channel, \
	  public sc_signal_inout_if<TYPE<W,I,S,Q,O> > \
	{ \
	public: \
\
		sc_signal() \
		: sc_prim_channel( sc_gen_unique_name( "signal" ) ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,I,S,Q,O>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,I,S,Q,O>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{ \
			sc_object::mti_set_use_default_name(true); \
		} \
\
		explicit sc_signal( const char* name_ ) \
		: sc_prim_channel( name_ ), \
			  m_change_event_p( 0 ), m_cur_val( TYPE<W,I,S,Q,O>() ), \
			  m_delta( ~sc_dt::UINT64_ONE ), m_new_val( TYPE<W,I,S,Q,O>() ), m_output( 0 ), \
			  m_writer( 0 ) \
		{} \
\
		virtual ~sc_signal() \
		{ \
			if ( !m_change_event_p ) delete m_change_event_p; \
		} \
\
		virtual void register_port( sc_port_base&, const char* ); \
\
		virtual const sc_event& default_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const sc_event& value_changed_event() const \
		{ \
			if ( !m_change_event_p ) { \
				m_change_event_p = new sc_event;  \
				m_change_event_p->mti_set_is_prim_channel_event(true);\
			} \
			return *m_change_event_p; \
		} \
\
		virtual const TYPE<W,I,S,Q,O>& read() const \
		{ return m_cur_val; } \
\
		virtual const TYPE<W,I,S,Q,O>& get_data_ref() const \
			{ sc_deprecated_get_data_ref(); return m_cur_val; } \
\
		virtual bool event() const \
			{ return simcontext()->event_occurred(m_delta + 1); } \
\
		virtual void write( const TYPE<W,I,S,Q,O>& ); \
\
		operator const TYPE<W,I,S,Q,O>& () const \
		{ return read(); } \
\
		sc_signal<TYPE<W,I,S,Q,O> >& operator = ( const TYPE<W,I,S,Q,O>& a ) \
		{ write( a ); return *this; } \
\
		sc_signal<TYPE<W,I,S,Q,O> >& operator = ( const sc_signal<TYPE<W,I,S,Q,O> >& a ) \
		{ write( a.read() ); return *this; } \
\
		const TYPE<W,I,S,Q,O>& get_new_value() const \
		{ return m_new_val; } \
\
		void trace( sc_trace_file* tf ) const \
		{ sc_deprecated_trace(); } \
\
		virtual void print( ::std::ostream&  = ::std::cout ) const; \
		virtual void dump( ::std::ostream&  = ::std::cout ) const; \
\
		virtual const char* kind() const \
			{ return "sc_signal"; } \
\
	protected: \
\
		virtual void update(); \
\
	protected: \
\
		mutable sc_event* m_change_event_p; \
		TYPE<W,I,S,Q,O>   m_cur_val; \
		sc_dt::uint64     m_delta; \
		TYPE<W,I,S,Q,O>   m_new_val; \
		sc_port_base*     m_output; \
		sc_object*        m_writer; \
\
	private: \
\
		sc_signal( const sc_signal<TYPE<W,I,S,Q,O> >& ); \
\
	protected: \
\
		mtiObjectHandleT mti_get_channel_id(sc_object* ); \
\
		virtual void* mti_get_channel_value(int is_driving_value); \
\
		virtual void mti_set_channel_value( \
			void* value, \
			int is_driving_value); \
\
		virtual void mti_get_construction_params( \
			int* elem_num_bits, \
			int* elem_num_integer_bits, \
			int* fifo_num_elem, \
			int* hasConstructionParams, \
			int* hasElementConstructionParams); \
\
		virtual mtiTypeIdT mti_get_channel_type(); \
\
	public: \
\
		bool control_foreign_signal(const char* hdl_name); \
\
		bool observe_foreign_signal(const char* hdl_name); \
	}; \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline \
	void \
	sc_signal<TYPE<W,I,S,Q,O> >::register_port( sc_port_base& port_, const char* if_typename_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			std::string nm( if_typename_ ); \
			if( nm == typeid( sc_signal_inout_if<TYPE<W,I,S,Q,O> > ).name() ) { \
				if( m_output != 0) \
					sc_signal_invalid_writer( this, m_output, &port_ ); \
				m_output = &port_; \
			} \
		} \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::write( const TYPE<W,I,S,Q,O>& value_ ) \
	{ \
		if ( sc_get_curr_simcontext()->write_check() ) \
		{ \
			sc_object* writer = sc_get_curr_simcontext()->get_current_writer(); \
			if( m_writer == 0 ) { \
				m_writer = writer; \
			} else if( m_writer != writer ) { \
				sc_signal_invalid_writer( this, m_writer, writer ); \
			} \
		} \
 \
		m_new_val = value_; \
		if( !( m_new_val == m_cur_val ) ) { \
			request_update(); \
		} \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::print( ::std::ostream& os ) const \
	{ \
		os << m_cur_val; \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::dump( ::std::ostream& os ) const \
	{ \
		os << "     name = " << name() << ::std::endl; \
		os << "    value = " << m_cur_val << ::std::endl; \
		os << "new value = " << m_new_val << ::std::endl; \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::update() \
	{ \
		if( !( m_new_val == m_cur_val ) ) { \
		m_cur_val = m_new_val; \
		if ( m_change_event_p ) m_change_event_p->notify_next_delta(); \
		m_delta = sc_delta_count(); \
		} \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline mtiObjectHandleT sc_signal<TYPE<W,I,S,Q,O> >::mti_get_channel_id(  \
			sc_object* ) \
	{ \
		return mti_get_prim_channel(); \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::mti_get_construction_params( \
		int* elem_num_bits, \
		int* elem_num_integer_bits, \
		int* fifo_num_elem, \
		int* hasConstructionParams, \
		int* hasElementConstructionParams) \
	{ \
		*elem_num_bits = 0; \
		*elem_num_integer_bits = 0; \
		*fifo_num_elem = 0; \
		*hasConstructionParams = 0; \
		*hasElementConstructionParams = 0; \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline mtiTypeIdT sc_signal<TYPE<W,I,S,Q,O> >::mti_get_channel_type() \
	{ \
		return NULL; \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void* sc_signal<TYPE<W,I,S,Q,O> >::mti_get_channel_value(int is_driving_value) \
	{ \
		if (is_driving_value) \
			return m_new_val.mti_get_value(); \
		else \
			return m_cur_val.mti_get_value(); \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline void sc_signal<TYPE<W,I,S,Q,O> >::mti_set_channel_value( \
		void* value, \
		int is_driving_value) \
	{ \
		m_new_val.mti_set_value(value); \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline bool sc_signal<TYPE<W,I,S,Q,O> >::control_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name); \
	} \
\
	template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> \
	inline bool sc_signal<TYPE<W,I,S,Q,O> >::observe_foreign_signal(const char* hdl_name) \
	{ \
		return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name); \
	}

template <class T>
inline mtiObjectHandleT sc_signal<T>::mti_get_channel_id( sc_object* bound_object)
{
    return mti_get_prim_channel();
}


template <class T>
inline void* sc_signal<T>::mti_get_channel_value(int is_driving_value)
{
    if (is_driving_value)
        m_mti_value = m_new_val;
    else
        m_mti_value = m_cur_val;

    return &m_mti_value;
}


template <class T>
inline void sc_signal<T>::mti_set_channel_value(
    void* value,
    int is_driving_value)
{
	m_new_val = *(T*)value;
}


template <class T>
inline void sc_signal<T>::mti_get_construction_params(
	int* elem_num_bits,
	int* elem_num_integer_bits,
	int* fifo_num_elem,
	int* hasConstructionParams,
	int* hasElementConstructionParams)
{
	*elem_num_bits = 0;
	*elem_num_integer_bits = 0;
	*fifo_num_elem = 0;
	*hasConstructionParams = 0;
	*hasElementConstructionParams = 0;
}


template <class T>
inline mtiTypeIdT sc_signal<T>::mti_get_channel_type()
{
	return NULL;
}


template<class T>
inline bool sc_signal<T>::control_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name);
}


template <class T>
inline bool sc_signal<T>::observe_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name);
}



inline mtiObjectHandleT sc_signal<bool>::mti_get_channel_id(
	sc_object* )
{
	return mti_get_prim_channel();
}


inline void* sc_signal<bool>::mti_get_channel_value(int is_driving_value)
{
	if (is_driving_value)
		m_mti_value = m_new_val;
	else
		m_mti_value = m_cur_val;
	return &m_mti_value;
}


inline void sc_signal<bool>::mti_set_channel_value(
	void* value,
	int is_driving_value)
{
	m_new_val = *(bool*)value;
}


inline void sc_signal<bool>::mti_get_construction_params(
	int* elem_num_bits,
	int* elem_num_integer_bits,
	int* fifo_num_elem,
	int* hasConstructionParams,
	int* hasElementConstructionParams)
{
	*elem_num_bits = 0;
	*elem_num_integer_bits = 0;
	*fifo_num_elem = 0;
	*hasConstructionParams = 0;
	*hasElementConstructionParams = 0;
}


inline mtiTypeIdT sc_signal<bool>::mti_get_channel_type()
{
	return mti_CreateNativeCType(MTI_TYPE_C_BOOL);
}


inline bool sc_signal<bool>::control_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name);
}


inline bool sc_signal<bool>::observe_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name);
}


inline mtiObjectHandleT sc_signal<sc_dt::sc_logic>::mti_get_channel_id( 
		sc_object* )
{
	return mti_get_prim_channel();
}


inline void* sc_signal<sc_dt::sc_logic>::mti_get_channel_value(int is_driving_value)
{
	if (is_driving_value)
		return m_new_val.mti_get_value();
	else
		return m_cur_val.mti_get_value();
}


inline void sc_signal<sc_dt::sc_logic>::mti_set_channel_value(
	void* value,
	int is_driving_value)
{
	m_new_val.mti_set_value(value);
}


inline void sc_signal<sc_dt::sc_logic>::mti_get_construction_params(
	int* elem_num_bits,
	int* elem_num_integer_bits,
	int* fifo_num_elem,
	int* hasConstructionParams,
	int* hasElementConstructionParams)
{
	*hasElementConstructionParams = 0;
	*elem_num_bits = 0;
	*elem_num_integer_bits = 0;
	*fifo_num_elem = 0;
	*hasConstructionParams = 0;
	*hasElementConstructionParams = 0;
}


inline mtiTypeIdT sc_signal<sc_dt::sc_logic>::mti_get_channel_type()
{
	return mti_CreateScScalarType(MTI_TYPE_SC_LOGIC);
}

inline bool sc_signal<sc_dt::sc_logic>::control_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_control_foreign_signal(this, hdl_name);
}


inline bool sc_signal<sc_dt::sc_logic>::observe_foreign_signal(const char* hdl_name)
{
    return sc_get_curr_simcontext()->mti_observe_foreign_signal(this, hdl_name);
}

MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(unsigned char, MTI_TYPE_C_UCHAR)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(char, MTI_TYPE_C_CHAR)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(unsigned int, MTI_TYPE_C_UINT)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(int, MTI_TYPE_C_INT)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(mtiUlongT, MTI_TYPE_C_ULONG)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(mtiLongT, MTI_TYPE_C_LONG)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(unsigned short, MTI_TYPE_C_USHORT)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(short, MTI_TYPE_C_SHORT)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(unsigned long long, MTI_TYPE_C_ULONGLONG)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(long long, MTI_TYPE_C_LONGLONG)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(float, MTI_TYPE_C_FLOAT)
MTI_DECLARE_CTYPE_SIGNAL_FUNCTIONS(double, MTI_TYPE_C_DOUBLE)

MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_bit)
MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_unsigned)
MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_signed)

MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_bv)
MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_lv)
MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_uint)
MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_int)
MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_biguint)
MTI_DECLARE_SC_VECTOR_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_bigint)

#ifdef SC_INCLUDE_FX
	MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_ufix)
	MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_ufix_fast)
	MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_fix)
	MTI_DECLARE_SCTYPE_SIGNAL_FUNCTIONS(sc_dt::sc_fix_fast)

	MTI_DECLARE_SC_FIXED_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_ufixed)
	MTI_DECLARE_SC_FIXED_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_ufixed_fast)
	MTI_DECLARE_SC_FIXED_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_fixed)
	MTI_DECLARE_SC_FIXED_TYPE_SIGNAL_FUNCTIONS(sc_dt::sc_fixed_fast)
#endif

#if !defined(__HP_aCC) && defined(SC_INCLUDE_MTI_AC)
	MTI_DECLARE_AC_INT_TYPE_SIGNAL_FUNCTIONS(ac_int)
	MTI_DECLARE_AC_FIXED_TYPE_SIGNAL_FUNCTIONS(ac_fixed)
#endif // SC_INCLUDE_MTI_AC


} // namespace sc_core
