
#include "cstyxc.h"
#include <stddef.h>

EXPORT void cstyx_connection_connect(cstyx_connection_t* pConnection, const cstyx_connection_vptbl_t* pVpTbl)
{
    pConnection->pVpTbl = pVpTbl;
}

EXPORT void cstyx_connection_disconnect(cstyx_connection_t* pConnection)
{
    pConnection->pVpTbl->disconnect(pConnection);
    pConnection->pVpTbl = NULL;
}

