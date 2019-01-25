/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/memory_storage_creator.h>
#include <scml2/memory_heap_storage.h>

#include <cassert>

namespace scml2 {

memory_storage_creator::
memory_storage_creator() :
  mStorageCreator(&memory_heap_storage::create_storage)
{
}

memory_storage_creator&
memory_storage_creator::
get_instance()
{
  static memory_storage_creator c;
  return c;
}

void
memory_storage_creator::
set_storage_creator(StorageCreator c)
{
  assert(c);
  mStorageCreator = c;
}

memory_storage_creator::StorageCreator
memory_storage_creator::
get_storage_creator() const
{
  return mStorageCreator;
}

memory_storage_base*
memory_storage_creator::
create_storage(toplevel_memory_base& m)
{
  assert(mStorageCreator);
  return (*mStorageCreator)(m);
}

}
