/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CALLBACK_MACROS_H_INCLUDED
#define SCML2_BASE_CALLBACK_MACROS_H_INCLUDED

#include <scml2_base/utils.h>

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/callback.h"
#include "scml2_base/callback_impl.h"
#include "scml2/tagged_message_macros.h"

#define SCML2_BASE_CALLBACK(NAME, RET, ...)                               \
  scml2::base::callback<RET, ## __VA_ARGS__ > NAME{#NAME};                \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback(Model* m, typename scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n) { \
    NAME.set_behavior(scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >(n, *m, cb)); \
  };                                                                      \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback_tagged(Model* m, typename scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n, int tag) { \
    NAME.set_behavior(scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >(n, *m, cb, tag)); \
  };

#define SCML2_BASE_CALLBACK_IGNORE(NAME, RET, ...)                        \
  scml2::base::callback<RET, ## __VA_ARGS__ > NAME{#NAME, [](__VA_ARGS__) -> RET { return RET(); }}; \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback(Model* m, typename scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n) { \
    NAME.set_behavior(scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >(n, *m, cb)); \
  };                                                                      \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback_tagged(Model* m, typename scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n, int tag) { \
    NAME.set_behavior(scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >(n, *m, cb, tag)); \
  };

#define SCML2_BASE_CALLBACK_IGNORE_W_VAL(NAME, RET, RET_V, ...)                                                             \
  scml2::base::callback<RET, ## __VA_ARGS__ > NAME{#NAME, [](__VA_ARGS__) -> RET { return RET(RET_V); }}; \
  template<typename Model>                                                                                    \
  void set_ ## NAME ## _callback(Model* m, typename scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n) { \
    NAME.set_behavior(scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >(n, *m, cb)); \
  };                                                                      \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback_tagged(Model* m, typename scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n, int tag) { \
    NAME.set_behavior(scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >(n, *m, cb, tag)); \
  };

#define SCML2_BASE_CALLBACK_WARNING(NAME, RET, ...)                                \
  scml2::base::callback<RET, ## __VA_ARGS__ > NAME{#NAME, [this](__VA_ARGS__) -> RET { SCML2_WARNING(this, ACCESS_IGNORED) << "call " << #NAME << " ignored in " << this->get_name() << std::endl; return RET(); }}; \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback(Model* m, typename scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n) { \
    NAME.set_behavior(scml2::base::callback_impl<Model, RET, ## __VA_ARGS__ >(n, *m, cb)); \
  };                                                                      \
  template<typename Model>                                                \
  void set_ ## NAME ## _callback_tagged(Model* m, typename scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >::callback_type cb, const std::string& n, int tag) { \
    NAME.set_behavior(scml2::base::callback_impl_tagged<Model, RET, ## __VA_ARGS__ >(n, *m, cb, tag)); \
  };

#endif

#endif // SCML2_BASE_CALLBACK_MACROS_H_INCLUDED
