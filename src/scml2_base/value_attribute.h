/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_VALUE_ATTRIBUTE_H_INCLUDED
#define SCML2_BASE_VALUE_ATTRIBUTE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/setter_owner.h"
#include "scml2_base/getter_owner.h"
#include "scml2_base/attribute_value.h"
#include "scml2_base/atom.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class value_attribute
      : public atom
      , private setter_owner<T>
      , private getter_owner<T>
      ,	private attribute_value<T>
    {
    public:
      value_attribute(const std::string& name) 
	: atom(name)
	, setter_owner<T>(name)
	, getter_owner<T>(name)
	, attribute_value<T>(*this, *this)
	, m_post_set_callback(name + ".post_set_callback", [](){})
      {}

      value_attribute(const std::string& name, const T& value) 
	: value_attribute(name)
      {
	set(value);
      }

      using getter_owner<T>::get;
      using getter_owner<T>::operator T;
      value_attribute<T>& operator=(T value) { set(value); return *this; }

      template<typename C>
      void set_post_set_callback(C* c, typename callback_impl<C, void>::callback_type memfun, const std::string& n = "post_set_callback_impl")
      {
	m_post_set_callback.set_behavior(scml2::base::callback_impl<C, void>(n, *c, memfun));
      }

      void set(T value) {
	setter_owner<T>::set(value);
	if (m_post_set_callback.has_behavior()) {
	  m_post_set_callback();
	}
      }
    private:
      callback<void> m_post_set_callback;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_VALUE_ATTRIBUTE_H_INCLUDED
