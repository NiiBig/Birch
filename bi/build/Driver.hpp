/**
 * @file
 */
#pragma once

#include "bi/build/misc.hpp"
#include "bi/statement/Package.hpp"

#undef line // line() macro in indentable_iostream clashes with property_tree
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include <map>
#include <set>

namespace bi {
/**
 * Driver.
 *
 * @ingroup driver
 */
class Driver {
public:
  /**
   * Constructor.
   */
  Driver(int argc, char** argv);

  /**
   * Run user program.
   */
  void run(const std::string& prog);

  /**
   * Build package.
   */
  void build();

  /**
   * Install package.
   */
  void install();

  /**
   * Uninstall package.
   */
  void uninstall();

  /**
   * Distribute package.
   */
  void dist();

  /**
   * Clean package.
   */
  void clean();

  /**
   * Create a new package.
   */
  void init();

  /**
   * Validate an existing package.
   */
  void check();

  /**
   * Produce documentation.
   */
  void docs();

private:
  /**
   * Read in the META.json file.
   */
  void meta();

  /**
   * Set up build directory.
   */
  void setup();

  /**
   * Create the package.
   */
  Package* createPackage();

  /**
   * Compile Birch files to C++.
   */
  void compile();

  /**
   * Run autogen.sh.
   */
  void autogen();

  /**
   * Run configure.
   */
  void configure();

  /**
   * Run make with a given target.
   *
   * @param cmd The target, empty string for the default target.
   */
  void target(const std::string& cmd = "");

  /**
   * Consume a list of files from the meta file.
   *
   * @param meta Property tree of the meta file.
   * @param key The key.
   * @param checkExists Check if the files exists?
   */
  void readFiles(const boost::property_tree::ptree& meta,
      const std::string& key, const bool checkExists);

  /**
   * @name Command-line options
   */
  //@{
  /**
   * Working directory.
   */
  fs::path work_dir;

  /**
   * Build directory.
   */
  fs::path build_dir;

  /**
   * Built libraries directory.
   */
  fs::path lib_dir;

  /**
   * Share directories.
   */
  std::list<fs::path> share_dirs;

  /**
   * Include directories.
   */
  std::list<fs::path> include_dirs;

  /**
   * Library directories.
   */
  std::list<fs::path> lib_dirs;

  /**
   * Target architecture.
   */
  std::string arch;

  /**
   * Installation directory.
   */
  fs::path prefix;

  /**
   * Name of the package.
   */
  std::string packageName;

  /**
   * Description of the package.
   */
  std::string packageDesc;

  /**
   * Use unity build?
   */
  bool unity;

  /**
   * Are compiler warnings enabled?
   */
  bool warnings;

  /**
   * Is debugging mode enabled?
   */
  bool debug;

  /**
   * Is verbose reporting enabled?
   */
  bool verbose;
  //@}

  /**
   * Is the autogen.sh file new?
   */
  bool newAutogen;

  /**
   * Is the configure.ac file new?
   */
  bool newConfigure;

  /**
   * Is the Makefile.am file new?
   */
  bool newMake;

  /**
   * Lists of files from meta.
   */
  std::map<std::string,std::set<fs::path>> metaFiles;
  std::set<fs::path> allFiles;

  /**
   * Leftover command-line arguments for program calls.
   */
  std::vector<char*> largv;
};
}
