// Файл 02_CFileInfo.cpp
// Информация о файле на диске
//

//=====================================================================================================================
// Заголовочные файлы
#include "02_CFileInfo.h"
#include "../03_Encoding/UTF8.h"




//=====================================================================================================================
// Конструктор класса
CFileInfo::CFileInfo()
{
    myIsDirectory = false;
}




//=====================================================================================================================
// Доступ к информации о файле
const string & CFileInfo::getFileName() const
{
    return myFileName;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации о файле
const string & CFileInfo::getFilePath() const
{
    return myFilePath;
}




//---------------------------------------------------------------------------------------------------------------------
// Доступ к информации о файле
bool CFileInfo::isDirectory() const
{
    return myIsDirectory;
}




//=====================================================================================================================
// Сбор информации о содержимом каталога
vector<CFileInfo> CFileInfo::listDirectory(const string &path, const string &mask)
{
#ifdef SYSTEM_WINDOWS
    //=================================================================================================================
    // Версия для Windows
    vector<CFileInfo> result;
    string maskedPath = path;
    if (maskedPath.empty() == false)
    {
        char sym = maskedPath[maskedPath.size() - 1];
        if (sym != '/' && sym != '\\')
            maskedPath += "\\";
    }
    maskedPath += mask;

    WIN32_FIND_DATA findFileData;
    HANDLE hFindFile = FindFirstFile(maskedPath.c_str(), &findFileData);
    if (hFindFile == INVALID_HANDLE_VALUE)
        return result;

    do
    {
        if (strcmp(findFileData.cFileName, ".") == 0)
            continue;
        if (strcmp(findFileData.cFileName, "..") == 0)
            continue;

        CFileInfo newFile;
        newFile.myFilePath = path;
        newFile.myFileName = findFileData.cFileName;
        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            newFile.myIsDirectory = true;
        else
            newFile.myIsDirectory = false;

        result.push_back(newFile);
    } while (FindNextFile(hFindFile, &findFileData) != FALSE);

    return result;


#else
    //=================================================================================================================
    // Версия для Linux
    vector<CFileInfo> result;
    string utfpath = toUTF8(path);
    string utfmask = toUTF8(mask);
    DIR * dir = opendir(utfpath.c_str());
    if (dir == NULL)
        return result;

    dirent * info;
    while (info = readdir(dir), info != NULL)
    {
        if (fnmatch(utfmask.c_str(), info->d_name, 0) != 0)
            continue;

        CFileInfo newFile;
        newFile.myFilePath = path;
        newFile.myFileName = fromUTF8(info->d_name);
        if ((info->d_type & DT_DIR) != 0)
            newFile.myIsDirectory = true;
        else
            newFile.myIsDirectory = false;

        result.push_back(newFile);
    }

    closedir(dir);
    return result;
#endif
}

