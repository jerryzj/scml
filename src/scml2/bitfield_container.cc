/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/bitfield_container.h>

#include <scml2/bitfield.h>
#include <scml2/streams.h>
#include <scml2/utils.h>

#include <limits>
#include <cassert>

namespace scml2 {

template <typename DT>
bitfield_container<DT>::
bitfield_container()
{
}

template <typename DT>
void
bitfield_container<DT>::
register_bitfield(bitfield<DT>& bitfield)
{
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();
  for (; it != end; ++it) {
    if ((*it)->get_offset() < (bitfield.get_offset() + bitfield.get_size()) &&
        bitfield.get_offset() < ((*it)->get_offset() + (*it)->get_size())) {
      SCML2_LOG_ERROR(&bitfield, bitfield.get_streams().error(), CONFIGURATION_ERROR)
        << "The bitfield '" << bitfield.get_name()
        << "' overlaps with bitfield '" << (*it)->get_name() << "'"
        << std::endl;
    }
  }

  mBitfields.push_back(&bitfield);
}

template <typename DT>
bool
bitfield_container<DT>::
has_default_read_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_default_read_behavior() ||
        !(*it)->mBitfieldContainer.has_default_read_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
has_default_write_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_default_write_behavior() ||
        !(*it)->mBitfieldContainer.has_default_write_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
has_default_debug_read_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_default_debug_read_behavior() ||
        !(*it)->mBitfieldContainer.has_default_debug_read_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
has_default_debug_write_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_default_debug_write_behavior() ||
        !(*it)->mBitfieldContainer.has_default_debug_write_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
has_never_syncing_read_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_never_syncing_read_behavior() ||
        !(*it)->mBitfieldContainer.has_never_syncing_read_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
has_never_syncing_write_behavior() const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (!(*it)->has_never_syncing_write_behavior() ||
        !(*it)->mBitfieldContainer.has_never_syncing_write_behavior()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
bitfieldAccessed(bitfield<DT>* b, const DT& bitMask) const
{
  assert(b);
  const DT mask = get_bit_mask<DT>(b->get_size());
  return ((bitMask >> b->get_offset()) & mask) != 0;
}

template <typename DT>
bool
bitfield_container<DT>::
execute_write_behavior(const DT& data, const DT& bitMask, sc_core::sc_time& t, DT& bitsWritten)
{
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (bitfieldAccessed(*it, bitMask)) {
      if (!(*it)->mBitfieldContainer.execute_write_behavior(data,
                                                            bitMask,
                                                            t,
                                                            bitsWritten)) {
        return false;
      }
      if (!(*it)->has_default_write_behavior()) {
        if (!(*it)->execute_write_behavior(extract_bits<DT>(data,
                                                            (*it)->get_size(),
                                                            (*it)->get_offset()),
                                           extract_bits<DT>(bitMask & ~bitsWritten,
                                                            (*it)->get_size(),
                                                            (*it)->get_offset()),
                                           t)) {
          return false;
        }
        bitsWritten = insert_bits<DT>(bitsWritten,
                                      get_bit_mask<DT>((*it)->get_size()),
                                      (*it)->get_size(),
                                      (*it)->get_offset());
      }
      

    }
  }
  

  
  return true;
}

template <typename DT>
void bitfield_container<DT>::
execute_post_write_behavior(const DT& olddata, const DT& bitMask, sc_core::sc_time& t){
  
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();
  
  for (; it != end; ++it) {
    if (bitfieldAccessed(*it, bitMask)) {
      (*it)->mBitfieldContainer.execute_post_write_behavior(olddata,
                                                            bitMask,
                                                            t);
      
      if ((*it)->has_post_write_behavior()) {

        (*it)->execute_post_write_behavior(
              extract_bits<DT>(olddata,
                               (*it)->get_size(),
                               (*it)->get_offset())
            , (*it)->get()
            , extract_bits<DT>(bitMask,
                               (*it)->get_size(),
                               (*it)->get_offset())

            , t);
      }
    }
  }
}

template <typename DT>
bool
bitfield_container<DT>::
execute_read_behavior(DT& data, const DT& bitMask, sc_core::sc_time& t, DT& bitsRead)
{
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (bitfieldAccessed(*it, bitMask)) {
      if (!(*it)->mBitfieldContainer.execute_read_behavior(data,
                                                           bitMask,
                                                           t,
                                                           bitsRead)) {
        return false;
      }
      if (!(*it)->has_default_read_behavior()) {
        DT bitfieldData = 0;
        if (!(*it)->execute_read_behavior(bitfieldData,
                                          extract_bits<DT>(bitMask & ~bitsRead,
                                                           (*it)->get_size(),
                                                           (*it)->get_offset()),
                                          t)) {
          return false;
        }
        data = (data & bitsRead) | ((bitfieldData << (*it)->get_offset()) & ~bitsRead);
        bitsRead = insert_bits<DT>(bitsRead,
                                   get_bit_mask<DT>((*it)->get_size()),
                                   (*it)->get_size(),
                                   (*it)->get_offset());
      }
    }
  }

  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
execute_debug_write_behavior(const DT& data, const DT& bitMask, DT& bitsWritten)
{
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (bitfieldAccessed(*it, bitMask)) {
      if (!(*it)->mBitfieldContainer.execute_debug_write_behavior(data,
                                                                  bitMask,
                                                                  bitsWritten)) {
        return false;
      }
      if (!(*it)->has_default_debug_write_behavior()) {
        if (!(*it)->execute_debug_write_behavior(extract_bits<DT>(data,
                                                                  (*it)->get_size(),
                                                                  (*it)->get_offset()),
                                                 extract_bits<DT>(bitMask & ~bitsWritten,
                                                                  (*it)->get_size(),
                                                                  (*it)->get_offset()))) {
          return false;
        }
        bitsWritten = insert_bits<DT>(bitsWritten,
                                      get_bit_mask<DT>((*it)->get_size()),
                                      (*it)->get_size(),
                                      (*it)->get_offset());
      }
    }
  }

