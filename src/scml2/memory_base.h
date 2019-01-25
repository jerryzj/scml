/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_BASE_H
#define SCML2_MEMORY_BASE_H

#include <scml2/memory_region.h>
#include <scml2/memory_callback_base.h>
#include <scml2/memory_access_restriction_base.h>
#include <scml2/memory_debug_callback_base.h>
#include <scml2/callback_observers.h>
#include <scml2/symbolic_names.h>
#include <scml2/restriction_owner.h>

#include <scml2/utils.h>
#include <scml2/scoped_reference.h>
#include <scml2/types.h>
#include <scml2_logging/snps_vp_dll.h>

#include <string>
#include <set>
#include <vector>
#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class toplevel_memory_base;
class bitfield_base;

class SNPS_VP_API memory_base 
  : public memory_region
  , public restriction_owner
{
public:
  typedef std::vector<memory_base*> Aliases;
  typedef std::vector<memory_base*>::iterator AliasesIterator;
  typedef std::vector<memory_base*>::const_iterator AliasesConstIterator;
  typedef symbolic_names::SymbolicNames SymbolicNames;

protected:
  memory_base(const std::string& name,
              memory_base* parent,
              unsigned long long offset,
              unsigned long long size,
              unsigned int width);

public:
  virtual ~memory_base();

  memory_base* get_parent() const;

  bool has_default_read_behavior() const;
  bool has_default_write_behavior() const;
  bool has_default_debug_read_behavior() const;
  bool has_default_debug_write_behavior() const;
  bool has_never_syncing_read_behavior() const;
  bool has_never_syncing_write_behavior() const;

  bool has_default_read_restriction() const;
  bool has_default_write_restriction() const;

  bool is_dmi_allowed() const;
  virtual bool is_dmi_read_allowed() const;
  virtual bool is_dmi_write_allowed() const;

  virtual void set_callback(memory_callback_base* cb);
  virtual void set_read_callback(memory_callback_base* cb);
  virtual void set_write_callback(memory_callback_base* cb);

  virtual void remove_callback();
  virtual void remove_read_callback();
  virtual void remove_write_callback();

  virtual memory_callback_base* get_read_callback() const;
  virtual memory_callback_base* get_write_callback() const;

  virtual void set_debug_callback(memory_debug_callback_base* cb);
  virtual void set_debug_read_callback(memory_debug_callback_base* cb);
  virtual void set_debug_write_callback(memory_debug_callback_base* cb);

  virtual void remove_debug_callback();
  virtual void remove_debug_read_callback();
  virtual void remove_debug_write_callback();

  virtual memory_debug_callback_base* get_debug_read_callback() const;
  virtual memory_debug_callback_base* get_debug_write_callback() const;

  virtual memory_access_restriction_base* get_read_restriction() const;
  virtual memory_access_restriction_base* get_write_restriction() const;

  virtual void set_restriction(memory_access_restriction_base* r);
  virtual void set_read_restriction(memory_access_restriction_base* r);
  virtual void set_write_restriction(memory_access_restriction_base* r);

  virtual void remove_read_restriction();
  virtual void remove_write_restriction();

  virtual unsigned int collect_bitfields(std::vector<bitfield_base*>& b) const;

  virtual bool content_is_observable() const;

  virtual void enable_dmi();
  virtual void disable_dmi();
  virtual bool is_dmi_enabled() const;

  bool set_symbolic_name(const std::vector<unsigned char>& value,
                         const std::string& name);
  std::string get_symbolic_name(const std::vector<unsigned char>& value) const;
  unsigned int collect_symbolic_names(SymbolicNames& symbolicNames) const;

  void filter(tlm::tlm_generic_payload& trans) const;
protected:
  virtual void post_filter(tlm::tlm_generic_payload& trans) const;
  void init_default_callback(memory_callback_base* cb);
  void init_default_debug_callback(memory_debug_callback_base* cb);
  void init_default_restriction(memory_access_restriction_base* r);

  virtual void invalidate_dmi() = 0;

  virtual void update_fast_access() = 0;
  void propagate_update_fast_access();

private:
  virtual restriction_owner* get_restriction_parent() const;

  void do_set_read_callback(memory_callback_base* cb);
  void do_set_write_callback(memory_callback_base* cb);
  void do_set_debug_read_callback(memory_debug_callback_base* cb);
  void do_set_debug_write_callback(memory_debug_callback_base* cb);

  void do_set_read_restriction(memory_access_restriction_base* r);
  void do_set_write_restriction(memory_access_restriction_base* r);

  friend class toplevel_memory_base;

  void execute_behavior(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  unsigned int execute_debug_behavior(tlm::tlm_generic_payload& trans);

public:
  AliasesConstIterator begin_alias() const;
  AliasesIterator begin_alias();
  AliasesConstIterator end_alias() const;
  AliasesIterator end_alias();

  const memory_base* get_toplevel() const;


private:
  void register_alias(memory_base& alias);

private:
  memory_base* mParent;

  memory_callback_base* mDefaultCallback;
  memory_callback_base* mReadCallback;
  memory_callback_base* mWriteCallback;
  memory_debug_callback_base* mDebugReadCallback;
  memory_debug_callback_base* mDebugWriteCallback;
  memory_debug_callback_base* mDefaultDebugCallback;

  memory_access_restriction_base* mDefaultRestriction;
  memory_access_restriction_base* mReadRestriction;
  memory_access_restriction_base* mWriteRestriction;

  Aliases mAliases;

  bool mDmiEnabled;

  symbolic_names mSymbolicNames;

};

inline
restriction_owner* 
memory_base::
get_restriction_parent() const
{
  return get_parent();
}

inline
memory_base*
memory_base::
get_parent() const
{
  return mParent;
}

inline
bool
memory_base::
has_default_read_behavior() const
{
  assert(mReadCallback);
  return mReadCallback->has_default_behavior();
}

inline
bool
memory_base::
has_default_write_behavior() const
{
  assert(mWriteCallback);
  return mWriteCallback->has_default_behavior();
}


inline
bool
memory_base::
has_default_read_restriction() const
{
  assert(mReadRestriction);
  return mReadRestriction->has_default_restriction();
}

inline
bool
memory_base::
has_default_write_restriction() const
{
  assert(mWriteRestriction);
  return mWriteRestriction->has_default_restriction();
}

inline
bool
memory_base::
has_default_debug_read_behavior() const
{
  assert(mDebugReadCallback);
  return mDebugReadCallback->has_default_behavior();
}

inline
bool
memory_base::
has_default_debug_write_behavior() const
{
  assert(mDebugWriteCallback);
  return mDebugWriteCallback->has_default_behavior();
}

inline
bool
memory_base::
has_never_syncing_read_behavior() const
{
  assert(mReadCallback);
  return mReadCallback->has_never_syncing_behavior();
}

inline
bool
memory_base::
has_never_syncing_write_behavior() const
{
  assert(mWriteCallback);
  return mWriteCallback->has_never_syncing_behavior();
}

inline
bool
memory_base::
is_dmi_allowed() const
{
  return is_dmi_read_allowed() && is_dmi_write_allowed();
}

inline
bool
memory_base::
is_dmi_read_allowed() const
{
  return has_default_read_behavior() &&
         has_default_debug_read_behavior() &&
         has_default_read_restriction() &&
         is_dmi_enabled();
}

inline
bool
memory_base::
is_dmi_write_allowed() const
{
  return has_default_write_behavior() &&
         has_default_debug_write_behavior() &&
         has_default_write_restriction() &&
         is_dmi_enabled();
}

inline
memory_base::AliasesConstIterator
memory_base::
begin_alias() const
{
  return mAliases.begin();
}

inline
memory_base::AliasesIterator
memory_base::
begin_alias()
{
  return mAliases.begin();
}

inline
memory_base::AliasesConstIterator
memory_base::
end_alias() const
{
  return mAliases.end();
}

inline
memory_base::AliasesIterator
memory_base::
end_alias()
{
  return mAliases.end();
}

inline
void
memory_base::
execute_behavior(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  if (trans.is_read()) {
    assert(mReadCallback);
    scoped_reference<memory_callback_base> scopedRef(mReadCallback);
    unsigned int id =
      mCallbackObservers.notify_begin(trans, ACCESS_MODE_REGULAR);
    mReadCallback->execute(trans, t);
    mCallbackObservers.notify_end(trans, id);

  } else if (trans.is_write()){
    assert(mWriteCallback);
    scoped_reference<memory_callback_base> scopedRef(mWriteCallback);
    unsigned int id =
      mCallbackObservers.notify_begin(trans, ACCESS_MODE_REGULAR);
    mWriteCallback->execute(trans, t);
    mCallbackObservers.notify_end(trans, id);

  } else {
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }
}

inline
unsigned int
memory_base::
execute_debug_behavior(tlm::tlm_generic_payload& trans)
{
  if (trans.is_read()) {
    assert(mDebugReadCallback);
    scoped_reference<memory_debug_callback_base> scopedRef(mDebugReadCallback);
    unsigned int id =
      mCallbackObservers.notify_begin(trans, ACCESS_MODE_DEBUG);
    unsigned int result = mDebugReadCallback->execute(trans);
    mCallbackObservers.notify_end(trans, id);
    return result;

  } else if (trans.is_write()){
    assert(mDebugWriteCallback);
    scoped_reference<memory_debug_callback_base> scopedRef(mDebugWriteCallback);
    unsigned int id =
      mCallbackObservers.notify_begin(trans, ACCESS_MODE_DEBUG);
    unsigned int result = mDebugWriteCallback->execute(trans);
    mCallbackObservers.notify_end(trans, id);
    return result;

  } else {
    return 0;
  }
}

}

#endif
