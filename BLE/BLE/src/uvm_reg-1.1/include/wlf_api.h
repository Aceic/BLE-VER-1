#ifndef WLFAPI_H
#define WLFAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* 
*  Copyright 1991-2011 Mentor Graphics Corporation
* 
*  All Rights Reserved.
* 
*  THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
*  MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
* 
******************************************************************************/

/******************************************************************************
* 
*   External MTI WLF interface header file.
* 
*   WLF Naming Convention   Prefix  Example
*       Function names:     wlf     wlfValueToString
*       Visible data:       wlf     wlfDataNames
*       Data types:         Wlf     WlfDataTypes    (enum types end in "Enum");
*       Data handles:       Wlf     WlfPackId       (always end in "Id")
*       Enum values:        WLF_    WLF_ENUM_VALUE
*       Macro definitions:  WLF_    WLF_MACRO_DEFINITION
* 
******************************************************************************/

#define WLF_API_VERSION  "1.10"

#define WLF_OK            0
#define WLF_ERROR         1
#define WLF_ERR           WLF_ERROR
#define WLF_LAST_DELTA    0x7fffffff

#include "mti.h"

typedef enum WlfRadixEnum_ {
    WLF_RADIX_SYMBOLIC,
    WLF_RADIX_DEFAULT,
    WLF_RADIX_BINARY,
    WLF_RADIX_OCTAL,
    WLF_RADIX_DECIMAL,
    WLF_RADIX_HEX,
    WLF_RADIX_ASCII,   /* for Verilog */
    WLF_RADIX_UNSIGNED
} WlfRadixEnum;

typedef enum WlfPropEnum_ {
    WLF_PROP_SYMBOL_NAME,               /* return: char *               */
    WLF_PROP_SYMBOL_FULLNAME,           /* return: char *               */
    WLF_PROP_LIBRARY_NAME,              /* return: char *               */
    WLF_PROP_PRIMARY_NAME,              /* return: char *               */
    WLF_PROP_SECONDARY_NAME,            /* return: char *               */
    WLF_PROP_SOURCE_NAME,               /* return: char *               */
    WLF_PROP_EXPRESSION,                /* return: char *               */ 

    WLF_PROP_SYMBOL_TYPE,               /* return: WlfSymbolSel         */
    WLF_PROP_PORT_TYPE,                 /* return: WlfModeSel           */
    WLF_PROP_HIGHER_REGION_SYMBOL_ID,   /* return: WlfSymbolId *        */
    WLF_PROP_PARENT_SYMBOL_ID,          /* return: WlfSymbolId *        */
    WLF_PROP_ARCHIVE_NUMBER,            /* return: int                  */
    WLF_PROP_TYPE_ID,                   /* return: WlfTypeId *          */
    WLF_PROP_SUBELEMENT_COUNT,          /* return: int                  */

    WLF_PROP_SYMBOL_ABSOLUTE_PATH,      /* return: char *               */
    WLF_PROP_SYMBOL_DATASET_NAME,       /* return: char *               */
    WLF_PROP_SYMBOL_PATH,               /* return: char *               */
    WLF_PROP_SYMBOL_ACCTYPE,            /* return: int                  */
    WLF_PROP_SYMBOL_SEL64,              /* return: WlfSymbolSel64       */
    WLF_PROP_ALIAS_SYMBOL_ID,           /* return: WlfSymbolId *        */
    WLF_PROP_SYMBOL_SOURCE              /* return: int                  */
} WlfPropEnum;

typedef enum WlfTypePropEnum_ {
    WLF_TYPE_TYPE,                      /* return: WlfDataType            */
    WLF_TYPE_ARRAY_RIGHT,               /* return: int                    */
    WLF_TYPE_ARRAY_LEFT,                /* return: int                    */
    WLF_TYPE_ARRAY_LENGTH,              /* return: int                    */
    WLF_TYPE_ARRAY_DIR,                 /* return: int                    */
    WLF_TYPE_ARRAY_ELEMENT_TYPE,        /* return: wlfDataType            */
    WLF_TYPE_ARRAY_ELEMENT_TYPE_ID,     /* return: wlfTypeId *            */
    WLF_TYPE_REGISTER_WIDTH,            /* return: int                    */
    WLF_TYPE_SUBELEMENT_COUNT,          /* return: int                    */
    WLF_TYPE_VALUE_SIZE,                /* return: int                    */
    WLF_TYPE_VLOG_SIGNED                /* return: int                    */
} WlfTypePropEnum;

typedef enum WlfFilePropEnum_ {
    WLF_FILE_PROP_INVALID = -1,         /* invalid                        */
    WLF_FILE_PROP_REGION_BEGIN,         /* return: char *                 */
    WLF_FILE_PROP_ARGC,                 /* return: int                    */
    WLF_FILE_PROP_ARGV,                 /* return: char *                 */
    WLF_FILE_PROP_REGION_END,           /* return: void                   */
    WLF_FILE_PROP_PARTITION,            /* return: int                    */
    WLF_FILE_PROP_SOURCE,               /* return: int                    */
    WLF_FILE_PROP_FILE_INFO,            /* return: WlfFileInfo *          */
    WLF_FILE_PROP_ARGV_STRING           /* return: char *                 */
} WlfFilePropEnum;

typedef enum WlfCallbackRequestEnum_ {
    WLF_REQUEST_IMMEDIATE,  /* callback occurs immed on prim value change */
    WLF_REQUEST_DELTA,      /* occurs at end of delta cycle */
    WLF_REQUEST_POSTPONED,  /* occurs at end of time step */
    WLF_REQUEST_NONE,       /* silent stuffing of the data values */
    WLF_REQUEST_CONDITIONAL /* POSTPONED if data ch, IMM if summary */
} WlfCallbackRequest;

typedef enum WlfCallbackReasonEnum_ {
    WLF_STARTLOG = 0x01,
    WLF_ENDLOG,
    WLF_EVENT,
    WLF_WAKEUP,
    WLF_STARTINSTANT,
    WLF_NOTIFYINSTANT,
    WLF_ENDINSTANT,
    WLF_SOURCEEOF,
    WLF_SEEKONLY,
    WLF_RECON_SYNC
} WlfCallbackReason;

typedef enum WlfCallbackResponseEnum_ {
    WLF_CONTINUE_SCAN,
    WLF_STOP_SCAN,
    WLF_ABORT_SCAN
} WlfCallbackResponse;

typedef enum WlfTimeModeEnum_ {
    WLF_TIMEMODE_ABSOLUTE,
    WLF_TIMEMODE_RELATIVE
} WlfTimeMode;

typedef enum WlfFileModeEnum_ {
    WLF_FILEMODE_UNKNOWN=0,
    WLF_FILEMODE_SIM,       /* current active simulation */
    WLF_FILEMODE_VIEW,      /* simulation logfile */
    WLF_FILEMODE_STREAM,    /* in-memory data stream */
    WLF_FILEMODE_VIRTUAL,   /* mix of sim and view */
    WLF_FILEMODE_COMPARE,   /* mix of sim, view and stream */
    WLF_FILEMODE_UCDB,      /* special ucdb view dataset */
    WLF_FILEMODE_VIEW_LITE, /* simulation logfile, parse only */
    WLF_FILEMODE_MONITOR    /* remote view of active simulation */
} WlfFileMode;

