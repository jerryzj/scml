/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_GETTER_OWNER_H_INCLUDED
#define SCML2_BASE_GETTER_OWNER_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/callback.h"
#include "scml2_base/callback_impl.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class getter_owner
    {
    public:
      getter_owner(const std::string& name) 
	: m_getter_callback(name + " getter")	  
      {}

      T get() const
      {
	return const_cast<getter_owner<T>*>(this)->m_getter_callback();
      }

      operator T() const
      {
	return get();
      }

      template<typename C>
	void set_getter(C* c, typename callback_impl<C, T>::callback_type memfun, const std::string& n = "getter")
      {
	m_getter_callback.set_behavior(scml2::base::callback_impl<C, T>(n, *c, memfun));
      }
    private:
      callback<T> m_getter_callback;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_GETTER_OWNER_H_INCLUDED
