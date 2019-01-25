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
 * Purpose of file : Header file for scml_property_registry
 *
 */

#ifndef SCML_PROPERTY_REGISTRY_H
#define SCML_PROPERTY_REGISTRY_H

#include <string>
#include <iostream>
#include <vector>
#include <scml2_logging/snps_vp_dll.h>

class scml_property_server_if;
class scml_inmemory_property_server;


class SNPS_VP_API scml_property_registry
{


public:
  static scml_property_registry& inst();
  ~scml_property_registry ();
  
  enum PropertyType {
    GLOBAL,
    CONSTRUCTOR,
    MODULE,
    PORT,
    PROTOCOL,
    PORT_ARRAY
  };
  
  // PropertyAPI
  long long getIntProperty(PropertyType type, const std::string& scHierName, 
  		           const std::string& name);
  unsigned long long getUIntProperty(PropertyType type, 
  				     const std::string& scHierName, 
  		                     const std::string& name);

  bool getBoolProperty(PropertyType type, const std::string& scHierName, 
   		       const std::string& name);
  std::string getStringProperty(PropertyType type, 
  				  const std::string& scHierName, 
  			          const std::string& name);
  double getDoubleProperty(PropertyType type, const std::string& scHierName, 
  			   const std::string& name);

  bool setCustomPropertyServer( scml_property_server_if * );
  scml_property_server_if * getCustomPropertyServer() const;

  void setXMLFile(const std::string& filePath);
  
  // Default is cerr
  void setOutputStream(std::ostream* output);

  enum PropertyCType {
    INTEGER, STRING, BOOL, DOUBLE, UNSIGNED_INTEGER, SIGNED_INTEGER
  };
  
  bool hasProperty(PropertyType type, PropertyCType ctype, 
    		   const std::string& scHierName, const std::string& name);
		   
  std::string getPropertyXmlFilePath() const;


  // Set properties
  void setIntProperty(PropertyType type, const std::string& scHierName, const std::string& name, long long value);
  void setUIntProperty(PropertyType type, const std::string& scHierName, const std::string& name, unsigned long long value);
  void setBoolProperty(PropertyType type, const std::string& scHierName, const std::string& name, bool value);
  void setStringProperty(PropertyType type, const std::string& scHierName, const std::string& name, const std::string& value);
  void setDoubleProperty(PropertyType type, const std::string& scHierName, const std::string& name, double value);

  std::string getTopLevelName() const;

  std::vector<std::string> getPropertyNames(const std::string& scHierName) const;
  PropertyCType getPropertyType( const std::string& scHierName, const std::string& name) const;
private:	
  std::ostream* mOs;
  std::string mXmlFilePath;
  scml_property_server_if * mCustomPropServer;
  scml_inmemory_property_server* mSetProperties;

private:

  scml_property_registry ();
  // disable assignment operator
  scml_property_registry& operator= (const scml_property_registry&);
  // disable copy constructor
  scml_property_registry(const scml_property_registry&);
};


#endif // SCML_PROPERTY_REGISTRY_H
