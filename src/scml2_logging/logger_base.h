/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_LOGGER_LOGGER_H
#define SCML2_LOGGING_LOGGER_LOGGER_H

#include <scml2_logging/rule.h>
#include <scml2_logging/snps_vp_dll.h>

#include <iosfwd>
#include <list>
#include <string>

namespace scml2 { namespace logging {

class stream_base;

class SNPS_VP_API logger_base
{
public:
  logger_base(const std::string& name);
  virtual ~logger_base();

  const std::string& get_name() const;

  void set_enable(bool enable);
  template <typename Match> const rule_base* enable(const Match& m);
  template <typename Match> const rule_base* disable(const Match& m);
  void add_rule(const rule_base* rule);
  bool remove_rule(const rule_base* rule);
  void clear_rules();
  void dump_rules(std::ostream& os) const;

  bool is_enabled(const stream_base& s) const;

  virtual void log(const stream_base& q, const char* msg) = 0;

private:
  void update_streams() const;

private:
  const std::string mName;
  bool mEnabled;
  typedef std::list<const rule_base*> Rules;
  typedef Rules::iterator RulesIter;
  typedef Rules::const_iterator RulesConstIter;
  Rules mRules;
};

inline
const std::string&
logger_base::
get_name() const
{
  return mName;
}

template <typename Match>
inline
const rule_base*
logger_base::
enable(const Match& m)
{
  const rule_base* r = new rule<Match>(true, m);
  add_rule(r);
  return r;
}

template <typename Match>
inline
const rule_base*
logger_base::
disable(const Match& m)
{
  const rule_base* r = new rule<Match>(false, m);
  add_rule(r);
  return r;
}

} }

#endif
