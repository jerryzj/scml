/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/memory_region_registry.h>
#include <scml2/memory_region.h>

#include <cassert>

namespace scml2 {

memory_region_registry&
memory_region_registry::
get_instance()
{
  static memory_region_registry instance;
  return instance;
}

memory_region_registry::
memory_region_registry()
{
}

memory_region_registry::
~memory_region_registry()
{
}

bool
memory_region_registry::
register_memory_region(memory_region* m)
{
  assert(m);
  const std::pair<MemoryRegionsIter, bool> r =
    mMemoryRegions.insert(std::make_pair(m->get_name(), m));
  return r.second;
}

bool
memory_region_registry::
unregister_memory_region(memory_region* m)
{
  return (mMemoryRegions.erase(m->get_name()) != 0);
}

memory_region*
memory_region_registry::
find_memory_region_by_name(const std::string& name) const
{
  MemoryRegionsConstIter it = mMemoryRegions.find(name);
  if (it != mMemoryRegions.end()) {
    return it->second;
  }
  return 0;
}

unsigned int
memory_region_registry::
collect_memory_regions(std::vector<memory_region*>& v) const
{
  unsigned int count = 0;
  MemoryRegionsConstIter it = mMemoryRegions.begin();
  MemoryRegionsConstIter end = mMemoryRegions.end();
  for (; it != end; ++it, ++count) {
    v.push_back(it->second);
  }
  return count;
}

}
