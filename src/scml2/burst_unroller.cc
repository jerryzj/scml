/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/burst_unroller.h>
#include <scml2/utils.h>

#include <tlm.h>
#include <algorithm>
#include <cassert>

namespace scml2 {

burst_unroller::
burst_unroller(unsigned int width) :
  mWordSize(width)
{
}

burst_unroller::
~burst_unroller()
{
}

unsigned int 
burst_unroller::
get_access_size_at_address(unsigned long long /*address*/) const
{
  return mWordSize;
}

void
burst_unroller::
unroll_transaction(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  tlm::tlm_generic_payload unrolledTrans;
  copy_extension_pointers(trans, unrolledTrans);

  assert(trans.get_streaming_width());
  assert(!trans.get_byte_enable_ptr() || trans.get_byte_enable_length());

  const unsigned long long address = trans.get_address();
  unsigned char* dataPtr = trans.get_data_ptr();
  const unsigned int dataLength = trans.get_data_length();
  unsigned char* byteEnablePtr = trans.get_byte_enable_ptr();
  const unsigned int byteEnableLength = trans.get_byte_enable_length();
  const unsigned int streamingWidth =
    std::min(dataLength, trans.get_streaming_width());

  unrolledTrans.set_command(trans.get_command());

  {
    std::vector<unsigned char> data;
    std::vector<unsigned char> byteEnables;

    const unsigned int offset = address % get_access_size_at_address(address);
    const unsigned long long baseAddress = address - offset;
    unsigned long long currentAddress = baseAddress;
    unsigned int bytesLeftInStreamingBeat =
      std::min(streamingWidth, dataLength);
    for (unsigned int currentIndex = 0; currentIndex < dataLength; ) {
      unsigned int access_size = get_access_size_at_address(currentAddress);
      unrolledTrans.set_streaming_width(access_size);
      unrolledTrans.set_data_length(access_size);
      unsigned int currentDataLength;
      unsigned int currentOffset;
      if (currentIndex % streamingWidth == 0) {
        currentOffset = offset;
        currentDataLength =
          std::min(access_size - offset, bytesLeftInStreamingBeat);

      } else {
        currentOffset = 0;
        currentDataLength = std::min(access_size, bytesLeftInStreamingBeat);
      }
      assert(currentDataLength);

      bool needDataCopy = (currentIndex + currentDataLength > dataLength);
      if (needDataCopy) {
	// copy data array
	data.resize(access_size);
	unrolledTrans.set_data_ptr(&data[0]);
	memset(&data[0], 0, access_size);
	memcpy(&data[0] + currentOffset,
	       dataPtr + currentIndex,
	       currentDataLength);
      } else {
	unrolledTrans.set_data_ptr(&dataPtr[0]+currentIndex-currentOffset);
      }

      // copy byte enables
      if (byteEnablePtr && !currentOffset && (byteEnableLength >= currentIndex + dataLength)) {
	unrolledTrans.set_byte_enable_length(byteEnableLength-currentIndex);
	unrolledTrans.set_byte_enable_ptr(&byteEnablePtr[0]+currentIndex);
      } else if (!byteEnablePtr && currentDataLength == access_size) {
	unrolledTrans.set_byte_enable_ptr(0);
	unrolledTrans.set_byte_enable_length(0);
      } else { 
	byteEnables.resize(access_size);
	unrolledTrans.set_byte_enable_ptr(&byteEnables[0]);
	unrolledTrans.set_byte_enable_length(access_size);
	memset  (&byteEnables[0],                                     TLM_BYTE_DISABLED,               currentOffset);
	if (byteEnablePtr) {
	  for (unsigned int i = 0; i < currentDataLength; ++i) {
	    byteEnables[currentOffset + i] =
	      byteEnablePtr[(currentIndex + i) % byteEnableLength];
	  }
	} else {
	  memset(&byteEnables[0] + currentOffset,                     TLM_BYTE_ENABLED,                                currentDataLength);	  
	}
	memset  (&byteEnables[0] + currentOffset + currentDataLength, TLM_BYTE_DISABLED, access_size - currentOffset - currentDataLength);
      }

      unrolledTrans.set_address(currentAddress);
      unrolledTrans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

      process_unrolled_transaction(unrolledTrans, t);

      if (unrolledTrans.get_response_status() != tlm::TLM_OK_RESPONSE) {
        trans.set_response_status(unrolledTrans.get_response_status());
        clear_extension_pointers(unrolledTrans);
        return;
      }
  
      if (needDataCopy && trans.is_read()) {
        // copy back read data
        memcpy(dataPtr + currentIndex,
               &data[0] + currentOffset,
               currentDataLength);
      }

      currentIndex += currentDataLength;
      bytesLeftInStreamingBeat -= currentDataLength;
      if (currentIndex % streamingWidth == 0) {
        bytesLeftInStreamingBeat =
          std::min(streamingWidth, dataLength - currentIndex);
        currentAddress = baseAddress;

      } else {
        currentAddress += access_size;
      }
    }
  }

  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  clear_extension_pointers(unrolledTrans);
}

}
