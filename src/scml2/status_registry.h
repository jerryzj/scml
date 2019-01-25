/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STATUS_REGISTRY_H
#define SCML2_STATUS_REGISTRY_H

#include <scml2_logging/snps_vp_dll.h>
#include <set>
#include <vector>
#include <string>

namespace scml2 {

class status;

class SNPS_VP_API status_registry
{
public:
  static status_registry& get_instance();

  status* find_status_by_name(const std::string& name) const;
  unsigned int collect_statuses(std::vector<status*>& v) const;

private:
  friend class status;

  bool register_status(status* m);
  bool unregister_status(status* m);

private:
  status_registry();
  ~status_registry();

private:
  typedef std::set<status*> States;
  typedef States::iterator StatesIter;
  typedef States::const_iterator StatesConstIter;
  States mStates;
};

}

#endif
