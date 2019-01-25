/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CALLBACK_OBSERVERS_OWNER_H_INCLUDED
#define SCML2_BASE_CALLBACK_OBSERVERS_OWNER_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/callback_observer.h"

#include "scml2/tagged_message_macros.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace scml2 { namespace base {

    template<typename... Args>
    class callback_observers_owner_base
    {
    public:
      void register_observer(callback_observer<Args...>* observer) {
	m_observers.push_back(observer);
      }

      void unregister_observer(callback_observer<Args...>* observer) {
	auto i = std::find(m_observers.begin(), m_observers.end(), observer);
	if (i != m_observers.end()) {
	  m_observers.erase(i);
	}
      }
    protected:
      std::vector<callback_observer<Args...>*> m_observers; 
    };
    
    namespace {
#if 0 // this code doesn't work for MSVC-12.0, so using workaround
      template<typename STREAM_TYPE, typename TYPE>
      struct printer_helper
      {
	typedef char yes[1];
	typedef char no[2];
	
	template <std::size_t N> struct helper {};
	
	template <typename ST, typename T>
	static yes& canBePrinted(helper<sizeof( *static_cast<ST*>(0) << *static_cast<T*>(0) )>* = 0); 

	template <typename ST, typename T>
	static no& canBePrinted(...);
	
	static const bool value = sizeof(canBePrinted<STREAM_TYPE, TYPE>(NULL)) == sizeof(yes);
      };
#else
      template<typename STREAM_TYPE, typename TYPE>
      struct printer_helper
      {
	static const bool value = false;
      };
#define HELPER(type) template<typename STREAM_TYPE> struct printer_helper<STREAM_TYPE, type> {static const bool value = true;};
      HELPER(bool);
      HELPER(char);
      HELPER(signed char);
      HELPER(unsigned char);
      HELPER(short);
      HELPER(unsigned short);
      HELPER(int);
      HELPER(unsigned int);
      HELPER(long);
      HELPER(unsigned long);
      HELPER(long long);
      HELPER(unsigned long long);
      HELPER(float);
      HELPER(double);
      HELPER(std::string);
      HELPER(const char*);
      HELPER(char*);
      HELPER(sc_core::sc_time);
#undef HELPER
#endif

      
      template<typename T, bool printable>
      struct printer {
	static inline void print_to_stream(std::ostream& os, const T& t)
	{
	  os << "\"" << t << "\" ";
	}
      };

      template<typename T>
      struct printer<T, false> {
	static inline void print_to_stream(std::ostream& os, const T& /*t*/)
	{
	  os << "<unprintable>";
	}
      };

      template<typename T>
      inline std::string to_string_s(const T& s) {
	std::stringstream ss;
	printer<T, printer_helper<std::stringstream,T>::value>::print_to_stream(ss, s) ;
	return ss.str();
      }

      template<typename... Args>
      std::string to_string(const Args&... args) {
	std::string result;
	int tmp[]{0, (result.append(to_string_s(args)),0)...};
	(void)tmp;
	return result;
      }
    }

    template<typename Ret, typename... Args>
    class callback_observers_owner
      : public callback_observers_owner_base<Ret, Args...>
    {
      const object* m_object;
    public:
      callback_observers_owner(const object* object) : m_object(object)
      {}

      Ret call_observers(const named_base& n, const std::function<Ret(Args...)>& f, Args... args) 
      {
	std::string args_str = to_string(args...);
	Ret ret = f(args...);
	SCML2_MODEL_INTERNAL_TO(m_object, SCML_CALLBACK_ENTRY) << "Callback '" << n.get_name() << "' returned \"" << ret << "\"" << (args_str.length() > 0 ? ", called with args: " : "' called") << args_str << std::endl;
	for (auto o : this->m_observers) {
	  o->callback_called(ret, args...);
	}
	return ret;
      };
    };

    template<typename... Args>
    class callback_observers_owner<void, Args...>
      : public callback_observers_owner_base<Args...>
    {
      const object* m_object;
    public:
	callback_observers_owner(const object* object) : m_object(object)
      {}

      void call_observers(const named_base& n, const std::function<void(Args...)>& f, Args... args) 
      {
	std::string args_str = to_string(args...);
	f(args...);
	SCML2_MODEL_INTERNAL_TO(m_object, SCML_CALLBACK_ENTRY) << "Callback '" << n.get_name() << (args_str.length() > 0 ? "' called with args: " : "' called") << args_str << std::endl;
	for (auto o : this->m_observers) {
	  o->callback_called(args...);
	}
      };
    };
    
  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_CALLBACK_OBSERVERS_OWNER_H_INCLUDED
