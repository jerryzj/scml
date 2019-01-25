/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_REGION_REGISTRY_H
#define SCML2_MEMORY_REGION_REGISTRY_H

#include <scml2_logging/snps_vp_dll.h>
#include <map>
#include <vector>
#include <string>

namespace scml2 {

class memory_region;

class SNPS_VP_API memory_region_registry
{
public:
  static memory_region_registry& get_instance();

  memory_region* find_memory_region_by_name(const std::string& name) const;
  unsigned int collect_memory_regions(std::vector<memory_region*>& v) const;

private:
  friend class memory_region;

  bool register_memory_region(memory_region* m);
  bool unregister_memory_region(memory_region* m);

private:
  memory_region_registry();
  ~memory_region_registry();

private:
  typedef std::map<std::string, memory_region*> MemoryRegions;
  typedef MemoryRegions::iterator MemoryRegionsIter;
  typedef MemoryRegions::const_iterator MemoryRegionsConstIter;
  MemoryRegions mMemoryRegions;
};

}

#endif
