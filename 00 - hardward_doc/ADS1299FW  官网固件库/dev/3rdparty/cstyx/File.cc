
#include "File.hh"
#include "Client.hh"

using namespace std;
using namespace cstyx;

File::File(Client& client, const std::string& path,
        bool r, bool w)
    : _client(client), _fid(client,path,r,w),
    _pos(0), _eof(false)
{
}

File::~File()
{
}

bool File::read(std::string& buf, u16& len)
{
    if (!read(buf,len,_pos)) { return false; }
    _pos+=len;
    return true;
}

void File::seek(u32 pos)
{
    _eof = (static_cast<long>(pos) < _pos);
    _pos=pos;
}

int File::readAll(std::string& buf, int requested, u32 pos)
{
    int got = 0;
    u16 needed = requested - got;

    seek(pos);
    while (needed > 0) {
        // return error if read failed
        if (!read(buf,needed)) return -1;
        // stop looping when 0 bytes are read
        // this implies the end of the file
        if (0 == needed) { break; }
        // keep track of how much was read otherwise
        got += needed;
        needed = requested - got;
    }
    return got;
}

bool File::write(const std::string& buf, u16& len)
{
    bool written = write(buf,len,_pos);
    if (written) _pos+=len;
    return written;
}

bool File::write(const std::string& buf, u16& len, u32 pos)
{
    bool written = _fid.write(buf,len,pos);
    if (written) _eof=(len==0);
    return written;
}

int File::write(const std::string& buf)
{
    u16 len=buf.size();
    if (len>32767) len=32767;
    bool written = _fid.write(buf,len,_pos);
    if (written) _eof=(len==0);
    return written ? len : -1;
}
