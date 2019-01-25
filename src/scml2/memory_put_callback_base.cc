/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include <scml2/memory_put_callback_base.h>
#include <scml2/memory_put_callback_owner.h>

namespace scml2 {

memory_put_callback_base::
memory_put_callback_base(memory_put_callback_owner *owner,
			 unsigned long long startIndex,
			 unsigned long long endIndex) 
  : resource()
  , memory_content_observer(owner, ACCESS_TYPE_WRITE, ACCESS_MODE_ACCESS, startIndex, endIndex) // ?? should the put callbacks be triggered for debug writes??
{
}

}
