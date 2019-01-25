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
 * Purpose of file : Implementation file for scml_property_registry
 *
 */

#include "scml_property_registry.h"
#include "scml_property_server_if.h"
#include "scmlinc/scml_inmemory_property_server.h"

#include <sys/stat.h>
#include <cassert>


using std::string;
using std::cerr;
using std::endl;

static const std::string TAG_PARAMETERS                 ("parameters");
static const std::string TAG_PARAMETER                  ("parameter");
static const std::string TAG_CONSTRUCTOR_ARGUMENTS      ("constructor_arguments");
static const std::string TAG_PROTOCOL_PARAMETERS        ("protocol_parameters");
static const std::string TAG_MODULES                    ("modules");
static const std::string TAG_MODULE                     ("module");
static const std::string TAG_PORTS                      ("ports");
static const std::string TAG_PORT                       ("port");
static const std::string TAG_NAME                       ("name");
static const std::string TAG_VALUE                      ("value");

static const std::string TAG_PORT_ARRAYS                ("port_arrays");
static const std::string TAG_PORT_ARRAY                 ("port_array");

static const std::string STR_INTEGER                    ("integer");
static const std::string STR_DOUBLE                     ("double");
static const std::string STR_BOOL                       ("bool");
static const std::string STR_STRING                     ("string");
static const std::string STR_ONE                        ("1");
static const std::string STR_ZERO                       ("0");
static const std::string STR_TRUE                       ("true");
static const std::string STR_FALSE                      ("false");
static const std::string STR_EMPTY;


scml_property_registry& scml_property_registry::inst() 
{
  static scml_property_registry instance;
  return instance;
}

scml_property_registry::scml_property_registry() : 
  mOs( &cerr), 
  mCustomPropServer( 0)
{
  mSetProperties = new scml_inmemory_property_server();
}

scml_property_registry::~scml_property_registry()
{
  delete mSetProperties;
}

bool scml_property_registry::setCustomPropertyServer( scml_property_server_if * p )
{
  mCustomPropServer = p;
  return (0 != mCustomPropServer);
}

scml_property_server_if * 
scml_property_registry::getCustomPropertyServer() const
{
  return mCustomPropServer;
}


long long scml_property_registry::getIntProperty(PropertyType type, const string& moduleName, const string& name)
{
  (void) type;
  const string completeName( moduleName + '.' + name);
  if (mSetProperties->hasIntProperty(completeName)) {
    return mSetProperties->getIntProperty(completeName);
  }
  if( 0 != mCustomPropServer) {
    if (mCustomPropServer->hasIntProperty(type, completeName) || mCustomPropServer->isFinal()) {
      return mCustomPropServer->getIntProperty(type, completeName);
    }
  } 
  return 0;
}
		   
unsigned long long scml_property_registry::getUIntProperty(PropertyType type, const string& moduleName, const string& name)
{
  (void) type;
  const string completeName( moduleName + '.' + name);
  if (mSetProperties->hasUIntProperty(completeName)) {
    return mSetProperties->getUIntProperty(completeName);
  }
  if( 0 != mCustomPropServer) {
    if (mCustomPropServer->hasUIntProperty(type, completeName) || mCustomPropServer->isFinal()) {
      return mCustomPropServer->getUIntProperty(type, completeName);
    }
  }
  return 0;
}

bool scml_property_registry::getBoolProperty(PropertyType type, const string& moduleName, const string& name)
{
  (void) type;
  const string completeName( moduleName + '.' + name);
  if (mSetProperties->hasBoolProperty(completeName)) {
    return mSetProperties->getBoolProperty(completeName);
  }
  if( 0 != mCustomPropServer) {
    if (mCustomPropServer->hasBoolProperty(type, completeName) || mCustomPropServer->isFinal()) {
      return mCustomPropServer->getBoolProperty(type, completeName);
    }
  } 
  return false;
}
				       
string scml_property_registry::getStringProperty(PropertyType type, const string& moduleName, const string& name)
{
  (void) type;
  const string completeName( moduleName + '.' + name);
  if (mSetProperties->hasStringProperty(completeName)) {
    return mSetProperties->getStringProperty(completeName);
  }
  if( 0 != mCustomPropServer) {
    if (mCustomPropServer->hasStringProperty(type, completeName) || mCustomPropServer->isFinal()) {
      return mCustomPropServer->getStringProperty(type, completeName);
    }
  }
  return "";
}
					   
