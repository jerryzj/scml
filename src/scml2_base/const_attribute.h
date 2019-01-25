/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CONST_ATTRIBUTE_H_INCLUDED
#define SCML2_BASE_CONST_ATTRIBUTE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/attribute.h"
#include "scml2_base/attribute_constant_initializer.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class const_attribute
      : public attribute<T>
      , private attribute_constant_initializer<T>
    {
    public:
      using attribute<T>::operator=;
      using attribute_constant_initializer<T>::get_initialized;

      const_attribute(const std::string& name) 
	: attribute<T>(name)
	, attribute_constant_initializer<T>(*((attribute<T>*)this))
      {}
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_CONST_ATTRIBUTE_H_INCLUDED
