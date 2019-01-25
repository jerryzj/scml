/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TYPES_H
#define SCML2_TYPES_H

namespace scml2 {

enum SyncType { AUTO_SYNCING, SELF_SYNCING, NEVER_SYNCING };

enum access_type { ACCESS_TYPE_UNKNOWN = 0,
                   ACCESS_TYPE_READ    = 1,
                   ACCESS_TYPE_WRITE   = 2,
                   ACCESS_TYPE_ACCESS  = 3 };
enum access_mode { ACCESS_MODE_UNKNOWN = 0,
                   ACCESS_MODE_REGULAR = 1,
                   ACCESS_MODE_DEBUG   = 2,
                   ACCESS_MODE_ACCESS  = 3 };
enum access_restriction_result { RESTRICT_NO_ERROR = 0,
				 RESTRICT_ERROR = 1 };
  
}

#endif
