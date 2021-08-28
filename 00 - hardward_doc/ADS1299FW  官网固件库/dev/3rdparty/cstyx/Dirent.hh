
#ifndef GUARD_cstyx_Dirent_hh
#define GUARD_cstyx_Dirent_hh

#include <string>
#include "types.h"

typedef struct fcall_t fcall_t;

namespace cstyx {

class File;
class Client;

class Dirent {
    friend class Client;
    friend class Directory;

private:
    std::string _name;
    u32 _mode;
    long _len;
    
    // if true is returned, len is updated to be the
    // number of bytes converted
    bool initFromBuf(std::string& buf, u16& len);
    
    Dirent(const std::string& name, u32 mode, long len)
    : _name(name), _mode(mode), _len(len)
    {}
    
    // makes a Dirent from a buffer
    Dirent(const char* buf, u16 len)
    {
        std::string b=std::string(buf,len);
        u16 bytesConverted = 0;
        (void) initFromBuf(b,bytesConverted);
        // TODO: What if initFromBuf fails?
    }

    Dirent(std::string& buf, u16& len)
    {
        (void) initFromBuf(buf,len);
        // TODO: What if initFromBuf fails?
    }

    // makes a Dirent from fcall fields
    Dirent(fcall_t& f);

public:
    Dirent() 
        : _name(), _mode(0), _len(-1)
    {}
    const std::string& name() const { return _name; }
    long mode() const { return _mode; }
    bool isdir() const { return( 0 != (_mode&0x80000000) ); }
    bool canRead() const { return( 0 != (_mode&4) ); }
    bool canWrite() const { return( 0 != (_mode&2) ); }
    long len() const { return _len; }
    long size() const { return _len; }
    std::string owner() const { return "nobody"; }
    std::string group() const { return "nobody"; }
};

};

#endif
