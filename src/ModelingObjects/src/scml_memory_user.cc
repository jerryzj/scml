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
// Modifications : 
//
// Purpose of file : implementation of class scml_memory_user
//
*/

#include "scml_memory_user.h"

scml_memory_user & scml_memory_user::instance() 
{
  static scml_memory_user inst;
  return inst;  
}

scml_memory_user::scml_memory_user() :
  mLastTopLevelAddress( 0),
  mLastUserId( 0),
  mLastMemory(0),
  mLastBitfield(0),
  mDebug( false)
{
}

scml_memory_user::~scml_memory_user()
{
}

