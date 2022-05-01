#include "toml++/toml.h"
#include <link.h>
#include <iostream>
#include <string_view>
#include "src/strategy/strategy.h"
#include "src/strategy/factory.h"

__attribute__((constructor)) static void init(void)
{
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
unsigned int la_version(unsigned int version)
{
    // If version == 0 the library will be ignored by the linker.
    if (version == 0)
    {
        return version;
    }

    std::cout << "Hello World" << std::endl;

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
char *la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag)
{

    // If the environment variable is unset then just dont do anything.
    // Pretty simple.
    // We can't print in the module constructor, so we chose to do so now instead
    static toml::v3::ex::parse_result config;
    static std::unique_ptr<Strategy> strategy;
    static bool setup = []()
    {
        char *filepath = getenv("OLDAUDIT_CONFIG");
        if (filepath == nullptr) {
            std::cerr << "OLDAUDIT_CONFIG environment variable is not set. Skipping functionality." << std::endl;
            return false;
        }

        config = toml::parse_file(filepath);
        auto type = config["strategy"].value_or("do_nothing");
        auto type_config = config[type].as_table();
        strategy = CreateStrategy(type, *type_config);
        return true;
    }();

    if (!setup) {
        return (char *)name;
    }

    switch (flag)
    {
    case LA_SER_ORIG:
    {
        std::string new_name = strategy->resolve(name);
        char *ptr = new char[new_name.size() + 1]; // +1 for terminating NUL
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

    return (char *)name;
}