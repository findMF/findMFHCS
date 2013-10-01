#include "findmf/utils/parsesql.h"
#include <functional>
#include "base/utils/Copy_if.h"
#include "base/utils/readwritelines.h"
#include "findmf/utils/utilities/sqlparse.h"

namespace ralab{
  namespace findmf{
    namespace utils{
      //parses and sqlfile and returns the commands contained.
      std::vector<std::string> sqlparse(const std::string & sqlfile){
        std::vector<std::string> lines, lines2;
        ralab::base::utils::readLines(sqlfile , lines);
        //removes comment and empty lines
        std::string y = "--"; // sqlcomment string
        utilities::beginMatch p(y);
        ralab::base::utils::copy_if_not(lines.begin(),lines.end(),std::back_inserter(lines2),p);
        lines.clear();
        std::transform(lines2.begin(),lines2.end(),std::back_inserter(lines),utilities::commentremover(y));
        lines2.clear();
        utilities::strmerge(lines.begin(),lines.end(),std::back_inserter(lines2),";");
        return(lines2);
      }
    }
  }
}
