#include <link.h>

#include <filesystem>
#include <iostream>

#include "glog/logging.h"
#include "src/config/builder.h"
#include "src/config/config.h"
static KeyValueBuilder key_value_builder;

void atexit_handler() {
  Config config = Config::build()
                      .strategy(Config::Strategy::KeyValue)
                      .key_value(key_value_builder);
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

  [[maybe_unused]] static bool setup = []() {
    // We register this here because it must be called after the initialization
    // of our static variables. Unfortunately __attribute constructor() is
    // called before oddly.
    std::atexit(atexit_handler);

    // for simplicity, let's just log to STDERR for now
    // we may chose to log to STDERR and files in the future
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging("libold.so");
    return true;
  }();

  return LAV_CURRENT;
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