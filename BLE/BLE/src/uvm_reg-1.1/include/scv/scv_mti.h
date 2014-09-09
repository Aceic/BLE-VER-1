/* $Id$ */

#ifndef _SCV_MTI_H
#define _SCV_MTI_H

#include "systemc.h"
#include "scv.h"

enum scv_hdl_direction {
  SCV_INPUT = 1,
  SCV_OUTPUT = 2
};


extern void mti_handle_scv_connect_call(
    sc_prim_channel* sc_signal_obj,
    const char* hdl_name,
    scv_hdl_direction dir,
    unsigned hdl_sim_inst);


template <typename T> 
inline void scv_connect(sc_signal<T>& signal,
                 const char* hdl_name,
                 scv_hdl_direction dir = SCV_OUTPUT,
                 unsigned hdl_sim_inst = 0)
{
    mti_handle_scv_connect_call(&signal,
                                hdl_name,
                                dir,
                                hdl_sim_inst);
}


inline void scv_connect(sc_signal_resolved& signal,
                 const char* hdl_name,
                 scv_hdl_direction dir = SCV_OUTPUT,
                 unsigned hdl_sim_inst = 0)
{
    mti_handle_scv_connect_call(DCAST<sc_prim_channel*>(&signal),
                                hdl_name,
                                dir,
                                hdl_sim_inst);
}


template <int W> 
inline void scv_connect(sc_signal_rv<W>& signal,
                 const char* hdl_name,
                 scv_hdl_direction dir = SCV_OUTPUT,
                 unsigned hdl_sim_inst = 0)
{
    mti_handle_scv_connect_call(&signal,
                                hdl_name,
                                dir,
                                hdl_sim_inst);
}

#endif
