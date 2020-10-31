#pragma once

#include <sstream>
#include <string>
#include <Windows.h>

class FixedFileInfo
{
   std::string file;
   DWORD fileFlags;
   std::string fileVersion;
   std::string productVersion;

   static std::string buildVersion( DWORD ms, DWORD ls )
   {
      std::stringstream str;

      str << HIWORD( ms ) << ".";
      str << LOWORD( ms ) << ".";
      str << HIWORD( ls ) << ".";
      str << LOWORD( ls );

      return str.str();
   }

public:
   FixedFileInfo( const std::string& file, VS_FIXEDFILEINFO* info )
   {
      this->file = file;
      this->fileFlags = info->dwFileFlags;
      this->fileVersion = buildVersion( info->dwFileVersionMS, info->dwFileVersionLS );
      this->productVersion = buildVersion( info->dwProductVersionMS, info->dwProductVersionLS );
   }

   FixedFileInfo( const FixedFileInfo& copy )
   {
      this->file = copy.file;
      this->fileFlags = copy.fileFlags;
      this->fileVersion = copy.fileVersion;
      this->productVersion = copy.productVersion;
   }

   std::string getFilename() const
   {
      return file;
   }

   std::string getFileVersion() const
   {
      return fileVersion;
   }

   std::string getProductVersion() const
   {
      return productVersion;
   }
};

