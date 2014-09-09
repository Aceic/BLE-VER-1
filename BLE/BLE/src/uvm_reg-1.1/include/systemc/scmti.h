 /*****************************************************************************
 *
 * SystemC Interface Definitions
 *
 * Copyright 1991-2011 Mentor Graphics Corporation
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY 
 * OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
 *
 *****************************************************************************/

/* $Id: //dvt/mti/rel/10.0b/src/vsim/scmti.h#1 $ */

#ifndef SCMTI_H
#define SCMTI_H

#include "mti.h"
#include "svdpi.h"

/*****************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef void  (*mtiDoubleIntFuncPtrT)(double, int);
typedef void  (*mtiDoubleIntIntFuncPtrT)(double, int, int);
typedef void  (*mtiVoidVoidFuncPtrT)(void*, void*);
typedef void  (*mtiVoidVoidPtrFuncPtrT)(void*, void**);
typedef void  (*mtiVoidVoidPtrIntFuncPtrT)(void*, void**, int);
typedef void  (*mtiVoidIntFuncPtrT)(void*, int);
typedef void  (*mtiVoidVoidIntFuncPtrT)(void*, void*, int);
typedef int   (*mtiVoidCharFuncPtrT)(void*, char*);
typedef void  (*mtiIntFuncPtrT)(int);
typedef void  (*mtiVoidIntIntIntFuncPtrT)(void*, int*, int*, int*);
typedef int   (*mtiCharFuncPtrT)(char*);
typedef char* (*mtiCharVoidFuncPtrT)(void*);
typedef void  (*mtiVoidIntIntFuncPtrT)(void*, int, int);
typedef void  (*mtiVoidIntUintFuncPtrT)(void*, int, unsigned int);
typedef int   (*mtiVoidRIntFuncPtrT)(void*);
typedef void  (*mtiVoidVoidIntIntFuncPtrT)(void*, void*, int, int);

typedef void (*mtiCustomDebugCB)(void*, char*, char);

typedef void* mtiObjectHandleT;

typedef enum mtiScProcessPriorityEnum_ {
    MTI_PROC_SC_METHOD = 11,  /* SystemC method process. */
    MTI_PROC_SC_THREAD = 12   /* SystemC thread process. */
} mtiScProcessPriorityT;

typedef enum mtiScApiFuncKindEnum_ {
    MTI_SC_INITIALIZE,
    MTI_SC_EXECUTE_METHOD,
    MTI_SC_EXECUTE_METHOD_DEBUG,
    MTI_SC_EXECUTE_THREAD,
    MTI_SC_TRIGGER_EVENT,
    MTI_SC_TRIGGER_CLOCK_EVENT,
    MTI_SC_UPDATE_PRIMITIVE_CHANNEL,
    MTI_SC_CLEAR_PRIMITIVE_CHANNEL_UPDATE_REQUESTED_FLAG,
    MTI_SC_GET_PRIMITIVE_CHANNEL_VALUE,
    MTI_SC_SET_PRIMITIVE_CHANNEL_VALUE,
    MTI_SC_FANOUT_TO_PRIMITIVE_CHANNEL,
	MTI_SC_GET_PRIMITIVE_CHANNEL_TYPE,
    MTI_SC_BIND_MODULE_NAME,
    MTI_SC_IS_VALID_MODULE,
    MTI_SC_IS_VALID_PRIM_CHANNEL,
    MTI_SC_IS_VALID_PORT,
    MTI_SC_IS_VALID_EXPORT,
    MTI_SC_DELETE_MODULE,
    MTI_SC_START_OF_SIMULATION,
    MTI_SC_END_OF_SIMULATION,
    MTI_SC_TRACE_CYCLE,
    MTI_SC_GET_VARIABLE_VALUE,
    MTI_SC_SET_VARIABLE_VALUE,
    MTI_SC_GET_VARIABLE_VALUE_SIZE,
    MTI_SC_DELETE_MODULE_DYNALLOC_LIST,
    MTI_SC_GET_OBJECT_TYPENAME,
    MTI_SC_GET_OBJECT_NAME,
    MTI_SC_GET_OBJECT_REGION_ID,
    MTI_SC_IS_TYPENAME_CHECKED,
    MTI_SC_BLOCK_SYSTEMC_THREAD,
    MTI_SC_WAKEUP_SYSTEMC_THREAD,
    MTI_SC_INCR_DEFAULT_STACK_SIZE,
    MTI_SC_SET_PROPERTY_FOR_INI_VAR,
    MTI_SC_EXECUTE_SC_MAIN,
    MTI_SC_EXECUTE_SWITCH_THREAD,
    MTI_SC_GET_THREAD_STATE,
    MTI_SC_DUMP_API_TRACE,
    MTI_SC_DISABLE_SYSTEMC_THREAD
} mtiScApiFuncKindT;