typedef enum WlfWrtLogModeEnum_ {
    WLFWRT_LOG_EVENT,       /* Used in wlfWrt...SetValue() functions */
    WLFWRT_LOG_START,
    WLFWRT_LOG_END
} WlfWrtLogMode;

typedef enum WlfPortModeEnum_ {
    WLF_PORTMODE_INTERNAL=0,
    WLF_PORTMODE_IN,
    WLF_PORTMODE_OUT,
    WLF_PORTMODE_INOUT
} WlfPortMode;

/******************************************************************************
*   Special flags used in search functions.
******************************************************************************/
#define WLF_FLAG_CHARCLASSGLOB  0x01

/* create a builtin 64-bit integer data type */
#if defined(_MSC_VER)
/* 64-bit integer builtin for Windows */
typedef __int64 WlfInt64;
# define LLDSTR "%I64d"
#elif !defined(__64BIT__)
# if defined(_AIX) && defined(__GNUC__)
    /* AIX alignment rules differ in gcc versus native cc */
    typedef long long WlfInt64 __attribute__((aligned(8)));
# else
    /* 64-bit integer builtin for 32-bit machines */
    typedef long long WlfInt64;
# endif
# define LLDSTR "%lld"
#else
   /* 64-bit integer builtin for 64-bit machines */
   typedef mtiLongT WlfInt64;
#  define LLDSTR "%ld"
#endif

#if defined(__64BIT__)

/* 64-bit scalar time type */

typedef mtiLongT WlfTime64;
#define WLF_TIME64_INIT(h,l)    ((mtiLongT)(h)<<32 | (unsigned int)(l))
#define WLF_TIME64_HI32(t)      ((int)((t)>>32))
#define WLF_TIME64_LO32(t)      ((unsigned int)(t))
#define WLF_TIME64_ASGN(t,h,l)  {(t) = (mtiLongT)(h)<<32 | (unsigned int)(l);}

#else

/* 64-bit scalar time type */
typedef WlfInt64 WlfTime64;
#define WLF_TIME64_INIT(h,l)    ((WlfTime64)(h)<<32 | (unsigned int)(l))
#define WLF_TIME64_HI32(t)      ((int)((t)>>32))
#define WLF_TIME64_LO32(t)      ((unsigned int)(t))
#define WLF_TIME64_ASGN(t,h,l)  {(t) = (WlfTime64)(h)<<32 | (unsigned int)(l);}

#endif

typedef struct WlfFileInfoStruct_ {
    WlfTime64   lastTime;
    int         lastDelta;
    int         signalCount;
    int         resolutionLimit;
    char        apiVersion[16];
    char        fileFormatVersion[16];
    char        productName[128];
    mtiLongT    creationTime;
    int         compressed;
    char        MTIVersion[16];
    char        platform[16];
    WlfTime64   startTime;
    int         startDelta;
    int         optimized;
    short       magic;
} WlfFileInfo;

typedef struct WlfPhysUnitRecStruct_ {
    char *units;
    int position;
} WlfPhysUnitRec;


typedef WlfInt64     WlfSymbolSel64;
typedef unsigned int WlfSymbolSel;          /* obsolete - use WlfSymbolSel64 */
typedef unsigned int WlfModeSel;
typedef unsigned int WlfDataType;
typedef unsigned int WlfSearchFlag;


/******************************************************************************
*   Values for type: WlfSymbolSel & WlfSymbolSel64
*   WlfSymbolSel supports 32-bits and may become obsolete.
*   Masks with greater than 32-bits MUST use WlfSymbolSel64.
*   Functions with a WlfSymbolSel64 argument have a "64" added to there
*   old WlfSymbolSel version.
******************************************************************************/
#define wlfSelNone              0
#define wlfSelRoot              0x00000001
#define wlfSelArchitecture      0x00000002
#define wlfSelBlock             0x00000004
#define wlfSelGenerate          0x00000008
#define wlfSelPackage           0x00000010
#define wlfSelForeign           0x00000020
#define wlfSelProcess           0x00000040
#define wlfSelVlGenerateBlock   0x00000080
#define wlfSelEPackage          wlfSelVlGenerateBlock
#define wlfSelSignal            0x00000100
#define wlfSelVariable          0x00000200
#define wlfSelConstant          0x00000400
#define wlfSelGeneric           0x00000800
#define wlfSelAlias             0x00001000
/*  Reserved                    0x00002000 */
#define wlfSelCompare           0x00004000
#define wlfSelModule            0x00008000
#define wlfSelPrimitive         0x00010000
#define wlfSelTask              0x00020000
#define wlfSelFunction          0x00040000
#define wlfSelStatement         0x00080000

#define wlfSelNet               0x00200000
#define wlfSelParameter         0x00400000
#define wlfSelReg               0x00800000
#define wlfSelInteger           0x01000000
#define wlfSelTime              0x02000000
#define wlfSelReal              0x04000000
#define wlfSelSpecparam         0x08000000
#define wlfSelMemory            0x10000000
#define wlfSelNamedEvent        0x20000000

#define wlfSelVirtualRegion     0x00100000
#define wlfSelVirtualSignal     0x40000000
#define wlfSelVirtualExpr       0x80000000

/* must use WlfSymbolSel64 */
/* LL needed here for HP cc 32-bit compiles */
#ifndef WIN32
#define wlfSelScModule          0x0000000100000000LL
#define wlfSelScPrimChannel     0x0000000200000000LL
#define wlfSelAssertion         0x0000000400000000LL
#define wlfSelVlConfig          0x0000000800000000LL

#define wlfSelCover             0x0000001000000000LL
#define wlfSelEndpoint          0x0000002000000000LL
#define wlfSelScVariable        0x0000004000000000LL
#define wlfSelScPort            0x0000008000000000LL

#define wlfSelScMethod          0x0000010000000000LL
#define wlfSelScThread          0x0000020000000000LL
#define wlfSelSVClass           0x0000040000000000LL
#define wlfSelSVCross           0x0000080000000000LL

#define wlfSelSVCovergroup      0x0000100000000000LL
#define wlfSelSVCoverpoint      0x0000200000000000LL
#define wlfSelTransaction       0x0000400000000000LL
#define wlfSelScExport          0x0000800000000000LL

#define wlfSelVlPackage         0x0001000000000000LL
#define wlfSelVlProgram         0x0002000000000000LL
#define wlfSelVlTypedef         0x0004000000000000LL
#define wlfSelSVParamClass      0x0008000000000000LL

#define wlfSelCellInternal      0x0010000000000000LL
#define wlfSelClockingBlock     0x0020000000000000LL

#define wlfSelTaThreadMon       0x0100000000000000LL
#define wlfSelSVInterface       0x0200000000000000LL

