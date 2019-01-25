/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_ATTRIBUTE_CONSTANT_INITIALIZER_H_INCLUDED
#define SCML2_BASE_ATTRIBUTE_CONSTANT_INITIALIZER_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/attribute.h"
#include "scml2/tagged_message_macros.h"
#include "scml2_base/object.h"

namespace scml2 { namespace base {
    
    template<typename T>
    class attribute_constant_initializer
    {
    public:
      attribute_constant_initializer(attribute<T>& a)
	: m_attribute(a)
	, m_initialized(false)
	, m_value()
      {
	a.set_setter(this, &attribute_constant_initializer<T>::set_value);
	a.set_getter(this, &attribute_constant_initializer<T>::get_value);
      }

      std::string get_value() const 
      {
	return m_value;
      }

      void set_value(T value)
      {
	if (!m_initialized) {
	  m_value = value;
	  m_initialized = true;
	} else {
	  SCML2_ERROR_TO(m_attribute.get_parent(), SCML_INVALID_API_USAGE) << "attribute " << m_attribute.get_name() << " is initialized more than once. Initial value is '" << m_value << "', duplicate initialization value is '" << value << "'" << std::endl;
	}
      }

      T get_value()
      {
	if (!m_initialized) {
	  SCML2_ERROR_TO(m_attribute.get_parent(), GENERIC_ERROR) << "attribute " << m_attribute.get_name() << " has not yet been initialized." << std::endl;
	}
	return m_value;
      }

      bool get_initialized() {
	return m_initialized;
      }
    private:
      const attribute<T>& m_attribute;
      bool m_initialized;
      T m_value;
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_ATTRIBUTE_CONSTANT_INITIALIZER_H_INCLUDED
