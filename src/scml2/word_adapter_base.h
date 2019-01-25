/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WORD_ADAPTER_BASE_H
#define SCML2_WORD_ADAPTER_BASE_H

#include <scml2_logging/snps_vp_dll.h>
#include <systemc>
#include <scml2/memory_base.h>
#include <scml2/tagged_message.h>
#include <tlm.h>

namespace scml2 {

class memory_base;

template<typename M>
class word_adapter_base 
{
public:
  word_adapter_base(unsigned int wordSize, const M* memory_object, bool check_word_access);

  bool checkAlignedWordAccesses(tlm::tlm_generic_payload& trans);

protected:
  template<typename DT>
  void update_read_data(DT& data, tlm::tlm_generic_payload& trans, unsigned offset)
  {
    if (!mCheckWordAccess) {
      // no streaming width for new variants of word callbacks, so offset must be 0
      assert(offset == 0);
      unsigned access_size = (std::min)(mWordSize, trans.get_data_length());
      if (trans.get_byte_enable_ptr() != 0) {
	to_char_array(data, 
		      trans.get_data_ptr(), 
		      trans.get_byte_enable_ptr(), 
		      access_size, 
		      trans.get_address());
      } else {
	to_char_array(data, trans.get_data_ptr(), access_size, trans.get_address());
      }
    } else {
      to_char_array(data, trans.get_data_ptr() + offset, mWordSize, 0);
    }
  }

  template<typename DT>
  void update_write_data(DT& data, tlm::tlm_generic_payload& trans, unsigned offset)
  {
    if (!mCheckWordAccess) {
      // no streaming width for new variants of word callbacks, so offset must be 0
      assert(offset == 0);
      unsigned access_size = (std::min)(mWordSize, trans.get_data_length());
      from_char_array(trans.get_data_ptr(), data, access_size, trans.get_address());
      DT inv_be_data = ~DT();
      unsigned char* inv_be = (unsigned char*)&inv_be_data;
      for (unsigned i = 0; i != access_size; ++i) {
	inv_be[trans.get_address()+i] = trans.get_byte_enable_ptr() != 0 ? (~trans.get_byte_enable_ptr()[i%trans.get_byte_enable_length()]) : 0;
      }
      mMemory->get(0, reinterpret_cast<unsigned char*>(&data), mWordSize, inv_be, mWordSize);
    } else {
      from_char_array(trans.get_data_ptr() + offset, data, mWordSize, 0);    
    }
  }

  unsigned int mWordSize;
  const M* mMemory;
  bool mCheckWordAccess;
};


template<typename M>
word_adapter_base<M>::
word_adapter_base(unsigned int wordSize, const M* memory_object, bool check_word_access) :
  mWordSize(wordSize),
  mMemory(memory_object),
  mCheckWordAccess(check_word_access)
{
}

template<typename M>
bool
word_adapter_base<M>::
checkAlignedWordAccesses(tlm::tlm_generic_payload& trans)
{
  if (!mCheckWordAccess) {
    if ( trans.get_streaming_width() < trans.get_data_length()) {
      SCML2_ERROR(ACCESS_PERMISSION_CHECK_FAIL) 
	<< "Streaming width for access to address 0x" << std::hex << trans.get_address() 
	<< "should be equal to either total data length (" << std::dec << trans.get_data_length() << std::endl;
      trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
      return false;
    }
    return true;
  }

  if ( trans.get_streaming_width() < trans.get_data_length() &&
       trans.get_streaming_width() != mWordSize) {
    SCML2_ERROR(ACCESS_PERMISSION_CHECK_FAIL) << "Streaming width for access to address 0x" << std::hex << trans.get_address() 
          << "should be equal to either total data length (" << std::dec << trans.get_data_length()
          <<") or word width (" << mWordSize << ")" << std::endl;
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return false;
  }

  if (trans.get_address() % mWordSize) {
    SCML2_ERROR(ACCESS_PERMISSION_CHECK_FAIL) << "Access to address 0x" << std::hex << trans.get_address() 
          << " is not aligned to a multiple of " << std::dec << mWordSize << std::endl;
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return false;
  }

  if (trans.get_data_length() % mWordSize) {
    SCML2_ERROR(ACCESS_PERMISSION_CHECK_FAIL) << "Size (" << std::dec << trans.get_data_length() 
          << ") of access to address 0x" << std::hex << trans.get_address()
          << " should be a multiple of " << std::dec << mWordSize << std::endl;
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return false;
  }

  if (trans.get_byte_enable_ptr()) {
    for (unsigned int i = 0; i < trans.get_byte_enable_length(); ++i) {
      if (trans.get_byte_enable_ptr()[i] == TLM_BYTE_DISABLED) {
	SCML2_ERROR(ACCESS_PERMISSION_CHECK_FAIL) << "Access to address 0x" << std::hex << trans.get_address() 
						  << " does not support sparse accesses (disabled bytelanes)" << std::endl;
	trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
	return false;
      }
    }
  }

  return true;
}

}

#endif
