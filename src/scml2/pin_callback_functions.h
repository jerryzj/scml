/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_PIN_CALLBACK_FUNCTIONS_H
#define SCML2_PIN_CALLBACK_FUNCTIONS_H

#include <scml2/pin_callback.h>
#include <scml2/pin_callback_tagged.h>
#include <scml2/vector.h>
#include <systemc>

namespace scml2 {

  template <typename C, typename T>
  void set_change_callback(sc_core::sc_in<T>& p, C* c, typename pin_callback<C,T>::CallbackType cb, const std::string& /*cbName*/) {
    assert(c);
    new pin_callback<C,T>(p, &p.value_changed(), *c, cb);
  }

  template <typename C, typename T>
  void set_change_callback(sc_core::sc_in<T>& p, C* c, typename pin_callback_tagged<C,T>::CallbackType cb, const std::string& /*cbName*/, int tag) {
    assert(c);
    new pin_callback_tagged<C,T>(p, &p.value_changed(), *c, cb, tag);
  }

  template <typename C, typename T>
  void set_change_callback(vector< sc_core::sc_in<T> >& p, C* c, typename pin_callback_tagged<C,T>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_change_callback(p[i], c, cb, cbName, i);
    }
  }

#ifndef SYSC220
  template <typename C, typename T>
  void set_change_callback(sc_core::sc_vector< sc_core::sc_in<T> >& p, C* c, typename pin_callback_tagged<C,T>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_change_callback(p[i], c, cb, cbName, i);
    }
  }
#endif //SYSC220

  template <typename C>
  void set_posedge_callback(sc_core::sc_in<bool>& p, C* c, typename pin_callback<C, bool>::CallbackType cb, const std::string& /*cbName*/) {
    assert(c);
    new pin_callback<C, bool>(p, &p.pos(), *c, cb);
  }

  template <typename C>
  void set_posedge_callback(sc_core::sc_in<bool>& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& /*cbName*/, int tag) {
    assert(c);
    new pin_callback_tagged<C, bool>(p, &p.pos(), *c, cb, tag);
  }

  template <typename C>
  void set_posedge_callback(vector< sc_core::sc_in<bool> >& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_posedge_callback(p[i], c, cb, cbName, (int)i);
    }
  }

#ifndef SYSC220
  template <typename C>
  void set_posedge_callback(sc_core::sc_vector< sc_core::sc_in<bool> >& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_posedge_callback(p[i], c, cb, cbName, (int)i);
    }
  }
#endif //SYSC220

  template <typename C>
  void set_negedge_callback(sc_core::sc_in<bool>& p, C* c, typename pin_callback<C, bool>::CallbackType cb, const std::string& /*cbName*/) {
    assert(c);
    new pin_callback<C, bool>(p, &p.neg(), *c, cb);
  }

  template <typename C>
  void set_negedge_callback(sc_core::sc_in<bool>& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& /*cbName*/, int tag) {
    assert(c);
    new pin_callback_tagged<C, bool>(p, &p.neg(), *c, cb, tag);
  }

  template <typename C>
  void set_negedge_callback(vector< sc_core::sc_in<bool> >& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_negedge_callback(p[i], c, cb, cbName, (int)i);
    }
  }

#ifndef SYSC220
  template <typename C>
  void set_negedge_callback(sc_core::sc_vector< sc_core::sc_in<bool> >& p, C* c, typename pin_callback_tagged<C, bool>::CallbackType cb, const std::string& cbName) {
    for (size_t i = 0; i < p.size(); ++i) {
      set_negedge_callback(p[i], c, cb, cbName, (int)i);
    }
  }
#endif //SYSC220
}

#endif
