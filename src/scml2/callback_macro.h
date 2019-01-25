/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_CALLBACK_MACRO_H
#define SCML2_CALLBACK_MACRO_H

#define SCML2_CALLBACK(func) this, &SC_CURRENT_USER_MODULE::func, #func

#endif
