/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_FIND_ALIAS_H
#define SCML2_FIND_ALIAS_H

#include <scml2/toplevel_memory_base.h>
#include <scml2/memory_base.h>

#include <tuple>

namespace {
  struct Whatever
  {
    bool operator()(scml2::memory_base* /*alias*/) const
    {
      return true;
    }
  };

  struct HasNonDefaultReadBehavior
  {
    bool operator()(scml2::memory_base* alias) const
    {
      return !alias->has_default_read_behavior();
    }
  };

  struct HasNonDefaultWriteBehavior
  {
    bool operator()(scml2::memory_base* alias) const
    {
      return !alias->has_default_write_behavior();
    }
  };

  struct HasNonDefaultDebugReadBehavior
  {
    bool operator()(scml2::memory_base* alias) const
    {
      return !alias->has_default_debug_read_behavior();
    }
  };

  struct HasNonDefaultDebugWriteBehavior
  {
    bool operator()(scml2::memory_base* alias) const
    {
      return !alias->has_default_debug_write_behavior();
    }
  };
}

namespace scml2 {

namespace {
  struct alias_to_index_comparator {
    bool operator()(const memory_base* lhs, unsigned long long idx) const {
      return lhs->get_offset() < idx;
    }
    bool operator()(unsigned long long idx, const memory_base* lhs) const {
      return idx < lhs->get_offset();
    }
  };
}

inline
memory_base*
find_alias(toplevel_memory_base& mem,
           unsigned long long lowIndex,
           unsigned long long highIndex)
{
  if (highIndex >= mem.get_size()) {
    return 0;
  }

  memory_base* alias = &mem;
  bool stop;

  do {
    stop = true;

    memory_base::AliasesConstIterator child = std::upper_bound
      (alias->begin_alias(), alias->end_alias(), lowIndex, alias_to_index_comparator());
    if (child != alias->begin_alias()) {
      child = std::prev(child);
      if (highIndex < (*child)->get_offset() + ((*child)->get_size()*(*child)->get_width())/mem.get_width()) {
	alias = *child;
	stop = false;
      }
    }
  } while (!stop);

  return alias;
}

inline 
std::pair<memory_base::AliasesIterator, memory_base::AliasesIterator>
get_alias_range(memory_base& alias,
		unsigned long long lowIndex, 
		unsigned long long highIndex)
{
  memory_base::AliasesIterator it = std::upper_bound
    (alias.begin_alias(), alias.end_alias(), lowIndex, alias_to_index_comparator());
  if (it != alias.begin_alias()) {
    it = std::prev(it);
    return std::make_pair(it, std::lower_bound(it, alias.end_alias(), highIndex+1, alias_to_index_comparator()));
  } else {
    return std::make_pair(alias.begin_alias(), alias.begin_alias());
  }
}

template <typename F>
inline
bool
has_refined_behavior(memory_base& alias,
                     unsigned long long lowIndex,
                     unsigned long long highIndex,
                     const F& condition)
{
  memory_base::AliasesIterator it;
  memory_base::AliasesIterator end;
  std::tie(it, end) = get_alias_range(alias, lowIndex, highIndex);
  for (; it != end; ++it) {
    if (highIndex >= (*it)->get_offset() &&
	lowIndex < (*it)->get_offset() + (*it)->get_size()*(alias.get_width()/(*it)->get_toplevel()->get_width())) {
      if (condition(*it) ||
	  has_refined_behavior(*(*it), lowIndex, highIndex, condition)) {
	return true;
      }
    }
  }

  return false;
}

template <typename F>
inline
memory_base*
find_alias(toplevel_memory_base& mem,
           unsigned long long lowIndex,
           unsigned long long highIndex,
           const F& condition)
{
  memory_base* alias = find_alias(mem, lowIndex, highIndex);
  if (alias == 0) {
    return 0;
  }
  
  while (alias && !condition(alias)) {
    alias = alias->get_parent();
  }

  if (!alias) {
    alias = &mem;
  }

  if (lowIndex != highIndex &&
      has_refined_behavior(*alias, lowIndex, highIndex, condition)) {
    return 0;

  } else {
    return alias;
  }
}

template <typename F>
inline
const memory_base*
find_alias(const toplevel_memory_base& mem,
           unsigned long long lowIndex,
           unsigned long long highIndex,
           const F& condition)
{
  return find_alias(const_cast<toplevel_memory_base&>(mem), lowIndex, highIndex, condition);
}

inline
const memory_base*
find_alias(const toplevel_memory_base& mem,
           unsigned long long lowIndex,
           unsigned long long highIndex)
{
  return find_alias(const_cast<toplevel_memory_base&>(mem), lowIndex, highIndex);
}

}

#endif