#define wlfSelImmediateAssert   0x1000000000000000LL
#define wlfSelAssertionScope    0x2000000000000000LL

#else

#define wlfSelScModule          0x0000000100000000
#define wlfSelScPrimChannel     0x0000000200000000
#define wlfSelAssertion         0x0000000400000000
#define wlfSelVlConfig          0x0000000800000000

#define wlfSelCover             0x0000001000000000
#define wlfSelEndpoint          0x0000002000000000
#define wlfSelScVariable        0x0000004000000000
#define wlfSelScPort            0x0000008000000000

#define wlfSelScMethod          0x0000010000000000
#define wlfSelScThread          0x0000020000000000
#define wlfSelSVClass           0x0000040000000000
#define wlfSelSVCross           0x0000080000000000

#define wlfSelSVCovergroup      0x0000100000000000
#define wlfSelSVCoverpoint      0x0000200000000000
#define wlfSelTransaction       0x0000400000000000
#define wlfSelScExport          0x0000800000000000

#define wlfSelVlPackage         0x0001000000000000
#define wlfSelVlProgram         0x0002000000000000
#define wlfSelVlTypedef         0x0004000000000000
#define wlfSelSVParamClass      0x0008000000000000

#define wlfSelCellInternal      0x0010000000000000
#define wlfSelClockingBlock     0x0020000000000000

#define wlfSelTaThreadMon       0x0100000000000000
#define wlfSelSVInterface       0x0200000000000000

#define wlfSelImmediateAssert   0x1000000000000000
#define wlfSelAssertionScope    0x2000000000000000

#endif


// Temporary definition to support MVC & wlfProbe for 10.0x.
#define wlfSelTrStreams         wlfSelTransaction


/* Commonly grouped selections */
#define wlfSelVhdlScopes        ( wlfSelArchitecture |      \
                                  wlfSelBlock |             \
                                  wlfSelGenerate |          \
                                  wlfSelPackage |           \
                                  wlfSelForeign )

#define wlfSelVlogScopes        ( wlfSelModule |            \
                                  wlfSelTask |              \
                                  wlfSelBlock |             \
                                  wlfSelFunction |          \
                                  wlfSelStatement |         \
                                  wlfSelSVCovergroup |      \
                                  wlfSelSVCoverpoint |      \
                                  wlfSelSVCross |           \
                                  wlfSelSVClass |           \
                                  wlfSelSVParamClass |      \
                                  wlfSelSVInterface |       \
                                  wlfSelVlPackage |         \
                                  wlfSelVlGenerateBlock |   \
                                  wlfSelAssertionScope |    \
                                  wlfSelClockingBlock |     \
                                  wlfSelVlTypedef )

#define wlfSelVlogVars          ( wlfSelParameter |         \
                                  wlfSelReg |               \
                                  wlfSelInteger |           \
                                  wlfSelTime |              \
                                  wlfSelReal |              \
                                  wlfSelSpecparam |         \
                                  wlfSelMemory |            \
                                  wlfSelNamedEvent )

#define wlfSelHdlScopes         ( wlfSelVhdlScopes | wlfSelVlogScopes | wlfSelEPackage )

#define wlfSelVirtualSignals    ( wlfSelVirtualSignal | wlfSelVirtualExpr )

#define wlfSelVirtual           ( wlfSelVirtualRegion | wlfSelVirtualSignals )

#define wlfSelHdlSignals        ( wlfSelSignal |            \
                                  wlfSelNet |               \
                                  wlfSelVlogVars |          \
                                  wlfSelVirtualSignals )

#define wlfSelHdlVars           ( wlfSelVariable |          \
                                  wlfSelConstant |          \
                                  wlfSelGeneric |           \
                                  wlfSelAlias |             \
                                  wlfSelVlogVars )

#define wlfSelHdlSignalsVars    ( wlfSelHdlSignals | wlfSelHdlVars )

#define wlfSelHdlAll            ( wlfSelHdlScopes |         \
                                  wlfSelHdlSignals |        \
                                  wlfSelVariable ) 

/* must use WlfSymbolSel64 */
#define wlfSelAllScopes         ( wlfSelHdlScopes |         \
                                  wlfSelEPackage |          \
                                  wlfSelVirtualRegion |     \
                                  wlfSelVirtualRegion |     \
                                  wlfSelScModule)

/* must use WlfSymbolSel64 */
#define wlfSelAllSignals        ( wlfSelHdlSignals |        \
                                  wlfSelVirtualSignals |    \
                                  wlfSelScPrimChannel |     \
                                  wlfSelScPort |            \
                                  wlfSelScExport |          \
                                  wlfSelAssertion |         \
                                  wlfSelCover |             \
                                  wlfSelImmediateAssert |   \
                                  wlfSelTransaction |       \
                                  wlfSelEndpoint)

/* must use WlfSymbolSel64 */
#define wlfSelAllSignalsVars    ( wlfSelHdlSignals |        \
                                  wlfSelHdlVars |           \
                                  wlfSelVirtualSignals |    \
                                  wlfSelScPrimChannel |     \
                                  wlfSelScPort |            \
                                  wlfSelScExport |          \
                                  wlfSelScVariable |        \
                                  wlfSelAssertion |         \
                                  wlfSelCover |             \
                                  wlfSelImmediateAssert |   \
                                  wlfSelTransaction |       \
                                  wlfSelCellInternal |      \
                                  wlfSelEndpoint)

#define wlfSelAllConstants      ( wlfSelConstant |          \
                                  wlfSelGeneric |           \
                                  wlfSelParameter |         \
                                  wlfSelSpecparam )

#define wlfSelVlog              ( wlfSelVlogScopes |        \
                                  wlfSelVlogVars |          \
                                  wlfSelNet |               \
                                  wlfSelPrimitive )

#define wlfSelVhdl              ( wlfSelVhdlScopes |        \
                                  wlfSelProcess |           \
                                  wlfSelSignal |            \
                                  wlfSelVariable |          \
                                  wlfSelConstant |          \
                                  wlfSelGeneric |           \
                                  wlfSelAlias )

#define wlfSelScProcess         (wlfSelScThread |           \
                                 wlfSelScMethod)

/* must use WlfSymbolSel64 */
#define wlfSelSystemc           ( wlfSelScModule |          \
                                  wlfSelScProcess |         \
                                  wlfSelScPrimChannel |     \
                                  wlfSelScPort |            \
                                  wlfSelScExport |          \
                                  wlfSelScVariable)

#define wlfSelAll               ( wlfSelVhdl |              \
                                  wlfSelVlog |              \
                                  wlfSelVirtual |           \
                                  wlfSelSystemc |           \
                                  wlfSelAssertion |         \
                                  wlfSelCover |             \
                                  wlfSelImmediateAssert |   \
                                  wlfSelTransaction |       \
                                  wlfSelEPackage |          \
                                  wlfSelEndpoint)

 
