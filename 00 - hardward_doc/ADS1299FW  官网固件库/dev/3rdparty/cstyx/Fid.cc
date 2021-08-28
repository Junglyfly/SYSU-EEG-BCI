
#include "Fid.hh"
#include "Client.hh"
#include "Error.hh"

using namespace cstyx;

bool Fid::walk(const std::string& p, u32 fid, std::string& newpath)
{
    u32 walkfid;
    
    if (!p.size()) return true;
    walkfid=(p[0]=='/')?_client._pRoot->fid():_client._pCwd->fid();
    std::vector<std::string> pl;
    pathutil::split(p,pl);
    if (false == _client.twalk(walkfid,fid,pl)) return false;
    if (p[0]!='/')
        newpath=_client._pCwd->path()+"/"+p;
    pathutil::normalise(newpath);
    return true;
}

bool Fid::walk(const std::string& s)
{
    for (int i=0;i<10;++i) {
        if (walk(s,_fid,_path)) return true;
    }
    return false;
}

Fid::Fid(const Fid& f)
    : _client(f._client), _path(f._path), _open(false)
{
    for (int i=0;i<10;++i) {
        _fid=_client.getFid();
        // TODO: How to handle errors from twalk?
        if (_client.twalk(f._fid,_fid)) return;
    }
}

Fid::Fid(Client& c, const std::string& s)
    : _client(c), _path(s), _fid(c.getFid()), _open(false)
{
    if (_path.size()==0) {
      // TODO: How to handle errors from cwd?
      (void) c.cwd(_path);
    }
    // TODO: How to handle errors from walk?
    (void) walk(_path);
}

// private constructor
Fid::Fid(Client& c, const std::string& s, u32 f)
    : _client(c), _path(s), _fid(f), _open(false)
{
}

Fid::Fid(Client& c, const std::string& s, bool r, bool w)
    : _client(c), _path(s), _fid(c.getFid()), _open(false)
{
    // TODO: How to handle errors from walk?
    (void) walk(s);
    // TODO: How to handle errors from open?
    open(r,w);
}

Fid::~Fid()
{
    // Suppress any errors since there is nowhere to return them.
    (void) _client.tclunk(_fid);
}

Fid& Fid::operator=(const Fid& f)
{
    if (this!=&f) {
        _path=f._path;
        // TODO: What if there is an error returned from twalk?
        (void) _client.twalk(f._fid,_fid);
    }
    return *this;
}

bool Fid::open(bool r, bool w)
{
    u8 mode=0;
    
    if (r) mode|=0444;
    if (w) mode|=0222;
    _open = _client.topen(_fid,mode);
    return _open;
}

bool Fid::read(std::string& buf, u16& len, u32 pos)
{
    return _client.tread(_fid,buf,len,pos);
}

bool Fid::write(const std::string& buf, u16& len, u32 pos)
{
    return _client.twrite(_fid,buf,len,pos);
}

bool Fid::stat(Dirent& dirent) const
{
    return _client.tstat(_fid, dirent);
}
