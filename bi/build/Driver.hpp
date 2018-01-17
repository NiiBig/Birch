/**
 * @file
 */
#pragma once

#include "bi/build/misc.hpp"
#include "bi/statement/Package.hpp"

#include "libubjpp/libubjpp.hpp"

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
   * @param meta The root object of the meta file.
   * @param key The key to use in the list of files.
   * @param path The path from which to read in the meta file.
   * @param checkExists Check if the files exists?
   */
  void readFiles(const libubjpp::value meta, const std::string& key,
      const std::initializer_list<std::string>& path, const bool checkExists);

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
   * Enable compiler warnings.
   */
  bool warnings;

  /**
   * Enable debugging mode.
   */
  bool debug;

  /**
   * Verbose reporting.
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
   * Is META.json new?
   */
  bool newMeta;

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
