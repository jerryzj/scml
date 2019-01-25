/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML_INMEMORY_PROPERTY_SERVER_H
#define SCML_INMEMORY_PROPERTY_SERVER_H

#include <map>
#include <string>
#include <scmlinc/scml_property_server_if.h>
#include <scml2_logging/snps_vp_dll.h>

class SNPS_VP_API scml_inmemory_property_server : public scml_property_server_if {
  public:
    virtual long long getIntProperty( const std::string& name);
    virtual unsigned long long getUIntProperty( const std::string& name);
    virtual bool getBoolProperty( const std::string& name);
    virtual std::string getStringProperty( const std::string& name);
    virtual double getDoubleProperty( const std::string& name);
    virtual bool hasIntProperty( const std::string& name);
    virtual bool hasUIntProperty( const std::string& name);
    virtual bool hasBoolProperty( const std::string& name);
    virtual bool hasStringProperty( const std::string& name);
    virtual bool hasDoubleProperty( const std::string& name);
    void setProperty( const std::string& name, long long value);
    void setProperty( const std::string& name, unsigned long long value);
    void setProperty( const std::string& name, bool value);
    void setProperty( const std::string& name, const std::string& value);
    void setProperty( const std::string& name, double value);

  private:
    std::map<std::string, long long> intProperties;
    std::map<std::string, unsigned long long> unsignedIntProperties;
    std::map<std::string, bool> boolProperties;
    std::map<std::string, double> doubleProperties;
    std::map<std::string, std::string> stringProperties;
};

#endif
