/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_PUT_CALLBACK_BASE_H
#define SCML2_MEMORY_PUT_CALLBACK_BASE_H

#include <scml2/memory_content_observer.h>
#include <scml2/resource.h>

#include <scml2/types.h>

#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {

class memory_put_callback_owner;

class SNPS_VP_API memory_put_callback_base 
  : public resource
  , public memory_content_observer
{
public:
  memory_put_callback_base(memory_put_callback_owner* owner,
		      unsigned long long startIndex,
		      unsigned long long endIndex);
  virtual void store_old_value(unsigned long long index) = 0;
  virtual void execute(unsigned long long index) = 0;
};

}

#endif