typedef enum mtiScStopModeEnum_ {
    MTI_SC_STOP_IMMEDIATE,
    MTI_SC_STOP_FINISH_DELTA,
	MTI_CY_STOP_IMMEDIATE,
    MTI_CY_STOP_FINISH_DELTA
} mtiScStopModeT;

typedef enum mtiIniVarTypeEnum_ {
    MTI_INI_IEEE_WARNINGS,
	MTI_INI_SIGNAL_WRITER_CHECK
} mtiIniVarNameT;

typedef enum mtiScStartModeEnum_ {
	MTI_SC_START_NO_ARG,
	MTI_SC_START_MINUS_ONE,
	MTI_SC_START_DEFAULT
} mtiScStartModeT;

/* SystemC hierarchical connection type */
typedef enum mtiHierConnTypeEnum_ {
    MTI_HIERCONN_CONTROL, /* control_foreign_signal */
    MTI_HIERCONN_OBSERVE, /* observe_foreign_signal */
    MTI_HIERCONN_SCV_CONNECT_IN,  /* scv_connect in input direction */
    MTI_HIERCONN_SCV_CONNECT_OUT,  /* scv_connect in output direction */
    MTI_HIERCONN_SIGSPY  /* SignalSpy */
} mtiHierConnT;

/* SystemC cleanup phase */
typedef enum mtiScCleanupPhase_ {
	MTI_SC_CLEANUP_NONE,
	MTI_SC_CLEANUP_DELETE_HIER,
	MTI_SC_CLEANUP_UNLOAD_SO
} mtiScCleanupPhaseT;

/****************************************************************************
 * Vsim/SystemC Kernel flags routine
 *****************************************************************************/
typedef enum {
  SC_KF_SCDPI_DEBUG = 0,    /* whether -scdpidebug switch has been turned on */
  SC_KF_USING_PTHREAD,      /* whether -scpthread switch has been turned on */
  SC_KF_SCCLK_DEBUG,        /* wehther -scclkdebug switch has been turned on */
  SC_KF_LAST
} sc_kernel_flag_bit;

#if !defined(WIN32) || defined(VSIM)
extern mtiUInt32T sc_kernel_flags;
#define SC_GETKF(n) ((sc_kernel_flags & (0x1U << (n))) >> (n))
#else
#define SC_GETKF(n) ((mti_ScGetKernelFlags() & (0x1U << (n))) >> (n))
#endif

#if defined(VSIM)
#define SC_CLRKF(n) (sc_kernel_flags &= ~(0x1U << (n)))
#define SC_SETKF(n) (sc_kernel_flags |= (0x1U << (n)))
#endif

#define SC_INITKF(n) (0x1U << (n))

/*****************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


extern void          mti_AddScApiToFuncTray     PROTO((mtiScApiFuncKindT kind,
                                                      void* func_ptr));

extern void          mti_GetScTimeUnit    		PROTO((double* time_value,
													   int* time_unit));

/***********
 * Regions *
 ***********/

extern mtiObjectHandleT  mti_CreateScRegion         PROTO((mtiObjectHandleT parent,
                                                           void* sc_module_inst,
                                                           char * name));

extern mtiObjectHandleT  mti_LoadScHdlRegion        PROTO((char * hdlName,
                                                       mtiObjectHandleT parent,
                                                       char * instName,
                                                       int num_generics,
                                                       const char** generics_list));

extern void             mti_SetScRegionDebugInfo   PROTO((char * inst_name,
                                                          char * user_inst_name,
                                                       mtiObjectHandleT region));

