#pragma once

#include <string>
#include <vector>

class FileConfiguration
{
private:
   std::vector<std::string> filespecs;

   static void expandFileSpec( const std::string& filespec, std::vector<std::string>& files );

public:
   FileConfiguration()
   {

   }

   FileConfiguration( const FileConfiguration& copy )
   {
      for( std::vector<std::string>::const_iterator it = copy.filespecs.cbegin(); it != copy.filespecs.cend(); it++ )
      {
         this->filespecs.push_back( *it );
      }
   }

   ~FileConfiguration()
   {
      filespecs.clear();
   }

   void addFileSpec( std::string& filespec )
   {
      filespecs.push_back( filespec );
   }

   std::vector<std::string> getFiles() const;

   static std::string getFilename( const std::string& filespec );

   static std::string getAppName( const char* argv0 );
};
