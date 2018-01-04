/**
 * @file
 */
#include "Driver.hpp"

#include "bi/build/Compiler.hpp"
#include "bi/build/misc.hpp"
#include "bi/io/md_ostream.hpp"
#include "bi/exception/DriverException.hpp"

#include "boost/filesystem/fstream.hpp"
#include "boost/algorithm/string.hpp"

#include <getopt.h>
#include <dlfcn.h>

#include <iostream>
#include <regex>
#include <unordered_set>

using namespace boost::filesystem;
using namespace boost::algorithm;

bi::Driver::Driver(int argc, char** argv) :
    work_dir(current_path()),
    build_dir(current_path() / "build"),
    lib_dir(current_path() / "build" / ".libs"),
    prefix(""),
    packageName("Untitled"),
    warnings(true),
    debug(true),
    verbose(true),
    newAutogen(false),
    newConfigure(false),
    newMake(false),
    newMeta(false),
    isLocked(false) {
  enum {
    SHARE_DIR_ARG = 256,
    INCLUDE_DIR_ARG,
    LIB_DIR_ARG,
    PREFIX_ARG,
    NAME_ARG,
    ENABLE_WARNINGS_ARG,
    DISABLE_WARNINGS_ARG,
    ENABLE_DEBUG_ARG,
    DISABLE_DEBUG_ARG,
    ENABLE_VERBOSE_ARG,
    DISABLE_VERBOSE_ARG
  };

  int c, option_index;
  option long_options[] = {
      { "share-dir", required_argument, 0, SHARE_DIR_ARG },
      { "include-dir", required_argument, 0, INCLUDE_DIR_ARG },
      { "lib-dir", required_argument, 0, LIB_DIR_ARG },
      { "prefix", required_argument, 0, PREFIX_ARG },
      { "name", required_argument, 0, NAME_ARG },
      { "enable-warnings", no_argument, 0, ENABLE_WARNINGS_ARG },
      { "disable-warnings", no_argument, 0, DISABLE_WARNINGS_ARG },
      { "enable-debug", no_argument, 0, ENABLE_DEBUG_ARG },
      { "disable-debug", no_argument, 0, DISABLE_DEBUG_ARG },
      { "enable-verbose", no_argument, 0, ENABLE_VERBOSE_ARG },
      { "disable-verbose", no_argument, 0, DISABLE_VERBOSE_ARG },
      { 0, 0, 0, 0 } };
  const char* short_options = "-";  // treats non-options as short option 1

  /* mutable copy of argv and argc */
  largv.insert(largv.begin(), argv, argv + argc);
  std::vector<char*> fargv;

  /* read options */
  std::vector<char*> unknown;
  opterr = 0;  // handle error reporting ourselves
  c = getopt_long_only(largv.size(), largv.data(), short_options,
      long_options, &option_index);
  while (c != -1) {
    switch (c) {
    case SHARE_DIR_ARG:
      share_dirs.push_back(optarg);
      break;
    case INCLUDE_DIR_ARG:
      include_dirs.push_back(optarg);
      break;
    case LIB_DIR_ARG:
      lib_dirs.push_back(optarg);
      break;
    case PREFIX_ARG:
      prefix = optarg;
      break;
    case NAME_ARG:
      packageName = optarg;
      break;
    case ENABLE_WARNINGS_ARG:
      warnings = true;
      break;
    case DISABLE_WARNINGS_ARG:
      warnings = false;
      break;
    case ENABLE_DEBUG_ARG:
      debug = true;
      break;
    case DISABLE_DEBUG_ARG:
      debug = false;
      break;
    case ENABLE_VERBOSE_ARG:
      verbose = true;
      break;
    case DISABLE_VERBOSE_ARG:
      verbose = false;
      break;
    case '?':  // unknown option
    case 1:  // not an option
      unknown.push_back(largv[optind - 1]);
      largv.erase(largv.begin() + optind - 1, largv.begin() + optind);
      --optind;
      break;
    }
    c = getopt_long_only(largv.size(), largv.data(), short_options,
        long_options, &option_index);
  }
  largv.insert(largv.end(), unknown.begin(), unknown.end());

  /* environment variables */
  char* BIRCH_SHARE_PATH = getenv("BIRCH_SHARE_PATH");
  char* BIRCH_INCLUDE_PATH = getenv("BIRCH_INCLUDE_PATH");
  char* BIRCH_LIBRARY_PATH = getenv("BIRCH_LIBRARY_PATH");
  std::string input;

  /* share dirs */
  if (BIRCH_SHARE_PATH) {
    std::stringstream birch_share_path(BIRCH_SHARE_PATH);
    while (std::getline(birch_share_path, input, ':')) {
      share_dirs.push_back(input);
    }
  }
#ifdef DATADIR
  share_dirs.push_back(path(STRINGIFY(DATADIR)) / "birch");
#endif

  /* include dirs */
  include_dirs.push_back(work_dir);
  include_dirs.push_back(build_dir);
  if (BIRCH_INCLUDE_PATH) {
    std::stringstream birch_include_path(BIRCH_INCLUDE_PATH);
    while (std::getline(birch_include_path, input, ':')) {
      include_dirs.push_back(input);
    }
  }
#ifdef INCLUDEDIR
  include_dirs.push_back(STRINGIFY(INCLUDEDIR));
#endif

  /* lib dirs */
  if (BIRCH_LIBRARY_PATH) {
    std::stringstream birch_library_path(BIRCH_LIBRARY_PATH);
    while (std::getline(birch_library_path, input, ':')) {
      lib_dirs.push_back(input);
    }
  }
#ifdef LIBDIR
  lib_dirs.push_back(STRINGIFY(LIBDIR));
#endif
}

