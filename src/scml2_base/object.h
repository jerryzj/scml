/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_OBJECT_H_INCLUDED
#define SCML2_BASE_OBJECT_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/atom.h"

#include <systemc>

namespace scml2 { namespace base {

    class callback_base;

    class SNPS_VP_API object_name
    {
    public:
      ~object_name();
      object_name(const char* name);
      object_name(const std::string& name);
      const char* c_str() const;
    private:
      std::string m_name;
    };

    class SNPS_VP_API object
      : public atom
    {
    public:
      object(const object_name& name);
      virtual ~object();
      typedef std::vector<atom*> children_t;
      const children_t& get_children() const;

      void finalize_construction();
      bool finalized() const;
    protected:
      friend class callback_base;
      virtual void callback_registered(callback_base& /*cbb*/) {}
      virtual void finalize_local_construction() {}

      friend class atom;
      void add_child(atom* child);
      void remove_child(atom* child);
    private:
      virtual void invoke_callback();
      virtual void invoke_before_callback();

      children_t m_children;
      bool m_finalized;
    };

  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_OBJECT_H_INCLUDED
