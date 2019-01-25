/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
/*
 * Modifications :
 *
 * Purpose of file : Definition of the scml_mapable_if
 *
 */

#ifndef SCML_MAPABLE_IF
#define SCML_MAPABLE_IF

#include "scmlinc/scml_transport_if.h"
#include "scmlinc/scml_types.h"

class scml_mapable_if : public scml_transport_if
{
public:

public:
  virtual const char* getName() const = 0;


  // FIXME
  virtual void setPVInterfaceType(scml::PVInterfaceType pvInterfaceType) = 0;
  virtual scml::PVInterfaceType getPVInterfaceType() const = 0;

public:
  virtual sc_object* get_interface_as_sc_object() { return 0; }
};

#endif
