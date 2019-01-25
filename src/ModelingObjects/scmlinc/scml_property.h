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
 * Purpose of file : Header file for Properties
 *
 */

#ifndef SLS_SCML_PROPERTY_H
#define SLS_SCML_PROPERTY_H

#include <string>
#include <iostream>
#include <systemc.h>
#include <scml2_logging/snps_vp_dll.h>

/************************************************************************
 * scml_property_b non-templatized base-class
 */
class SNPS_VP_API scml_property_b
{
public:
  virtual ~scml_property_b();

  virtual int getIntValue() const;
  virtual unsigned int getUIntValue() const;
  virtual bool getBoolValue() const;
  virtual double getDoubleValue() const;
  virtual ::std::string getStringValue() const;
  
  virtual ::std::string getType() const;

  virtual void setIntValue(int value);
  virtual void setBoolValue(bool value);
  virtual void setDoubleValue(double value);
  virtual void setStringValue(const ::std::string &value);
  virtual void setUIntValue(unsigned int value);
  virtual void setLongLongValue(long long value);
  virtual void setULongLongValue(unsigned long long value);

  ::std::string getName() const;
  
  class SNPS_VP_API WrongTypeException {
  public:
    WrongTypeException(const ::std::string &badType,
                       const ::std::string &goodType);
    ::std::string getGoodType() const;
    ::std::string getBadType() const;
    ::std::string getDescription() const;
  private:
    ::std::string mBadType;
    ::std::string mGoodType;
    
  };

protected:
  explicit scml_property_b(const ::std::string& name);
  
protected:
  ::std::string mName;
  sc_module *mModule;
};


/************************************************************************
 * scml_property_base class
 */

template<typename T>
class scml_property_base : public scml_property_b
{
public:
  typedef T value_type;
  typedef scml_property_base<value_type> this_type;
  typedef this_type* this_pointer_type;
  typedef this_type& this_reference_type;
  typedef scml_property_b::WrongTypeException WrongTypeException;
  
  // T variable behaviour
  this_reference_type operator += ( value_type );
  this_reference_type operator -= ( value_type );
  this_reference_type operator /= ( value_type );
  this_reference_type operator *= ( value_type );
  this_reference_type operator %= ( value_type );
  this_reference_type operator ^= ( value_type );
  this_reference_type operator &= ( value_type );
  this_reference_type operator |= ( value_type );
  this_reference_type operator <<= ( value_type );
  this_reference_type operator >>= ( value_type );

  // decrement & increment
  this_reference_type operator -- ();     // prefix
  value_type operator -- ( int); // postfix
  this_reference_type operator ++ ();     // prefix
  value_type operator ++ ( int); // postfix  
  
  // T behavior
  operator T() const;
  const T& getValue() const;
  T& getValue();
    
protected:
  explicit scml_property_base(const ::std::string& name);
  virtual ~scml_property_base();

protected:
  value_type mValue;
  
  friend class PropertyTest;
};

/************************************************************************
 * scml_property class
 */

#ifdef _WIN32
template<typename T> class scml_property;
#else
template<typename T> class SNPS_VP_API scml_property;
#endif

/************************************************************************
 * specializations for supported types:
 */

/************************************************************************
 * int
 */

template<>
class scml_property<int> : public scml_property_base<int>
{
public:

  typedef int value_type;
  typedef scml_property<int> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;

  SNPS_VP_API this_reference_type operator=( const scml_property<int>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);
  SNPS_VP_API virtual ~scml_property();
  
  SNPS_VP_API int getIntValue() const;
  SNPS_VP_API void setIntValue(int value);
  SNPS_VP_API ::std::string getType() const; 
};


/************************************************************************
 * unsigned int
 */

template<>
class scml_property<unsigned int> : public scml_property_base<unsigned int>
{
public:

  typedef unsigned int value_type;
  typedef scml_property<unsigned int> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;

  SNPS_VP_API this_reference_type operator=( const scml_property<unsigned int>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);  
  SNPS_VP_API virtual ~scml_property();
  
  SNPS_VP_API unsigned int getUIntValue() const;
  SNPS_VP_API void setUIntValue(unsigned int value);
  SNPS_VP_API ::std::string getType() const; 
};

/************************************************************************
 * long long
 */

template<>
class scml_property<long long> : public scml_property_base<long long>
{
public:

  typedef long long value_type;
  typedef scml_property<long long> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;

  SNPS_VP_API this_reference_type operator=( const scml_property<long long>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);
  SNPS_VP_API virtual ~scml_property();
  
  SNPS_VP_API long long getLongLongValue() const;
  SNPS_VP_API void setLongLongValue(long long value);
  SNPS_VP_API ::std::string getType() const; 
};


/************************************************************************
 * unsigned long long
 */

template<>
class scml_property<unsigned long long> : public scml_property_base<unsigned long long>
{
public:

  typedef unsigned long long value_type;
  typedef scml_property<unsigned long long> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;

  SNPS_VP_API this_reference_type operator=( const scml_property<unsigned long long>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);  
  SNPS_VP_API virtual ~scml_property();
  
  SNPS_VP_API unsigned long long getULongLongValue() const;
  SNPS_VP_API void setULongLongValue(unsigned long long value);
  SNPS_VP_API ::std::string getType() const; 
};


/************************************************************************
 * bool
 */

template<>
class scml_property<bool> : public scml_property_base<bool>
{
public:

  typedef bool value_type;
  typedef scml_property<bool> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;
  
  SNPS_VP_API this_reference_type operator=( const scml_property<bool>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);  
  SNPS_VP_API virtual ~scml_property();