bi::Driver::~Driver() {
  unlock();
}

void bi::Driver::run(const std::string& prog) {
  /* get package information */
  meta();

  /* dynamically load possible programs */
  typedef void prog_t(int argc, char** argv);

  void* handle;
  void* addr;
  char* msg;
  prog_t* fcn;

  path so = std::string("lib") + tarname(packageName);
#ifdef __APPLE__
  so.replace_extension(".dylib");
#else
  so.replace_extension(".so");
#endif

  /* Look in built libs first. */
  if (exists(lib_dir / so))
    so = lib_dir / so;

  handle = dlopen(so.c_str(), RTLD_NOW);
  msg = dlerror();
  if (handle == NULL) {
    std::stringstream buf;
    buf << "Could not load " << so.string() << ", " << msg << '.';
    throw DriverException(buf.str());
  } else {
    addr = dlsym(handle, prog.c_str());
    msg = dlerror();
    if (msg != NULL) {
      std::stringstream buf;
      buf << "Could not find symbol " << prog << " in " << so.string() << '.';
      throw DriverException(buf.str());
    } else {
      fcn = reinterpret_cast<prog_t*>(addr);
      fcn(largv.size(), largv.data());
    }
    dlclose(handle);
  }
}

void bi::Driver::build() {
  meta();
  setup();
  compile();
  autogen();
  configure();
  target();
  unlock();
}

void bi::Driver::install() {
  meta();
  setup();
  compile();
  autogen();
  configure();
  target("install");
  unlock();
}

void bi::Driver::uninstall() {
  meta();
  setup();
  compile();
  autogen();
  configure();
  target("uninstall");
  unlock();
}

void bi::Driver::dist() {
  meta();
  setup();
  compile();
  autogen();
  configure();
  target("dist");
  unlock();
}

void bi::Driver::clean() {
  meta();
  setup();
  remove_all("autom4te.cache");
  remove_all("m4");
  remove("aclocal.m4");
  remove("autogen.log");
  remove("autogen.sh");
  remove("compile");
  remove("config.guess");
  remove("config.sub");
  remove("configure");
  remove("configure.ac");
  remove("depcomp");
  remove("install-sh");
  remove("ltmain.sh");
  remove("Makefile.am");
  remove("Makefile.in");
  remove("missing");
  remove_all(build_dir);
  unlock();
}

