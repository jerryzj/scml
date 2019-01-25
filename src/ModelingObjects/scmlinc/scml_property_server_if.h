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

#ifndef SCML_PROPERTY_SERVER_IF
#define SCML_PROPERTY_SERVER_IF

#include <string>
#include <scml2_logging/snps_vp_dll.h>
#include <scmlinc/scml_property_registry.h>

class SNPS_VP_API scml_property_server_if {
public:
  virtual long long getIntProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual unsigned long long getUIntProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual bool getBoolProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual std::string getStringProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual double getDoubleProperty(scml_property_registry::PropertyType type, const std::string & name);

  virtual bool hasIntProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual bool hasUIntProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual bool hasBoolProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual bool hasStringProperty(scml_property_registry::PropertyType type, const std::string & name);
  virtual bool hasDoubleProperty(scml_property_registry::PropertyType type, const std::string & name);

  virtual long long getIntProperty( const std::string & name);
  virtual unsigned long long getUIntProperty( const std::string & name);
  virtual bool getBoolProperty( const std::string & name);
  virtual std::string getStringProperty( const std::string & name);
  virtual double getDoubleProperty( const std::string & name);

  virtual bool hasIntProperty( const std::string & name);
  virtual bool hasUIntProperty( const std::string & name);
  virtual bool hasBoolProperty( const std::string & name);
  virtual bool hasStringProperty( const std::string & name);
  virtual bool hasDoubleProperty( const std::string & name);

  virtual bool isFinal() const;

protected:
  scml_property_server_if();
  virtual ~scml_property_server_if();

private: 
  // disable
  scml_property_server_if( const scml_property_server_if & );
  scml_property_server_if & operator= ( const scml_property_server_if & );
};

#endif
