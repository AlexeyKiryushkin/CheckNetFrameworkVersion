#include "stdafx.h"

//
// Выделяет имя файла из полного пути к файлу.
//
std::string GetApplicationExecutablePath()
{
	char applicationExecutablePath[_MAX_PATH] = {0};

	::GetModuleFileName(NULL, applicationExecutablePath, _MAX_PATH);

   return applicationExecutablePath;
}

//
// Выделяет путь к каталогу из полного пути к файлу.
//
std::string DirectoryPathFromFilePath(const std::string & filePath)
{
   std::string::size_type indexSlash = filePath.find_last_of('/');
   std::string::size_type indexBackSlash = filePath.find_last_of('\\');

   if (indexSlash == std::string::npos)
   {
      if (indexBackSlash == std::string::npos)
         return filePath;
      else
         return filePath.substr(0, indexBackSlash);
   }
   else
   {
      if (indexBackSlash == std::string::npos)
         return filePath.substr(0, indexSlash);
      else
				return filePath.substr(0, std::max(indexSlash, indexBackSlash));
   }
}

