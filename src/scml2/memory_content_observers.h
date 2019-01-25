/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_MEMORY_CONTENT_OBSERVERS_H
#define SCML2_MEMORY_CONTENT_OBSERVERS_H

#include <scml2/memory_content_observer.h>
#include <scml2/types.h>
#include <scml2_logging/snps_vp_dll.h>

#include <map>
#include <vector>

namespace scml2 {

class memory_content_observer_owner;
class memory_dmi;

class SNPS_VP_API memory_content_observers
{
public:
  memory_content_observers(unsigned long long size);
  ~memory_content_observers();

  bool has_observers() const;

  void notify_observers(unsigned long long lowIndex,
                        unsigned long long highIndex,
                        access_type t,
                        access_mode m,
                        bool preNotPost) const;

  void notify_observers(unsigned long long lowIndex,
                        unsigned long long highIndex,
						unsigned int dataWidth, // in number of bytes
                        access_type t,
                        access_mode m,
                        bool preNotPost) const;

  bool register_observer(memory_content_observer* o,
                         unsigned long long offset);
  bool unregister_all_observers();
  bool unregister_observer(memory_content_observer* o);

  void disable_dmi(memory_dmi& readDmiData, memory_dmi& writeDmiData);

private:
  typedef std::map<memory_content_observer*, unsigned long long> Observers;
  typedef Observers::iterator ObserversIter;
  typedef Observers::const_iterator ObserversConstIter;

  Observers& get_mutable_observers();

  typedef std::vector<bool> PageMap;
  typedef PageMap::size_type PageIndexType;
  static inline PageIndexType get_page_index(unsigned long long index);
  static void update_page_map(PageMap& map,
			      memory_content_observer* o,
			      unsigned long long offset);
  
private:
  unsigned long long mMemorySize;
  bool mObservingEnabled;
  mutable Observers* mObservers;
  mutable Observers* mMutableObservers;
  mutable int mNotifyLevel;

  std::vector<bool> mWriteObserverMap;
  std::vector<bool> mReadObserverMap;
};

inline
bool
memory_content_observers::
has_observers() const
{
  return mObservingEnabled;
}

}

#endif
