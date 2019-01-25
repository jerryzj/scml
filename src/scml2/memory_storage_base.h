/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_STORAGE_BASE_H
#define SCML2_MEMORY_STORAGE_BASE_H

#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {

class toplevel_memory_base;

class SNPS_VP_API memory_storage_base
{
public:
  struct fast_backdoor_region
  {
    unsigned long long startAddress;
    unsigned long long endAddress;
    unsigned char* fastBackdoor;
  };

public:
  memory_storage_base();
  virtual ~memory_storage_base();

  virtual unsigned char* get_fast_backdoor() const = 0;
  virtual fast_backdoor_region get_fast_backdoor(unsigned long long address) = 0;
  virtual void read(unsigned long long address,
                    unsigned char* data,
                    unsigned int size) const = 0;
  virtual void write(unsigned long long address,
                     const unsigned char* data,
                     unsigned int size) = 0;

private:
  static memory_storage_base* create_storage(toplevel_memory_base& m);

private:
  memory_storage_base(const memory_storage_base&);
  memory_storage_base& operator=(const memory_storage_base&);
};

}

#endif
