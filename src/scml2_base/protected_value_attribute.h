/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_PROTECTED_VALUE_ATTRIBUTE_H_INCLUDED
#define SCML2_BASE_PROTECTED_VALUE_ATTRIBUTE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/protected_attribute.h"
#include "scml2_base/attribute_value.h"

namespace scml2 { namespace base {
    
    template<typename C, typename T>
    class protected_value_attribute
      : public protected_attribute<C, T>
      ,	private attribute_value<T>
    {
    public:
      protected_value_attribute(const std::string& name) 
	: protected_attribute<C, T>(name)
	, attribute_value<T>(*this, *this)
      {}

      protected_value_attribute<C, T>(const std::string& name, const T& value) 
	: protected_value_attribute(name)
      {
	this->set(value);
      }

      using protected_attribute<C, T>::get;
      using protected_attribute<C, T>::operator T;
    protected:
      friend C;
      protected_value_attribute<C,T>& operator=(T value) { set(value); return *this; }
      using protected_attribute<C, T>::set;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_PROTECTED_VALUE_ATTRIBUTE_H_INCLUDED
