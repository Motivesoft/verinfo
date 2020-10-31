// verinfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h>

#include "FixedFileInfo.h"
#include "FileConfiguration.h"
#include "LaunchConfiguration.h"

bool scan( const LaunchConfiguration& configuration, const std::vector<std::string>& filelist );
bool readVersionInfo( const std::string& filename, std::function<void( FixedFileInfo& )> fn );

int main( int argc, char** argv )
{
   bool success = true;

   FileConfiguration files;
   LaunchConfiguration::Builder builder;
   for ( int loop = 1; loop < argc; )
   {
      std::string arg( argv[ loop++ ] );
      if ( arg.length() > 0 )
      {
         if( builder.isSwitch( arg ) )
         {
            if ( !builder.process( arg ) )
            {
               std::cerr << "Unrecognised argument: " << arg << std::endl;
               success = false;
               break;
            }
         }
         else
         {
            files.addFileSpec( arg );
         }
      }
   }

   if ( success )
   {
      LaunchConfiguration configuration = builder.build();
      std::vector<std::string> filelist = files.getFiles();

      if ( !filelist.empty() )
      {
         if ( configuration.showVerboseInfo() )
         {
            std::cout << "Scanning " << filelist.size() << " files" << std::endl;
         }

         if ( !scan( configuration, filelist ) && configuration.showVerboseInfo() )
         {
            std::cout << "No matching files have version information" << std::endl;
         }
      }
      else
      {
         if ( configuration.showVerboseInfo() )
         {
            std::cerr << "No matching files found" << std::endl;
         }
      }
   }
}

bool scan( const LaunchConfiguration& configuration, const std::vector<std::string>& filelist )
{
   bool success = false;
   for ( std::vector<std::string>::const_iterator it = filelist.cbegin(); it != filelist.cend(); it++ )
   {
      if ( readVersionInfo( *it, [configuration] ( const FixedFileInfo& info )
      {
         std::cout << info.getFilename() << std::endl;
         if ( configuration.showFileVersion() )
         {
            std::cout << "  File version: " << info.getFileVersion() << std::endl;
         }
         if ( configuration.showProductVersion() )
         {
            std::cout << "  Product version: " << info.getProductVersion() << std::endl;
         }
      } ) )
      {
         success = true;
      }
      else if( !configuration.skipFilesWithoutVersion() )
      {
         std::cout << *it << std::endl;
         std::cout << "  No version information" << std::endl;
      }
   }
   return success;
}

bool readVersionInfo( const std::string& filename, std::function<void( FixedFileInfo& )> fn )
{
   bool result = false;

   DWORD handle;
   DWORD size = ::GetFileVersionInfoSizeA( filename.c_str(), &handle );

   LPBYTE data = new BYTE[ size ];
   if ( ::GetFileVersionInfoA( filename.c_str(), 0, size, data ) )
   {
      LPBYTE buffer;
      UINT length;
      if ( ::VerQueryValueA( data, "\\", (LPVOID*) &buffer, &length ) )
      {
         if ( size )
         {
            VS_FIXEDFILEINFO* fixedFileInfo = (VS_FIXEDFILEINFO*) buffer;
            if ( fixedFileInfo->dwSignature == 0xFEEF04BD )
            {
               FixedFileInfo verInfo( filename, fixedFileInfo );
               
               fn( verInfo );
               result = true;
            }
         }
      }
   }
   delete[] data;

   return result;
}