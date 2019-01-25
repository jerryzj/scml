/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_PIN_CALLBACK_H
#define SCML2_PIN_CALLBACK_H

#include <scml2/pin_callback_base.h>

namespace scml2 {
  template <typename C, typename T>
  class pin_callback : public pin_callback_base<T> {
    public:
      typedef void (C::*CallbackType)();

      pin_callback(sc_core::sc_in<T>& p, sc_core::sc_event_finder* e, C& c, CallbackType cb) : pin_callback_base<T>(p, e), mClass(c), mCallback(cb)
      {
      }

      virtual void execute() {
        assert(mCallback);
        (mClass.*mCallback)();
      }

    private:
      C& mClass;
      CallbackType mCallback;
  };
}

#endif
