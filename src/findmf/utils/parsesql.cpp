// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "findmf/utils/parsesql.h"
#include <functional>
#include "base/utils/copyif.h"
#include "base/utils/readwritelines.h"
#include "base/misc/sqlparse.h"

namespace ralab{
  namespace findmf{
    namespace utils{
      //parses and sqlfile and returns the commands contained.
      std::vector<std::string> sqlparse(const std::string & sqlfile)
      {
        std::vector<std::string> lines, lines2;
        ralab::base::utils::readLines(sqlfile , lines);
        //removes comment and empty lines
        std::string y = "--"; // sqlcomment string
        base::misc::beginMatch p(y);
        ralab::base::utils::copy_if_not(lines.begin(),lines.end(),std::back_inserter(lines2),p);
        lines.clear();
        std::transform(lines2.begin(),lines2.end(),std::back_inserter(lines),base::misc::commentremover(y));
        lines2.clear();
        base::misc::strmerge(lines.begin(),lines.end(),std::back_inserter(lines2),";");
        return(lines2);
      }
    }
  }
}
