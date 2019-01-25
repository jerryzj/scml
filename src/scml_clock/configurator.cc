/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "configurator.h"
#include <sysc/kernel/sc_simcontext.h>

n_scml_clock::configurator::~configurator() {
   // notify children
   tChildren copy = mChildren;
   for (size_t i=0; i<mChildren.size(); ++i) {
      mChildren[i]->handle_parent_deleted(this);
   }
}

void n_scml_clock::configurator::register_child(child* c) {
   mChildren.push_back(c);
}

void n_scml_clock::configurator::unregister_child(child* c) {
   for (tChildren::iterator i=mChildren.begin(); i!=mChildren.end(); ++i) {
      if (*i == c) {
	 mChildren.erase(i);
	 break;
      }
   }
}

void n_scml_clock::configurator::notify_childen() {
   tChildren copy = mChildren;
   for (size_t i=0; i<mChildren.size(); ++i) {
      mChildren[i]->handle_parent_parameters_changed(this);
   }
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