  return true;
}

template <typename DT>
bool
bitfield_container<DT>::
execute_debug_read_behavior(DT& data, const DT& bitMask, DT& bitsRead)
{
  BitfieldsIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  for (; it != end; ++it) {
    if (bitfieldAccessed(*it, bitMask)) {
      if (!(*it)->mBitfieldContainer.execute_debug_read_behavior(data, bitMask, bitsRead)) {
        return false;
      }
      if (!(*it)->has_default_debug_read_behavior()) {
        DT bitfieldData = 0;
        if (!(*it)->execute_debug_read_behavior(bitfieldData,
                                                extract_bits<DT>(bitMask & ~bitsRead,
                                                                 (*it)->get_size(),
                                                                 (*it)->get_offset()))) {
          return false;
        }
        data = (data & bitsRead) | ((bitfieldData << (*it)->get_offset()) & ~bitsRead);
        bitsRead = insert_bits<DT>(bitsRead,
                                   get_bit_mask<DT>((*it)->get_size()),
                                   (*it)->get_size(),
                                   (*it)->get_offset());
      }
    }
  }

  return true;
}

template <typename DT>
unsigned int
bitfield_container<DT>::
collect_bitfields(std::vector<bitfield_base*>& b) const
{
  BitfieldsConstIter it = mBitfields.begin();
  BitfieldsConstIter end = mBitfields.end();

  size_t size = mBitfields.size();
  for (; it != end; ++it) {
    size += (*it)->mBitfieldContainer.collect_bitfields(b);
    b.push_back(*it);
  }

  assert(size < std::numeric_limits<unsigned int>::max());
  return (unsigned int)size;
}

template <typename DT>
bool
bitfield_container<DT>::
filter(bool readNotWrite, DT& data, DT& bitMask) const
{
  for (BitfieldsConstIter it = mBitfields.begin(); it != mBitfields.end(); ++it) {
    if ((*it)->filter(readNotWrite, data, bitMask)) {
      return true;
    }
  }
  return false;
}

template <typename DT>
bool 
bitfield_container<DT>::
is_dmi_read_allowed() const
{
  for (BitfieldsConstIter it = mBitfields.begin(); it != mBitfields.end(); ++it) {
    if (!(*it)->is_dmi_read_allowed()) {
      return false;
    }
  }
  return true;
}

template <typename DT>
bool 
bitfield_container<DT>::
is_dmi_write_allowed() const
{
  for (BitfieldsConstIter it = mBitfields.begin(); it != mBitfields.end(); ++it) {
    if (!(*it)->is_dmi_write_allowed()) {
      return false;
    }
  }
  return true;
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API bitfield_container<type>;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class bitfield_container<type>;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
