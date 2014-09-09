/* $Id: //dvt/mti/rel/10.0b/src/systemc/member_fptr_handle.h#1 $ */

#ifndef MEMBER_FPTR_HANDLE
#define MEMBER_FPTR_HANDLE

#include "mti.h"

/* 
 * Two classes are defined in this header file.
 *
 *    mti_member_fptr_base and mti_member_fptr_handle
 *
 *
 *  mti_member_fptr_base is the base class representing a pointer
 *  to a class member function. It defines a bunch of the interfaces 
 *  (execute* and execute*_noret) for actually making the function calls.
 *
 *
 *  mti_member_fptr_handle is the concrete template-based class representing
 *  member function pointer derived from mti_member_fptr_base. It supports
 *  the functions with up to 8 arguments.
 * 
 */

class mti_member_fptr_base
{
 public:
  virtual ~mti_member_fptr_base() {};

  /* function with return value */

  virtual void execute0() = 0;
  virtual void execute1(void* arg1) = 0;
  virtual void execute2(void* arg1, void* arg2) = 0;
  virtual void execute3(void* arg1, void* arg2, void* arg3) = 0;
  virtual void execute4(void* arg1, void* arg2, void* arg3,
                        void* arg4) = 0;
  virtual void execute5(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5) = 0;
  virtual void execute6(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6) = 0;
  virtual void execute7(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7) = 0;
  virtual void execute8(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7, void* arg8) = 0;
  virtual void execute9(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7, void* arg8, void* arg9) = 0;

  virtual int   has_ret() = 0;
  virtual void* get_retval() = 0;


  /* function has no return value */

  virtual void execute0_noret() = 0;
  virtual void execute1_noret(void* arg1) = 0;
  virtual void execute2_noret(void* arg1, void* arg2) = 0;
  virtual void execute3_noret(void* arg1, void* arg2, void* arg3) = 0;
  virtual void execute4_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4) = 0;
  virtual void execute5_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5) = 0;
  virtual void execute6_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6) = 0;
  virtual void execute7_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7) = 0;
  virtual void execute8_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7, void* arg8) = 0;
  virtual void execute9_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7, void* arg8, void* arg9) = 0;

 protected:
  mti_member_fptr_base() {};

 private:
  mti_member_fptr_base(const mti_member_fptr_base&);
  mti_member_fptr_base& operator=(const mti_member_fptr_base&);
};


typedef struct mystr {
} unused;

static mtiNoParamFuncPtrT mti_cdbg_stop;

extern "C" mtiNoParamFuncPtrT mti_set_numof_step_scdpi_stop(int);

template <typename Env  = unused, typename RetType = unused,
          typename Arg1 = unused, typename Arg2 = unused, 
          typename Arg3 = unused, typename Arg4 = unused, 
          typename Arg5 = unused, typename Arg6 = unused,
          typename Arg7 = unused, typename Arg8 = unused, 
          typename Arg9 = unused>

class mti_member_fptr_handle : public mti_member_fptr_base
{
 public:
  typedef RetType (Env::*Action0)();
  typedef RetType (Env::*Action1)(Arg1);
  typedef RetType (Env::*Action2)(Arg1, Arg2);
  typedef RetType (Env::*Action3)(Arg1, Arg2, Arg3);
  typedef RetType (Env::*Action4)(Arg1, Arg2, Arg3, Arg4);
  typedef RetType (Env::*Action5)(Arg1, Arg2, Arg3, Arg4, Arg5);
  typedef RetType (Env::*Action6)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
  typedef RetType (Env::*Action7)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
  typedef RetType (Env::*Action8)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
                                  Arg7, Arg8);
  typedef RetType (Env::*Action9)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
                                  Arg7, Arg8, Arg9);

  typedef void (Env::*Action0_noret)();
  typedef void (Env::*Action1_noret)(Arg1);
  typedef void (Env::*Action2_noret)(Arg1, Arg2);
  typedef void (Env::*Action3_noret)(Arg1, Arg2, Arg3);
  typedef void (Env::*Action4_noret)(Arg1, Arg2, Arg3, Arg4);
  typedef void (Env::*Action5_noret)(Arg1, Arg2, Arg3, Arg4, Arg5);
  typedef void (Env::*Action6_noret)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
  typedef void (Env::*Action7_noret)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
  typedef void (Env::*Action8_noret)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
                                     Arg7, Arg8);
  typedef void (Env::*Action9_noret)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
                                     Arg7, Arg8, Arg9);

  /* **************************************************************************
   * Note that the insertion of mti_cdbg_stop() is to allow Cdebug
   * catch the HDL-SystemC boundary for 'step' functionality support.
   * If you have to change any of the execute*() functions, make sure to update
   * macro MTI_SET_NUMOF_STEP_SCDPI_STOP.
   **************************************************************************/

