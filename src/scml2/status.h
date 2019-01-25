/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STATUS_H
#define SCML2_STATUS_H

#include <scml2_logging/snps_vp_dll.h>
#include <systemc>
#include <set>
#include <string>

namespace scml2 {

class status_observer;

class SNPS_VP_API status : public sc_core::sc_object
{
public:
  explicit status(const std::string& name);
  virtual ~status();

  std::string get_name() const;

  void set_status(const std::string& status);
  const std::string& get_status() const;

  void set_description(const std::string& d);
  const std::string& get_description() const;

  bool register_observer(status_observer* o);
  bool unregister_all_observers();
  bool unregister_observer(status_observer* o);

  bool has_observers() const;

private:
  void notify_observers() const;

private:
  std::string mStatus;
  std::string mDescription;

  typedef std::set<status_observer*> Observers;
  typedef Observers::iterator ObserversIter;
  typedef Observers::const_iterator ObserversConstIter;
  Observers mObservers;
};

inline
std::string
status::
get_name() const
{
  return name();
}

inline
const std::string&
status::
get_status() const
{
  return mStatus;
}
 
inline
bool
status::
has_observers() const
{
  return !mObservers.empty();
}

}

#endif
