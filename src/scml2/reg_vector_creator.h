/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_REG_VECTOR_CREATOR_H
#define SCML2_REG_VECTOR_CREATOR_H

#include <scml2/reg.h>

namespace scml2 {

template <typename DT> class memory;
template <typename DT> class memory_alias;

template <typename DT, typename T = scml2::reg<DT> >
class reg_vector_creator
{
public:
  reg_vector_creator(scml2::memory<DT>& parent, size_t offset = 0, size_t size = 1) : mParent(parent), mOffset(offset), mSize(size) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize);
  }

private:
  scml2::memory<DT>& mParent;
  size_t mOffset;
  size_t mSize;
};

template <typename DT, typename T = scml2::reg<DT> >
class reg_alias_vector_creator
{
public:
  reg_alias_vector_creator(scml2::memory_alias<DT>& parent, size_t offset = 0, size_t size = 1) : mParent(parent), mOffset(offset), mSize(size) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize);
  }

private:
  scml2::memory_alias<DT>& mParent;
  size_t mOffset;
  size_t mSize;
};

template <typename DT, typename T = scml2::reg<DT> >
class reg_vector_creator_base
{
public:
  reg_vector_creator_base(scml2::toplevel_memory_base& parent, size_t offset = 0, size_t size = 1) : mParent(parent), mOffset(offset), mSize(size) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize);
  }

private:
  scml2::toplevel_memory_base& mParent;
  size_t mOffset;
  size_t mSize;
};

template <typename DT, typename T, typename U>
class reg_alias_vector_creator_with_extra_parameter
{
public:
  reg_alias_vector_creator_with_extra_parameter(scml2::memory_alias<DT>& parent, size_t offset, size_t size, U& parameter) : mParent(parent), mOffset(offset), mSize(size), mParameter(parameter) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize, mParameter);
  }

private:
  scml2::memory_alias<DT>& mParent;
  size_t mOffset;
  size_t mSize;
  U& mParameter;
};

template <typename DT, typename T, typename U>
class reg_vector_creator_with_extra_parameter
{
public:
  reg_vector_creator_with_extra_parameter(scml2::memory<DT>& parent, size_t offset, size_t size, U& parameter) : mParent(parent), mOffset(offset), mSize(size), mParameter(parameter) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize, mParameter);
  }

private:
  scml2::memory<DT>& mParent;
  size_t mOffset;
  size_t mSize;
  U& mParameter;
};

template <typename DT, typename T, typename U>
class reg_vector_creator_with_extra_parameter_base
{
public:
  reg_vector_creator_with_extra_parameter_base(scml2::toplevel_memory_base& parent, size_t offset, size_t size, U& parameter) : mParent(parent), mOffset(offset), mSize(size), mParameter(parameter) {
  }

  T* operator()(const char* prefix, size_t i) {
    return new T(prefix, mParent, mOffset + i*mSize, mParameter);
  }

private:
  scml2::toplevel_memory_base& mParent;
  size_t mOffset;
  size_t mSize;
  U& mParameter;
};

template <typename DT>
inline reg_vector_creator<DT>
create_reg_vector_creator(scml2::memory<DT>& parent, size_t offset = 0, size_t size = 1) {
  return reg_vector_creator<DT>(parent, offset, size);
}

template <typename DT>
inline reg_alias_vector_creator<DT>
create_reg_alias_vector_creator(scml2::memory_alias<DT>& parent, size_t offset = 0, size_t size = 1) {
  return reg_alias_vector_creator<DT>(parent, offset, size);
}

template <typename DT>
inline reg_vector_creator_base<DT>
create_reg_vector_creator(scml2::toplevel_memory_base& parent, size_t offset = 0, size_t size = 1) {
  return reg_vector_creator_base<DT>(parent, offset, size);
}

template <typename DT, typename T>
inline reg_vector_creator<DT, T>
  create_reg_vector_creator(scml2::memory<DT>& parent, size_t offset = 0, size_t size = 1) {
  return reg_vector_creator<DT, T>(parent, offset, size);
}

template <typename DT, typename T>
inline reg_alias_vector_creator<DT, T>
  create_reg_alias_vector_creator(scml2::memory_alias<DT>& parent, size_t offset = 0, size_t size = 1) {
  return reg_alias_vector_creator<DT, T>(parent, offset, size);
}

template <typename DT, typename T>
inline reg_vector_creator_base<DT, T>
  create_reg_vector_creator(scml2::toplevel_memory_base& parent, size_t offset = 0, size_t size = 1) {
  return reg_vector_creator_base<DT, T>(parent, offset, size);
}

template <typename DT, typename T, typename U>
inline reg_vector_creator_with_extra_parameter<DT, T, U>
create_reg_vector_creator(scml2::memory<DT>& parent, size_t offset, size_t size, U& u) {
  return reg_vector_creator_with_extra_parameter<DT, T, U>(parent, offset, size, u);
}

template <typename DT, typename T, typename U>
inline reg_alias_vector_creator_with_extra_parameter<DT, T, U>
create_reg_alias_vector_creator(scml2::memory_alias<DT>& parent, size_t offset, size_t size, U& u) {
  return reg_alias_vector_creator_with_extra_parameter<DT, T, U>(parent, offset, size, u);
}

template <typename DT, typename T, typename U>
inline reg_vector_creator_with_extra_parameter_base<DT, T, U>
create_reg_vector_creator(scml2::toplevel_memory_base& parent, size_t offset, size_t size, U& u) {
  return reg_vector_creator_with_extra_parameter_base<DT, T, U>(parent, offset, size, u);
}

}

#endif