void bi::Driver::init() {
  create_directory("bi");
  create_directory("input");
  create_directory("output");
  copy_with_prompt(find(share_dirs, "gitignore"), ".gitignore");
  copy_with_prompt(find(share_dirs, "LICENSE"), "LICENSE");

  std::string contents;

  contents = read_all(find(share_dirs, "META.json"));
  replace_all(contents, "PACKAGE_NAME", packageName);
  ofstream metaStream(work_dir / "META.json");
  metaStream << contents;

  contents = read_all(find(share_dirs, "README.md"));
  replace_all(contents, "PACKAGE_NAME", packageName);
  ofstream readmeStream(work_dir / "README.md");
  readmeStream << contents;
}

void bi::Driver::check() {
  /* read META.json */
  if (!exists("META.json")) {
    warn("no META.json file.");
  } else {
    meta();
  }

  /* check LICENSE */
  if (!exists("LICENSE")) {
    warn("no LICENSE file; create a LICENSE file containing the "
        "distribution license (e.g. GPL or BSD) of the package.");
  } else if (allFiles.find("LICENSE") == allFiles.end()) {
    warn("LICENSE file is not listed in META.json file.");
  }

  /* check README.md */
  if (!exists("README.md")) {
    warn("no README.md file; create a README.md file documenting the "
        "package in Markdown format.");
  } else if (allFiles.find("README.md") == allFiles.end()) {
    warn("README.md file is not listed in META.json file.");
  }

  /* check for files that might be missing from META.json */
  std::unordered_set<std::string> interesting, exclude;

  interesting.insert(".bi");
  interesting.insert(".sh");
  interesting.insert(".m");
  interesting.insert(".R");
  interesting.insert(".json");
  interesting.insert(".ubj");
  interesting.insert(".cpp");
  interesting.insert(".hpp");

  exclude.insert("autogen.sh");
  exclude.insert("ltmain.sh");

  recursive_directory_iterator iter("."), end;
  while (iter != end) {
    auto path = remove_first(iter->path());
    auto name = path.filename().string();
    auto ext = path.extension().string();
    if (path.string() == "build" || path.string() == "results") {
      iter.no_push();
    } else if (interesting.find(ext) != interesting.end()
        && exclude.find(name) == exclude.end()) {
      if (allFiles.find(path.string()) == allFiles.end()) {
        warn(
            std::string("is ") + path.string()
                + " missing from META.json file?");
      }
    }
    ++iter;
  }
}

void bi::Driver::docs() {
  current_path(work_dir);
  meta();
  Package* package = createPackage();

  /* parse all files */
  Compiler compiler(package, work_dir, build_dir);
  compiler.parse();
  compiler.resolve();

  /* output everything, categorised by object type, and sorted */
  path path = "DOCS.md";
  ofstream stream(path);
  md_ostream output(stream);

  output << package;
  delete package;
}

void bi::Driver::unlock() {
  if (isLocked) {
    lockFile.unlock();
  }
  isLocked = false;
}

void bi::Driver::meta() {
  using namespace libubjpp;

  /* check for META.json */
  if (!exists("META.json")) {
    if (exists("MANIFEST")) {
      warn("MANIFEST is deprecated, use META.json instead.");
    }
    throw DriverException("META.json does not exist.");
  }

  /* parse META.json */
  JSONDriver driver;
  auto top = driver.parse("META.json");
  if (!top) {
    throw DriverException("syntax error in META.json");
  }

  /* meta */
  if (auto name = top.get().get<string_type>("name")) {
    packageName = name.get();
  } else {
    throw DriverException(
        "META.json must provide a 'name' entry with the name of this package.");
  }

  /*  manifest */
  readFiles(top.get(), "manifest.header", { "manifest", "header" }, true);
  readFiles(top.get(), "manifest.source", { "manifest", "source" }, true);
  readFiles(top.get(), "manifest.data", { "manifest", "data" }, true);
  readFiles(top.get(), "manifest.other", { "manifest", "other" }, true);

  /* external requirements */
  readFiles(top.get(), "require.package", { "require", "package" }, false);
  readFiles(top.get(), "require.header", { "require", "header" }, false);
  readFiles(top.get(), "require.library", { "require", "library" }, false);
  readFiles(top.get(), "require.program", { "require", "program" }, false);
}

