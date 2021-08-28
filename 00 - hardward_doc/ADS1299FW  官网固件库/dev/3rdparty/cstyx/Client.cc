
#include "pathutil.hh"
#include "Error.hh"
#include "Dirent.hh"
#include "File.hh"
#include "styxmsg.hh"
#include "Connection.hh"
#include "Client.hh"
#include <stdlib.h>

using namespace cstyx;
using namespace std;

namespace cstyx {

static const int rootfid=1;
static const int cwdfid=2;
static const int firstfid=10;
static const int lastfid=32760;

};

/* tmsg

- marshals f into a byte array
- sends f using connection
- receives a new array using connection (may do multiple times)
- unmarshals connection into f (reuses f)
- returns false if there is an error
*/
static bool tmsg(Connection& c, fcall_t& f)
{
    u32 l=cstyx_sizeS2M(&f);
    std::string buf;
    
    cstyx_convS2M(&f,buf);
    c.tx(buf,l);
    l=4;
    const int rxSizeStatus = c.rx(f.data,l);
    if (CSTYX_STATUS_OK != rxSizeStatus) {
        return false;
    }
    l=(f.data[0]&0xff)|(f.data[1]<<8);
    if (l<4) return false;
    u32 l2=l-4;
    c.rx(f.data,l2);
    c.flushInput();
    l2=cstyx_convM2S(const_cast<char*>(f.data.data()),static_cast<u16>(l),&f);
    if (!l2) return false;
    return (f.type != Rerror);
}

bool Client::tattach(u32 fid)
{
    fcall_t f;

    f.type=Tattach;
    f.fid=fid;
    f.tag=getTag();
    f.u.attach.afid=0xffffffff;
    f.u.attach.uname="nobody";
    f.u.attach.aname="nobody";
    if (false == tmsg(*_pConn,f)) return false;
    return (Rattach == f.type); // expect an Rattach response
}

bool Client::twalk(u32 fid, u32 newfid, const std::vector<std::string>& path)
{
    fcall_t f;

    f.type=Twalk;
    f.fid=fid;
    f.u.twalk.newfid=newfid;
    f.u.twalk.nwname=path.size();
    int i=0;
    vector<string>::const_iterator ii;
    for (ii=path.begin();ii!=path.end();++ii)
        f.u.twalk.wname[i++]=const_cast<char*>(ii->c_str());
    if (false == tmsg(*_pConn,f)) return false;
    return (Rwalk == f.type);
}

bool Client::topen(u32 fid, u8 mode)
{
    fcall_t f;

    f.type=Topen;
    f.fid=fid;
    f.u.create.mode=mode;
    if (false == tmsg(*_pConn,f)) return false;
    return (Ropen == f.type);
}

bool Client::tclunk(u32 fid)
{
    fcall_t f;

    f.type=Tclunk;
    f.fid=fid;
    if (false == tmsg(*_pConn,f)) return false;
    return (Rclunk == f.type);
}

bool Client::tstat(u32 fid, Dirent& dirent)
{
    fcall_t f;

    f.type=Tstat;
    f.fid=fid;
    if (false == tmsg(*_pConn,f)) return false;
    if (Rstat != f.type) {
        return false;
    }
    dirent = Dirent(f);
    return true;
}

bool Client::tread(u32 fid, std::string& buf, u16& len, u32 pos)
{
    fcall_t f;
    
    f.type=Tread;
    f.fid=fid;
    f.u.rw.offset=pos;
    f.u.rw.count=len;
    if (false == tmsg(*_pConn,f)) {
        return false;
    }
    len=static_cast<u16>(f.u.rw.count);
    if (len) {
        buf.append(const_cast<const char*>(f.u.rw.data),len);
    }
    return true;
}

bool Client::twrite(u32 fid, const std::string& buf, u16& len, u32 pos)
{
    fcall_t f;
    
    f.type=Twrite;
    f.fid=fid;
    f.u.rw.offset=pos;
    f.u.rw.count=len;
    f.u.rw.data=const_cast<char*>(buf.data());
    if (false == tmsg(*_pConn,f)) {
        return false;
    }
    else {
      len = static_cast<u16>(f.u.rw.count);
    }
    return true;
}

Client::Client(Connection* c)
    : _pConn(c), _nextfid(firstfid), _connected(false),
    _pRoot(NULL), _pCwd(NULL)
{
    _pRoot = new Fid(*this,"/",1);
    _pCwd  = new Fid(*this,"/",2);
    connect();
}

Client::~Client()
{
    if (NULL != _pRoot) { delete _pRoot; }
    if (NULL != _pCwd)  { delete _pCwd;  }
}

u32 Client::getFid()
{
    int f=_nextfid++;
    if (_nextfid>lastfid) {
        _nextfid=firstfid;
    }
    return f;
}

bool Client::twalk(u32 fid, u32 newfid)
{
    vector<string> pl;
    return twalk(fid,newfid,pl);
}

bool Client::connect()
{
    if (connected()) { return true; }
    for (int i=0; i<5; ++i) {
        if (false == tattach(_pRoot->fid())) { continue; }
        if (twalk(_pRoot->fid(),_pCwd->fid())) {
            _connected = true;
            break;
        }
    }
    return _connected;
}

bool Client::setCWD(const std::string& p)
{
    std::string old=_pCwd->path();
    if (false == _pCwd->walk(p)) {
        return false;
    }
    if (!_pCwd->isdir()) {
        // Ignore the return from walk because we've already failed
        (void) _pCwd->walk(old);
        return false;
    }
    return true;
}

bool Client::isdir(const std::string &p)
{
    Fid f(*this,p);
    return f.isdir();
}

bool Client::exists(const std::string &p)
{
    Fid f(*this,p);
    return true;
}

int Client::write(const std::string& path, const std::string& data)
{
    File f(*this,path);
    return f.write(data);
}

int Client::readAll(const std::string& path, std::string& buf, int lim)
{
    File f(*this,path);
    return f.readAll(buf,lim);
}
