#include <soci/soci.h>
//#include <soci/sqlite3/soci-sqlite3.h>
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
      struct WriteSampleParams{

        soci::session sql_;
        WriteSampleParams(const std::string & fileloc):sql_(soci::sqlite3,fileloc)
        {}

        ///
        ///return sample id
        /// this stuff you need to get from the db.
        int insertSample(const std::string & name, const std::string & description,
                         const std::string & file)
        {
          using namespace soci;

          std::string p1 =  "insert into sample(name, file, description )";
          std::string p2 =  "values(:name,:file, :description)";
          std::string name1;
          std::string file1;
          std::string description1;
          sql_ << p1 + " " + p2, use(name1,":name"), use(file1,":file"),use(description1,":description");
          int lastid;
          sql_ << "SELECT last_insert_rowid()" , into(lastid);
          return lastid;
        }

        //
        void insertInstrumentInfo(int sampleid, ralab::findmf::datastruct::Instrument & inst){
          using namespace soci;

          std::string p1 = "insert into instrumentinfo(id,manufacturer,model,ionisation,analyser,detector)";
          std::string p2 = "values(:id, :manufacturer, :model, :ionisation, :analyser, :detector)";
          sql_ << p1 + " " + p2, use(sampleid, ":id"), use(inst.manufacturer, ":manufacturer"),
              use(inst.model, ":model"), use(inst.ionisation,":ionisation"),
              use(inst.analyser,":analyser"),use(inst.detector,":detector");
        }

        //
        void insertProcessParameters(int sampleid, ralab::findmf::apps::Params & params){
          using namespace soci;

          std::string p1 = "insert into sofwareparam(id, resolution,nrthreads,mzpixelwidth,rtpixelwidth,scalemz,scalert,minintensity,minmass,maxmass,rt2sum)";
          std::string p2 = "values(:id, :ppm,:nrthreads,:mzpixelwidth,:rtpixelwidth,:scalemz,:scalert,:minintensity,:minmass,:maxmass,:rt2sum)";
          sql_ << p1 + " " + p2, use(sampleid,":id"), use(1/params.ppm*1e6,":ppm"),
              use(params.nrthreads,":nrthreads"),use(params.mzpixelwidth,":mzpixelwidth"),use(params.rtpixelwidth,":rtpixelwidth"),
              use(params.mzscale , ":scalemz"),use(params.rtscale,":scalert:"),use(params.minintensity,":minintensity"),
              use(params.minmass , ":minmass"),use(params.maxmass,":maxmass"),use(params.rt2sum_,":rt2su");
        }
      };

      //creates the database
      struct CreateTables{
        soci::session sql_;

        CreateTables(const std::string & fileloc, const std::string & schemafile):sql_(soci::sqlite3,fileloc)
        {
          using namespace soci;

          if(boost::filesystem3::exists(schemafile) && boost::filesystem3::exists(boost::filesystem3::path(fileloc).parent_path()) ){
              std::vector<std::string> creatstatements = ralab::findmf::utils::sqlparse(schemafile);
              sql_.open(fileloc);
              for(int i = 0 ; i < creatstatements.size(); ++i )
                {
                  try{
                    sql_.once << creatstatements[i];
                  }
                  catch( std::exception const &e )
                  {
                    LOG(INFO) << e.what() << std::endl;
                  }
                }
              sql_.close();
            }
        }
      };
    }//end fileio
  }//end findmf
}//end ralab
