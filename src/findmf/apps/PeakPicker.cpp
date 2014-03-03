// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include <fstream>
#include <pwiz/data/msdata/MSDataFile.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "findmf/fileiopwiz/qtofpeakpickerfilter.h"
#include "pwiz/data/vendor_readers/ExtendedReaderList.hpp"

namespace b_po = boost::program_options;
namespace b_fs = boost::filesystem3;

///
struct PPParams
{
  std::string infile_;
  std::string outfile_;
  //uint32_t nrthreads;
  double resolution_; // with of mz bins in ppms
  double smoothwidth_;
  double integrationwidth_;
  bool area_;
  double threshold_;

  std::string filestem_;
  boost::filesystem::path outdir_;

  PPParams(): infile_() ,
    outfile_() ,
    resolution_(0.) ,
    smoothwidth_(1.),
    integrationwidth_(4.) ,
    area_(false),
    threshold_(10.),
    filestem_(),
    outdir_(){
  }

  ///
  void prepareOutputFile()
  {
    if( !boost::filesystem::exists(infile_) )
    {
      std::cerr << "could not find specified file :" << std::endl;
      std::cerr << infile_ << std::endl;
      return;
    }
    //filestem_ = boost::filesystem::path(infile).stem().stem().stem().string(); //createOutputs(p1.string() ,"fitered");
    outdir_ = boost::filesystem::path(outfile_).parent_path();
    //create outdir
    try{
      if(!boost::filesystem::exists(outdir_)){
        boost::filesystem3::create_directory(outdir_);
      }
    }catch(std::exception & e ){
      std::cout << e.what() << std::endl;
    }
  }
};//end struct param


/// parameters
inline int defineParameters(
    int ac,
    char* av[],
    b_po::variables_map & vmgeneral
    )
{
  try
  {
    b_po::options_description general("File Handling:");
    general.add_options()
        ("help,H", "produce help message")
        ("version,V", "produces version information")
        ("in,I", b_po::value<std::string>(), "input file")
        ("out,O", b_po::value<std::string>(), "output file")
        ("config-file,C", b_po::value<std::string>(), "configuration file");

    b_po::options_description processing("Processing Options:");
    processing.add_options()
        ("resolution",b_po::value<double>()->default_value(20000.),
         "instrument resolution (default 50000).")
        ("area", b_po::value<bool>()->default_value(true),"otherwise store intensity")
        ("threshold", b_po::value<double>()->default_value(3.),"multiplicative factor of smallest intensity in spectrum");

    b_po::options_description advancedprocessing("Advanced Processing Options:");
    advancedprocessing.add_options()
        ("smoothwidth",b_po::value<double>()->default_value(1),"smoothing width")
        ("intwidth", b_po::value<double>()->default_value(2.5),"integration width");

    b_po::options_description cmdloptions;
    cmdloptions.add(general).add(processing).add(advancedprocessing);
    b_po::store( b_po::parse_command_line( ac , av , cmdloptions) , vmgeneral);
    b_po::notify(vmgeneral);
    std::string configfile;
    if(vmgeneral.count("config-file"))
    {
      configfile = vmgeneral["config-file"].as<std::string>();
    }

    b_po::options_description config_file_options;
    config_file_options.add(general).add(processing).add(advancedprocessing);
    if(configfile.size() > 0 && b_fs::exists(configfile))
    {
      std::ifstream ifs(configfile.c_str());
      store(parse_config_file(ifs, config_file_options), vmgeneral);
      b_po::notify(vmgeneral);
    }
    else if(configfile.size() == 0){
    }
    else
    {
      std::cerr << "Could not find config file." << std::endl;
      return -1;
    }

    if(!vmgeneral.count("in"))
    {
      std::cerr << "input file is obligatory" << std::endl;
      std::cerr << cmdloptions << "\n";
      return -1;
    }
    if(!vmgeneral.count("out"))
    {
      std::cerr << "output file is obligatory" << std::endl;
      std::cerr << cmdloptions << "\n";
      return -1;
    }
    if(vmgeneral.count("help"))
    {
      std::cerr << cmdloptions << "\n";
      return -1;
    }
    if(vmgeneral.count("version"))
    {
      std::cerr << "1.0.0.3" << "\n";
      return -1;
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "error: " << e.what() << "\n";
    return -1;
  }
  catch(...)
  {
    std::cerr << "Exception of unknown type!\n";
  }
  return 0;
}//end parse command line

inline void analysisParameters(PPParams & ap,b_po::variables_map & vmgeneral){
  if(vmgeneral.count("in"))
  {
    ap.infile_ =  vmgeneral["in"].as<std::string>();
  }

  if(vmgeneral.count("out"))
  {
    ap.outfile_ = vmgeneral["out"].as<std::string>();
  }

  ap.resolution_ = 2*vmgeneral["resolution"].as<double>();
  ap.smoothwidth_ = 2*vmgeneral["smoothwidth"].as<double>();
  ap.integrationwidth_ = 2*vmgeneral["intwidth"].as<double>();
  ap.area_ = vmgeneral["area"].as<bool>(); //do you want to store areas
  ap.threshold_ = vmgeneral["threshold"].as<double>();
}

///
int main(int argc, char *argv[])
{
  b_po::variables_map vmgeneral;
  if(defineParameters(argc, argv, vmgeneral) != 0){
    return 0;
  }
  PPParams aparam;
  analysisParameters(aparam,vmgeneral);
  pwiz::msdata::ExtendedReaderList readers;
  pwiz::msdata::MSDataPtr msdataptr_ = pwiz::msdata::MSDataPtr(new pwiz::msdata::MSDataFile(aparam.infile_,&readers));
  pwiz::msdata::SpectrumListPtr sl = msdataptr_->run.spectrumListPtr;
  pwiz::msdata::SpectrumListPtr mp(
        new ralab::QTOFPeakPickerFilter(sl,
                                        aparam.resolution_,
                                        aparam.smoothwidth_,
                                        aparam.integrationwidth_,
                                        aparam.threshold_,
                                        aparam.area_
                                        )
        );
  msdataptr_->run.spectrumListPtr = mp;
  pwiz::msdata::MSDataFile::Format format;
  if(std::string("mzML").compare(boost::filesystem::extension(aparam.outfile_)) ==0 ){
    format = pwiz::msdata::MSDataFile::Format_mzML;
  }
  else{
    format =  pwiz::msdata::MSDataFile::Format_mzXML;
  }
  pwiz::msdata::MSDataFile::write(*msdataptr_,
                                  aparam.outfile_,
                                  format
                                  ); // uncomment if you want mzXML
}
