
#ifndef GUARD_cstyx_Fid_hh
#define GUARD_cstyx_Fid_hh

#include <string>
#include "types.h"
#include "pathutil.hh"
#include "Dirent.hh"

namespace cstyx {

class Client;

class Fid {
    friend class Client;
private:
    Client& _client;
    std::string _path;
    u32 _fid;
    bool _open;

    bool walk(const std::string& p, u32 fid, std::string& newpath);

    /* private constructor for Client.  Makes a fid with given path 
    and fid.  Does not walk or open the fid. */
    Fid(Client&, const std::string& s, u32 f);

public:
    //! Constructor
    /*! Allocates a new fid for path \p s.
    
    A new fid number is obtained from Client, and this is walked.  If the
    path is relative, the walk is performed from this fid.  If the path is
    absolute, the walk is performed from the root fid, which is obtained
    from the client.
    */
    Fid(Client& c, const std::string& s);

    Fid(Client& c, const std::string& s, bool r, bool w);

    //! Copy constructor
    /*! A new fid is allocated and walked to the location of the old fid. */
    Fid(const Fid&);

    //! Destructor
    /*! Clunks the fid. */
    ~Fid();

    //! Path
    const std::string& path() const { return _path; }

    //! Walk
    bool walk(const std::string& s);

    //! Assignment
    /*! Causes this fid to point to the same path as the other fid, 
    using a walk.
    
    The fid number and client do not change; only the path data is 
    actually copied.
    */
    Fid& operator=(const Fid& f);

    u32 fid() const { return _fid; }

    bool stat(Dirent& dirent) const;
    std::string name() const { return pathutil::name(path()); }
    long size() const
    {
        Dirent dirent;
        if (false == stat(dirent)) return -1;
        return dirent.size();
    }
    bool read(std::string& buf, u16& len, u32 pos);
    bool write(const std::string& buf, u16& len, u32 pos);
    bool isdir()
    {
        Dirent dirent;
        if (false == stat(dirent)) return false;
        // TODO: What if there is an error?
        return dirent.isdir();
    }
    bool open(bool r, bool w);
    bool isopen() const { return _open; }
};

};

#endif
