/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CALLBACK_H_INCLUDED
#define SCML2_BASE_CALLBACK_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/object.h"
#include "scml2_base/atom.h"
#include "scml2_base/callback_impl.h"
#include "scml2_base/empty_callback_exception.h"
#include "scml2_base/callback_observers_owner.h"
#include "scml2/tagged_message_macros.h"

#include <cassert>

namespace scml2 { namespace base {
    
    class SNPS_VP_API callback_base 
      : public atom
    {
    public:
      callback_base(const std::string& name);
    protected:
      void call_callback_registered();
    };

    template<typename Ret, typename... Args>
    class callback
      : public callback_base
      , public callback_observers_owner<Ret, Args...>
    {
    public:
      callback(const std::string& name)
	: callback_base(name) 
	, callback_observers_owner<Ret, Args...>(get_parent())
	, m_behavior()
      {}

      template<typename F>
      callback(const std::string& name, const F& behavior)
	: callback_base(name) 
	, callback_observers_owner<Ret, Args...>(get_parent())
	, m_behavior(behavior)
      {}
      
      template<typename F>
      void set_behavior(const F& behavior) 
      { 
	m_behavior = behavior; 
	call_callback_registered();
      }
      
      Ret operator()(Args... args) 
      {
	if (m_behavior) {
	  return this->call_observers(*this, m_behavior, args...);
	} else {
	  throw scml2::base::empty_callback_exception(get_name());
	}
      };
      
      bool has_behavior() const
      {
	return (bool)m_behavior;
      }
    private:
      std::function<Ret(Args...)> m_behavior;

      virtual void invoke_before_callback() {assert(false);}
      virtual void invoke_callback()
      {
	if (!has_behavior()) {
	  SCML2_ERROR_TO(get_parent(), CONFIGURATION_ERROR) << "Callback '" << get_name() << "' has not been associated with any behavior." << std::endl;	  
	}
      }
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_CALLBACK_H_INCLUDED
