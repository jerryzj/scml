@REM *************************************************************************
@REM Copyright 1996-2017 Synopsys, Inc.
@REM
@REM This Synopsys software and all associated documentation are proprietary
@REM to Synopsys, Inc. and may only be used pursuant to the terms and
@REM conditions of a written license agreement with Synopsys, Inc.
@REM All other use, reproduction, modification, or distribution of the
@REM Synopsys software or the associated documentation is strictly prohibited.
@REM *************************************************************************
::
:: batch file to copy header files 
::
@ECHO OFF 
echo Copying scml2.h to ..\src\scml2
copy scml2.h ..\src\scml2
echo Copying scml2_logging.h to ..\src\scml2_logging
copy scml2_logging.h ..\src\scml2_logging
echo Copying scml2_base.h to ..\src\scml2_base
copy scml2_base.h ..\src\scml2_base
