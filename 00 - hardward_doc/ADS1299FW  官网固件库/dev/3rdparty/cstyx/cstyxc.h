
#ifndef GUARD_cstyxc_c_h
#define GUARD_cstyxc_c_h

//:wrap=soft:

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

/*! Status code for success/failure results */
typedef int cstyx_status_t;

#define CSTYX_STATUS_OK 0
#define CSTYX_STATUS_ERROR -1

typedef struct cstyx_connection_t cstyx_connection_t;

/*!
 * The vptbl for a cstyx_connection_t
 */
typedef struct cstyx_connection_vptbl_t
{
    /*! Sends data to the remote peer */
    cstyx_status_t (*send) (cstyx_connection_t* pConnection, const u8* pBuffer, u32* pBufferSize);
    /*! Receives data from the remote peer */
    cstyx_status_t (*recv) (cstyx_connection_t* pConnection, u8* pBuffer, u32* pBufferSize);
    /*! Flushes the input/send side of the connection */
    void (*flushsend) (cstyx_connection_t* pConnection);
    /*! Disconnects from a vfstl device. */
    void (*disconnect) (cstyx_connection_t* pConnection);
} cstyx_connection_vptbl_t;

/*! A cstyx connection is an abstract interface to either a USB Device
 * or alternate remote eStyx/Styx server
 */
struct cstyx_connection_t
{
    const cstyx_connection_vptbl_t* pVpTbl;
};

//! Create a connection
EXPORT void cstyx_connection_connect(cstyx_connection_t* pConnection, const cstyx_connection_vptbl_t* pVpTbl);

//! Destroy/dispose of a connection
EXPORT void cstyx_connection_disconnect(cstyx_connection_t* pConnection);

#ifdef __cplusplus
}
#endif

#endif

