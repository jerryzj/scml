/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CALLBACK_IMPL_H_INCLUDED
#define SCML2_BASE_CALLBACK_IMPL_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

namespace scml2 { namespace base {

    template <typename Model, typename Ret, typename... Args>
    class callback_impl
    {
    public:
      typedef Ret (Model::*callback_type)(Args...);

      callback_impl(const std::string& name, Model& m, callback_type cb): m_name(name), m_model(m), m_callback(cb) {};

      Ret operator()(Args... args) { return (m_model.*m_callback)(args...); };

      const std::string& get_name() const { return m_name; } 
    private:
      std::string m_name;
      Model& m_model;
      callback_type m_callback;
    };
    
    template <typename Model, typename Ret, typename... Args>
    class callback_impl_tagged
    {
    public:
      typedef Ret (Model::*callback_type)(int tag, Args...);

      callback_impl_tagged(const std::string& name, Model& m, callback_type cb, int tag)
      : m_name(name), m_model(m), m_callback(cb), m_tag(tag) {};

      Ret operator()(Args... args) { return (m_model.*m_callback)(m_tag, args...); };

      const std::string& get_name() const { return m_name; } 
    private:
      std::string m_name;
      Model& m_model;
      callback_type m_callback;
      int m_tag;
    };
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_CALLBACK_IMPL_H_INCLUDED