void bi::Driver::setup() {
  /* internal name of package */
  auto internalName = tarname(packageName);

  /* create build directory */
  if (!exists(build_dir)) {
    if (!create_directory(build_dir)) {
      std::stringstream buf;
      buf << "Could not create build directory " << build_dir << '.';
      throw DriverException(buf.str());
    }
  }

  /* create lock file */
  if (!exists(build_dir / "lock")) {
    ofstream stream(build_dir / "lock");
  }

  /* exclusive access to build directory */
  lock();

  /* copy build files into build directory */
  newAutogen = copy_if_newer(find(share_dirs, "autogen.sh"),
      work_dir / "autogen.sh");
  permissions(work_dir / "autogen.sh", add_perms | owner_exe);
  newConfigure = copy_if_newer(find(share_dirs, "configure.ac"),
      work_dir / "configure.ac");
  newMake = copy_if_newer(find(share_dirs, "Makefile.am"),
      work_dir / "Makefile.am");
  newMeta = last_write_time("META.json")
      > last_write_time(work_dir / "Makefile.am");

  path m4_dir = work_dir / "m4";
  if (!exists(m4_dir)) {
    if (!create_directory(m4_dir)) {
      std::stringstream buf;
      buf << "Could not create m4 directory " << m4_dir << '.';
      throw DriverException(buf.str());
    }
  }
  copy_if_newer(find(share_dirs, "ax_cxx_compile_stdcxx.m4"),
      m4_dir / "ax_cxx_compile_stdcxx.m4");
  copy_if_newer(find(share_dirs, "ax_cxx_compile_stdcxx_11.m4"),
      m4_dir / "ax_cxx_compile_stdcxx_11.m4");
  copy_if_newer(find(share_dirs, "ax_cxx_compile_stdcxx_14.m4"),
      m4_dir / "ax_cxx_compile_stdcxx_14.m4");

  /* update configure.ac */
  if (newConfigure || newMeta) {
    std::string contents = read_all(find(share_dirs, "configure.ac"));
    replace_all(contents, "PACKAGE_NAME", packageName);
    replace_all(contents, "PACKAGE_TARNAME", internalName);
    ofstream configureStream(work_dir / "configure.ac");
    configureStream << contents << "\n\n";

    /* required headers */
    for (auto file : metaFiles["require.header"]) {
      configureStream << "AC_CHECK_HEADERS([" << file.string() << "], [], " <<
          "[AC_MSG_ERROR([header required by " << packageName <<
          " package not found.])], [-])\n";
    }

    /* required libraries */
    for (auto file : metaFiles["require.library"]) {
      configureStream << "AC_CHECK_LIB([" << file.string() << "], [main], " <<
          "[], [AC_MSG_ERROR([library required by " << packageName <<
          " package not found.])])\n";
    }

    /* required programs */
    for (auto file : metaFiles["require.program"]) {
      configureStream << "AC_PATH_PROG([PROG], [" << file.string() << "], [])\n";
      configureStream << "if test \"$PROG\" = \"\"; then\n";
      configureStream << "  AC_MSG_ERROR([" << file.string() << " program " <<
          "required by " << packageName << " package not found.])\n";
      configureStream << "fi\n";
    }

    /* footer */
    configureStream << "AC_CONFIG_FILES([Makefile])\n";
    configureStream << "AC_OUTPUT\n";

    configureStream.close();
  }

  /* update Makefile.am */
  if (newMake || newMeta) {
    std::string contents = read_all(find(share_dirs, "Makefile.am"));
    replace_all(contents, "PACKAGE_NAME", packageName);
    replace_all(contents, "PACKAGE_TARNAME", internalName);

    ofstream makeStream(work_dir / "Makefile.am");
    makeStream << contents << "\n\n";
    makeStream << "lib_LTLIBRARIES = lib" << internalName << ".la\n\n";

    /* *.cpp files */
    makeStream << "lib" << internalName << "_la_SOURCES = ";
    for (auto file : metaFiles["manifest.source"]) {
      if (file.extension().compare(".cpp") == 0) {
        makeStream << " \\\n  " << file.string();
      }
    }
    makeStream << '\n';

    /* sources derived from *.bi files */
    makeStream << "nodist_lib" << internalName << "_la_SOURCES = ";
    for (auto file : metaFiles["manifest.source"]) {
      if (file.extension().compare(".bi") == 0) {
        path cppFile = file;
        cppFile.replace_extension(".cpp");
        makeStream << " \\\n  " << cppFile.string();
      }
    }
    makeStream << '\n';

    /* headers to install and distribute */
    makeStream << "nobase_include_HEADERS =";
    makeStream << " \\\n  bi/" << internalName << ".hpp";
    makeStream << " \\\n  bi/" << internalName << ".bih";
    for (auto file : metaFiles["manifest.header"]) {
      if (file.extension().compare(".hpp") == 0) {
        makeStream << " \\\n  " << file.string();
      }
    }
    makeStream << '\n';

    /* data files to distribute */
    makeStream << "dist_pkgdata_DATA = ";
    for (auto file : metaFiles["manifest.data"]) {
      makeStream << " \\\n  " << file.string();
    }
    makeStream << '\n';

    /* other files to not distribute */
    makeStream << "noinst_DATA = ";
    for (auto file : metaFiles["manifest.other"]) {
      makeStream << " \\\n  " << file.string();
    }
    makeStream << '\n';

    makeStream.close();
  }
}

