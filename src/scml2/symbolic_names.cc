/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/symbolic_names.h>

#include <algorithm>
#include <iterator>
#include <cassert>
#include <limits>

namespace scml2 {

symbolic_names::
~symbolic_names()
{
}

bool
symbolic_names::
set_symbolic_name(const std::vector<unsigned char>& value,
                  const std::string& name)
{
  if (mNameValueMap.find(name) != mNameValueMap.end() ||
    mValueNameMap.find(value) != mValueNameMap.end()) {
    return false;

  } else {
    mNameValueMap[name] = value;
    mValueNameMap[value] = name;
    return true;
  }
}

std::string 
symbolic_names::
get_symbolic_name(const std::vector<unsigned char>& value) const {
  ValueNameMap::const_iterator i = mValueNameMap.find(value);
  if (i != mValueNameMap.end()) {
    return i->second;
  }
  return "";
}

unsigned int
symbolic_names::
collect_symbolic_names(SymbolicNames& symbolicNames) const
{
  assert(mNameValueMap.size() == mValueNameMap.size());
  std::copy(mValueNameMap.begin(),
            mValueNameMap.end(),
            std::back_insert_iterator<SymbolicNames>(symbolicNames));
  size_t size = mValueNameMap.size();
  assert( size < std::numeric_limits<unsigned int>::max());
  return (unsigned int)size;
}

}
