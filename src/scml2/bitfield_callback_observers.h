/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_CALLBACK_OBSERVERS_H
#define SCML2_BITFIELD_CALLBACK_OBSERVERS_H

#include <scml2/bitfield_callback_observer.h>
#include <scml2/types.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

#include <set>

namespace scml2 {

template <typename DT> class bitfield_callback_observer_owner;

template <typename DT>
class bitfield_callback_observers
{
public:
  bitfield_callback_observers();
  ~bitfield_callback_observers();

  bool has_observers() const;

  unsigned int notify_begin(const DT* data,
                            const DT* bitMask,
                            access_type type,
                            access_mode mode) const;
  void notify_end(const DT* data, unsigned int id) const;
  void notify_callback_changed(access_type type, access_mode mode);

  bool register_observer(bitfield_callback_observer<DT>* o);
  bool unregister_all_observers();
  bool unregister_observer(bitfield_callback_observer<DT>* o);

private:
  typedef std::set<bitfield_callback_observer<DT>*> Observers;
  typedef typename Observers::iterator ObserversIter;
  typedef typename Observers::const_iterator ObserversConstIter;

  Observers& get_mutable_observers();
  void update_observers() const;

private:
  bool mObservingEnabled;
  mutable Observers* mObservers;
  mutable Observers* mMutableObservers;
  mutable int mNotifyLevel;
};

template <typename DT>
inline
bool
bitfield_callback_observers<DT>::
has_observers() const
{
  return mObservingEnabled;
}

#define SCML2_INSTANTIATE_TEMPLATE(type) \
   SNPS_VP_API_TMPL_CLASS(bitfield_callback_observers<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
