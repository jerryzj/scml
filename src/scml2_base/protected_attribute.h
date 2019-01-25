/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_PROTECTED_ATTRIBUTE_H_INCLUDED
#define SCML2_BASE_PROTECTED_ATTRIBUTE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/atom.h"
#include "scml2_base/setter_owner.h"
#include "scml2_base/getter_owner.h"

#include <string>

namespace scml2 { namespace base {
    
    template<typename C, typename T>
    class protected_attribute
      : public atom
      , protected setter_owner<T>
      , protected getter_owner<T>
    {
    public:
      protected_attribute(const std::string& name) 
	: atom(name)
	, setter_owner<T>(name)
	, getter_owner<T>(name)
      {}

      using getter_owner<T>::get;
      using getter_owner<T>::operator T;
    protected:
      friend C;
      protected_attribute<C,T>& operator=(T value) { set(value); return *this; }
      using setter_owner<T>::set;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_PROTECTED_ATTRIBUTE_H_INCLUDED
