/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_STREAM_BASE_H
#define SCML2_LOGGING_STREAM_BASE_H

#include <scml2_logging/severity.h>
#include <scml2_logging/snps_vp_dll.h>

#include <string>


namespace scml2 { namespace logging {

class registry;

class SNPS_VP_API stream_base
{
public:
  stream_base(const std::string& name, const severity& severity);
  virtual ~stream_base();

  std::string get_name() const;
  const severity& get_severity() const;

  bool is_enabled() const;
private:
  friend class registry;
  virtual void set_enabled(bool enabled);   //Called from registry. Must reach derived classes.

  void initialize_enabled() const;

protected:
  const std::string mName;
  const severity mSeverity;
  mutable bool mEnabledOrRequiresInit;
  mutable bool mRequiresInit;

  // disabled
  stream_base();
  stream_base(const stream_base&);
  stream_base& operator= (const stream_base&);
};

inline
std::string
stream_base::
get_name() const
{
  return mName;
}

inline
const severity&
stream_base::
get_severity() const
{
  return mSeverity;
}

inline
bool
stream_base::
is_enabled() const
{
  if (mEnabledOrRequiresInit) {
    initialize_enabled();
  }
  return mEnabledOrRequiresInit;
}

inline
void
stream_base::
set_enabled(bool enabled)
{
  mEnabledOrRequiresInit = enabled;
  mRequiresInit = false;
}

} }

#endif