#define MTI_SET_NUMOF_STEP_SCDPI_STOP \
    mti_cdbg_stop = mti_set_numof_step_scdpi_stop(2); (*mti_cdbg_stop)()

#ifdef  ASSERT_RETURN
#undef  ASSERT_RETURN
#endif
#define ASSERT_RETURN  assert(_has_ret); MTI_SET_NUMOF_STEP_SCDPI_STOP;

#ifdef  ASSERT_NO_RETURN
#undef  ASSERT_NO_RETURN
#endif
#define ASSERT_NO_RETURN  assert(! _has_ret); MTI_SET_NUMOF_STEP_SCDPI_STOP;

#ifdef CLASSMEMBERINIT
#undef CLASSMEMBERINIT
#endif

#ifdef CLASSMEMBERINIT_NORET
#undef CLASSMEMBERINIT_NORET
#endif

#define CLASSMEMBERINIT(n)       _env (e), _has_ret (1)\
                                 {\
                                   _fptr.action##n = a##n; \
                                 }
#define CLASSMEMBERINIT_NORET(n) _env (e), _has_ret (0)\
                                 {\
                                   _fptr.action##n##_noret = a##n; \
                                 }

  mti_member_fptr_handle(Env *e, Action0 a0) :
    CLASSMEMBERINIT(0)

  mti_member_fptr_handle(Env *e, Action1 a1) :
    CLASSMEMBERINIT(1) 

  mti_member_fptr_handle(Env *e, Action2 a2) :
    CLASSMEMBERINIT(2) 

  mti_member_fptr_handle(Env *e, Action3 a3) :
    CLASSMEMBERINIT(3) 

  mti_member_fptr_handle(Env *e, Action4 a4) :
    CLASSMEMBERINIT(4) 

  mti_member_fptr_handle(Env *e, Action5 a5) :
    CLASSMEMBERINIT(5) 

  mti_member_fptr_handle(Env *e, Action6 a6) :
    CLASSMEMBERINIT(6) 

  mti_member_fptr_handle(Env *e, Action7 a7) :
    CLASSMEMBERINIT(7) 

  mti_member_fptr_handle(Env *e, Action8 a8) :
    CLASSMEMBERINIT(8) 

  mti_member_fptr_handle(Env *e, Action9 a9) :
    CLASSMEMBERINIT(9) 

  mti_member_fptr_handle(Env *e, Action0_noret a0) :
    CLASSMEMBERINIT_NORET(0) 

  mti_member_fptr_handle(Env *e, Action1_noret a1) :
    CLASSMEMBERINIT_NORET(1) 

  mti_member_fptr_handle(Env *e, Action2_noret a2) :
    CLASSMEMBERINIT_NORET(2) 

  mti_member_fptr_handle(Env *e, Action3_noret a3) :
    CLASSMEMBERINIT_NORET(3) 

  mti_member_fptr_handle(Env *e, Action4_noret a4) :
    CLASSMEMBERINIT_NORET(4) 

  mti_member_fptr_handle(Env *e, Action5_noret a5) :
    CLASSMEMBERINIT_NORET(5) 

  mti_member_fptr_handle(Env *e, Action6_noret a6) :
    CLASSMEMBERINIT_NORET(6) 

  mti_member_fptr_handle(Env *e, Action7_noret a7) :
    CLASSMEMBERINIT_NORET(7) 

  mti_member_fptr_handle(Env *e, Action8_noret a8) :
    CLASSMEMBERINIT_NORET(8) 

  mti_member_fptr_handle(Env *e, Action9_noret a9) :
    CLASSMEMBERINIT_NORET(9) 


