/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_PAIR_SIGNAL_H
#define SCML2_PAIR_SIGNAL_H

#include <ostream>
#include <systemc>
#include <map>
#include <string>

namespace sc_core {

template <typename T1, typename T2>
inline
::std::ostream&
operator<<(::std::ostream& os, const std::pair<T1,T2>& a)
{
  os << "{" << a.first << "," << a.second << "}";
  return os;
}

template <class T1, class T2>
inline
::std::ostream&
operator << (::std::ostream& os, const std::pair<T1,T2>* a)
{
  os << "{" << a->first << "," << a->second << "}";
  return os;
}

template <class T1, class T2> 
inline void
sc_trace(sc_trace_file* tf , const std::pair<T1,T2>& cp, const std::string& name)
{
  sc_trace(tf, cp.first, name + ".first");
  sc_trace(tf, cp.second, name + ".second");
}

template <class T1, class T2> 
inline void
sc_trace(sc_trace_file* tf , const std::pair<T1,T2>* cp, const std::string& name )
{
  sc_trace<T1,T2>(tf, *cp, name);
}


template <class T1,class T2>
inline
void
sc_trace( sc_trace_file* tf,
          const sc_signal_in_if<std::pair<T1,T2> >* object,
          const char * cname )
{
  std::string name (cname);
  sc_trace( tf, &(object->read().first), name + ".first", &(object->value_changed_event()));
  sc_trace( tf, &(object->read().second), name + ".second", &(object->value_changed_event()));
}
template <class T1,class T2>
inline
void
sc_trace( sc_trace_file* tf,
          const sc_signal_in_if<std::pair<T1,T2> >* object,
          const std::string &name )
{
  sc_trace( tf, &(object->read().first), name + ".first", &(object->value_changed_event()));
  sc_trace( tf, &(object->read().second), name + ".second", &(object->value_changed_event()));
}

}

#endif