bi::Package* bi::Driver::createPackage() {
  Package* package = new Package(packageName);
  if (packageName != "Birch.Standard") {
    /* disable inclusion of the standard library when the project is, itself,
     * the standard library (!) */
    auto header = path("bi") / "birch_standard.bih";
    package->addHeader(find(include_dirs, header).string());
  }
  for (auto name: metaFiles["require.package"]) {
    /* add *.bih and *.hpp header and library dependencies */
    auto internalName = tarname(name.string());
    path header = path("bi") / internalName;
    header.replace_extension(".bih");
    package->addHeader(find(include_dirs, header).string());
    header.replace_extension(".hpp");
    metaFiles["require.header"].insert(header.string());
    metaFiles["require.library"].insert(internalName);
  }
  for (auto file : metaFiles["manifest.source"]) {
    if (file.extension().compare(".bi") == 0) {
      package->addSource(file.string());
    }
  }
  return package;
}

void bi::Driver::compile() {
  Package* package = createPackage();
  Compiler compiler(package, work_dir, build_dir);
  compiler.parse();
  compiler.resolve();
  compiler.gen();
  delete package;
}

void bi::Driver::autogen() {
  if (newAutogen || newConfigure || newMake || newMeta
      || !exists(work_dir / "configure")
      || !exists(work_dir / "install-sh")) {
    std::stringstream cmd;

    cmd << (path(".") / "autogen.sh").string();
    if (verbose) {
      std::cerr << cmd.str() << std::endl;
    } else {
      cmd << " > autogen.log 2>&1";
    }

    int ret = system(cmd.str().c_str());
    if (ret == -1) {
      throw DriverException("autogen.sh failed to execute.");
    } else if (ret != 0) {
      std::stringstream buf;
      buf << "autogen.sh died with signal " << ret
          << ". Make sure autoconf, automake and libtool are installed.";
      if (!verbose) {
        buf << " See " << (build_dir / "autogen.log").string()
            << " for details.";
      }
      throw DriverException(buf.str());
    }
  }
}

