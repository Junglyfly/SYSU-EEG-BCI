
#include "Directory.hh"
#include "Fid.hh"
#include "Error.hh"
#include "Client.hh"
using namespace cstyx;

Directory::Directory(Client& c, const std::string& p)
    : _client(c), _list()
{
    {
        Fid f(c,p);
        // TODO: What if f is not a directory?
        (void) f.isdir();
        _path=f.path();
    }
    update();
}

Directory::Directory(Client& c)
    : _client(c), _list()
{
    // Ignoring error from cwd.
    (void) c.cwd(_path);
    update();
}

void Directory::update()
{
    std::string b;
    u16 l;
    u32 pos=0;
    _list.clear();
    Fid f(_client,_path,true,false);
    do {
        l=65535;
        if (!f.read(b,l,pos)) {
          break;
        }
        pos+=l;
    } while (l >= 65535);
    l=b.size();
    while (b.size()) {
        _list.push_back(Dirent(b,l));
        if (!l) break;
    }
}

