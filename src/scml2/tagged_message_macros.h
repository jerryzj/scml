/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef TAGGED_MESSAGE_MACROS_INCLUDED
#define TAGGED_MESSAGE_MACROS_INCLUDED

#include <systemc>


namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

  enum eInfoTypes {
    TEST_INFO,
    GENERIC_INFO,
    FILE_OPEN,
    FILE_CLOSE,
    SOFTWARE_LOG,
    FUNCTIONAL_LOG,
    FUNCTIONAL_LOG_VERBOSE,
    FUNCTIONAL_LOG_INTERNAL,
    STATE_TRANSITION,
    CONFIGURATION_INFO
  };
  enum eWarningTypes {
    GENERIC_WARNING,
    SCML_IGNORED_CALL,
    FEATURE_NOT_MODELED,
    FEATURE_TBD,
    FILE_SW_IMAGE_OVERWRITE,
    ACCESS_IGNORED,
    ACCESS_INVALID_PERMISSION,
    UNDEFINED_WARNING,
    CONFIGURATION_WARNING,
    PROGRAMMING_WARNING
  };
  enum eErrorTypes { 
    GENERIC_ERROR,
    SCML_INVALID_API_USAGE,
    ACCESS_WRITE_RESERVED_VALUE,
    ACCESS_WRITE_UNDEFINED_VALUE,
    CONFIGURATION_ERROR,
    ACCESS_PERMISSION_CHECK_FAIL,
    ACCESS_WRITE_TO_READ_ONLY,
    ACCESS_READ_FROM_WRITE_ONLY,
    ACCESS_UNMAPPED_ADDRESS,
    FUNCTIONAL_ERROR,
    FILE_NOT_FOUND,
    FILE_FORMAT_ERROR,
    STATE_ILLEGAL_TRANSITION,
    STATE_ILLEGAL,
    UNDEFINED_ERROR,
    BUFFER_UNDERFLOW,
    BUFFER_OVERFLOW,
    PROGRAMMING_ERROR,
    FATAL_ERROR
  };
  
  enum eModelInternalTypes {
    SCML_CALLBACK_ENTRY = -12,
    SCML_CALLBACK_EXIT = -11,
    LEGACY_LOGGING = -10,
    LEVEL0 = 0,
    LEVEL1 = 1,
    LEVEL2 = 2,
    LEVEL3 = 3,
    LEVEL4 = 4,
    LEVEL5 = 5,
    LEVEL6 = 6
  };

  std::ostream& info(const sc_core::sc_module* mod, scml2::eInfoTypes type, tlm::tlm_generic_payload* trans = 0);
  std::ostream& warning(const sc_core::sc_module* mod, scml2::eWarningTypes type, tlm::tlm_generic_payload* trans = 0);
  std::ostream& error(const sc_core::sc_module* mod, scml2::eErrorTypes type, tlm::tlm_generic_payload* trans = 0);
  std::ostream& model_internal_log(const sc_core::sc_module* mod, scml2::eModelInternalTypes type, tlm::tlm_generic_payload* trans = 0);


}

#define SCML2_ASSERT(a) \
if (a) { \
} else \
  SCML2_ERROR(FATAL_ERROR)


// internal usage only
#define SCML2_INFO(type, ...) (std::cout)
#define SCML2_WARNING(type, ...) (std::cout)
#define SCML2_ERROR(type, ...) (std::cout)
#define SCML2_MODEL_INTERNAL(type, ...) (std::cout)

#define SCML2_INFO_TO(t, type, ...) ((void)t, std::cout)
#define SCML2_WARNING_TO(t, type, ...) ((void)t, std::cout)
#define SCML2_ERROR_TO(t, type, ...) ((void)t, std::cout)
#define SCML2_MODEL_INTERNAL_TO(t, type, ...) ((void)t, std::cout)

#define SCML2_LOG_INFO(t, s, type, ...) ((void)t, s)
#define SCML2_LOG_WARNING(t, s, type, ...) ((void)t, s)
#define SCML2_LOG_ERROR(t, s, type, ...)  ((void)t, s)
#define SCML2_LOG_MODEL_INTERNAL(t, s, type, ...)  ((void)t, s)


#endif // TAGGED_MESSAGE_MACROS_INCLUDED