void bi::Driver::configure() {
  if (newAutogen || newConfigure || newMake || newMeta
      || !exists(build_dir / "Makefile")) {
    /* working directory */
    std::stringstream cppflags, cxxflags, ldflags, options, cmd;

    /* compile and link flags */
    if (debug) {
      cppflags << " -D_GLIBCXX_DEBUG";
      cxxflags << " -O0 -g -fno-inline";
      ldflags << " -O0 -g -fno-inline";
    } else {
      cppflags << " -DNDEBUG";

      /*
       * -flto enables link-time code generation, which is used in favour
       * of explicitly inlining functions written in Birch. The gcc manpage
       * recommends passing the same optimisation options to the linker as
       * to the compiler when using this.
       */
      cxxflags << " -O3 -g -funroll-loops -flto";
      ldflags << " -O3 -g -funroll-loops -flto";
    }
    if (warnings) {
      cxxflags << " -Wall";
      ldflags << " -Wall";
    }
    cxxflags << " -Wno-overloaded-virtual";

    for (auto iter = include_dirs.begin(); iter != include_dirs.end();
        ++iter) {
      cppflags << " -I" << iter->string();
    }
    for (auto iter = lib_dirs.begin(); iter != lib_dirs.end(); ++iter) {
      ldflags << " -L" << iter->string();
    }
    for (auto iter = lib_dirs.begin(); iter != lib_dirs.end(); ++iter) {
      ldflags << " -Wl,-rpath," << iter->string();
    }

    /* configure options */
    if (!prefix.empty()) {
      options << " --prefix=" << absolute(prefix).string();
    }
    options << " --disable-static";
    options << " INSTALL=\"install -p\"";
    options << " --config-cache";
    if (packageName == "Birch.Standard") {
      /* disable inclusion of the standard library when the project is, itself,
       * the standard library (!) */
      options << " --disable-std";
    } else {
      options << " --enable-std";
    }

    /* command */
    cmd << (work_dir / "configure").string() << " " << options.str()
        << " CPPFLAGS='" << cppflags.str() << "' CXXFLAGS='" << cxxflags.str()
        << "' LDFLAGS='" << ldflags.str() << "'";
    if (verbose) {
      std::cerr << cmd.str() << std::endl;
    } else {
      cmd << " > configure.log 2>&1";
    }

    /* change into build dir */
    current_path(build_dir);

    int ret = system(cmd.str().c_str());
    if (ret == -1) {
      throw DriverException("configure failed to execute.");
    } else if (ret != 0) {
      std::stringstream buf;
      buf << "configure died with signal " << ret
          << ". Make sure all dependencies are installed.";
      if (!verbose) {
        buf << "See " << (build_dir / "configure.log").string() << " and "
            << (build_dir / "config.log").string() << " for details.";
      }
      throw DriverException(buf.str());
    }

    /* change back to original working dir */
    current_path(work_dir);
  }
}

void bi::Driver::target(const std::string& cmd) {
  /* command */
  std::stringstream buf;
  buf << "make -j 4 " << cmd;

  /* handle output */
  std::string log = cmd + ".log";
  if (verbose) {
    std::cerr << buf.str() << std::endl;
  } else {
    buf << " > " << log << " 2>&1";
  }

  /* change into build dir */
  current_path(build_dir);

  int ret = system(buf.str().c_str());
  if (ret != 0) {
    buf.str("make ");
    buf << cmd;
    if (ret == -1) {
      buf << " failed to execute.";
    } else {
      buf << " died with signal " << ret << '.';
    }
    if (!verbose) {
      buf << " See " << (build_dir / log).string() << " for details.";
    }
    throw DriverException(buf.str());
  }

  /* change back to original working dir */
  current_path(work_dir);
}

void bi::Driver::lock() {
  boost::interprocess::file_lock lockFile1((build_dir / "lock").c_str());
  lockFile.swap(lockFile1);
  lockFile.lock();
  isLocked = true;
}

void bi::Driver::readFiles(const libubjpp::value meta, const std::string& key,
    const std::initializer_list<std::string>& path, bool checkExists) {
  if (auto files = meta.get<libubjpp::array_type>(path)) {
    if (files) {
      for (auto file : files.get()) {
        if (auto str = file.get<libubjpp::string_type>()) {
          if (str) {
            boost::filesystem::path filePath(str.get());
            if (checkExists && !exists(filePath)) {
              warn(filePath.string() + " in META.json does not exist.");
            }
            auto inserted = allFiles.insert(filePath);
            if (!inserted.second) {
              warn(filePath.string() + " repeated in META.json.");
            }
            metaFiles[key].insert(filePath);
          }
        }
      }
    }
  }
}