/*************
 * Processes *
 *************/

extern mtiObjectHandleT  mti_CreateScProcess     PROTO((char * name,
                                                    void * sc_process,
                                                    mtiObjectHandleT parent,
                                                    mtiScProcessPriorityT priority,
                                                    int is_dynamic,
                                                    int is_sc_method_debug)); 

extern void           mti_FreeScProcess           PROTO((void *sc_process_obj));

extern void           mti_SetScProcessDebugInfo   PROTO((mtiObjectHandleT process));

extern void           mti_ActivateScMethodProcess PROTO((mtiObjectHandleT proc,
                                                      int is_immediate_notify));

extern void           mti_ActivateScThreadProcess PROTO((mtiObjectHandleT proc,
                                                      int is_immediate_notify));

/**********
 * Events *
 **********/

extern mtiObjectHandleT  mti_CreateScEvent             PROTO((void * sc_event, int is_clock_event));

extern void         mti_ScheduleScEvent           PROTO((mtiObjectHandleT sc_event,
                                                      void*  delay));

extern void         mti_ScheduleScClockEvent      PROTO((mtiObjectHandleT sc_event,
                                                      void*  delay));

extern mtiObjectHandleT  mti_CreateAndScheduleScEvent  PROTO((void * sc_event,
                                                      mtiDelayT delay));

extern void         mti_CancelScEvent             PROTO((mtiObjectHandleT event));

extern void         mti_RemoveEventFromEventList  PROTO((mtiObjectHandleT event));

/*******************
 * Signals & Ports *
 *******************/

extern mtiObjectHandleT mti_CreateScPrimChannel     PROTO((char* name,
                                                        char* declared_name,
                                                        mtiObjectHandleT region,
                                                        void* sc_prim_channel_handle,
                                                        int isNumBitsValid,
                                                        int isElementNumBitsValid,
                                                        int numBits,
                                                        int numIntegerBits,
                                                        int numFifoElements));

extern void           mti_SetBoundaryChannelDebugInfo PROTO((char* name,
                                                           mtiObjectHandleT prim_channel));

extern mtiObjectHandleT mti_CreateScSignalAlias   PROTO((char* name,
                                                        char* actual_obj_name,
														void* sc_port_handle,
                                                        mtiObjectHandleT region,
                                                        mtiObjectHandleT actual_signal,
                                                        mtiDirectionT mode,
                                                        int is_export));

extern void           mti_ActivateScPrimChannel   PROTO((mtiObjectHandleT sc_prim_channel));

extern void           mti_ConnectToScParentSignal PROTO((mtiObjectHandleT sc_prim_channel_obj,
                                                        mtiObjectHandleT hdl_region,
                                                        const char* hdl_port_name));

extern void           mti_ConnectToHdlParentSignal PROTO((mtiObjectHandleT sc_port_handle,
                                                          mtiObjectHandleT sc_region,
                                                          mtiDirectionT mode));

extern void           mti_MakeScPrimChannelResolved PROTO((mtiObjectHandleT sc_prim_channel_obj));

extern void           mti_SetScSignalValue PROTO((mtiObjectHandleT sc_prim_channel_obj,
                                                            void* value));

extern void           mti_WarnIncompleteAggregates PROTO(());

/* Variables */

extern void           mti_CreateModuleVariables PROTO((mtiObjectHandleT region,
                                                       void* sc_module_handle));

/* Misc Functions */

extern void           mti_ScStop PROTO((mtiScStopModeT stop_mode));

extern void           mti_ScError PROTO(());

extern char*          mti_GetScMessage PROTO((char *msg, int id));

extern void           mti_PrintScMessage PROTO((char *msg));

extern mtiScCleanupPhaseT  mti_IsSystemcCleanup PROTO(());

extern void*          mti_GetHdlGenericList PROTO(());

extern void*          mti_FindGeneric PROTO((void* generic_list,
                                             char* generic_name,
                                             int* is_pos_association));

extern void           mti_DestroyGenericList PROTO((void* generic_list));

extern int            mti_GetIntGenericValue PROTO((void* generic, int* value));

