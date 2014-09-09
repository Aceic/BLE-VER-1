/* $Id: //dvt/mti/rel/10.0b/src/systemc/tlm_fifo_resize_mti.h#1 $ */

#include "sc_report.h"

#define MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W> \
	inline \
	void \
	tlm_fifo<TYPE<W> >::nb_expand( unsigned int n ) { \
 \
	  m_expand = true; \
	  m_size += n; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
	  request_update(); \
 \
	} \
 \
	template <int W> \
	inline \
	void \
	tlm_fifo<TYPE<W> >::nb_unbound( unsigned int n ) { \
 \
	  m_expand = true; \
	  m_size = -n; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
 \
	  if( buffer->size() < static_cast<int>( n ) ) { \
		buffer->resize( n ); \
	  } \
 \
	  request_update(); \
 \
	} \
 \
	template <int W> \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_reduce( unsigned int n ) { \
 \
	  if( m_size < 0 ) { \
		return false; \
	  } \
 \
	  return nb_bound( size() - n ); \
 \
	} \
 \
	template <int W> \
	inline \
	bool \
	tlm_fifo<TYPE<W> >::nb_bound( unsigned int new_size ) { \
 \
	  bool ret = true; \
 \
	  if( static_cast<int>( new_size ) < used() ) { \
 \
		new_size = used(); \
		ret = false; \
 \
	  } \
 \
	  m_size = new_size; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
	  return ret; \
	}

#define MTI_DECLARE_SC_TLM_RESIZE_FIXED_TYPE_FIFO_FUNCTIONS(TYPE) \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	void \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_expand( unsigned int n ) { \
 \
	  m_expand = true; \
	  m_size += n; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
	  request_update(); \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	void \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_unbound( unsigned int n ) { \
 \
	  m_expand = true; \
	  m_size = -n; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
 \
	  if( buffer->size() < static_cast<int>( n ) ) { \
		buffer->resize( n ); \
	  } \
 \
	  request_update(); \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_reduce( unsigned int n ) { \
 \
	  if( m_size < 0 ) { \
		return false; \
	  } \
 \
	  return nb_bound( size() - n ); \
 \
	} \
 \
	template <int W, int I, sc_q_mode Q, sc_o_mode O, int N> \
	inline \
	bool \
	tlm_fifo<TYPE<W,I,Q,O,N> >::nb_bound( unsigned int new_size ) { \
 \
	  bool ret = true; \
 \
	  if( static_cast<int>( new_size ) < used() ) { \
 \
		new_size = used(); \
		ret = false; \
 \
	  } \
 \
	  m_size = new_size; \
	  if (m_mti_is_resize_warning_not_given) { \
		char buf[256]; \
		sprintf(buf, "%d", m_mti_initial_fifo_size); \
		std::string msg = std::string("The tlm_fifo object '"); \
		msg += name(); \
		msg += "' is being resized from its originally specified size: "; \
		msg += buf; \
		msg += ". The simulator will not display the resized fifo object in the GUI, although the actual simulation object will be correctly resized."; \
		SC_REPORT_WARNING( SC_ID_TLM_FIFO_RESIZE_NOT_SUPPORTED_, msg.c_str() ); \
		m_mti_is_resize_warning_not_given = 0; \
	  } \
	  return ret; \
	}

MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bigint)
MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_biguint)
MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_bv)
MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_lv)
MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_int)
MTI_DECLARE_SC_TLM_RESIZE_VECTOR_TYPE_FIFO_FUNCTIONS(sc_uint)

#ifdef SC_INCLUDE_FX
	MTI_DECLARE_SC_TLM_RESIZE_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed)
	MTI_DECLARE_SC_TLM_RESIZE_FIXED_TYPE_FIFO_FUNCTIONS(sc_fixed_fast)
	MTI_DECLARE_SC_TLM_RESIZE_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed)
	MTI_DECLARE_SC_TLM_RESIZE_FIXED_TYPE_FIFO_FUNCTIONS(sc_ufixed_fast)
#endif
