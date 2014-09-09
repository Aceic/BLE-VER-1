/* $Id: //dvt/mti/rel/10.0b/src/systemc/member_fptr_bind.h#1 $ */

#ifndef INCLUDE_MEMBER_FPTR_BIND
#define INCLUDE_MEMBER_FPTR_BIND

/*
 * This header file declares the routines (mti_member_fptr_bind) that allow 
 * binding a member function function to an instance of mti_member_fptr_handle 
 * class. The member function could have up to 8 arguments currently.
 *
 * NOTE: this header file is for ModelSim internal use only. It doesn't support 
 * free-floating C++ function pointer binding, or the static member function pointer
 * binding.
 */

#include "member_fptr_handle.h"

template <typename T, typename RetType>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(void)) 
{
  return 
    new mti_member_fptr_handle<T, RetType>(object, action);
}

template <typename T, typename RetType, typename Arg1>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4, Arg5>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
        RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                             Arg6 arg6)) 
{
  return new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
        RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                             Arg6 arg6, Arg7 arg7)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7, typename Arg8>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                             Arg6 arg6, Arg7 arg7, Arg8 arg8)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>(object, action);
}

template <typename T, typename RetType, typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7, typename Arg8, typename Arg9>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	RetType (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                             Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9)) 
{
  return 
    new mti_member_fptr_handle<T, RetType, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9>(object, action);
}

/* version without return value */

template <typename T>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(void)) 
{
  return 
    new mti_member_fptr_handle<T>(object, action);
}

template <typename T,  typename Arg1>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1>(object, action);
}

template <typename T,  typename Arg1, typename Arg2>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4, Arg5>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                          Arg6 arg6)) 
{
  return new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                          Arg6 arg6, Arg7 arg7)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7, typename Arg8>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                          Arg6 arg6, Arg7 arg7, Arg8 arg8)) 
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>(object, action);
}

template <typename T,  typename Arg1, typename Arg2, 
          typename Arg3, typename Arg4, typename Arg5, typename Arg6, 
          typename Arg7, typename Arg8, typename Arg9>
inline mti_member_fptr_base* mti_member_fptr_bind(
	T* object,
	void (T::*action)(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
                          Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9))
{
  return 
    new mti_member_fptr_handle<T, unused, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9>(object, action);
} 
#endif
