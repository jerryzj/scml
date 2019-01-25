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
#include <scml2_base/object_registry.h>
#include <scml2/tagged_message_macros.h>

#ifdef SNPS_SLS_VP_BASE

#include <sstream>
#include <cassert>

namespace scml2 { namespace base {

    object_name::~object_name()
    {
      object_registry::get_object_registry().remove_object_name(*this);
    }

    object_name::object_name(const char* name)
      : m_name(name) 
    {
      object_registry::get_object_registry().add_object_name(*this);
    }

    object_name::object_name(const std::string& name)
      : m_name(name) 
    {
      object_registry::get_object_registry().add_object_name(*this);
    }

    const char* object_name::c_str() const
    {
      return m_name.c_str();
    }

    object::object(const object_name& name)
      : atom()
      , m_children()
      , m_finalized(false)
    {
      object_registry::get_object_registry().add_object(name, *this);
    }

    object::~object()
    {
      if (!m_children.empty()) {
	SCML2_ERROR(SCML_INVALID_API_USAGE) << "some scml2::base::object children survived their parent '" << get_name() << "'." << std::endl;	  
	for (auto child : m_children) {
	  SCML2_ERROR(SCML_INVALID_API_USAGE) << "scml2::base::object child '" << child->get_name() << "' survived its parent." << std::endl;	  
	}
      }
    }

    void object::invoke_callback() 
    {
      if (!finalized()) {
	SCML2_ERROR(SCML_INVALID_API_USAGE) << "scml2::base::object '" << get_name() << "' was not finalized." << std::endl;
      }
    }
    void object::invoke_before_callback() 
    {
      assert(false);
    }

    void object::add_child(atom* child)
    {
      m_children.push_back(child);
    }

    void object::remove_child(atom* child)
    {
      children_t::iterator i = std::find(m_children.begin(), m_children.end(), child);
      if (i != m_children.end()) {
	m_children.erase(i);
      } else {
	SCML2_ERROR(SCML_INVALID_API_USAGE) << "scml2::base::object child '" << child->get_name() << "' was attempted to be removed from a non-parent '" << get_name() << "'." << std::endl;
      }
    }

    const object::children_t& object::get_children() const
    {
      return m_children;
    }

    bool object::finalized() const
    {
      return m_finalized;
    }

    void object::finalize_construction()
    {
      finalize_local_construction();
      for (auto child : m_children) {
	if (object* o = dynamic_cast<object*>(child)) {
	  o->finalize_construction();
	}
      }
      m_finalized = true;
    }

  }}

#endif // SNPS_SLS_VP_BASE
