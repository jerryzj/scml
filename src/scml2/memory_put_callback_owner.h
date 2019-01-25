/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_MEMORY_PUT_CALLBACK_OWNER
#define SCML2_MEMORY_PUT_CALLBACK_OWNER

#include <scml2/memory_content_observer_owner.h>
#include <scml2/memory_put_callback_base.h>
#include <scml2/types.h>

#include <cassert>

namespace scml2 {

class memory_put_callback_owner 
  : public memory_content_observer_owner
{
public:
  virtual ~memory_put_callback_owner() {}

  using memory_content_observer_owner::handle_pre_content_access;
  using memory_content_observer_owner::handle_post_content_access;

  virtual void handle_pre_content_access(memory_content_observer* o,
                                         access_type type,
                                         access_mode /*mode*/,
                                         unsigned long long index) {
    assert(type == ACCESS_TYPE_WRITE);
    assert(dynamic_cast<memory_put_callback_base*>(o) != 0);
    (void)type;
    ((memory_put_callback_base*)o)->store_old_value(index);
  };
  virtual void handle_post_content_access(memory_content_observer* o,
                                          access_type type,
                                          access_mode /*mode*/,
                                          unsigned long long index) {
    assert(type == ACCESS_TYPE_WRITE);
    assert(dynamic_cast<memory_put_callback_base*>(o) != 0);
    (void)type;
    ((memory_put_callback_base*)o)->execute(index);
  };
};

}

#endif
