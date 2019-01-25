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
 * Purpose of file : 
 *
 */

#include "scml_property_server_if.h"

scml_property_server_if::scml_property_server_if()
{
}

scml_property_server_if::~scml_property_server_if()
{
}

long long scml_property_server_if::getIntProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return getIntProperty(name);
}

unsigned long long scml_property_server_if::getUIntProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return getUIntProperty(name);
}

bool scml_property_server_if::getBoolProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return getBoolProperty(name);
}

std::string scml_property_server_if::getStringProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return getStringProperty(name);
}

double scml_property_server_if::getDoubleProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return getDoubleProperty(name);
}


bool scml_property_server_if::hasIntProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return hasIntProperty(name);
}

bool scml_property_server_if::hasUIntProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return hasUIntProperty(name);
}

bool scml_property_server_if::hasBoolProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return hasBoolProperty(name);
}

bool scml_property_server_if::hasStringProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return hasStringProperty(name);
}

bool scml_property_server_if::hasDoubleProperty(scml_property_registry::PropertyType, const std::string & name)
{
  return hasDoubleProperty(name);
}


long long
scml_property_server_if::getIntProperty(const std::string & ) 
{
  long long result( 0);  
  return result;
}

unsigned long long 
scml_property_server_if::getUIntProperty(const std::string & )
{
  unsigned long long result( 0);
  return result;
}

bool 
scml_property_server_if::getBoolProperty(const std::string & )
{
  bool result(  false);
  return result;
}

std::string 
scml_property_server_if::getStringProperty(const std::string & )
{
  std::string result( ""); 
  return result;
}

double 
scml_property_server_if::getDoubleProperty(const std::string & )
{
  double result( 0.0);
  return result;
}

bool
scml_property_server_if::hasIntProperty(const std::string & /*name*/)
{
  return true;
}

bool
scml_property_server_if::hasUIntProperty(const std::string & /*name*/)
{
  return true;
}

bool
scml_property_server_if::hasBoolProperty(const std::string & /*name*/)
{
  return true;
}

bool
scml_property_server_if::hasStringProperty(const std::string & /*name*/)
{
  return true;
}

bool
scml_property_server_if::hasDoubleProperty(const std::string & /*name*/)
{
  return true;
}

bool
scml_property_server_if::isFinal() const {
  return true;
}