/******************************************************************************
*   Values for type: WlfModeSel 
******************************************************************************/
#define wlfModeSelNone          0x0000
#define wlfModeSelInputs        0x0001
#define wlfModeSelOutputs       0x0002
#define wlfModeSelInouts        0x0004
#define wlfModeSelInternals     0x0008
#define wlfModeSelCells         0x0010
#define wlfModeSelPorts         (wlfModeSelInputs | wlfModeSelOutputs | wlfModeSelInouts)
#define wlfModeSelAll           (wlfModeSelPorts | wlfModeSelInternals)


/******************************************************************************
*   WlfSearchFlag values (bit vector, non-exclusive elements can be ORed together)
******************************************************************************/
#define wlfSearchAnyEdge        0x0000      /* default */
#define wlfSearchRisingEdge     0x0001
#define wlfSearchFallingEdge    0x0002

#define wlfSearchForward        0x0000      /* default */
#define wlfSearchReverse        0x0004

#define wlfSearchNoDeltas       0x0000      /* default */
#define wlfSearchDeltas         0x0008

#define wlfSearchMsgs           0x0010
#define wlfSearchFirstEvent     0x0020


/******************************************************************************
*   Values for type: WlfDataType
*   WlfDataType values enumerate the type of WlfTypeId.
******************************************************************************/
/********** VHDL ********/
#define wlfTypeScalar           0
#define wlfTypeArray            1
#define wlfTypeRecord           2
#define wlfTypeEnum             3
#define wlfTypeInteger          4
#define wlfTypePhysical         5
#define wlfTypeReal             6
#define wlfTypeAccess           7
#define wlfTypeFile             8
#define wlfTypeTime             9

/******* Verilog ********/
#define wlfTypeReg              10
#define wlfTypeNet              11
#define wlfTypeBit              12
#define wlfTypeMemElem          13
#define wlfTypeMemBits          14
#define wlfTypeVlogReal         15

#define wlfTypeVlogEnum         19

#define wlfTypeEvent            21 
#define wlfTypeClassRef         24 

#define wlfTypeString           201

#define wlfTypeUnknown          999

/******************************************************************************
*   WLF IDs
******************************************************************************/
#ifndef WLFIMP_H

typedef struct WlfPackIdStruct_   { char data; } *WlfPackId;
typedef struct WlfFileIdStruct_   { char data; } *WlfFileId;
typedef struct WlfWrtFileIdStruct_{ char data; } *WlfWrtFileId;
typedef struct WlfWrtChannelIdStruct_{ char data; } *WlfWrtChannelId;
typedef struct WlfSymbolIdStruct_ { char data; } *WlfSymbolId;
typedef struct WlfIterIdStruct_   { char data; } *WlfIterId;
typedef struct WlfValueIdStruct_  { char data; } *WlfValueId;
typedef struct WlfTypeIdStruct_   { char data; } *WlfTypeId;

typedef struct WlfSymPStruct_     { char data; } *WlfSymP;

#endif


/******************************************************************************
*   WLF Initialize and cleanup.
******************************************************************************/
extern int            wlfInit(void);
extern int            wlfCleanup(void);
extern void           wlfExit( int exit_code );


/******************************************************************************
*   WLF Pack Functions
******************************************************************************/
extern WlfPackId      wlfPackCreate(void);
extern int            wlfPackDestroy(WlfPackId packId);
extern int            wlfPackReset(WlfPackId packId);


/******************************************************************************
*   WLF File Functions
* 
*   Special WlfFileIds:
* 
*   wlfSimFileId        Automatic file Id associated with simulation.
*   wlfVirtualFileId    Automatic file Id associated with virtual context.
*                       (analogy: stdin, stdout) Cannot be opened or closed.
******************************************************************************/
extern WlfFileId     wlfSimFileId;
extern WlfFileId     wlfVirtualFileId;

extern WlfFileId     wlfFileOpen       (char *fname, char* lname );
extern int           wlfFileClose      (WlfFileId fp);
extern int           wlfFileNumSigs    (WlfFileId fp, int *num);
extern int           wlfFileNumTypes   (WlfFileId fp, int *num);
extern int           wlfFileNumFiles   (WlfFileId fp, int *num);
extern int           wlfFileResolution (WlfFileId fp, int *resolution);
extern int           wlfFileEndTime    (WlfFileId fp, WlfTime64 *endtime);
extern int           wlfFileEndDelta   (WlfFileId fp, int *delta);
extern int           wlfFileInfo       (WlfFileId fp, WlfFileInfo *buf);
extern int           wlfFileHeader     (char *filename, WlfFileInfo *buf);

extern char         *wlfFileGetName( WlfFileId fileId );
extern char         *wlfFileGetLogicalName( WlfFileId fileId );
extern char         *wlfFileSetLogicalName( WlfFileId fileId, char *new_name );

extern int           wlfFileCount( void );
extern WlfFileId     wlfFileFind( char *logical_name );

extern WlfSymbolId   wlfFileGetTopRegion( WlfFileId fileId );
extern WlfSymbolId   wlfFileGetTopContext( WlfFileId fileId );
extern WlfSymbolId   wlfFileGetCurrentContext( WlfFileId fileId );
extern WlfSymbolId   wlfFileSetCurrentContext( WlfSymbolId context );



/******************************************************************************
*   WLF Iterator Functions
******************************************************************************/
extern WlfSymbolId   wlfIterate        (WlfIterId id);
extern int           wlfIteratorReset  (WlfIterId id);
extern int           wlfIteratorDestroy(WlfIterId id);
extern int           wlfIteratorCount  (WlfIterId id);


/******************************************************************************
*   WLF Value Functions
* 
*   WARNING: WlfValueIds retrieved with wlfValueGet are internal Ids which
*            *WILL* be deallocated upon a wlfPackReset() operation.
*            The application is responsible for ensuring these Ids are not
*            used following the wlfPackReset().
* 
*   User WlfValueIds created with wlfValueCreate() must be explicitly 
*   deallocated with wlfValueDestroy() and are not deallocated as part of
*   wlfPackReset().
* 
******************************************************************************/
extern WlfValueId wlfValueGet(
                        WlfPackId      packId,
                        WlfSymbolId    signalId );
                
extern int        wlfValueCompare(
                        WlfValueId     value1,
                        WlfValueId     value2 );

extern char      *wlfValueToString(
                        WlfValueId     id,
                        WlfRadixEnum   radix,
                        int            maxWidth );

extern void      *wlfValueGetValue(
                        WlfValueId     id);

struct type_descript;
extern WlfValueId wlfValueTypedCreate (
                        struct type_descript * td );

extern WlfValueId wlfValueCreate(
                        WlfSymbolId    signalId );

extern int        wlfValueDestroy (
                        WlfValueId     valueId );

extern int        wlfValueCopy(
                        WlfValueId     valueTo,
                        WlfValueId     valueFrom );

extern int        wlfValueCopyToSymbol( 
                        WlfValueId     valueFrom,
                        WlfSymbolId    symbolTo ); 

