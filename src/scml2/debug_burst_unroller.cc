/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/debug_burst_unroller.h>
#include <scml2/utils.h>

#include <tlm.h>
#include <algorithm>

namespace scml2 {

debug_burst_unroller::
debug_burst_unroller(unsigned int width) :
  mWordSize(width)
{
}

debug_burst_unroller::
~debug_burst_unroller()
{
}

unsigned int 
debug_burst_unroller::
get_debug_access_size_at_address(unsigned long long /*address*/) const
{
  return mWordSize;
}

unsigned int
debug_burst_unroller::
unroll_debug_transaction(tlm::tlm_generic_payload& trans)
{
  tlm::tlm_generic_payload unrolledTrans;
  copy_extension_pointers(trans, unrolledTrans);

  const unsigned long long address = trans.get_address();
  unsigned char* dataPtr = trans.get_data_ptr();
  const unsigned int dataLength = trans.get_data_length();

  unrolledTrans.set_command(trans.get_command());

  unsigned int result = 0;
  for (unsigned int currentIndex = 0; currentIndex < dataLength; ) {
    unsigned long long currentAddress = address + currentIndex;
    unsigned int access_size = get_debug_access_size_at_address(currentAddress);
    unsigned int currentDataLength;
    if (currentIndex == 0) {
      const unsigned int offset = address % access_size;
      currentDataLength = access_size - offset;
    } else {
      currentDataLength = access_size;
    }
    currentDataLength = std::min(currentDataLength, dataLength - currentIndex);

    unrolledTrans.set_address(currentAddress);
    unrolledTrans.set_data_ptr(dataPtr + currentIndex);
    unrolledTrans.set_data_length(currentDataLength);

    unsigned int tmp = process_unrolled_debug_transaction(unrolledTrans);
    result += tmp;
    if (tmp < currentDataLength) {
      clear_extension_pointers(unrolledTrans);
      return result;
    }

    currentIndex += currentDataLength;
  }

  clear_extension_pointers(unrolledTrans);
  return result;
}

}
