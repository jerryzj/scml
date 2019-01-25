/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_content_observers.h>
#include <scml2/memory_content_observer_owner.h>
#include <scml2/memory_dmi.h>
#include <algorithm>
#include <cassert>

#define PAGE_SIZE 0x200

namespace scml2 {

memory_content_observers::
memory_content_observers(unsigned long long size) :
  mMemorySize(size),
  mObservingEnabled(false),
  mObservers(new Observers),
  mMutableObservers(0),
  mNotifyLevel(0)
{
}

memory_content_observers::
~memory_content_observers()
{
  delete mObservers;
  assert(!mMutableObservers);
}

memory_content_observers::Observers&
memory_content_observers::
get_mutable_observers()
{
  Observers* observers = mObservers;
  if (mNotifyLevel) {
    if (!mMutableObservers) {
      // lazy creation of copy (aka copy-on-write)
      mMutableObservers = new Observers(*observers);
    }
    observers = mMutableObservers;
  }
  return *observers;
}

memory_content_observers::PageIndexType
memory_content_observers::
get_page_index(unsigned long long index)
{
  return (PageIndexType)(index / PAGE_SIZE);
}

void
memory_content_observers::
notify_observers(unsigned long long lowIndex,
                 unsigned long long highIndex,
                 access_type t,
                 access_mode m,
                 bool preNotPost) const
{
	notify_observers(lowIndex, highIndex, 0, t, m, preNotPost);
}

void
memory_content_observers::
notify_observers(unsigned long long lowIndex,
                 unsigned long long highIndex,
				 unsigned int dataWidth,
                 access_type t,
                 access_mode m,
                 bool preNotPost) const
{
  if (!mObservingEnabled) {
    return;
  }
  
  // shortcut: check if there is any observer for this page
  assert(!mWriteObserverMap.empty() && !mReadObserverMap.empty());
  bool hasObservers = false;
  if(t & scml2::ACCESS_TYPE_READ) {
    PageIndexType lowPageIndex = get_page_index(lowIndex);
    PageIndexType highPageIndex = std::min(get_page_index(highIndex), mReadObserverMap.size());
    for (PageIndexType i = lowPageIndex; i <= highPageIndex; ++i) {
      if (mReadObserverMap[i]) {
	hasObservers = true;
	break;
      }
    }
  } else if (t & scml2::ACCESS_TYPE_WRITE) {
    PageIndexType lowPageIndex = get_page_index(lowIndex);
    PageIndexType highPageIndex = std::min(get_page_index(highIndex), mWriteObserverMap.size());
    for (PageIndexType i = lowPageIndex; i <= highPageIndex; ++i) {
      if (mWriteObserverMap[i]) {
	hasObservers = true;
	break;
      }
    }
  }

  if (!hasObservers) {
    return;
  }

  // notify observers
  ++mNotifyLevel; 
  for (ObserversConstIter it=mObservers->begin(); it!=mObservers->end(); ++it) {
    if (mMutableObservers == 0 || mMutableObservers->find(it->first) != mMutableObservers->end()) {
      const unsigned long long low = std::max(lowIndex, it->second + it->first->get_start_index());
      const unsigned long long high = std::min(highIndex, it->second + it->first->get_end_index());
      for (unsigned long long index = low; index <= high; ++index) {
	if (!(it->first->get_access_type() & t) || !(it->first->get_access_mode() & m)) {
	  break;
	}
	if (preNotPost) {
	  it->first->get_owner()->handle_pre_content_access(it->first, t, m, index - it->second, dataWidth);
	} else {
	  it->first->get_owner()->handle_post_content_access(it->first, t, m, index - it->second, dataWidth);
	}
	if (mMutableObservers != 0 && mMutableObservers->find(it->first) == mMutableObservers->end()) {
	  break;
	}
      }
    }
  }

  if (!--mNotifyLevel && mMutableObservers) {
    delete mObservers;
    mObservers = mMutableObservers;
    mMutableObservers = 0;
  }
}

void
memory_content_observers::
update_page_map(PageMap& map,
		memory_content_observer* o,
		unsigned long long offset)
{
  PageIndexType lowPageIndex = get_page_index(offset + o->get_start_index());
  PageIndexType highPageIndex = std::min(get_page_index(offset + o->get_end_index()), map.size());
  for (PageIndexType i = lowPageIndex; i <= highPageIndex; ++i) {
    map[i] = true;
  }  
}
  
bool
memory_content_observers::
register_observer(memory_content_observer* o,
                  unsigned long long offset)
{
  Observers& observers = get_mutable_observers();
  if (observers.find(o) != observers.end()) {
    return false;
  }
  
  mObservingEnabled = true;
  observers[o] = offset;

  if (mWriteObserverMap.empty()) {
    PageIndexType pageMapSize = (PageIndexType)((mMemorySize + PAGE_SIZE - 1) / PAGE_SIZE);
    mWriteObserverMap.resize(pageMapSize);
    assert(mReadObserverMap.empty());
    mReadObserverMap.resize(pageMapSize);
  }

  if (o->get_access_type() & scml2::ACCESS_TYPE_WRITE) {
    update_page_map(mWriteObserverMap, o, offset);
  }
  if (o->get_access_type() & scml2::ACCESS_TYPE_READ) {
    update_page_map(mReadObserverMap, o, offset);
  }
  
  return true;
}

bool
memory_content_observers::
unregister_all_observers()
{
  Observers& observers = get_mutable_observers();
  observers.clear();
  mObservingEnabled = false;
  mWriteObserverMap.clear();
  mReadObserverMap.clear();
  return true;
}

bool
memory_content_observers::
unregister_observer(memory_content_observer* o)
{
  Observers& observers = get_mutable_observers();
  ObserversIter it = observers.find(o);
  if (it == observers.end()) {
    return false;
  }
  observers.erase(it);

  if (o->get_access_type() & scml2::ACCESS_TYPE_WRITE) {
    mWriteObserverMap.assign(mWriteObserverMap.size(), false);
    
    ObserversConstIter it = observers.begin();
    ObserversConstIter end = observers.end();
    for (; it != end; ++it) {
      update_page_map(mWriteObserverMap, it->first, it->second);
    }
  }
  if (o->get_access_type() & scml2::ACCESS_TYPE_READ) {
    mReadObserverMap.assign(mReadObserverMap.size(), false);

    ObserversConstIter it = observers.begin();
    ObserversConstIter end = observers.end();
    for (; it != end; ++it) {
      update_page_map(mReadObserverMap, it->first, it->second);
    }
  }
    
  if (observers.empty()) {
    mObservingEnabled = false;
  }
  return true;
}

void
memory_content_observers::
disable_dmi(memory_dmi& readDmiData, memory_dmi& writeDmiData)
{
  if (!mObservingEnabled) {
    return;
  }

  ObserversConstIter it = mObservers->begin();
  ObserversConstIter end = mObservers->end();
  for (; it != end; ++it) {
    const unsigned long long toplevelStartIndex = it->second + it->first->get_start_index();
    const unsigned long long toplevelEndIndex = it->second + it->first->get_end_index();
    if (it->first->get_access_type() & ACCESS_TYPE_READ) {
      readDmiData.disable(toplevelStartIndex, toplevelEndIndex);
    }
    if (it->first->get_access_type() & ACCESS_TYPE_WRITE) {
      writeDmiData.disable(toplevelStartIndex, toplevelEndIndex);
    }
  }
}

}
