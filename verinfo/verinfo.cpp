// verinfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h>

int expandFilespecs( const std::string& filespec, std::vector<std::string>& filenames );
int verinfo( const std::string& filename );

int main( int argc, char** argv )
{
   int result = 0;

   std::vector<std::string> filespecs;
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
            filespecs.push_back( arg );
         }
      }
   }

   std::vector<std::string> filenames;
   for ( std::vector<std::string>::const_iterator it = filespecs.cbegin(); it != filespecs.cend(); it++ )
   {
      expandFilespecs( *it, filenames );
   }

   for ( std::vector<std::string>::const_iterator it = filenames.cbegin(); it != filenames.cend(); it++ )
   {
      verinfo( *it );
   }

   return result;
}

int expandFilespecs( const std::string& filespec, std::vector<std::string>& filenames )
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
      std::cout << "  " << filename.c_str() << std::endl;

      filenames.push_back( filename );
   }
   while ( ::FindNextFileA( h, &findFileData ) );
   ::FindClose( h );

   return 0;
}

int verinfo( const std::string& filename )
{
   DWORD handle;
   DWORD size = ::GetFileVersionInfoSizeA( filename.c_str(), &handle );

   LPBYTE data = new BYTE[ size ];
   if ( ::GetFileVersionInfoA( filename.c_str(), 0, size, data ) )
   {
      std::cout << filename.c_str() << std::endl;

      LPBYTE buffer;
      UINT length;
      if ( ::VerQueryValueA( data, "\\", (LPVOID*) &buffer, &length ) )
      {
         if ( size )
         {
            VS_FIXEDFILEINFO* fixedFileInfo = (VS_FIXEDFILEINFO*) buffer;
            if ( fixedFileInfo->dwSignature == 0xFEEF04BD )
            {
               std::stringstream str;
               
               str << HIWORD( fixedFileInfo->dwFileVersionMS ) << ".";
               str << LOWORD( fixedFileInfo->dwFileVersionMS ) << ".";
               str << HIWORD( fixedFileInfo->dwFileVersionLS ) << ".";
               str << LOWORD( fixedFileInfo->dwFileVersionLS );

               std::cout << "  File version: " << str.str().c_str() << std::endl;
            }
            if ( fixedFileInfo->dwSignature == 0xFEEF04BD )
            {
               std::stringstream str;

               str << HIWORD( fixedFileInfo->dwProductVersionMS ) << ".";
               str << LOWORD( fixedFileInfo->dwProductVersionMS ) << ".";
               str << HIWORD( fixedFileInfo->dwProductVersionLS ) << ".";
               str << LOWORD( fixedFileInfo->dwProductVersionLS );

               std::cout << "  Prod version: " << str.str().c_str() << std::endl;
            }
         }
      }
      else
      {
         std::cout << filename.c_str() << " failed" << std::endl;
      }
   }
   else
   {
      std::cout << filename.c_str() << " failed" << std::endl;
   }

   delete[] data;
   return 0;
}