double scml_property_registry::getDoubleProperty(PropertyType type, const string& moduleName, const string& name)
{
  (void) type;
  const string completeName( moduleName + '.' + name);
  if (mSetProperties->hasDoubleProperty(completeName)) {
    return mSetProperties->getDoubleProperty(completeName);
  }
  if( 0 != mCustomPropServer) {
    if (mCustomPropServer->hasDoubleProperty(type, completeName) || mCustomPropServer->isFinal()) {
      return mCustomPropServer->getDoubleProperty(type, completeName);
    }
  }
  return 0.0;
}

bool scml_property_registry::hasProperty(PropertyType type, PropertyCType ctype, const std::string& scHierName, const std::string& name)
{
  (void) type;
  const string completeName( scHierName + '.' + name);

  switch (ctype) {
    case SIGNED_INTEGER:
      if (mSetProperties->hasIntProperty(completeName)) {
        return true;
      }
      break;
    case UNSIGNED_INTEGER:
      if (mSetProperties->hasUIntProperty(completeName)) {
        return true;
      }
      break;
    case INTEGER:
      if (mSetProperties->hasIntProperty(completeName) || mSetProperties->hasUIntProperty(completeName)) {
        return true;
      }
      break;
    case STRING:
      if (mSetProperties->hasStringProperty(completeName)) {
        return true;
      }
      break;
    case BOOL:
      if (mSetProperties->hasBoolProperty(completeName)) {
        return true;
      }
      break;
    case DOUBLE:
      if (mSetProperties->hasDoubleProperty(completeName)) {
        return true;
      }
      break;
    default:
      assert(0);
  }

  if( 0 != mCustomPropServer) {
    switch (ctype) {
      case SIGNED_INTEGER:
        if (mCustomPropServer->hasIntProperty(type, completeName)) {
          return true;
        }
        break;
      case UNSIGNED_INTEGER:
        if (mCustomPropServer->hasUIntProperty(type, completeName)) {
          return true;
        }
        break;
      case INTEGER:
        if (mCustomPropServer->hasIntProperty(type, completeName) || mCustomPropServer->hasUIntProperty(type, completeName)) {
          return true;
        }
        break;
      case STRING:
        if (mCustomPropServer->hasStringProperty(type, completeName)) {
          return true;
        }
        break;
      case BOOL:
        if (mCustomPropServer->hasBoolProperty(type, completeName)) {
          return true;
        }
        break;
      case DOUBLE:
        if (mCustomPropServer->hasDoubleProperty(type, completeName)) {
          return true;
        }
        break;
      default:
        assert(0);
    }
    if (mCustomPropServer->isFinal()) {
      return false;
    }
  }
  return false;
}


void scml_property_registry::setOutputStream(::std::ostream* output)
{
  mOs = output;
}

std::string scml_property_registry::getPropertyXmlFilePath() const
{
  return mXmlFilePath;
}  

void scml_property_registry::setXMLFile(const string& )
{
}

void scml_property_registry::setIntProperty(PropertyType /* type */, const std::string& scHierName, const std::string& name, long long value) {
  mSetProperties->setProperty(std::string( scHierName + '.' + name), value);
}

void scml_property_registry::setUIntProperty(PropertyType /* type */, const std::string& scHierName, const std::string& name, unsigned long long value) {
  mSetProperties->setProperty(std::string( scHierName + '.' + name), value);
}

void scml_property_registry::setBoolProperty(PropertyType /* type */, const std::string& scHierName, const std::string& name, bool value) {
  mSetProperties->setProperty(std::string( scHierName + '.' + name), value);
}

void scml_property_registry::setStringProperty(PropertyType /* type */, const std::string& scHierName, const std::string& name, const std::string& value) {
  mSetProperties->setProperty(std::string( scHierName + '.' + name), value);
}

void scml_property_registry::setDoubleProperty(PropertyType /* type */, const std::string& scHierName, const std::string& name, double value) {
  mSetProperties->setProperty(std::string( scHierName + '.' + name), value);
}

std::vector<std::string> scml_property_registry::getPropertyNames(const std::string& scHierName) const
{
  std::vector<std::string> result;
  (void) scHierName;

  return result;
}

scml_property_registry::PropertyCType scml_property_registry::getPropertyType( const std::string& scHierName, const std::string& name) const
{
  (void) scHierName;
  (void) name;

  return BOOL;
}
