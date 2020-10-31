#include "FileConfiguration.h"

#include <Windows.h>

std::vector<std::string> FileConfiguration::getFiles() const 
{
   std::vector<std::string> files;
   
   for ( std::vector<std::string>::const_iterator it = filespecs.cbegin(); it != filespecs.cend(); it++ )
   {
      expandFileSpec( *it, files );
   }

   return files;
}

void FileConfiguration::expandFileSpec( const std::string& filespec, std::vector<std::string>& files )
{
   // Extract the path part of the search
   char buffer[ _MAX_PATH ];
   char* ptr;

   ::GetFullPathNameA( filespec.c_str(), sizeof( buffer ), buffer, &ptr );

   if ( ptr )
   {
      // buffer will now contain the parent path
      *ptr = '\0';
   }

   WIN32_FIND_DATAA findFileData;
   HANDLE h = ::FindFirstFileA( filespec.c_str(), &findFileData );

   if ( h != INVALID_HANDLE_VALUE )
   {
      do
      {
         // Ignore directories
         if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
         {
            continue;
         }

         // Ignore hidden files
         if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
         {
            continue;
         }

         // Ignore . and ..
         if ( strcmp( findFileData.cFileName, "." ) == 0 || strcmp( findFileData.cFileName, "." ) == 0 )
         {
            continue;
         }

         std::string filename = std::string( buffer ) + std::string( findFileData.cFileName );

         files.push_back( filename );
      }
      while ( ::FindNextFileA( h, &findFileData ) );
      ::FindClose( h );
   }
}

std::string FileConfiguration::getFilename( const std::string& filespec )
{
   // Extract the path part of the search
   char buffer[ _MAX_PATH ];
   char* ptr;

   ::GetFullPathNameA( filespec.c_str(), sizeof( buffer ), buffer, &ptr );

   if ( ptr )
   {
      // ptr points to the first letter of the filename element
      return std::string( ptr );
   }

   return std::string();
}

std::string FileConfiguration::getAppName( const char* argv0 )
{
   // This will try and strip the extension from the filename, or just return the filename
   // This is not particularly efficient - but it doesn't really need to be, either
   std::string filename = getFilename( argv0 );
   std::string appname = filename;
   int pos = 0;
   for ( std::string::iterator it = filename.begin(); it != filename.end(); it++ )
   {
      // Keep doing this until the end
      if ( *it == '.' )
      {
         appname = filename.substr( 0, pos );
      }
      pos++;
   }
   return appname;
}
