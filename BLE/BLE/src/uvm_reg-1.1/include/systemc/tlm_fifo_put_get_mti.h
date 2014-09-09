/* $Id: //dvt/mti/rel/10.0b/src/systemc/tlm_fifo_put_get_mti.h#1 $ */


#define MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W> \
	inline \
	TYPE<W>   \
	tlm_fifo<TYPE<W> >::get( tlm_tag<TYPE<W> > * ) \
	{ \
 \
	  while( is_empty() ) { \
		this->wait( m_data_written_event ); \
	  } \
 \
	  m_num_read ++; \
	  request_update(); \
 \
	  return buffer->read(); \
 \
	} \
 \
	template <int W> \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_get( TYPE<W> & val_ ) \
	{ \
 \
	  if( is_empty() ) { \
		return false; \
	  } \
 \
	  m_num_read ++; \
	  request_update(); \
 \
	  val_ = buffer->read(); \
 \
	  return true; \
 \
	} \
 \
	template <int W> \
	inline  \
	bool \
	tlm_fifo<TYPE<W> >::nb_can_get( tlm_tag<TYPE<W> > * ) const { \
 \
	  return !is_empty(); \
 \
	} \
 \
	template <int W> \
	inline \
	void \
	tlm_fifo<TYPE<W> >::put( const TYPE<W> & val_ ) \
	{ \
		while( is_full() ) { \
		this->wait( m_data_read_event ); \
		} \
 \
		if( buffer->is_full() ) { \
 \
		  buffer->resize( buffer->size() * 2 ); \
 \
		} \
 \
		m_num_written ++; \
		buffer->write( val_ ); \
 \
		request_update(); \
	} \
 \
	template <int W> \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_put( const TYPE<W> & val_ ) \
	{ \
 \
	  if( is_full() ) { \
		return false; \
	  } \
 \
	  if( buffer->is_full() ) { \
 \
		buffer->resize( buffer->size() * 2 ); \
 \
	  } \
 \
	  m_num_written ++; \
	  buffer->write( val_ ); \
	  request_update(); \
 \
	  return true; \
	} \
 \
	template <int W> \
	inline  \
	bool \
	tlm_fifo<TYPE<W> >::nb_can_put( tlm_tag<TYPE<W> > * ) const { \
 \
	  return !is_full(); \
 \
	}

#define MTI_DECLARE_TLM_FIFO_PUT_GET_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	TYPE<W,I,Q,O,N>   \
	tlm_fifo<TYPE<W,I,Q,O,N> >::get( tlm_tag<TYPE<W,I,Q,O,N> > * ) \
	{ \
 \
	  while( is_empty() ) { \
		this->wait( m_data_written_event ); \
	  } \
 \
	  m_num_read ++; \
	  request_update(); \
 \
	  return buffer->read(); \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_get( TYPE<W,I,Q,O,N> & val_ ) \
	{ \
 \
	  if( is_empty() ) { \
		return false; \
	  } \
 \
	  m_num_read ++; \
	  request_update(); \
 \
	  val_ = buffer->read(); \
 \
	  return true; \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline  \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_can_get( tlm_tag<TYPE<W,I,Q,O,N> > * ) const { \
 \
	  return !is_empty(); \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	void \
	tlm_fifo<TYPE<W,I,Q,O,N> >::put( const TYPE<W,I,Q,O,N> & val_ ) \
	{ \
		while( is_full() ) { \
		this->wait( m_data_read_event ); \
		} \
 \
		if( buffer->is_full() ) { \
 \
		  buffer->resize( buffer->size() * 2 ); \
 \
		} \
 \
		m_num_written ++; \
		buffer->write( val_ ); \
 \
		request_update(); \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_put( const TYPE<W,I,Q,O,N> & val_ ) \
	{ \
 \
	  if( is_full() ) { \
		return false; \
	  } \
 \
	  if( buffer->is_full() ) { \
 \
		buffer->resize( buffer->size() * 2 ); \
 \
	  } \
 \
	  m_num_written ++; \
	  buffer->write( val_ ); \
	  request_update(); \
 \
	  return true; \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline  \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_can_put( tlm_tag<TYPE<W,I,Q,O,N> > * ) const { \
 \
	  return !is_full(); \
 \
	}

MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bv)
MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bigint)
MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_biguint)
MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_lv)
MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_int)
MTI_DECLARE_TLM_FIFO_PUT_GET_VECTOR_TYPE_FIFO_FUNCTIONS(sc_uint)

#ifdef SC_INCLUDE_FX
	MTI_DECLARE_TLM_FIFO_PUT_GET_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed)
	MTI_DECLARE_TLM_FIFO_PUT_GET_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed_fast)
	MTI_DECLARE_TLM_FIFO_PUT_GET_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed)
	MTI_DECLARE_TLM_FIFO_PUT_GET_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed_fast)
#endif
