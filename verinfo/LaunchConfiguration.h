#pragma once

#include <functional>
#include <string>
#include <vector>

#include "FixedFileInfo.h"

class LaunchConfiguration
{
private:
   bool fileVersion;
   bool productVersion;
   bool ignoreFilesWithoutVersion;
   bool verbose;
   bool version;
   bool help;

   LaunchConfiguration( bool fileVersion, bool productVersion, bool ignoreFilesWithoutVersion, bool verbose, bool version, bool help )
   {
      this->fileVersion = fileVersion;
      this->productVersion = productVersion;
      this->ignoreFilesWithoutVersion = ignoreFilesWithoutVersion;
      this->verbose = verbose;
      this->version = version;
      this->help = help;
   }

public:
   LaunchConfiguration( const LaunchConfiguration& copy )
   {
      this->fileVersion = copy.fileVersion;
      this->productVersion = copy.productVersion;
      this->ignoreFilesWithoutVersion = copy.ignoreFilesWithoutVersion;
      this->verbose = copy.verbose;
      this->version = copy.version;
      this->help = copy.help;
   }

   bool showFileVersion() const
   {
      return fileVersion;
   }

   bool showProductVersion() const
   {
      return productVersion;
   }

   bool skipFilesWithoutVersion() const
   {
      return ignoreFilesWithoutVersion;
   }

   bool showVerboseInfo() const
   {
      return verbose;
   }

   bool showVersion() const
   {
      return version;
   }

   bool showHelp() const
   {
      return help;
   }

   class Builder
   {
   private:
      bool fileVersion;
      bool productVersion;
      bool ignoreFilesWithoutVersion;
      bool verbose;
      bool version;
      bool help;

      std::vector<std::string> filespecs;

   public:
      Builder()
      {
         fileVersion = false;
         productVersion = false;
         ignoreFilesWithoutVersion = false;
         verbose = false;
         version = false;
         help = false;
      }

      bool isSwitch( const std::string& argv ) const
      {
         return !argv.empty() && ( argv[ 0 ] == '/' || argv[ 0 ] == '-' );
      }

      bool process( const std::string& argv )
      {
         bool result = true;

         // Strip the initial - or /
         // This way, we treat /pv and -pv and --product-version identically
         std::string arg = argv.substr( 1 );

         if ( arg == "fv" || arg == "-file-version" )
         {
            fileVersion = true;
         }
         else if ( arg == "pv" || arg == "-product-version" )
         {
            productVersion = true;
         }
         else if ( arg == "i" || arg == "-ignore-missing-verinfo" )
         {
            ignoreFilesWithoutVersion = true;
         }
         else if ( arg == "verbose" || arg == "-verbose" )
         {
            verbose = true;
         }
         else if ( arg == "v" || arg == "-version" )
         {
            version = true;
         }
         else if ( arg == "?" || arg == "h" || arg == "-help" )
         {
            help = true;
         }
         else
         {
            result = false;
         }

         return result;
      }

      LaunchConfiguration build()
      {
         // Set a default
         if ( fileVersion == false && productVersion == false )
         {
            productVersion = true;
         }

         return LaunchConfiguration( fileVersion, productVersion, ignoreFilesWithoutVersion, verbose, version, help );
      }
   };
};

