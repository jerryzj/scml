/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_VECTOR_H
#define SCML2_VECTOR_H

#include <vector>
#include <sstream>
#include <systemc>

namespace scml2 {

/*
 * This class is an enhancement of sc_core::sc_vector in the sense that the vector itself is not
 *  named with the same basename as the vector elements. This avoids the off-by-one element name 
 *  numbering, which sc_vector currently honors.
 */
template <typename T>
class vector : public sc_core::sc_object
{
public:
 vector(const char* name) : sc_core::sc_object(sc_core::sc_gen_unique_name((name + std::string("_vector")).c_str(), true)), mElementName(name) {
  }

 vector(const char* name, size_t size) : sc_core::sc_object(sc_core::sc_gen_unique_name((name + std::string("_vector")).c_str(), true)), mElementName(name) {
    init(size);
  }

  template<typename Creator>
    vector(const char* name, size_t size, Creator creator) : sc_core::sc_object(sc_core::sc_gen_unique_name((name + std::string("_vector")).c_str(), true)), mElementName(name) {
    init(size, creator);
  }

  ~vector() {
    for(size_t i = 0; i < mElements.size(); ++i) {
      delete mElements[i];
    }
    mElements.clear();
  }

  const T& operator[](size_t i) const {
    return *(mElements[i]);
  }

  T& operator[](size_t i) {
    return *(mElements[i]);
  }

  size_t size() const {
    return mElements.size();
  }

  void init(size_t size) {
    init(size, &vector<T>::create_element);
  }

  template<typename Creator>
  void init(size_t size, Creator creator) {
    mElements.resize(size);
    for (size_t i = 0; i < size; ++i) {
      mElements[i] = creator(sc_core::sc_gen_unique_name(mElementName.c_str()), i);
    }
  }

private:
  static T* create_element(const char* prefix, size_t /*i*/) {
    return new T(prefix);
  }

private:
  std::string mElementName;
  std::vector<T*> mElements;
};

}

#endif
