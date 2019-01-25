/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include "tlm2_gp_target_adapter.h"
#include <sysc/kernel/sc_dynamic_processes.h>


scml2::tlm2_gp_target_adapter_base::worker_thread::worker_thread() : mAdapter(0), mTrans(0) {}


void scml2::tlm2_gp_target_adapter_base::worker_thread::single_shot_do_work() {
  sc_core::wait(mWakeup);
  assert(mAdapter!=0 && mTrans!=0);
  mAdapter->nb2b_thread(*mTrans);
  delete this;
}

scml2::tlm2_gp_target_adapter_base::worker_thread* scml2::tlm2_gp_target_adapter_base::spawn_worker_thread() {
  worker_thread* t = new worker_thread;
  sc_core::sc_spawn(sc_bind(&worker_thread::single_shot_do_work, t), sc_core::sc_gen_unique_name("sc_worker_thread"));
  return t;
}