/******************************************************************************
*   WLF Symbol retrieval functions.
*   NOTE:   New functions added in 5.8 to support 64-bit wlfSymbolSel64 type.
*           These new function have a '64' appended to their names.
******************************************************************************/
extern int          wlfSymIsSymbolSelect( 
                        WlfSymbolId     symId, 
                        WlfSymbolSel    symSelect );

extern int          wlfSymIsSymbolSelect64( 
                        WlfSymbolId     symId, 
                        WlfSymbolSel64  symSelect );

extern int          wlfSymIsModeSelect( 
                        WlfSymbolId     symId, 
                        WlfModeSel      selmode );

extern WlfFileId    wlfSymGetFile( 
                        WlfSymbolId     symId );

extern WlfIterId    wlfSymChildren(
                        WlfSymbolId     parent,
                        WlfSymbolSel    symSelect );

extern WlfIterId    wlfSymChildren64(
                        WlfSymbolId     parent,
                        WlfSymbolSel64  symSelect );

extern WlfIterId    wlfSymSignals(
                        WlfSymbolId     parentScope,
                        WlfSymbolSel    symSelect ,
                        WlfModeSel      modeSelect );

extern WlfIterId    wlfSymSignals64(
                        WlfSymbolId     parentScope,
                        WlfSymbolSel64  symSelect ,
                        WlfModeSel      modeSelect );

extern WlfIterId    wlfSymSubelements( 
                        WlfSymbolId     signalId );

extern WlfIterId    wlfSymSubelementsFiltered( 
                        WlfSymbolId     signalId,
                        WlfSymbolSel64 acc_filter);

extern WlfIterId    wlfSymSiblings(
                        WlfSymbolId     symbolId,
                        WlfSymbolSel    symSelect );

extern WlfIterId    wlfSymSiblings64(
                        WlfSymbolId     symbolId,
                        WlfSymbolSel64  symSelect );

extern WlfIterId    wlfSymProcesses( 
                        WlfSymbolId     scope );

extern WlfIterId    wlfSymByPattern(
                        char           *pattern,
                        WlfSymbolId     refScope,
                        WlfSymbolSel    symSelect, 
                        WlfModeSel      modeSelect,
                        int             recursive );

extern WlfIterId    wlfSymByPattern64(
                        char           *pattern,
                        WlfSymbolId     refScope,
                        WlfSymbolSel64  symSelect, 
                        WlfModeSel      modeSelect,
                        int             recursive );

extern WlfIterId    wlfSymByPatternSpecial(
                        char           *pattern,
                        WlfSymbolId     refScope,
                        WlfSymbolSel    symSelect, 
                        WlfModeSel      modeSelect,
                        int             recursive,
                        int             flags );

extern WlfIterId    wlfSymByPatternSpecial64(
                        char           *pattern,
                        WlfSymbolId     refScope,
                        WlfSymbolSel64  symSelect, 
                        WlfModeSel      modeSelect,
                        int             recursive,
                        int             flags );

extern WlfSymbolId  wlfSymByName(
                        char           *name,
                        WlfSymbolId     refScope );

extern int          wlfSymMatchesPattern(
                        WlfSymbolId     sym,
                        char           *pattern,
                        WlfSymbolId     refScope,
                        WlfSymbolSel64  symSelect, 
                        WlfModeSel      modeSelect,
                        int             recursive );

extern int          wlfSymCountSubelements(    
                        WlfSymbolId     parent );

extern void         wlfSymMapSymbol( WlfSymbolId dst, WlfSymbolId src, int filenumOffset, int anOffset );


/******************************************************************************
*   Symbol and type property functions.
******************************************************************************/
extern char        *wlfSymPropString    ( WlfSymbolId symId, WlfPropEnum p);
extern int          wlfSymPropInt       ( WlfSymbolId symId, WlfPropEnum p);
extern WlfSymbolSel wlfSymPropSymbolSel ( WlfSymbolId symId, WlfPropEnum p);
extern WlfSymbolId  wlfSymPropSymbolId  ( WlfSymbolId symId, WlfPropEnum p);
extern WlfTypeId    wlfSymPropTypeId    ( WlfSymbolId symId, WlfPropEnum p);
extern WlfModeSel   wlfSymPropModeSel   ( WlfSymbolId symId, WlfPropEnum p);
extern WlfSymbolSel64 wlfSymPropSymbolSel64 ( WlfSymbolId symId, WlfPropEnum p);

extern int          wlfWrtSymPropInt    ( WlfSymbolId symId, WlfPropEnum p, int val);

extern int          wlfTypePropInt      ( WlfTypeId typeId, WlfTypePropEnum p);
extern WlfDataType  wlfTypePropDataType ( WlfTypeId typeId, WlfTypePropEnum p);
extern WlfTypeId    wlfTypePropTypeId   ( WlfTypeId typeId, WlfTypePropEnum p);

/* Set: each property/value pair is saved */
extern int          wlfWrtFileProp      ( WlfWrtFileId fileId, WlfFilePropEnum p); /* void val*/
extern int          wlfWrtFilePropString( WlfWrtFileId fileId, WlfFilePropEnum p, char *val);
extern int          wlfWrtFilePropInt   ( WlfWrtFileId fileId, WlfFilePropEnum p, int val);
extern int          wlfWrtFilePropFileInfo( WlfWrtFileId fileId, WlfFilePropEnum p, 
                                                    WlfFileInfo *);
/* utility, outputs argv using either string or string per arg */
extern int          wlfWrtFilePropArgv   ( WlfWrtFileId fileId, int argc, char **argv);

/*
 Get: Count returns # of properties, rest are indexed into list
 Kind tells the property kind at index, 
 Int/String returns value
*/
extern int          wlfFilePropCount    ( WlfFileId fileId);
extern WlfFilePropEnum wlfFilePropKind  ( WlfFileId fileId, int index);
extern char        *wlfFilePropString   ( WlfFileId fileId, int index);
extern int          wlfFilePropInt      ( WlfFileId fileId, int index);
extern WlfFileInfo *wlfFilePropFileInfo ( WlfFileId fileId, int index);


extern int          wlfEnumLiterals(
                        WlfTypeId           enumTypeId,
                        char             ***enumLiterals,
                        int                *count );

extern int          wlfPhysUnits(
                        WlfTypeId           physTypeId,
                        WlfPhysUnitRec    **physUnits,
                        int                *count );


/******************************************************************************
*   Typedefs for application registered callback functions.
******************************************************************************/
typedef WlfCallbackResponse (*WlfSignalEventCB)( 
                        void                *cbData, 
                        WlfCallbackReason    reason );

typedef WlfCallbackResponse (*WlfWakeupCB)( 
                        void                *cbData,
                        WlfTime64            newTime,
                        int                  newDelta );

typedef WlfCallbackResponse (*WlfTimeAdvanceCB)(
                        void                *cbData,
                        WlfTime64            newTime, 
                        int                  newDelta );

typedef WlfCallbackResponse (*WlfDeltaAdvanceCB)(  
                        void                *cbData,
                        WlfTime64            newTime, 
                        int                  newDelta );

