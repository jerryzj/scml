/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_SYMBOLIC_NAMES_H
#define SCML2_SYMBOLIC_NAMES_H

#include <scml2_logging/snps_vp_dll.h>
#include <string>
#include <vector>
#include <map>

namespace scml2 {

class SNPS_VP_API symbolic_names
{
public:
  typedef std::pair<std::vector<unsigned char>, std::string> SymbolicName;
  typedef std::vector<SymbolicName> SymbolicNames;
  typedef SymbolicNames::iterator SymbolicNamesIterator;
  typedef SymbolicNames::const_iterator SymbolicNamesConstIterator;
  
public:
  virtual ~symbolic_names();

  bool set_symbolic_name(const std::vector<unsigned char>& value,
                         const std::string& name);
  std::string get_symbolic_name(const std::vector<unsigned char>& value) const;
  unsigned int collect_symbolic_names(SymbolicNames& symbolicNames) const;

private:
  typedef std::map<std::vector<unsigned char>, std::string> ValueNameMap;
  ValueNameMap mValueNameMap;
  typedef std::map<std::string, std::vector<unsigned char> > NameValueMap;
  NameValueMap mNameValueMap;
};

}

#endif
