// -*- C++ -*-
/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "clocked_callback.h"

scml2::clocked_callback::~clocked_callback() {
   if (mCallback != 0) {
      mCallback->unref();
   }
}

void scml2::clocked_callback::set_callback(clocked_method_callback_base* callback) {
   if (mCallback != 0) {
      mCallback->unref();
   }
   mCallback = callback;
   if (mCallback != 0) {
      mCallback->ref();
   }
}

const char* scml2::clocked_callback::kind() const {
   return "scml2::clocked_callback";
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
