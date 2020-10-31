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

   LaunchConfiguration( bool fileVersion, bool productVersion, bool ignoreFilesWithoutVersion, bool verbose )
   {
      this->fileVersion = fileVersion;
      this->productVersion = productVersion;
      this->ignoreFilesWithoutVersion = ignoreFilesWithoutVersion;
      this->verbose = verbose;
   }

public:
   LaunchConfiguration( const LaunchConfiguration& copy )
   {
      this->fileVersion = copy.fileVersion;
      this->productVersion = copy.productVersion;
      this->ignoreFilesWithoutVersion = copy.ignoreFilesWithoutVersion;
      this->verbose = copy.verbose;
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

   class Builder
   {
   private:
      bool fileVersion;
      bool productVersion;
      bool ignoreFilesWithoutVersion;
      bool verbose;

      std::vector<std::string> filespecs;

   public:
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
         else if ( arg == "v" || arg == "-verbose" )
         {
            verbose = true;
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

         return LaunchConfiguration( fileVersion, productVersion, ignoreFilesWithoutVersion, verbose );
      }
   };
};

