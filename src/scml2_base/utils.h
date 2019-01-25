/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_UTILS_H_INCLUDED
#define SCML2_BASE_UTILS_H_INCLUDED

#include <scml2_base/snps_vp_dll.h>

#if defined(_WIN32) && _MSC_VER > 1700 
#define SNPS_SLS_VP_BASE
#endif

#ifndef _WIN32
#define GCC_VERSION (__GNUC__ * 10000 \
                   + __GNUC_MINOR__ * 100 \
                   + __GNUC_PATCHLEVEL__)
#if GCC_VERSION > 40702
#define SNPS_SLS_VP_BASE
#endif // GCC_VERSION > 40702
#endif // _WIN32

#endif // SCML2_BASE_UTILS_H_INCLUDED
