
/* $Id: //dvt/mti/rel/10.0b/src/systemc/tlm_fifo_peek_mti.h#1 $ */

#define MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(TYPE) \
	template < int W > \
	inline \
	TYPE<W> \
	tlm_fifo<TYPE<W> >::peek( tlm_tag<TYPE<W> > * ) const { \
\
	  while( is_empty() ) { \
		const_cast< tlm_fifo<TYPE<W> > * >( this )->wait( m_data_written_event ); \
	  } \
\
	  return buffer->read_data(); \
\
	} \
\
	template < int W > \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_peek( TYPE<W> &t ) const { \
\
	  if( used() < 1 ) { \
		return false; \
	  } \
\
	  t = buffer->peek_data( 0 ); \
	  return true; \
\
	} \
\
	template < int W > \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_peek( TYPE<W> &t , int n ) const { \
\
	  if( n >= used() || n < -1 ) { \
		return false; \
	  } \
	   \
	  if( n == -1 ) { \
		n = used() - 1; \
	  } \
\
	  t = buffer->peek_data( n ); \
	  return true; \
\
	} \
		 \
	template< int W > \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_can_peek( tlm_tag<TYPE<W> > * ) const \
	{ \
	  return !is_empty(); \
	} \
\
	template < int W > \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_poke( const TYPE<W> &t , int n ) { \
\
	  if( n >= used() || n < 0 ) { \
		return false; \
	  } \
\
	  buffer->poke_data( n ) = t; \
	  return true; \
\
	}


#define MTI_DECLARE_TLM_FIFO_PEEK_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	TYPE<W,I,Q,O,N> \
	tlm_fifo<TYPE<W,I,Q,O,N> >::peek( tlm_tag<TYPE<W,I,Q,O,N> > * ) const { \
\
	  while( is_empty() ) { \
\
		const_cast< tlm_fifo<TYPE<W,I,Q,O,N> > * >( this )->wait( m_data_written_event ); \
\
	  } \
\
	  return buffer->read_data(); \
\
	} \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_peek( TYPE<W,I,Q,O,N> &t ) const { \
\
	  if( used() < 1 ) { \
		return false; \
	  } \
\
	  t = buffer->peek_data( 0 ); \
	  return true; \
\
	} \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_peek( TYPE<W,I,Q,O,N> &t , int n ) const { \
\
	  if( n >= used() || n < -1 ) { \
		return false; \
	  } \
	   \
	  if( n == -1 ) { \
		n = used() - 1; \
	  } \
\
	  t = buffer->peek_data( n ); \
	  return true; \
\
	} \
		 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_can_peek( tlm_tag<TYPE<W,I,Q,O,N> > * ) const \
	{ \
	  return !is_empty(); \
	} \
\
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_poke( const TYPE<W,I,Q,O,N> &t , int n ) { \
\
	  if( n >= used() || n < 0 ) { \
		return false; \
	  } \
\
	  buffer->poke_data( n ) = t; \
	  return true; \
\
	}

MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bigint)
MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_biguint)
MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bv)
MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_lv)
MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_int)
MTI_DECLARE_TLM_FIFO_PEEK_VECTOR_TYPE_FIFO_FUNCTIONS(sc_uint)

#ifdef SC_INCLUDE_FX
	MTI_DECLARE_TLM_FIFO_PEEK_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed)
	MTI_DECLARE_TLM_FIFO_PEEK_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed_fast)
	MTI_DECLARE_TLM_FIFO_PEEK_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed)
	MTI_DECLARE_TLM_FIFO_PEEK_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed_fast)
#endif
