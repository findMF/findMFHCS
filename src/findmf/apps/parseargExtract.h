// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#ifndef PARSEARGEXTRACT_H
#define PARSEARGEXTRACT_H
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "findmf/apps/toolparameters.h"

namespace b_po = boost::program_options;
namespace b_fs = boost::filesystem;

/*!
 *\brief parses the command line arguments using boost::program_options
*/

inline void analysisParameters(ralab::findmf::apps::Params & ap,b_po::variables_map & vmgeneral){
  if(vmgeneral.count("in"))
  {
    ap.infile =  vmgeneral["in"].as<std::string>();
  }
  else{
    std::cerr << "in file argument is required" << std::endl;
    return;
  }
  if(vmgeneral.count("outdir"))
  {
    ap.outdir = vmgeneral["outdir"].as<std::string>();
  }
  else{
    boost::filesystem::path p(ap.infile);
    ap.outdir = p.parent_path().string();
  }
  ap.nrthreads = vmgeneral["nrthreads"].as< uint32_t >();

  //image generation options
  ap.ppm=1/vmgeneral["resolution"].as<double>()* 1.e6;

  ap.minmass = vmgeneral["minMass"].as<double>();
  if(vmgeneral.count("maxMass")){
    ap.maxmass = vmgeneral["maxMass"].as<double>();
  }
  else{
    ap.maxmass = std::numeric_limits<double>::max();
  }
  ap.rt2sum_ = vmgeneral["rt2sum"].as<uint32_t>();

  //  filtering options
  ap.mzpixelwidth = vmgeneral["width-MZ"].as<unsigned int>();
  if(vmgeneral.count("width-RT")){
    ap.rtpixelwidth = vmgeneral["width-RT"].as<unsigned int>();
  }else{
    ap.rtpixelwidth = ap.mzpixelwidth;
  }

  ap.mzscale = vmgeneral["mzscale"].as< double >();
  if(vmgeneral.count("rtscale")){
    ap.rtscale = vmgeneral["rtscale"].as< double >();
  }
  else{
    ap.rtscale = ap.mzscale;
  }
  ap.dofilter = vmgeneral["filter"].as<bool>();

  // segmentation options
  ap.minintensity=vmgeneral["minintensity"].as<double>();
  ap.writeprojections_ = vmgeneral["writeprojections"].as<bool>();
}


/// set up the parameters and pars them.
inline int parsecommandlineExtract(
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
        ("outdir,O", b_po::value<std::string>(), "output directory (default same as input)")
        ("config-file,I", b_po::value<std::string>(), "configuration file")
        ("nrthreads", b_po::value<uint32_t>()->default_value(4), "nr threads");

    b_po::options_description generation("Image Generation Options:");
    generation.add_options()
        ("resolution",b_po::value<double>()->default_value(50000.),
         "instrument resolution (default 50000).")
        ("minMass",b_po::value<double>()->default_value(400.), "minimum mass to consider (default 400)")
        ("maxMass",b_po::value<double>(),"maximum mass to consider")
        ("rt2sum",b_po::value<uint32_t>()->default_value(1u),"downsampling - number spectra to average (not supported yet)");


    b_po::options_description filtering("Image Preprocessing Options :");
    filtering.add_options()
        ("filter",b_po::value<bool>()->default_value(1),"should filtering be performed?")
        ("mzscale",b_po::value<double>()->default_value(1.5),
         "scale parameter for gausian smoothing in mz (default 1.5)")
        ("rtscale",b_po::value<double>(),
         "scale parameter for gausian smoothing in rt (default same as mzscale)")
        ("width-MZ", b_po::value<unsigned int>()->default_value(9),
         "width of MZ peak in pixel - used by background subtraction (default = 9)")
        ("width-RT", b_po::value<unsigned int>(),
         "width of RT peak in pixel - used by background subtraction (default = width-MZ)");

    b_po::options_description segment("Feature extraction and storage options :");
    segment.add_options()
        ("minintensity",b_po::value<double>()->default_value(5.),"minimum intensity")
        ("writeprojections",b_po::value<bool>()->default_value(true),"should feature projections be stored in database");


    b_po::options_description cmdloptions;
    cmdloptions.add(general).add(generation).add(filtering).add(segment);

    b_po::store(b_po::parse_command_line(ac, av, cmdloptions), vmgeneral);
    b_po::notify(vmgeneral);
    std::string configfile;
    if(vmgeneral.count("config-file"))
    {
      configfile = vmgeneral["config-file"].as<std::string>();
    }

    b_po::options_description config_file_options;
    config_file_options.add(general).add(generation).add(filtering).add(segment);
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
      exit(0);
    }

    if(!vmgeneral.count("in"))
    {
      std::cerr << "input file is obligatory" << std::endl;
      std::cerr << cmdloptions << "\n";
      exit(0);
    }

    if(vmgeneral.count("help"))
    {
      std::cerr << cmdloptions << "\n";
      exit(0);
    }
    if(vmgeneral.count("version"))
    {
      std::cerr << "1.0.0.3" << "\n";
      exit(0);
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "error: " << e.what() << "\n";
    exit(0);
  }
  catch(...)
  {
    std::cerr << "Exception of unknown type!\n";
  }
  return 1;
}//end parse command line

#endif // PARSEARGEXTRACT_H
