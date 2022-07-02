#include <link.h>

#include <filesystem>
#include <iostream>
#include <string_view>

#include "glog/logging.h"
#include "src/config/builder.h"
#include "src/config/config.h"
#include "src/strategy/factory.h"
#include "src/strategy/strategy.h"
#include "toml++/toml.h"

static KeyValueBuilder key_value_builder;

void atexit_handler() {
  Config config = Config::build().strategy(Config::Strategy::KeyValue).key_value(key_value_builder);
  LOG(INFO) << "Profiled the following libraries which were loaded" << std::endl
            << config;
}

__attribute__((constructor)) static void init() {
  // Note: Cannot use print here.
}

/**
 * @brief Many of the comments here are copied verbatim from
 * https://github.com/buildsi/ldaudit-yaml as they are *excellent*.
 * It was an amazing learning resource to build an LD_AUDIT library.
 */

/*
   unsigned int la_version(unsigned int version);
   This is the only function that must be defined by an auditing
   library: it performs the initial handshake between the dynamic
   linker and the auditing library.  When invoking this function,
   the dynamic linker passes, in version, the highest version of the
   auditing interface that the linker supports.
   A typical implementation of this function simply returns the
   constant LAV_CURRENT, which indicates the version of <link.h>
   that was used to build the audit module.  If the dynamic linker
   does not support this version of the audit interface, it will
   refuse to activate this audit module.  If the function returns
   zero, the dynamic linker also does not activate this audit
   module.
   In order to enable backwards compatibility with older dynamic
   linkers, an audit module can examine the version argument and
   return an earlier version than LAV_CURRENT, assuming the module
   can adjust its implementation to match the requirements of the
   previous version of the audit interface.  The la_version function
   should not return the value of version without further checks
   because it could correspond to an interface that does not match
   the <link.h> definitions used to build the audit module.
*/
unsigned int la_version(unsigned int version) {
  // If version == 0 the library will be ignored by the linker.
  if (version == 0) {
    return version;
  }

  // for simplicity, let's just log to STDERR for now
  // we may chose to log to STDERR and files in the future
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging("libold.so");

  LOG(INFO) << "Taking control of the linking search....";
  return LAV_CURRENT;
}

/*
    The dynamic linker invokes this function to inform the auditing
    library that it is about to search for a shared object.  The name
    argument is the filename or pathname that is to be searched for.
    cookie identifies the shared object that initiated the search.
    flag is set to one of the following values:
    LA_SER_ORIG
           This is the original name that is being searched for.
          Typically, this name comes from an ELF DT_NEEDED entry, or
          is the filename argument given to dlopen(3).
   LA_SER_LIBPATH
          name was created using a directory specified in
          LD_LIBRARY_PATH.
    LA_SER_RUNPATH
          name was created using a directory specified in an ELF
          DT_RPATH or DT_RUNPATH list.
   LA_SER_CONFIG
          name was found via the ldconfig(8) cache
          (/etc/ld.so.cache).
   LA_SER_DEFAULT
          name was found via a search of one of the default
          directories.
   LA_SER_SECURE
          name is specific to a secure object (unused on Linux).
   As its function result, la_objsearch() returns the pathname that
   the dynamic linker should use for further processing.  If NULL is
   returned, then this pathname is ignored for further processing.
   If this audit library simply intends to monitor search paths,
   then name should be returned.
*/
char *la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag) {
  // If the environment variable is unset then just dont do anything.
  // Pretty simple.
  // We can't print in the module constructor, so we chose to do so now instead
  static std::unique_ptr<Strategy> strategy;
  static Config config;
  static bool setup = []() {
    // We register this here because it must be called after the initialization
    // of our static variables. Unfortunately __attribute constructor() is
    // called before oddly.
    std::atexit(atexit_handler);

    char *filepath = getenv("OLDAUDIT_CONFIG");
    if (filepath == nullptr) {
      std::cerr << "OLDAUDIT_CONFIG environment variable is not set. Skipping "
                   "functionality."
                << std::endl;
      return false;
    }

    auto table = toml::parse_file(filepath);
    config = Config::fromTOML(table);

    strategy = CreateStrategy(config);
    return true;
  }();

  if (!setup) {
    return const_cast<char *>(name);
  }

  switch (flag) {
    case LA_SER_ORIG: {
      std::string new_name = strategy->resolve(name);
      char *ptr = new char[new_name.size() + 1];  // +1 for terminating NUL
      strcpy(ptr, new_name.c_str());
      return ptr;
    }
    case LA_SER_LIBPATH:
    case LA_SER_RUNPATH:
    case LA_SER_DEFAULT:
    case LA_SER_CONFIG:
    case LA_SER_SECURE:
      break;
  }

  return const_cast<char *>(name);
}

/*
    The dynamic linker calls this function when a new shared object
    is loaded.  The map argument is a pointer to a link-map structure
    that describes the object.  The lmid field has one of the
    following values
    LM_ID_BASE
          Link map is part of the initial namespace.
    LM_ID_NEWLM
          Link map is part of a new namespace requested via
          dlmopen(3).
    cookie is a pointer to an identifier for this object.  The
    identifier is provided to later calls to functions in the
    auditing library in order to identify this object.  This
    identifier is initialized to point to object's link map, but the
    audit library can change the identifier to some other value that
    it may prefer to use to identify the object.
    As its return value, la_objopen() returns a bit mask created by
    ORing zero or more of the following constants, which allow the
    auditing library to select the objects to be monitored by
    la_symbind*():
    LA_FLG_BINDTO
          Audit symbol bindings to this object.
    LA_FLG_BINDFROM
          Audit symbol bindings from this object.
    A return value of 0 from la_objopen() indicates that no symbol
    bindings should be audited for this object.
*/
unsigned int la_objopen(struct link_map *map, Lmid_t lmid, uintptr_t *cookie) {
  // TODO(fmzakari): Remove when we add the Audit flag
  LOG(INFO) << "Loaded: " << std::string(map->l_name);

  if (strlen(map->l_name) != 0) {
    key_value_builder.with(std::filesystem::path(map->l_name).filename(),
                           map->l_name);
  }

  return LA_FLG_BINDTO | LA_FLG_BINDFROM;
}