// verinfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

int verinfo( const std::string& filespec );

int main( int argc, char** argv )
{
   int result = 0;

   std::vector<std::string> files;
   for ( int loop = 1; loop < argc; )
   {
      std::string arg( argv[ loop++ ] );
      if ( arg.length() > 0 )
      {
         if ( arg[ 0 ] == '-' || arg[ 0 ] == '/' )
         {
            
         }
         else
         {
            files.push_back( arg );
         }
      }
   }

   for ( std::vector<std::string>::const_iterator it = files.cbegin(); it != files.cend(); it++ )
   {
      verinfo( *it );
   }

   return result;
}

int verinfo( const std::string& filespec )
{
   std::cout << filespec.c_str() << std::endl;

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

   if ( h == INVALID_HANDLE_VALUE )
   {
      // No matches
      return 1;
   }

   do
   {
      // Ignore directories
      if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
      {
         continue;
      }

      // Ignore . and ..
      if ( strcmp( findFileData.cFileName, "." ) == 0 || strcmp( findFileData.cFileName, "." ) == 0 )
      {
         continue;
      }

      std::string filename = std::string( buffer ) + std::string( findFileData.cFileName );
      std::cout << "  " << filename.c_str() << std::endl;
   }
   while ( ::FindNextFileA( h, &findFileData ) );
   ::FindClose( h );

   return 0;
}


