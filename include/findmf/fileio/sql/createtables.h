#include <soci/soci.h>
#include <soci/soci-sqlite3.h>
#include "findmf/utils/parsesql.h"
#include <boost/filesystem.hpp>
#include "findmf/apps/toolparameters.h"
#include "findmf/datastruct/Instrument.h"

namespace ralab
{
  namespace findmf
  {
    namespace fileio
    {



      /// creates the database
      struct CreateTables{
        soci::session sql_;
        CreateTables(const std::string & fileloc, const std::string & schemafile):sql_()
        {
          using namespace soci;
          bool eschem = boost::filesystem3::exists( schemafile );
          boost::filesystem3::path path = boost::filesystem3::path(fileloc).parent_path();
          bool fpath = boost::filesystem3::exists( boost::filesystem3::path(fileloc).parent_path()) || path.empty();

          if(eschem && fpath)
          {
            std::vector<std::string> creatstatements = ralab::findmf::utils::sqlparse(schemafile);
            sql_.open(soci::sqlite3,fileloc);
            for(int i = 0 ; i < creatstatements.size(); ++i )
            {
              try{
                sql_.once << creatstatements[i];
              }
              catch( std::exception const &e )
              {
                LOG(ERROR) << creatstatements[i];
                LOG(ERROR) << e.what() << std::endl;
                throw;
              }
            }

          }
        }

        ~CreateTables(){
          sql_.close();
        }
      };
    }//end fileio
  }//end findmf
}//end ralab
