/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_REGION_H
#define SCML2_MEMORY_REGION_H

#include <scml2/callback_observers.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

#include <scml2_logging/stream.h>
#include <systemc>
#include <string>

namespace scml2 {

class memory_content_observer;
class callback_observer;
class streams;

class SNPS_VP_API memory_region : public sc_core::sc_object
{
protected:
  memory_region(const std::string& name,
                unsigned long long offset,
                unsigned long long size,
                unsigned int width);

public:
  virtual ~memory_region();

  std::string get_name() const;
  unsigned long long get_offset() const;
  unsigned long long get_size() const;
  unsigned int get_width() const;

  void set_description(const std::string& d);
  const std::string& get_description() const;

  virtual unsigned int read(unsigned long long address,
                            unsigned char* data,
                            unsigned int size);
  virtual unsigned int write(unsigned long long address,
                             const unsigned char* data,
                             unsigned int size);

  virtual bool content_is_observable() const;
  virtual bool register_content_observer(memory_content_observer* o);
  virtual bool unregister_all_content_observers();
  virtual bool unregister_content_observer(memory_content_observer* o);

  bool register_callback_observer(callback_observer* o);
  bool unregister_all_callback_observers();
  bool unregister_callback_observer(callback_observer* o);

  virtual void enable_dmi();
  virtual void disable_dmi();
  virtual bool is_dmi_enabled() const;

  streams& get_streams() const;

protected:
  void init_streams(streams& s);

private:
  streams* mStreams;
  const unsigned long long mOffset;
  const unsigned long long mSize;
  const unsigned int mWidth;
  std::string mDescription;

protected:
  callback_observers mCallbackObservers;

private:
  // disabled
  memory_region();
  memory_region(const memory_region&);
  memory_region& operator= (const memory_region&);
};

inline
std::string
memory_region::
get_name() const
{
  return name();
}

inline
unsigned long long
memory_region::
get_offset() const
{
  return mOffset;
}

inline
unsigned long long
memory_region::
get_size() const
{
  return mSize;
}

inline
unsigned int
memory_region::
get_width() const
{
  return mWidth;
}

inline
streams&
memory_region::
get_streams() const
{
  assert(mStreams);
  return *mStreams;
}

}

#endif