extern int            mti_GetRealGenericValue PROTO((void* generic, double* value));

extern char*          mti_GetStringGenericValue PROTO((void* generic, char format_char));

extern void*          mti_ScGetNow PROTO(());

extern char           mti_GetScPathSeparator PROTO(());

extern char*          mti_GetScCurrContextPathName PROTO(());

extern int            mti_IsVoptMode PROTO(());

extern int            mti_IsScUsingPthread PROTO(());

extern void           mti_GetIniVarValue PROTO((mtiIniVarNameT ini_var, void* value));

extern int            mti_ScControlHdlSignal PROTO((mtiObjectHandleT sc_prim_channel_obj,
                                                    const char* hdl_name, int is_scvconnect));

extern int            mti_ScObserveHdlSignal PROTO((mtiObjectHandleT sc_prim_channel_obj,
                                                    const char* hdl_name, int is_scvconnect));

extern void           mti_ScInitSignalSpy PROTO((   mtiRegionIdT region,
                                                    const char* src_name,
                                                    const char* dest_name,
                                                    int verbose,
                                                    int control_state,
                                                    mtiObjectHandleT sc_prim_channel_obj,
                                                    mtiHierConnT hier_conn_type));

extern void           mti_ScDisableSignalSpy PROTO((mtiRegionIdT region,
                                                    const char* src_name,
                                                    const char* dest_name,
                                                    int verbose));

extern void           mti_ScEnableSignalSpy PROTO(( mtiRegionIdT region,
                                                    const char* src_name,
                                                    const char* dest_name,
                                                    int verbose));

extern void           mti_ScSignalForce PROTO(( mtiRegionIdT region,
                                                const char* dest_name,
                                                const char* value,
                                                const int64_t rel_time,
                                                int force_type,
                                                const int64_t cancel_period,
                                                int verbose));

extern void           mti_ScSignalRelease PROTO((   mtiRegionIdT region,
                                                    const char* dest_name,
                                                    int verbose));

extern void           mti_ScInitSignalDriver PROTO((mtiRegionIdT region,
                                                    const char* src_name,
                                                    const char* dest_name,
                                                    const int64_t delay,
                                                    int delay_type,
                                                    int verbose,
                                                    mtiObjectHandleT sc_prim_channel_obj,
                                                    mtiHierConnT hier_conn_type));

extern int            mti_GetScArgc PROTO(());

extern const char* const* mti_GetScArgv PROTO(());

extern mtiUInt32T     mti_ScDelta PROTO(());

extern mtiUInt32T     mti_ScGetKernelFlags PROTO(());

extern int            mti_AskScStdin PROTO((char * buf, mtiLongT bufsize, const char * prompt));

extern void           mti_ScSetTypename PROTO(());

extern int            mti_RegisterScImportFunc PROTO((const char* fName, void* objInst, void* fptr, uint64_t stackSize));

extern void           mti_RegisterScCustomDebugFunc PROTO((
									mtiObjectHandleT obj_inst, 
									size_t length, 
									mtiCustomDebugCB cb_func,
									const char* name,
									const char* filename,
									int lineno,
									mtiObjectHandleT parent_region));

extern void           mti_SetScMainScope PROTO((mtiRegionIdT scope));

extern void           mti_SetScStartMode PROTO((mtiScStartModeT mode));

extern char*          mti_find_scmod_handles  PROTO((const char* scmod_names));
extern void           mti_cdbg_suspend_me_wrapper PROTO(());

/* 
 * Type enumerations.  Should be merged with mtiTypeKindEnum_ defined
 * in mti.h when the type creation APIs declared below are moved.
 */
