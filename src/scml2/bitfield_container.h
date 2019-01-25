/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_CONTAINER_H
#define SCML2_BITFIELD_CONTAINER_H

#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>
#include <vector>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class bitfield_base;
template <typename DT> class bitfield;

template <typename DT>
class bitfield_container
{
public:
  bitfield_container();

  void register_bitfield(bitfield<DT>& bitfield);

  bool has_default_read_behavior() const;
  bool has_default_write_behavior() const;
  bool has_default_debug_read_behavior() const;
  bool has_default_debug_write_behavior() const;
  bool has_never_syncing_read_behavior() const;
  bool has_never_syncing_write_behavior() const;

  bool execute_write_behavior(const DT& data, const DT& bitMask, sc_core::sc_time& t, DT& bitsWritten);
  bool execute_read_behavior(DT& data, const DT& bitMask, sc_core::sc_time& t, DT& bitsRead);
  bool execute_debug_write_behavior(const DT& data, const DT& bitMask, DT& bitsWritten);
  bool execute_debug_read_behavior(DT& data, const DT& bitMask, DT& bitsRead);
  void execute_post_write_behavior(const DT& olddata, const DT& bitMask, sc_core::sc_time& t);

  unsigned int collect_bitfields(std::vector<bitfield_base*>& b) const;

  bool filter(bool readNotWrite, DT& data, DT& bitMask) const;

  bool is_dmi_read_allowed() const;
  bool is_dmi_write_allowed() const;
private:
  bool bitfieldAccessed(bitfield<DT>* b, const DT& mask) const;

private:
  typedef std::vector<bitfield<DT>*> Bitfields;
  typedef typename Bitfields::iterator BitfieldsIter;
  typedef typename Bitfields::const_iterator BitfieldsConstIter;
  Bitfields mBitfields;  
};

#define SCML2_INSTANTIATE_TEMPLATE(type)				\
   SNPS_VP_API_TMPL_CLASS(bitfield_container<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE
   
}

#endif