typedef WlfCallbackResponse (*WlfMessageCB)(          /*Used in examine*/
                        WlfTime64            newTime, 
                        int                  newDelta,
                        int                  msg_num,
                        int                  msg_id,
                        char                *mfs,
                        char                *afs,
                        void               **value,
                        void                *cbData);


/******************************************************************************
 *  Registering Callbacks and retrieving signal data.
******************************************************************************/
extern int wlfAddSignalEventCB(
                        WlfPackId            packId,
                        WlfSymbolId          signalId,
                        WlfValueId           valueId,
                        WlfCallbackRequest   request,
                        WlfSignalEventCB     cbFunc,
                        void                *cbData );

extern int wlfAddDelayedSignalEventCB(
                        WlfPackId            packId,
                        WlfSymbolId          signalId,
                        WlfValueId           valueId,
                        WlfCallbackRequest   request,
                        WlfSignalEventCB     cbFunc,
                        void                *cbData,
                        WlfTime64            timeOffset ); /* positive looks forward in time */

extern int wlfAppendSignalEventCB(
                        WlfPackId            packId,
                        WlfSymbolId          symId,
                        WlfValueId           valueId,
                        WlfCallbackRequest   request,
                        WlfSignalEventCB     cbFunc,
                        void                *cbData,
                        void               **cbPtr );   /* uses existing callback, or if NULL, creates one */

extern int wlfAppendDelayedSignalEventCB(
                        WlfPackId            packId,
                        WlfSymbolId          symId,
                        WlfValueId           valueId,
                        WlfCallbackRequest   request,
                        WlfSignalEventCB     cbFunc,
                        void                *cbData,
                        void               **cbPtr,         /* uses existing callback, or if NULL, creates one */
                        WlfTime64            timeOffset );  /* positive looks forward in time */

extern int wlfAddWakeupCB(
                        WlfPackId            packId,
                        WlfTime64            cbtime,
                        WlfTimeMode          timeMode,
                        WlfWakeupCB          cbFunc,
                        void                *cbData );

extern int wlfAddTimeDeltaCB(
                        WlfPackId            packId,
                        WlfTimeAdvanceCB     timeCB,
                        WlfDeltaAdvanceCB    deltaCB,
                        void *               cb_data );

extern int wlfAddPreTimeDeltaCB(
                        WlfPackId            packId,
                        WlfTimeAdvanceCB     timeCB,
                        WlfDeltaAdvanceCB    deltaCB,
                        void *               cb_data );

extern int wlfReadDataOverRange(
                        WlfPackId            packId,
                        WlfTime64            startTime,
                        int                  startDelta,        
                        WlfTime64            endTime, 
                        int                  endDelta, 
                        WlfTimeAdvanceCB     timeAdvCB,
                        WlfDeltaAdvanceCB    deltaAdvCB,
                        void                *cb_data );

extern int wlfReadMsgs(
                        WlfPackId            packId,
                        WlfTimeAdvanceCB     timeAdvCB,
                        WlfDeltaAdvanceCB    deltaAdvCB);

extern int wlfReadMsgsOverRange(
                        WlfPackId            packId,
                        WlfTime64            startTime,
                        int                  startDelta,        
                        WlfTime64            endTime, 
                        int                  endDelta, 
                        WlfTimeAdvanceCB     timeAdvCB,
                        WlfDeltaAdvanceCB    deltaAdvCB);

extern int wlfAddMsgCB(
                        WlfPackId            packId,
                        WlfFileId            wlf_file,     
                        WlfMessageCB         messageCB,
                        void                 *cb_data);

extern int wlfReadDataAtInstant(
                        WlfPackId            packId,
                        WlfTime64            packtime,
                        int                  packdelta );

extern char *wlfCallbackReasonToString(
                         WlfCallbackReason reason );


extern char *wlfCallbackResponseToString(
                         WlfCallbackResponse response );


extern char *wlfCallbackRequestToString(
                         WlfCallbackRequest request );


/******************************************************************************
*   Routines for handling lists of WlfSymbolId
******************************************************************************/

typedef struct WlfSymbolListStruct_  WlfSymbolList, *WlfSymbolListId;

struct WlfSymbolListStruct_ {
    WlfSymbolId      symbolId;       
    void            *data;
    char            *name;  /*Used when name of symbolId is different (i.e. class "@1" symbols) */
    WlfSymbolListId  next;       
};

extern WlfSymbolListId  wlfSymbolListAppend( 
                                   WlfSymbolListId      last, 
                                   WlfSymbolId          symbolId, 
                                   void                 *data );

extern void             wlfSymbolListInsert( 
                                   WlfSymbolListId      *firstPtr, 
                                   WlfSymbolId          symbolId, 
                                   void                 *data);

extern void             wlfSymbolListDelete( 
                                   WlfSymbolListId      head );

extern int              wlfSymbolListInit( 
                                   WlfSymbolListId      *firstPtr, 
                                   WlfSymbolListId      *lastPtr, 
                                   WlfSymbolId          symbolId, 
                                   void                 *data );


/******************************************************************************
*   Search Functions.
******************************************************************************/

extern int wlfSearchForMessages(  
                                   WlfPackId            packId, 
                                   WlfTime64            startTime, 
                                   int                  startDelta,
                                   int                  count,
                                   int                  flags,
                                   WlfTime64            *resultTimePtr,
                                   int                  *resultDeltaPtr,
                                   int                  *resultCountPtr,
                                   WlfTime64            *endTimePtr );

extern int wlfSearchForSignalEvent(  
                                   WlfPackId            packId, 
                                   WlfSymbolId          signalId,
                                   WlfValueId           valueId,
                                   WlfTime64            startTime, 
                                   int                  startDelta,
                                   int                  count,
                                   int                  flags,
                                   WlfTime64            *resultTimePtr,
                                   int                  *resultDeltaPtr,
                                   int                  *resultCountPtr,
                                   WlfTime64            *endTimePtr );

extern int wlfSearchForSignalValue(  
                                   WlfPackId            packId, 
                                   WlfSymbolId          symbolId,
                                   WlfValueId           valueId,
                                   char                 *valueString,
                                   WlfTime64            startTime, 
                                   int                  startDelta,
                                   int                  count,
                                   int                  flags,
                                   WlfTime64            *resultTimePtr,
                                   int                  *resultDeltaPtr,
                                   int                  *resultCountPtr,
                                   WlfTime64            *endTimePtr );

extern int wlfSearchForCondition(        
                                   WlfPackId            packId, 
                                   char                 *exprString,
                                   WlfTime64            startTime, 
                                   int                  startDelta,
                                   int                  count,
                                   int                  flags,
                                   WlfTime64            *resultTimePtr,
                                   int                  *resultDeltaPtr,
                                   int                  *resultCountPtr,
                                   WlfTime64            *endTimePtr );

