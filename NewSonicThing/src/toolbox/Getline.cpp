#include <istream>
#include <string>
#include "getline.hpp"

std::istream& getlineSafe(std::istream& is, std::string& t)
{
    std::string myline;

    if (getline(is, myline))
    {
       if (myline.size() && myline[myline.size()-1] == '\r')
       {
           t = myline.substr(0, myline.size() - 1);
       }
       else
       {
           t = myline;
       }
    }
    else
    {
        t.clear();
    }

    return is;

    //tried doing this but in_avail() would return 0 in the middle of some files and i dont know why
    /*
    //taken from https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf/6089413#6089413
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();
    //std::fprintf(stdout, "inavail = %lld\n", sb->in_avail());

    for (;;)
    {
        int c = sb->sbumpc();
        
        switch (c)
        {
            case '\n':
                std::fprintf(stdout, "line1 = '%s'\n", t.c_str());
                if (sb->in_avail() == 0)
                {
                    is.setstate(std::ios::eofbit);
                    std::fprintf(stdout, "End of File1\n");
                }
                return is;
                
            case '\r':
                std::fprintf(stdout, "line2 = '%s'\n", t.c_str());
                if (sb->sgetc() == '\n')
                {
                    sb->sbumpc();
                }
                if (sb->in_avail() == 0)
                {
                    is.setstate(std::ios::eofbit);
                    std::fprintf(stdout, "End of File2\n");
                }
                return is;
                
            case std::streambuf::traits_type::eof():
                std::fprintf(stdout, "line3 = '%s'\n", t.c_str());
                // Also handle the case when the last line has no line ending
                //if (t.empty())
                //{
                    //is.setstate(std::ios::eofbit);
                //}
                //if (sb->in_avail() == 0)
                {
                    is.setstate(std::ios::eofbit);
                    std::fprintf(stdout, "End of File3\n");
                }
                return is;
                
            default:
                t += (char)c;
        }
    }
    
    return is;
    */
}
