
#include "Dirent.hh"
#include "Error.hh"
#include "styxmsg.hh"

using namespace cstyx;

bool Dirent::initFromBuf(std::string& buf, u16& len)
{
    dir_t d;
    u16 l;
    char c[512];

    l=cstyx_convM2D(const_cast<char*>(buf.data()),buf.size(),&d,c);
    if (!l) return false;
    
    _name=d.name;
    _len=d.length;
    _mode=d.mode;
    
    buf.erase(0,l);
    len = l;
    return true;
}

Dirent::Dirent(fcall_t& f)
{
    std::string buf(f.u.stat.stat,f.u.stat.nstat);
    u16 len = 0;
    (void) initFromBuf(buf,len);
    // TODO: What if initFromBuf fails?
}
