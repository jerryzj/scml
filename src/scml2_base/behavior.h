/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_BEHAVIOR_H_INCLUDED
#define SCML2_BASE_BEHAVIOR_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/object.h"
#include "scml2_base/atom.h"
#include "scml2_base/named_base.h"
#include "scml2_base/callback_observers_owner.h"

#include <cassert>

namespace scml2 { namespace base {
    
    template<typename Ret, typename... Args>
    class behavior
      : public virtual atom
      , public callback_observers_owner<Ret, Args...>
    {
    public:
      template<typename C, typename F>
      behavior(const std::string& name, C* c, const F& behavior)
	: atom(name) 
	, callback_observers_owner<Ret, Args...>(get_parent())
	, m_behavior([c,behavior](Args... args) -> Ret {return (c->*behavior)(args...);})
      {}

      template<typename F>
      behavior(const std::string& name, const F& behavior)
	: atom(name) 
	, callback_observers_owner<Ret, Args...>(get_parent())
	, m_behavior(behavior)
      {}
      
      Ret operator()(Args... args) 
      {
	assert(m_behavior);
	return this->call_observers(*this, m_behavior, args...);
      };
    private:
      const std::function<Ret(Args...)> m_behavior;
    };

}}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_BEHAVIOR_H_INCLUDED
