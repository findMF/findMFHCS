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
      struct WriteMapInfo{
        soci::session sql_;
        WriteMapInfo(const std::string & fileloc):sql_(soci::sqlite3,fileloc)
        {}

        ~WriteMapInfo(){
          sql_.close();
        }
        void writeMapInfo(){}
      };


      struct WriteSampleParams{
        soci::session sql_;
        WriteSampleParams(const std::string & fileloc):sql_(soci::sqlite3,fileloc)
        {}

        ~WriteSampleParams(){
          sql_.close();
        }
        ///
        ///return sample id
        /// this stuff you need to get from the db.
        uint32_t insertSample(const std::string & name, const std::string & description,
                              const std::string & file)
        {
          using namespace soci;
          std::string p1 =  "insert into sample(name, file, description )";
          p1 +=  " values(:name,:file, :description)";

          sql_ << p1, use(name,"name"),
              use(file,"file"),
              use(description,"description");
          ;

          //          sql_ << p1, use(name),//":name"),
          //              use(file),//":file"),
          //              use(description)//",:description");
          ;
          int lastid;
          sql_ << "SELECT last_insert_rowid()" , into(lastid);
          return lastid;
        }

        //
        void insertInstrumentInfo(int sampleid, ralab::findmf::datastruct::Instrument & inst){
          using namespace soci;

          std::string p1 = "insert into instrumentinfo(id,manufacturer,model,ionisation,analyser,detector)";
          p1 += " values(:id, :manufacturer, :model, :ionisation, :analyser, :detector)";
          sql_ << p1, use(sampleid, "id"), use(inst.manufacturer, "manufacturer"),
              use(inst.model, "model"), use(inst.ionisation,"ionisation"),
              use(inst.analyser,"analyser"),use(inst.detector,"detector");
        }

        //
        void insertProcessParameters(int sampleid, ralab::findmf::apps::Params & params){
          using namespace soci;

          std::string p1 = "insert into softwareparam(id, resolution,nrthreads,mzpixelwidth,";
          p1+= "rtpixelwidth,scalemz,scalert,minintensity,minmass,maxmass,rt2sum)";
          p1 += " values(:id, :resolution,:nrthreads,:mzpixelwidth,:rtpixelwidth,:scalemz,:scalert,";
          p1 +=  ":minintensity,:minmass,:maxmass,:rt2sum)";
          double resolution = 1/params.ppm*1e6;
          sql_ << p1, use(sampleid,"id"), use(resolution,"resolution"),
              use(params.nrthreads,"nrthreads"),use(params.mzpixelwidth,"mzpixelwidth"),
              use(params.rtpixelwidth,"rtpixelwidth"),use(params.mzscale , "scalemz"),
              use(params.rtscale,"scalert"),use(params.minintensity,"minintensity"),
              use(params.minmass , "minmass"),use(params.maxmass,"maxmass"),
              use(params.rt2sum_,"rt2sum");
        }
      };

      //creates the database
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
                    LOG(INFO) << e.what() << std::endl;
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
