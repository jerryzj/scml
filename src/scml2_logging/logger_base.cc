/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2_logging/logger_base.h>
#include <scml2_logging/registry.h>

#include <ostream>

namespace scml2 { namespace logging {

logger_base::
logger_base(const std::string& name) :
  mName(name),
  mEnabled(false)
{
}

logger_base::
~logger_base()
{
  clear_rules();
}

void
logger_base::
set_enable(bool enable)
{
  clear_rules();
  mEnabled = enable;
  update_streams();
}

void
logger_base::
add_rule(const rule_base* rule)
{
  mRules.push_back(rule);
  update_streams();
}

bool
logger_base::
remove_rule(const rule_base* rule)
{
  RulesIter it = mRules.begin();
  RulesConstIter end = mRules.end();
  for ( ; it != end; ++it) {
    if (*it == rule) {
      mRules.erase(it);
      delete rule;
      update_streams();
      return true;
    }
  }
  return false;
}

void
logger_base::
clear_rules()
{
  RulesIter it = mRules.begin();
  RulesConstIter end = mRules.end();
  for ( ; it != end; ++it) {
    delete *it;
  }
  mRules.clear();
  update_streams();
}

bool
logger_base::
is_enabled(const stream_base& s) const
{
  bool enabled = mEnabled;
  RulesConstIter it = mRules.begin();
  RulesConstIter end = mRules.end();
  for ( ; it != end; ++it) {
    enabled = (*it)->filter(s, enabled);
  }
  return enabled;
}

void
logger_base::
dump_rules(std::ostream& os) const
{
  os << get_name() << " rules: " << std::endl;
  os << "- " << (mEnabled ? "enable all" : "disable all") << "\n";
  RulesConstIter it = mRules.begin();
  RulesConstIter end = mRules.end();
  for ( ; it != end; ++it) {
    os << "- " << (*it)->to_string() << "\n";
  }
}

void
logger_base::
update_streams() const
{
  registry::get_instance().update_streams();
}

} }
