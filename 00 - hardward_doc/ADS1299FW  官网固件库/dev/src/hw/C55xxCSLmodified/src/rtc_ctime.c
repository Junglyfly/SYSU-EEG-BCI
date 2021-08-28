/*****************************************************************************/
/* ctime     v#####                                                          */
/* Copyright (c) 1999@%%%% Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _CTIME
#define _CTIME

#include <csl_chiphal.h>

#if (_RTC_SUPPORT)

#define _CPP_STYLE_HEADER
#include <csl_time.h>
#undef _CPP_STYLE_HEADER

#ifdef __embedded_cplusplus
//----------------------------------------------------------------------------
// We cannot expect the embedded C++ user to use std namespace. So we include
// the using directive when compiled under embedded C++.
//----------------------------------------------------------------------------
using namespace std;
#endif /* __embedded_cplusplus */

#endif  /* _RTC_SUPPORT */
#endif  /* #ifndef _CTIME */

