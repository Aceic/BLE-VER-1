#ifndef WLFSERIAL_H
#define WLFSERIAL_H

#include "base.h"
#include "wlfCommon.h"

WlfSN_t wlfSerialAssignObject(
    WlfCmnFileId fileId,
    void*        pObject,
    WlfSN_t      SN);

void wlfSerialClear(
    WlfCmnFileId fileId);

void wlfSerialDeassignObject(
    WlfCmnFileId fileId,
    void*        pObject,
    WlfSN_t      SN);

WlfSN_t wlfSerialGetSN(
    WlfCmnFileId fileId,
    void*        pObject);

void* wlfSerialGetObject(
    WlfCmnFileId fileId,
    WlfSN_t      SN);

WlfSN_t wlfSerialRegisterObject(
    WlfCmnFileId fileId,
    void*        pObject);

void wlfSerialRequireSNs(
    WlfCmnFileId fileId);

#endif   /* #ifndef WLFSERIAL_H */
