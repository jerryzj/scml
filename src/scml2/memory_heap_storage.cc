/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_heap_storage.h>
#include <scml2/toplevel_memory_base.h>
#include <scml2/streams.h>

#include <scml2/tagged_message.h>
#include <scml2_logging/stream.h>
#include <limits>

namespace scml2 {

memory_heap_storage::
memory_heap_storage(toplevel_memory_base& m, memory_heap_storage_owner& owner) 
  : mMemory(m)
  , mOwner(owner)
  , mSize(m.get_size() == 0 ? 0 : index_to_address(m.get_size(), m.get_width()))
{
  if (mSize) {
    mStorage = new unsigned char[mSize];
    mOwner.add_memory_heap_storage(this);
  } else {
    mStorage = 0;
  }
}

memory_heap_storage::
~memory_heap_storage()
{
  mOwner.remove_memory_heap_storage(this);
  delete[] mStorage;
}

unsigned char*
memory_heap_storage::
get_fast_backdoor() const
{
  return mStorage;
}

memory_storage_base::fast_backdoor_region
memory_heap_storage::
get_fast_backdoor(unsigned long long)
{
  fast_backdoor_region region;
  region.startAddress = 0x0;
  region.endAddress = index_to_address(mMemory.get_size(),
                                       mMemory.get_width());
  region.fastBackdoor = mStorage;
  return region;
}

void
memory_heap_storage::
read(unsigned long long address,
     unsigned char* data,
     unsigned int size) const
{
  memcpy(data, mStorage + address, size);
}

void
memory_heap_storage::
write(unsigned long long address,
      const unsigned char* data,
      unsigned int size)
{
  memcpy(mStorage + address, data, size);
}

memory_heap_storage_owner::memory_heap_storage_owner() 
{}

memory_heap_storage_owner::~memory_heap_storage_owner() {
  std::vector<memory_heap_storage*> memories = mMemories;
  for (std::vector<memory_heap_storage*>::const_iterator i = memories.begin();
       i != memories.end(); ++i) {
    delete *i;
  }
}

void memory_heap_storage_owner::add_memory_heap_storage(memory_heap_storage* m) {
  mMemories.push_back(m);
}

void memory_heap_storage_owner::remove_memory_heap_storage(memory_heap_storage* m) {
  std::vector<memory_heap_storage*>::iterator i = std::find(mMemories.begin(), mMemories.end(), m);
  assert(i!=mMemories.end());
  mMemories.erase(i);
}

memory_storage_base*
memory_heap_storage::
create_storage(toplevel_memory_base& m)
{
  static memory_heap_storage_owner owner;
  memory_heap_storage* result = new memory_heap_storage(m, owner);
  return result;
}

}
