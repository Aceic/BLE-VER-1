/*
 *  SystemVerilog Direct Programming Interface (DPI).
 *
 *  These are implementation-only headers that are called from
 *  the export tf shared object at runtime.  They should not
 *  go in svdpi.h, since the latter is a standard LRM header file.
 *
 *  This public header file is viewable by users.  
 */

#ifndef INCLUDED_SVDPI_PRV
#define INCLUDED_SVDPI_PRV

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* val;
    void* td;
    void* q;
} Triplet;

extern void*
mti_svFindTFHandle(
    char*, char*, svScope, int);

extern void*
mti_svFindTFHandleDetails(
    char*, svScope, int , char** , char**, void**);

extern void*
mti_svFindTFHandleDetailsWithSig(
    char*, svScope, int , char** , char**, void**, const char*);

#ifndef VOPT
extern const char*
scGetScopeName();

extern const char*
scSetScopeByName(
    const char* name);

#endif

extern void*
mti_svFindScImportFptr(
    char *, void**, uint64_t*);

extern void*
mti_svFindScImportFptrWithSig(
    char *, void**, uint64_t*, void** jumpAddr, const char*);

extern int
mti_svCheckScMemFptrRetType(
    int, int, char*);

extern int
mti_svBlockVerilogThread(
    void*,
    void*);

extern int
mti_svWakeUpVerilogThread(
    void*);

extern void*
mti_svGetCurrVerilogProcHandle();

extern void*
mti_svSetScopeForSC(
    svScope scope);

extern void
mti_svCleanContext();

extern int
mti_svGetTotalWidth(
    int paramIdx, void* tfHandle);

extern int
mti_svGetPackedDimSize(
    int paramIdx, void* tfHandle);

extern int
mti_svGetUnPackedDimSize(
    int paramIdx, void* tfHandle);

extern void
mti_svExecuteTaskOrFunction(
    void* jumpAddr, void** scdpi_proc_data, const char* name, int isTask, int mode);

extern void
mti_svDumpOpenArray(const svOpenArrayHandle oarr);

extern int
mti_svNumOfOpenArrayElems(const svOpenArrayHandle oarr);

extern int
mti_svSizeOfOpenArrayElems(const svOpenArrayHandle oarr);

extern void
mti_svDumpElemIndices(const svOpenArrayHandle oarr);

extern int
svIsJaggedArray(const svOpenArrayHandle oarr);

extern int
svNumOfElems(const svOpenArrayHandle oarr);

extern int
svCIndex2DimIdxSet(const svOpenArrayHandle oarr, int cIndex, int* elemIdx);

extern int
svDimIdxSet2CIndex(const svOpenArrayHandle oarr, int* elemIdx);

extern void*
mti_svGetCTd();

extern void*
mti_svGetECIntfTd();

extern void*
mti_svGetStringTd();

extern void
mti_svConvertReturnValue(
    void* , void* , void* );

extern void*
mti_svAllocStringHandle(int auto_delete);

extern void
mti_svFreeStringHandle(void*);

extern char*
mti_svGetValueFromStringHandle(void*);

extern void
mti_svSetValueToStringHandle(void*, const char*);

extern void*
mti_svAllocClassHandle(int auto_delete);

extern void
mti_svFreeClassHandle(void*);

extern void*
mti_svGetValueFromClassHandle(void*);

extern void
mti_svSetValueToClassHandle(void*, void*);

extern int
mti_IsOutOfTheBlue();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
