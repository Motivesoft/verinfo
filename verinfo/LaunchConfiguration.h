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

   LaunchConfiguration( bool fileVersion, bool productVersion, bool ignoreFilesWithoutVersion )
   {
      this->fileVersion = fileVersion;
      this->productVersion = productVersion;
      this->ignoreFilesWithoutVersion = ignoreFilesWithoutVersion;
   }

public:
   LaunchConfiguration( const LaunchConfiguration& copy )
   {
      this->fileVersion = copy.fileVersion;
      this->productVersion = copy.productVersion;
      this->ignoreFilesWithoutVersion = copy.ignoreFilesWithoutVersion;
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

   void display( const FixedFileInfo& info );

   class Builder
   {
   private:
      bool fileVersion;
      bool productVersion;
      bool ignoreFilesWithoutVersion;

      std::vector<std::string> filespecs;

   public:
      void showFileVersion()
      {
         fileVersion = true;
      }

      void showProductVersion()
      {
         productVersion = true;
      }

      void skipFilesWithoutVersion()
      {
         ignoreFilesWithoutVersion = true;
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

         if ( arg == "pv" || arg == "-product-version" )
         {
            showProductVersion();
         }
         else if ( arg == "fv" || arg == "-file-version" )
         {
            showFileVersion();
         }
         else if ( arg == "i" || arg == "-ignore-missing-verinfo" )
         {
            skipFilesWithoutVersion();
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

         return LaunchConfiguration( fileVersion, productVersion, ignoreFilesWithoutVersion );
      }
   };
};

