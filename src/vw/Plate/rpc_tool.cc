// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <vw/Image.h>
#include <vw/FileIO.h>
#include <vw/Cartography.h>
#include <vw/Plate/PlateFile.h>
#include <vw/Plate/ToastPlateManager.h>
//#include <vw/Plate/KmlPlateManager.h>

using namespace vw;
using namespace vw::platefile;
using namespace vw::mosaic;
using namespace vw::cartography;

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// Erases a file suffix if one exists and returns the base string
static std::string prefix_from_filename(std::string const& filename) {
  std::string result = filename;
  int index = result.rfind(".");
  if (index != -1) 
    result.erase(index, result.size());
  return result;
}

// --------------------------------------------------------------------------
//                                DO_MOSAIC
// --------------------------------------------------------------------------

template <class ViewT>
void do_mosaic(boost::shared_ptr<PlateFile> platefile, 
               ImageViewBase<ViewT> const& view,
               std::string filename, GeoReference const& georef, 
               std::string output_mode, int num_threads) {

  std::ostringstream status_str;
  status_str << "\t    " << filename << " : ";

  if (output_mode == "toast") {

    boost::shared_ptr<ToastPlateManager<typename ViewT::pixel_type> > pm( 
                                                                         new ToastPlateManager<typename ViewT::pixel_type> (platefile, num_threads) );
    
    pm->insert(view.impl(), filename, georef,
               TerminalProgressCallback(InfoMessage, status_str.str()) );

  }  else if (output_mode == "kml")  {

    // boost::shared_ptr<KmlPlateManager> pm = 
    //   boost::shared_ptr<KmlPlateManager>( new KmlPlateManager(platefile, num_threads) );

    // pm->insert(view.impl(), filename, georef,
    //            TerminalProgressCallback(InfoMessage, status_str.str()) );

  }

}

// --------------------------------------------------------------------------
//                                    MAIN
// --------------------------------------------------------------------------

int main( int argc, char *argv[] ) {

  std::string url;
  unsigned transaction_id;

  po::options_description general_options("Turns georeferenced image(s) into a TOAST quadtree.\n\nGeneral Options");
  general_options.add_options()
    ("transaction_id,t", po::value<unsigned>(&transaction_id), "Transaction id to force as complete.")
    ("help", "Display this help message");

  po::options_description hidden_options("");
  hidden_options.add_options()
    ("url", po::value<std::string>(&url));

  po::options_description options("Allowed Options");
  options.add(general_options).add(hidden_options);

  po::positional_options_description p;
  p.add("url", -1);

  std::ostringstream usage;
  usage << "Usage: " << argv[0] << " [options] <url>..." <<std::endl << std::endl;
  usage << general_options << std::endl;

  po::variables_map vm;
  try { 
    po::store( po::command_line_parser( argc, argv ).options(options).positional(p).run(), vm );
    po::notify( vm );
  } catch (po::error &e) {
    std::cout << "An error occured while parsing command line arguments.\n\n";
    std::cout << usage.str();
    return 0;    
  }

  if( vm.count("help") ) {
    std::cout << usage.str();
    return 0;
  }

  if( vm.count("url") < 1 ) {
    std::cerr << "Error: must specify a platefile url.\n";
    std::cout << usage.str();
    return 1;
  }

  //------------------------- SET DEFAULT OPTIONS -----------------------------

  try {

    std::cout << "\nOpening plate file: " << url << "\n";
    boost::shared_ptr<PlateFile> platefile = 
      boost::shared_ptr<PlateFile>( new PlateFile(url) );
  
    if (vm.count("transaction_id"))
        platefile->transaction_failed(transaction_id);

  }  catch (vw::Exception &e) {
    std::cout << "An error occured: " << e.what() << "\nExiting.\n\n";
    exit(1);
  }
  
}