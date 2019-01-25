/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_STORAGE_H
#define SCML2_MEMORY_STORAGE_H

#include <scml2/memory_storage_base.h>

#include <vector>
#include <cstddef>

namespace scml2 {

class toplevel_memory_base;

class memory_heap_storage;

class memory_heap_storage_owner {
public:
  memory_heap_storage_owner();
  ~memory_heap_storage_owner();
  void add_memory_heap_storage(memory_heap_storage* m);
  void remove_memory_heap_storage(memory_heap_storage* m);
private:
  std::vector<memory_heap_storage*> mMemories;
};

class memory_heap_storage :
  public memory_storage_base
{
public:
  memory_heap_storage(toplevel_memory_base& m, memory_heap_storage_owner& owner);
  virtual ~memory_heap_storage();

  virtual unsigned char* get_fast_backdoor() const;
  virtual fast_backdoor_region get_fast_backdoor(unsigned long long address);
  virtual void read(unsigned long long address,
                    unsigned char* data,
                    unsigned int size) const;
  virtual void write(unsigned long long address,
                     const unsigned char* data,
                     unsigned int size);

public:
  static memory_storage_base* create_storage(toplevel_memory_base& m);

private:
  toplevel_memory_base& mMemory;
  memory_heap_storage_owner& mOwner;
  unsigned char* mStorage;
  size_t mSize;
};

}

#endif
