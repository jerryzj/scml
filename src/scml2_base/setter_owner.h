/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_SETTER_OWNER_H_INCLUDED
#define SCML2_BASE_SETTER_OWNER_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/callback.h"
#include "scml2_base/callback_impl.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class setter_owner
    {
    public:
      setter_owner(const std::string& name) 
	: m_setter_callback(name + " setter")	  
      {}

      void set(T value)
      {
	m_setter_callback(value);
      }

      setter_owner<T>& operator=(T value)
      {
	set(value);
	return *this;
      }

      template<typename C>
      void set_setter(C* c, typename callback_impl<C, void, T>::callback_type memfun, const std::string& n = "setter")
      {
	m_setter_callback.set_behavior(scml2::base::callback_impl<C, void, T>(n, *c, memfun));
      }
    private:
      callback<void, T> m_setter_callback;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_SETTER_OWNER_H_INCLUDED
