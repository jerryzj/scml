/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_SELECT_CALLBACK_H
#define SCML2_MEMORY_SELECT_CALLBACK_H

#include <scml2/memory_select_callback_base.h>

#include <string>

namespace scml2 {

template<typename C>
class memory_select_callback : public memory_select_callback_base {
public:
  typedef mappable_if* (C::*CallbackType)(tlm::tlm_generic_payload&);

public:
  memory_select_callback(C& c, CallbackType cb, const std::string& name) : memory_select_callback_base(name), mClass(c), mCallback(cb) {
  }

  virtual mappable_if* execute(tlm::tlm_generic_payload& trans) {
    assert(mCallback);
    return (mClass.*mCallback)(trans);
  }

private:
  C& mClass;
  CallbackType mCallback;
};

}

#endif
