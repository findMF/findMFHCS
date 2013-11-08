// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef WRITESAMPLEPARAMS_H
#define WRITESAMPLEPARAMS_H

#include <soci/soci.h>
#include <boost/filesystem.hpp>
#include "findmf/apps/toolparameters.h"
#include "findmf/datastruct/Instrument.h"

namespace ralab
{
  namespace findmf
  {
    namespace fileio
    {
      /// writing sample information
      struct WriteSampleParams{
        soci::session sql_;
        soci::transaction tr_;
        WriteSampleParams(const std::string & fileloc):sql_(soci::sqlite3,fileloc),tr_(sql_)
        {}

        ~WriteSampleParams(){
          sql_.close();
        }

        void commit(){
          tr_.commit();
        }

      private:
        /// @return sample id
        uint32_t insertSample(ralab::findmf::datastruct::SampleDescript & sample)
        {
          using namespace soci;
          std::string p1 =  "insert into sample(name, file, description )";
          p1 +=  " values(:name,:file, :description)";

          sql_ << p1, use(sample.name,"name"),
              use(sample.file,"file"),
              use(sample.description,"description");

          soci::rowid rowid(sql_);

          int lastid;
          sql_ << "SELECT last_insert_rowid()" , into(lastid);

          //std::cout << rowid << lastid << std::endl;
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

      public:
        /// @ return sample id
        uint32_t write(
            ralab::findmf::datastruct::SampleDescript & sampledescription,
            ralab::findmf::datastruct::Instrument & instrument,
            ralab::findmf::apps::Params & params
            ){
          uint32_t sampleid = insertSample(sampledescription);
          insertInstrumentInfo(sampleid,instrument);
          insertProcessParameters(sampleid,params);
          return sampleid;
        }

      };
    }//end fileio
  }//end findmf
}//end ralab



#endif // WRITESAMPLEPARAMS_H
