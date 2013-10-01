#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>



namespace ralab{
  namespace findmf{
    namespace fileio{
      struct CreateTables{
        soci::session sql_;

        CreateTable(const std::string & fileloc):sql_(soci::sqlite3,fileloc){}


        // stores summaries about map
        void createMapinfo(){

          sql_.once <<


      };

    }
  }
}