#undef CLASSMEMBERINIT
#undef CLASSMEMBERINIT_NORET

  virtual void execute0()
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action0))();
  }

  virtual void execute1(void* arg1)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action1))(*(Arg1*)arg1);
  }

  virtual void execute2(void* arg1, void* arg2)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action2))(*(Arg1*)arg1, *(Arg2*)arg2);
  }
  
  virtual void execute3(void* arg1, void* arg2, void* arg3)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action3))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3);
  }
  
  virtual void execute4(void* arg1, void* arg2, void* arg3,
                        void* arg4)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action4))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4);
  }
  
  virtual void execute5(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action5))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4, *(Arg5*)arg5);
  }
  
  virtual void execute6(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action6))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6);
  }
  
  virtual void execute7(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7) 
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action7))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                       *(Arg7*)arg7);
  }

  virtual void execute8(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7, void* arg8) 
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action8))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                       *(Arg7*)arg7, *(Arg8*)arg8);
  }

  virtual void execute9(void* arg1, void* arg2, void* arg3,
                        void* arg4, void* arg5, void* arg6,
                        void* arg7, void* arg8, void* arg9)
  {
    ASSERT_RETURN;
    _retval = (_env->*(_fptr.action9))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                       *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                       *(Arg7*)arg7, *(Arg8*)arg8, *(Arg9*)arg9);
  }

  int   has_ret()  { return _has_ret;};

  void* get_retval() 
  {
    return (void*) &_retval;
  }


  /* function has no return value */

   virtual void execute0_noret()
   {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action0_noret))();
   }

   virtual void execute1_noret(void* arg1)
   {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action1_noret))(*(Arg1*)arg1);
   }

   virtual void execute2_noret(void* arg1, void* arg2)
   {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action2_noret))(*(Arg1*)arg1, *(Arg2*)arg2);
   }

   virtual void execute3_noret(void* arg1, void* arg2, void* arg3) 
   {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action3_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3);
   }

   virtual void execute4_noret(void* arg1, void* arg2, void* arg3,
                               void* arg4)
   {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action4_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4);
   }

  virtual void execute5_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5)
  {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action5_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4, *(Arg5*)arg5);
  }

  virtual void execute6_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6)
  {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action6_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6);
  }

  virtual void execute7_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7)
  {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action7_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                    *(Arg7*)arg7);
  }

  virtual void execute8_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7, void* arg8) 
  {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action8_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                    *(Arg7*)arg7, *(Arg8*)arg8);
  }
  
  virtual void execute9_noret(void* arg1, void* arg2, void* arg3,
                              void* arg4, void* arg5, void* arg6,
                              void* arg7, void* arg8, void* arg9) 
  {
     ASSERT_NO_RETURN;
     (_env->*(_fptr.action9_noret))(*(Arg1*)arg1, *(Arg2*)arg2, *(Arg3*)arg3, 
                                    *(Arg4*)arg4, *(Arg5*)arg5, *(Arg6*)arg6,
                                    *(Arg7*)arg7, *(Arg8*)arg8, *(Arg9*)arg9);
  }

 private:

  union 
  {
    Action0 action0;
    Action1 action1;
    Action2 action2;
    Action3 action3;
    Action4 action4;
    Action5 action5;
    Action6 action6;
    Action7 action7;
    Action8 action8;
    Action9 action9;

    Action0_noret action0_noret;
    Action1_noret action1_noret;
    Action2_noret action2_noret;
    Action3_noret action3_noret;
    Action4_noret action4_noret;
    Action5_noret action5_noret;
    Action6_noret action6_noret;
    Action7_noret action7_noret;
    Action8_noret action8_noret;
    Action9_noret action9_noret;
  }  _fptr;

  Env*      _env;
  RetType   _retval;
  int       _has_ret;
};

#endif 