extern int wlfSearchForGroupEvents(  
                                   WlfPackId            packId, 
                                   WlfSymbolListId      symbolListId,
                                   WlfTime64            startTime, 
                                   int                  startDelta,
                                   int                  count,
                                   int                  flags,
                                   WlfTime64            *resultTimePtr,
                                   int                  *resultDeltaPtr,
                                   int                  *resultCountPtr,
                                   WlfTime64            *endTimePtr );


/******************************************************************************
*   Time Functions
******************************************************************************/
extern int  wlfPackTime( WlfPackId packId, WlfTime64 *crnttime );
extern int  wlfPackDelta( WlfPackId packId, int *delta );
extern int  wlfPackEndTime( WlfPackId packId, WlfTime64 *endtime );
extern int  wlfPackEndDelta( WlfPackId packId, int *delta );


/******************************************************************************
*   Error Retrieval
******************************************************************************/
extern char *wlfErrorMsg( void );
extern int   wlfErrorNum( void );
extern int   wlfErrorInfo( int *errorNum, char *errorMsg );


/******************************************************************************
*   MISC Routines
******************************************************************************/
extern int   wlfPrintf( char *fmt, ... );
extern int   wlfCompareSymbols( WlfSymbolId obj1, WlfSymbolId obj2 );
extern int   wlfCompareTypes( WlfTypeId td1, WlfTypeId td2 );
extern char *wlfTime64ToString( char* buf,  WlfTime64 time64);
extern char *wlfGetFileDivider( void );
extern char *wlfSetFileDivider( char *new_divider);
extern char *wlfGetPathSeparator( void );
extern char *wlfSetPathSeparator( char *new_separator);
extern int  wlfPackIsOptSource( WlfPackId packId );

/******************************************************************************
*   WlfWrite Interface 
******************************************************************************/

/* wlfWrtFileCreate() flags */
#define WLF_FILE_COMPRESSED    0x0001
#define WLF_FILE_OPTIMIZED     0x0002
#define WLF_FILE_INDEXED       0x0004

extern WlfWrtFileId wlfWrtFileCreate( char *filename, int resolution, int flags );
extern void         wlfWrtFileFlush( WlfWrtFileId fp );
extern int          wlfWrtFileClose( WlfWrtFileId fp, WlfTime64 endtime );
extern WlfSymbolId  wlfWrtCreateRegion( WlfSymbolId parent, int type, char *name, char *lib, char *primary, char *secondary);
extern WlfSymbolId  wlfWrtCreateProcess( WlfSymbolId parent, int type, char *name );
extern WlfSymbolId  wlfWrtCreateSignal( WlfSymbolId parent, char *name, WlfTypeId type, WlfPortMode portmode, void **value);
extern WlfSymbolId  wlfWrtCreateSignalWithArchiveNumbers( WlfSymbolId parent, char *name, WlfTypeId type, WlfPortMode portmode, void **value, int slot_count, int *an_array, int an_offset);
extern WlfSymbolId  wlfWrtCreateVariableWithArchiveNumbers( WlfSymbolId parent, char *name, WlfTypeId type, WlfPortMode portmode, void **value, int slot_count, int *an_array, int an_offset);
extern WlfSymbolId  wlfWrtCreateAssertion( WlfSymbolId parent, char *name, char *constituents);
extern void         wlfWrtCopyAssertionExtensions( WlfSymbolId fromObj, WlfSymbolId toObj);

extern WlfSymbolId  wlfWrtCreateVariable( WlfSymbolId parent, char *name, WlfTypeId type, WlfPortMode portmode, void **value);
extern WlfSymbolId  wlfWrtCreateSignalAlias( WlfSymbolId parent, WlfSymbolId actual_obj, char *name, WlfTypeId type);
extern WlfSymbolId  wlfWrtCreateVerilogSignal( WlfSymbolId parent, char *name, WlfTypeId type, void **value);
extern WlfSymbolId  wlfWrtGetRoot( WlfWrtFileId fp);
extern WlfWrtFileId wlfWrtGetFileId( WlfSymbolId obj);
extern WlfWrtChannelId wlfWrtGetChannelId( WlfWrtFileId fp);
extern int          wlfWrtGetType( WlfSymbolId obj);
extern char *       wlfWrtGetName( WlfSymbolId obj);

extern void         wlfWrtCopyFileInfo( WlfSymbolId parent, WlfSymbolId dst, WlfSymbolId src);
extern void         wlfWrtCopyDu( WlfSymbolId parent, WlfSymbolId dst, WlfSymbolId src);
extern WlfSymbolId  wlfWrtCreateSymbolFromSymbol( WlfSymbolId parent, WlfSymbolId src);

extern int wlfWrtTimeAdvance( WlfWrtFileId fp, WlfTime64 newtime, unsigned int iteration );

/******************************************************************************
*   WlfWrite - Object creation functions.
******************************************************************************/
extern WlfTypeId    wlfWrtCreateEnumType( WlfWrtFileId fp, unsigned num, char **enums );
extern WlfTypeId    wlfWrtCreateArrayType( WlfWrtFileId fp, int left, int right, WlfTypeId elem);
extern WlfTypeId    wlfWrtCreateMemelemType( WlfWrtFileId fp, int left, int right);
extern WlfTypeId    wlfWrtCreateRecordType( WlfWrtFileId fp, unsigned num, char **names, WlfTypeId *types);
extern WlfTypeId    wlfWrtCreateRegType( WlfWrtFileId fp, int left, int right);
extern WlfTypeId    wlfWrtCreateRegSignedType( WlfWrtFileId fp, int left, int right);
extern WlfTypeId    wlfWrtCreateRealType( WlfWrtFileId fp, double left, double right);
extern WlfTypeId    wlfWrtCreateNetType( WlfWrtFileId fp, int left, int right, int flags);
extern WlfTypeId    wlfWrtCreateScalarType( WlfWrtFileId fp, int left, int right);
extern WlfTypeId    wlfWrtRegisterType( WlfWrtFileId fp, WlfSymbolId obj);

/******************************************************************************
*   WlfWrite - Setting and logging object value functions.
******************************************************************************/
extern int          wlfWrtGetArchiveNumber( WlfSymbolId obj, int index);
extern int          wlfWrtSetArchiveNumber( WlfSymbolId obj, int index, int num );
extern int          wlfWrtGetSlotCount( WlfSymbolId obj );
extern int *        wlfWrtGetArchiveNumberArray( WlfSymbolId obj );

extern int wlfWrtEnumSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int val );
extern int wlfWrtArraySetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int val, int index );
extern int wlfWrtCompositeListSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int val, int depth, int *elements );
extern int wlfWrtCompositeSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int val, int depth, ... );
extern int wlfWrtRegSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int index, unsigned int val, unsigned int unk );
extern int wlfWrtRealSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int index, double val );
extern int wlfWrtNetSetValue( WlfSymbolId symbol, WlfWrtLogMode flag, int index, int val );

