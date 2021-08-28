
#ifndef GUARD_connection_hh
#define GUARD_connection_hh

#include "cstyx.h"
#include "cstyxc.h"
#include <string>

namespace cstyx {

//! Stream connection
/*! Connection objects are used by Clients to communicate with Styx servers.

Connection is an abstract base.  The cstyx library does not include any real
Connection subclasses.  These are available in other libraries, such as
cstyx_local and cstyx_usb.

Connection subclasses must establish the connection at construction time, 
and disconnect at destruction time.  If the connection cannot be
established, the subclass must return false from IsConnected.

It may be possible for a Connection to lose connectivity.  If this
happens, isConnected() must return false, and rx() and tx() must return
failure codes.

A Connection cannot be re-established.  If a Connection loses connectivity, 
it must be discarded and a new Connection obtained.  

Connections are not copyable.

Connection libraries and Connection subclasses may include additional
methods for obtaining a Connection. Because of the wide range of possible
connections and ways of obtaining them, these methods cannot practically be
standardised.
*/
class Connection {
private:
    Connection(const Connection&);
    const Connection& operator=(const Connection&);

public:
    Connection(cstyx_connection_t* pConnection) : m_pConnection(pConnection) {}
    virtual ~Connection()
    {
        m_pConnection->pVpTbl->disconnect(m_pConnection);
    }

    virtual const std::string name() const { return ""; }

    // WARNING: Inefficient interface as requires copy/allocation
    virtual cstyx_status_t rx(std::string& buf, u32& len)
    {
        buf.resize(len);
        cstyx_status_t status = m_pConnection->pVpTbl->recv(m_pConnection, (u8*) &buf[0], &len);
        buf.resize(len);
        return status;
    }

    virtual cstyx_status_t tx(const std::string& buf, u32& len)
    {
        return m_pConnection->pVpTbl->send(m_pConnection, (const u8*) buf.c_str(), &len);
    }

    virtual void flushInput()
    {
        return m_pConnection->pVpTbl->flushsend(m_pConnection);
    }

private:
    cstyx_connection_t* m_pConnection;
};

};

#endif
