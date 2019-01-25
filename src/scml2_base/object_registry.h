/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_OBJECT_REGISTRY_H_INCLUDED
#define SCML2_BASE_OBJECT_REGISTRY_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include <systemc>

namespace scml2 { namespace base {

    class object;
    class object_name;

    class SNPS_VP_API object_registry
    {
    public:
      static object_registry& get_object_registry();
      object* get_current_object() const;
      object* get_previous_object() const;
      std::string get_current_hierarchical_name() const;
    protected:
      void add_object_name(const object_name& name);
      void remove_object_name(const object_name& name);
      void add_object(const object_name& name, object& o);
      friend class object;
      friend class object_name;
    private:
      object_registry();
      std::vector<std::pair<const object_name*, object*> > m_objects;
    };

  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_OBJECT_REGISTRY_H_INCLUDED
