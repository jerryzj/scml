/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_ATTRIBUTE_VALUE_H_INCLUDED
#define SCML2_BASE_ATTRIBUTE_VALUE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <string>
#include "scml2_base/setter_owner.h"
#include "scml2_base/getter_owner.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class attribute_value
    {
    public:
      attribute_value(setter_owner<T>& s, getter_owner<T>& g)
	: m_value()
      {
	s.set_setter(this, &attribute_value<T>::set_value);
	g.set_getter(this, &attribute_value<T>::get_value);
      }

      std::string get_value() const 
      {
	return m_value;
      }

      void set_value(T value)
      {
	m_value = value;
      }

      T get_value()
      {
	return m_value;
      }

    private:
      T m_value;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_ATTRIBUTE_VALUE_H_INCLUDED
