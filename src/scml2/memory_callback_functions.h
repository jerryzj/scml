/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_CALLBACK_FUNCTIONS_H
#define SCML2_MEMORY_CALLBACK_FUNCTIONS_H

#include <scml2/memory_callback.h>
#include <scml2/transport_adapter.h>
#include <scml2/read_adapter.h>
#include <scml2/null_adapter.h>
#include <scml2/on_write_adapter.h>
#include <scml2/write_adapter.h>
#include <scml2/transport_adapter_tagged.h>
#include <scml2/read_adapter_tagged.h>
#include <scml2/write_adapter_tagged.h>
#include <scml2/untimed_transport_adapter.h>
#include <scml2/untimed_read_adapter.h>
#include <scml2/untimed_write_adapter.h>
#include <scml2/untimed_transport_adapter_tagged.h>
#include <scml2/untimed_read_adapter_tagged.h>
#include <scml2/untimed_write_adapter_tagged.h>
#include <scml2/store_after_read_adapter.h>
#include <scml2/memory_disallow_access_callback.h>
#include <scml2/default_behavior_adapter.h>
#include <scml2/set_on_read_adapter.h>
#include <scml2/write_once_adapter.h>
#include <scml2/word_read_adapter.h>
#include <scml2/word_read_adapter_tagged.h>
#include <scml2/word_write_adapter.h>
#include <scml2/word_write_adapter_tagged.h>
#include <scml2/untimed_word_read_adapter.h>
#include <scml2/untimed_word_read_adapter_tagged.h>
#include <scml2/untimed_word_write_adapter.h>
#include <scml2/untimed_word_write_adapter_tagged.h>
#include <scml2/read_extensions_adapter.h>
#include <scml2/word_read_extensions_adapter.h>
#include <scml2/write_extensions_adapter.h>
#include <scml2/word_write_extensions_adapter.h>
#include <scml2/untimed_read_extensions_adapter.h>
#include <scml2/untimed_word_read_extensions_adapter.h>
#include <scml2/untimed_write_extensions_adapter.h>
#include <scml2/untimed_word_write_extensions_adapter.h>
#include <scml2/read_extensions_adapter_tagged.h>
#include <scml2/word_read_extensions_adapter_tagged.h>
#include <scml2/write_extensions_adapter_tagged.h>
#include <scml2/word_write_extensions_adapter_tagged.h>
#include <scml2/untimed_read_extensions_adapter_tagged.h>
#include <scml2/untimed_word_read_extensions_adapter_tagged.h>
#include <scml2/untimed_write_extensions_adapter_tagged.h>
#include <scml2/untimed_word_write_extensions_adapter_tagged.h>
#include <scml2/post_write_adapter.h>
#include <scml2/post_write_adapter_tagged.h>
#include <scml2/word_access_unroller.h>
#include <scml2/filter_adapter.h>
#include <scml2/streams.h>
#include <scml2/types.h>

#include <scml2_logging/stream.h>
#include <string>
#include <cassert>

#include <scml2/memory_callback_functions_include.h>

namespace scml2 {


template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_ignore_access(MEM_OBJECT<DT>& mem)
{
  mem.set_callback(new scml2::memory_disallow_access_callback(mem, false));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_ignore_read_access(MEM_OBJECT<DT>& mem)
{
  mem.set_read_callback(new scml2::memory_disallow_access_callback(mem, false));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_ignore_write_access(MEM_OBJECT<DT>& mem)
{
  mem.set_write_callback(new scml2::memory_disallow_access_callback(mem, false));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_disallow_access(MEM_OBJECT<DT>& mem)
{
  mem.set_callback(new scml2::memory_disallow_access_callback(mem, true));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_disallow_read_access(MEM_OBJECT<DT>& mem)
{
  mem.set_read_callback(new scml2::memory_disallow_access_callback(mem, true));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_disallow_write_access(MEM_OBJECT<DT>& mem)
{
  mem.set_write_callback(new scml2::memory_disallow_access_callback(mem, true));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_read_only(MEM_OBJECT<DT>& mem)
{
  set_disallow_write_access(mem);
}

template <typename DT, template <typename> class MEM_OBJECT>
inline
void
set_write_only(MEM_OBJECT<DT>& mem)
{
  set_disallow_read_access(mem);
}

template <typename DT, template <typename> class MEM_OBJECT, typename FC>
inline
void 
set_read_filter(MEM_OBJECT<DT>& mem, SyncType syncType, FC* filterClass, bool (FC::*filterCB)(tlm::tlm_generic_payload&, sc_core::sc_time&), const std::string& filterCBName) {
  mem.set_read_callback(create_memory_callback(create_filter_adapter(create_default_behavior_adapter(null_adapter(), mem), *filterClass, filterCB), filterCBName, syncType));
}

template <typename DT, template <typename> class MEM_OBJECT, typename FC>
inline
void 
set_write_filter(MEM_OBJECT<DT>& mem, SyncType syncType, FC* filterClass, bool (FC::*filterCB)(tlm::tlm_generic_payload&, sc_core::sc_time&), const std::string& filterCBName) {
  mem.set_write_callback(create_memory_callback(create_filter_adapter(create_default_behavior_adapter(null_adapter(), mem), *filterClass, filterCB), filterCBName, syncType));
}


}

#endif