extern int wlfWrtRegLogValue( WlfSymbolId symbol, WlfWrtLogMode flag, char *val );
extern int wlfWrtMemLogValue( WlfSymbolId symbol, WlfWrtLogMode flag );
extern int wlfWrtSymbolLogValue( WlfSymbolId symbol, WlfWrtLogMode flag, WlfValueId newValue );

/**************************************************
*  Time Resolution used with wlfSetResolution
**************************************************/
#define WLF_TIME_RES_1FS     0
#define WLF_TIME_RES_10FS    1
#define WLF_TIME_RES_100FS   2
#define WLF_TIME_RES_1PS     3
#define WLF_TIME_RES_10PS    4
#define WLF_TIME_RES_100PS   5
#define WLF_TIME_RES_1NS     6
#define WLF_TIME_RES_10NS    7
#define WLF_TIME_RES_100NS   8
#define WLF_TIME_RES_1US     9
#define WLF_TIME_RES_10US   10
#define WLF_TIME_RES_100US  11
#define WLF_TIME_RES_1MS    12
#define WLF_TIME_RES_10MS   13
#define WLF_TIME_RES_100MS  14
#define WLF_TIME_RES_1SEC   15
#define WLF_TIME_RES_10SEC  16
#define WLF_TIME_RES_100SEC 17

#include "acc_user.h"
#include "acc_vhdl.h"

#ifdef __cplusplus
}
#endif

/*
*   Functions/Macros maintained for backwards compatibility
*   These functions/macros should not be used as they may go away in the future.
*/
extern WlfSymbolSel wlfSymPropSymbolType( WlfSymbolId symId, WlfPropEnum p);
extern WlfModeSel   wlfSymPropPortType( WlfSymbolId symId, WlfPropEnum p);

extern int wlfWrtTriggerEvent( WlfSymbolId symbol );
extern int wlfWrtLogValue( WlfSymbolId symbol );

extern char *wlfFileGetDivider( void );
extern char *wlfFileSetDivider( char *new_divider);

#define WLF_SYMBOL_ARCHITECTURE     accArchitecture
#define WLF_SYMBOL_PACKAGE          accPackage
#define WLF_SYMBOL_PROCESS          accProcess
#define WLF_SYMBOL_SIGNAL           accSignal
#define WLF_SYMBOL_VARIABLE         accVariable
#define WLF_SYMBOL_MODULE           accModule
#define WLF_SYMBOL_PROGRAMPROC      accProgramProc
#define WLF_SYMBOL_NET              accNet
#define WLF_SYMBOL_REG              accReg
#define WLF_SYMBOL_PORT             accPort
#define WLF_SYMBOL_TASK             accTask
#define WLF_SYMBOL_FUNCTION         accFunction



/*
*  typedefs and defines for SV type support (Feb 08)
*    (integrate into the above structure once working - tj)
*/

/* Function to create 2 and 4 state reg types. */
typedef enum {
    WLF_TYPE_SV_BYTE,
    WLF_TYPE_SV_SHORTINT,
    WLF_TYPE_SV_INT,
    WLF_TYPE_SV_LONGINT,
    WLF_TYPE_SV_INTEGER,
    WLF_TYPE_SV_TIME
} WlfTypeSVIntegerAtomKind;
    
typedef enum {
    WLF_TYPE_SV_BIT,
    WLF_TYPE_SV_LOGIC,
    WLF_TYPE_SV_REG
} WlfTypeSVIntegerVectorKind;

typedef enum {
    WLF_TYPE_SV_SHORTREAL,
    WLF_TYPE_SV_REAL,
    WLF_TYPE_SV_REALTIME
} WlfTypeSVRealKind;

#define WLF_TYPE_SV_FLAG_PACKED    0x01
#define WLF_TYPE_SV_FLAG_SIGNED    0x02
#define WLF_TYPE_SV_FLAG_2STATE    0x04

#define WLF_VALUE_NO_ALLOC_VAL     0x0100

/*WlfWrtValue is needed to set more complex signals (i.e. SV enums, structs, unions, etc*/
typedef struct {
    WlfTypeId td;
    int offset;      /* Used to offset to values in packed elements */
    char *val;       /* (i.e. this can pt to a WlfVreg or WlfVbit or etc) */
} WlfWrtValue, *WlfWrtValueId;

/*
 * 4-state types have a value component and an unknown component
 *    0:  val=0  unk=0
 *    1:  val=1  unk=0
 *    x:  val=0  unk=1
 *    z:  val=1  unk=1
 */
typedef struct {
    unsigned int val;
    unsigned int unk;
} WlfVreg;

/* 2-state types have only a value component */
typedef struct {
    unsigned int val;
} WlfVbit;


/* Flags added to all functions for future use */
extern WlfWrtValueId wlfWrtCreateValueId( WlfTypeId type, unsigned flags );
extern WlfWrtValueId wlfWrtCreateValueIdFrom( WlfSymbolId symbol, unsigned flags );
extern WlfWrtValueId wlfWrtGetChildValueId( WlfWrtValueId parent_value_id, unsigned index, unsigned flags );
extern WlfWrtValueId wlfWrtSVIntegerSetValue( WlfWrtValueId v, unsigned index, unsigned int val, unsigned int unk, unsigned flags );
extern WlfWrtValueId wlfWrtSVVregSetValue( WlfWrtValueId v, WlfVreg *vreg, unsigned flags );
extern WlfWrtValueId wlfWrtSVRealSetValue( WlfWrtValueId v, double newval, unsigned flags );
extern WlfTypeId  wlfWrtCreateSVIntegerAtomType( WlfTypeSVIntegerAtomKind kind, unsigned flags );
extern WlfTypeId  wlfWrtCreateSVIntegerVectorType( WlfTypeSVIntegerVectorKind kind, int left, int right, unsigned flags );
extern WlfTypeId  wlfWrtCreateSVRealType( WlfTypeSVRealKind, unsigned flags );
extern WlfTypeId  wlfWrtCreateCompositeType( WlfTypeId base_type, int left, int right, unsigned flags );
extern WlfTypeId  wlfWrtCreateSVEnumType(WlfTypeId base_td, unsigned enum_count, char **enum_names, WlfWrtValueId *enum_vals, unsigned flags );
 /*
  *  'enum_names' array must be 'enum_count' length array of enum names.
  *  'enum_vals' array may be NULL indicating default values,
  *  otherwise a full enum_count length array of values must be provided.
  */
extern WlfTypeId  wlfWrtCreateStructType( unsigned field_count, char **field_names, WlfTypeId *field_types, unsigned flags );
extern WlfTypeId  wlfWrtCreateUnionType( unsigned field_count, char **field_names, WlfTypeId *field_types, unsigned flags );

extern WlfSymbolId wlfWrtCreateSVSignal( WlfSymbolId parent, char *name, WlfTypeId type, WlfPortMode, unsigned flags);
extern int wlfWrtSVLogValue( WlfSymbolId symbolId, WlfWrtLogMode logmode, WlfWrtValueId valueId, unsigned flags );
extern int wlfWrtAssignValue( WlfWrtValueId dest,  WlfWrtValueId src );

#endif
