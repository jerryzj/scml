/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_DIAGNOSTICS_H
#define SCML2_LOGGING_DIAGNOSTICS_H

#include <scml2_logging/snps_vp_dll.h>
#include <iosfwd>
#include <string>
#include <set>
#include <vector>

namespace snps_tlm {
    class scml_stream_link;
}

namespace scml2 { namespace logging {

class stream_base;
class logger_base;

// FIXME: lock mutex?
// FIXME: add callback on changing rules
class SNPS_VP_API registry
{
public:
  static registry& get_instance();

  ~registry();

  void log(const stream_base& s, const char* msg);

  bool register_logger(logger_base* l);
  bool unregister_logger(logger_base* l);
  logger_base* find_logger_by_name(const std::string& name) const;
  unsigned int collect_loggers(std::vector<logger_base*>& l) const;

  typedef std::set<stream_base*> Streams;
  void update_streams(Streams* streams = 0);

  unsigned int collect_streams(std::vector<stream_base*>& s) const;

  void set_exit_after_log();

  void dump_logger_rules(std::ostream& os) const;

private:
  friend class stream_base;
  friend class ::snps_tlm::scml_stream_link;
  friend class Initializer;

  registry();

  bool register_stream(stream_base* s);
  bool unregister_stream(stream_base* s);

  bool must_be_enabled(const stream_base& s) const;

private:
  typedef Streams::iterator StreamsIter;
  typedef Streams::const_iterator StreamsConstIter;
  Streams mStreams;
  typedef std::set<logger_base*> Loggers;
  typedef Loggers::iterator LoggersIter;
  typedef Loggers::const_iterator LoggersConstIter;
  Loggers mLoggers;
  bool mExitAfterLog;
};

// Initializes default loggers
static class SNPS_VP_API Initializer
{
public:
  Initializer();
  ~Initializer();
  static bool in_shutdown();
private:
  static unsigned int counter;
  static logger_base* cerrLogger;
  static logger_base* exitLogger;
} initializer;

} }

#endif
