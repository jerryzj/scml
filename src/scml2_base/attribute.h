/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_ATTRIBUTE_H_INCLUDED
#define SCML2_BASE_ATTRIBUTE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/setter_owner.h"
#include "scml2_base/getter_owner.h"
#include "scml2_base/callback.h"
#include "scml2_base/callback_impl.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class attribute
      : public atom
      , public setter_owner<T>
      , public getter_owner<T>
    {
    public:
      attribute(const std::string& name) 
	: atom(name)
	, setter_owner<T>(name)
	, getter_owner<T>(name)
      {}

      attribute<T>& operator=(T value)
      {
	set(value);
	return *this;
      }

      using setter_owner<T>::set_setter;
      using setter_owner<T>::set;

      using getter_owner<T>::set_getter;
      using getter_owner<T>::get;
      using getter_owner<T>::operator T;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_ATTRIBUTE_H_INCLUDED