  SNPS_VP_API bool getBoolValue() const;
  SNPS_VP_API void setBoolValue(bool value);
  SNPS_VP_API ::std::string getType() const; 
};

/************************************************************************
 * double
 */

template<>
class scml_property<double> : public scml_property_base<double>
{
public:

  typedef double value_type;
  typedef scml_property<double> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException; 
  
  SNPS_VP_API this_reference_type operator=( const scml_property<double>& );
  SNPS_VP_API this_reference_type operator=( value_type );

  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);
  SNPS_VP_API virtual ~scml_property();

  SNPS_VP_API double getDoubleValue() const;
  SNPS_VP_API void setDoubleValue(double value);
  SNPS_VP_API ::std::string getType() const;
};

/************************************************************************
 * ::std::string
 */

template<>
class scml_property< ::std::string> : public scml_property_base< ::std::string>
{
public:

  typedef ::std::string value_type;
  typedef scml_property< ::std::string> this_type;
  typedef this_type& this_reference_type;
  typedef this_type::WrongTypeException WrongTypeException;
  
  SNPS_VP_API this_reference_type operator=( const scml_property< ::std::string>& );
  SNPS_VP_API this_reference_type operator=( value_type );
  
  SNPS_VP_API explicit scml_property(const ::std::string& name);
  SNPS_VP_API scml_property(const ::std::string& name, value_type default_value);  
  SNPS_VP_API virtual ~scml_property();
  
  SNPS_VP_API ::std::string getStringValue() const;
  SNPS_VP_API void setStringValue(const ::std::string &value);
  SNPS_VP_API ::std::string getType() const;  
};

/************************************************************************
 * ::std::string operators
 */

#define SLS_STRING_OPERATOR_DECLARATION(otype)\
SNPS_VP_API bool operator otype (const scml_property< ::std::string>& lhs, \
				 const scml_property< ::std::string>& rhs); \
SNPS_VP_API bool operator otype (const ::std::string& lhs,		\
				 const scml_property< ::std::string>& rhs); \
SNPS_VP_API bool operator otype (const scml_property< ::std::string>& lhs, \
				 const ::std::string& rhs);		\

SLS_STRING_OPERATOR_DECLARATION(==);
SLS_STRING_OPERATOR_DECLARATION(!=);
SLS_STRING_OPERATOR_DECLARATION(<);
SLS_STRING_OPERATOR_DECLARATION(<=);
SLS_STRING_OPERATOR_DECLARATION(>);
SLS_STRING_OPERATOR_DECLARATION(>=);

#undef SLS_STRING_OPERATOR_DECLARATION

SNPS_VP_API ::std::ostream& operator<<(::std::ostream& os, 
				       const scml_property< ::std::string>& str);
SNPS_VP_API ::std::istream& operator>>(::std::istream& os, 
				       scml_property< ::std::string>& str);
SNPS_VP_API ::std::string operator+ (const scml_property< ::std::string>& lhs, 
				     const scml_property< ::std::string>& rhs);
SNPS_VP_API ::std::string operator+ (const scml_property< ::std::string>& lhs, 
				     const ::std::string& rhs);
SNPS_VP_API ::std::string operator+ (const ::std::string& lhs, 
				     const scml_property< ::std::string>& rhs);

/************************************************************************
 * scml_property_base INLINE IMPLEMENTATIONS
 */

template<typename T>
inline scml_property_base<T>::scml_property_base(const ::std::string& name) 
  : scml_property_b(name), mValue()
{
}

template<typename T>
inline scml_property_base<T>::~scml_property_base()
{
}

#define scml_property_ASSIGNMENT_IMPLEMENTATION(otype)\
template<typename T>\
inline typename scml_property_base<T>::this_reference_type \
scml_property_base<T>::operator otype (value_type v)\
{\
  mValue otype v;\
  return *this;\
}\

scml_property_ASSIGNMENT_IMPLEMENTATION(+=);
scml_property_ASSIGNMENT_IMPLEMENTATION(-=);
scml_property_ASSIGNMENT_IMPLEMENTATION(/=);
scml_property_ASSIGNMENT_IMPLEMENTATION(*=);
scml_property_ASSIGNMENT_IMPLEMENTATION(%=);
scml_property_ASSIGNMENT_IMPLEMENTATION(^=);
scml_property_ASSIGNMENT_IMPLEMENTATION(|=);
scml_property_ASSIGNMENT_IMPLEMENTATION(&=);
scml_property_ASSIGNMENT_IMPLEMENTATION(<<=);
scml_property_ASSIGNMENT_IMPLEMENTATION(>>=);

#undef scml_property_ASSIGNMENT_IMPLEMENTATION

// decrement & increment
template<typename T>
inline typename scml_property_base<T>::this_reference_type 
scml_property_base<T>::operator -- ()     // prefix
{
  --mValue;
  return *this;
}

template<typename T>
inline typename scml_property_base<T>::value_type 
scml_property_base<T>::operator -- ( int) // postfix
{
  value_type oldVal = mValue;
  mValue--;
  return oldVal;
}

template<typename T>
inline typename scml_property_base<T>::this_reference_type 
scml_property_base<T>::operator ++ ()     // prefix
{
  ++mValue;
  return *this;
}

template<typename T>
inline typename scml_property_base<T>::value_type 
scml_property_base<T>::operator ++ ( int) // postfix
{
  value_type oldVal = mValue;
  mValue++;
  return oldVal;
}

template<typename T>
inline scml_property_base<T>::operator T() const
{
  return mValue;
}

template<typename T>
inline const T& scml_property_base<T>::getValue() const
{
  return mValue;
}

template<typename T>
inline T& scml_property_base<T>::getValue()
{
  return mValue;
}

#endif //SCML_PROPERTY_H
