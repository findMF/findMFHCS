#ifndef PARSESQL_H
#define PARSESQL_H

#include <string>

#include <vector>


//lightway and limited sql parser;
namespace ralab
{
  namespace findmf
  {
    namespace utils
    {
      //parses and sqlfile and returns the commands contained.
      std::vector<std::string> sqlparse(const std::string & sqlfile);
    }
  }
}



#endif // PARSESQL_H