typedef enum mtiScTypeKindEnum_ {
    MTI_TYPE_SC_ENUM        = 10, /* Had to use MTI_TYPE_SC_ENUM to avoid
                                   * conflict with MTI_TYPE_C_ENUM in mti.h
                                   */
    MTI_TYPE_C_BOOL         = 11,
    MTI_TYPE_C_UCHAR        = 12,
    MTI_TYPE_C_CHAR         = 13,
    MTI_TYPE_C_USHORT       = 14,
    MTI_TYPE_C_SHORT        = 15,
    MTI_TYPE_C_UINT         = 16,
    MTI_TYPE_C_INT          = 17,
    MTI_TYPE_C_ULONG        = 18,
    MTI_TYPE_C_LONG         = 19,
    MTI_TYPE_C_ULONGLONG    = 20,
    MTI_TYPE_C_LONGLONG     = 21,
    MTI_TYPE_C_FLOAT        = 22,
    MTI_TYPE_C_DOUBLE       = 23,
    MTI_TYPE_C_LONG_DOUBLE  = 24,
    MTI_TYPE_C_POINTER      = 25,
    MTI_TYPE_C_ARRAY        = 26,
    MTI_TYPE_C_STRUCT       = 27,
    MTI_TYPE_C_CLASS        = 28,
    MTI_TYPE_C_UNION        = 29,
    MTI_TYPE_SC_BIT         = 30,
    MTI_TYPE_SC_LOGIC       = 31,
    MTI_TYPE_SC_BV          = 32,
    MTI_TYPE_SC_LV          = 33,
    MTI_TYPE_SC_UINT        = 34,
    MTI_TYPE_SC_INT         = 35,
    MTI_TYPE_SC_BIGUINT     = 36,
    MTI_TYPE_SC_BIGINT      = 37,
    MTI_TYPE_SC_UNSIGNED    = 38,
    MTI_TYPE_SC_SIGNED      = 39,
    MTI_TYPE_SC_UFIXED      = 40,
    MTI_TYPE_SC_FIXED       = 41,
    MTI_TYPE_SC_UFIXED_FAST = 42,
    MTI_TYPE_SC_FIXED_FAST  = 43,
    MTI_TYPE_SC_UFIX        = 44,
    MTI_TYPE_SC_FIX         = 45,
    MTI_TYPE_SC_UFIX_FAST   = 46,
    MTI_TYPE_SC_FIX_FAST    = 47,
    MTI_TYPE_C_STRING       = 53
} mtiScTypeKindT;

typedef struct mtiTdEnumListStruct_ {
    char* name;
    int value;
    struct mtiTdEnumListStruct_* next;
} mtiTdEnumListT;


typedef struct mtiTdMemberListStruct_ {
    char* name;
    int offset;
    mtiTypeIdT type;
    struct mtiTdMemberListStruct_* next;
} mtiTdMemberListT;

/*
 * Type creation functions.  Should be moved to mti.h when we decide
 * to make them public.  Putting them here as they will be used internally
 * in SCV for now.
 */
extern mtiTypeIdT      mti_CreateNativeCType   PROTO((mtiScTypeKindT kind));
extern mtiTypeIdT      mti_CreateCEnumType     PROTO((mtiTdEnumListT* enum_list));
extern mtiTypeIdT      mti_CreateCPointer      PROTO((void));
extern mtiTypeIdT      mti_CreateCStringType   PROTO((void));
extern mtiTypeIdT      mti_CreateCArrayType    PROTO((mtiInt32T  size,
                                                      mtiTypeIdT elem_type));
                                                      
extern mtiTypeIdT      mti_CreateDynamicArrayType    PROTO((mtiTypeIdT elem_type));                                                      
                                                      
                                                      
extern mtiTypeIdT      mti_CreateCComposite    PROTO((mtiScTypeKindT kind,
                                                      mtiInt32T size,
                                                      mtiInt32T is_aligned,
                                                      mtiTdMemberListT* members));

extern mtiTypeIdT      mti_CreateScScalarType  PROTO((mtiScTypeKindT kind));
extern mtiTypeIdT      mti_CreateScVectorType  PROTO((mtiInt32T size, mtiScTypeKindT kind));
extern mtiTypeIdT      mti_CreateScFixedPointType PROTO((mtiInt32T length, 
											             mtiInt32T integer_length,
													     mtiScTypeKindT kind));

extern mtiInt32T       mti_GetTypeSize         PROTO((mtiTypeIdT type));

extern mtiInt32T       mti_GetFieldValueOffset PROTO((mtiTypeIdT type, mtiInt32T field));

extern int             mti_PrepareScForSpin PROTO((void));

extern void            mti_ResetScSpin PROTO((void));


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
