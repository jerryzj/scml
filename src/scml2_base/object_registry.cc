/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include <scml2_base/object.h>

#ifdef SNPS_SLS_VP_BASE

#include <scml2_base/object_registry.h>
#include <scml2/tagged_message_macros.h>

#include <sstream>

namespace scml2 { namespace base {

    std::string object_registry::get_current_hierarchical_name() const
    {
      std::stringstream ss;
      bool first = true;
      for (auto& o: m_objects) {
	if (!first) {
	  ss << ".";
	}
	ss << o.first->c_str();
	first = false;
      }
      return ss.str();
    }

    object_registry& object_registry::get_object_registry()
    {
      static object_registry s_instance;
      return s_instance;
    }

    object* object_registry::get_previous_object() const
    {
      if (m_objects.size() < 2) {
	return 0;
      } 
      return m_objects[m_objects.size()-2].second;
    }

    object* object_registry::get_current_object() const
    {
      if (m_objects.empty()) {
	return 0;
      } 
      return m_objects.back().second;
    }

    void object_registry::add_object_name(const object_name& name)
    {
      if (!m_objects.empty()) {
	if (m_objects.back().second == 0) {
	  SCML2_ERROR_TO(object_registry::get_object_registry().get_current_object(), SCML_INVALID_API_USAGE) << "during construction of scml2::base::object_name '" << name.c_str() << "', it was found that the previous scml2::base::object_name '" << m_objects.back().first->c_str() << "' had no associated scml2::base::object." << std::endl;
	}
      }
      m_objects.push_back(std::make_pair(&name, (object*)0));
    }

    void object_registry::remove_object_name(const object_name& name)
    {
      if (m_objects.empty()) {
	SCML2_ERROR_TO(object_registry::get_object_registry().get_current_object(), SCML_INVALID_API_USAGE) << "during destruction of scml2::base::object_name '" << name.c_str() << "', it was found that the object was no longer available in the object_registry." << std::endl;
	return;
      }
      if (m_objects.back().first != &name) {
	SCML2_ERROR_TO(object_registry::get_object_registry().get_current_object(), SCML_INVALID_API_USAGE) << "during destruction of scml2::base::object_name '" << name.c_str() << "', it was found that the object was not considered the current object to destroy in the object registry, but rather '" << m_objects.back().first->c_str() << "' should have been." << std::endl;
      }
      m_objects.pop_back();
    }

    void object_registry::add_object(const object_name& name, object& o)
    {
      if (m_objects.empty()) {
	SCML2_ERROR_TO(&o, SCML_INVALID_API_USAGE) << "scml2::base::object '" << o.get_name() << "' is not correctly associated with a scml2::base::object_name." << std::endl; 
	return;
      }
      if (m_objects.back().first != &name) {
	SCML2_ERROR_TO(&o, SCML_INVALID_API_USAGE) << "scml2::base::object '" << o.get_name() << "' is not correctly associated with a scml2::base::object_name. Current name is '" << m_objects.back().first->c_str() << "'" << std::endl; 
	return;
      }
      m_objects.back().second = &o;
    }

    object_registry::object_registry()
    {}

  }}

#endif // SNPS_SLS_VP_BASE
