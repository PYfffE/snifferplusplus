#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <Shlobj.h>
//#include <linkedList.h>
#include "linkedList.h"

void getBackupFiles(WCHAR* wcPlusplusBackupDir) {
    // Создаем новый список
    List* filePathList = makelist();

    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR pathWithAsterisk[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    LPVOID lpFileData = NULL;

    StringCchCopy(pathWithAsterisk, MAX_PATH, wcPlusplusBackupDir);

    if (!DirectoryExists(wcPlusplusBackupDir)) {
        wprintf(L"I don't think path %s exist on current user :(\n", wcPlusplusBackupDir);
        return -1;
    }

    StringCchCat(pathWithAsterisk, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(pathWithAsterisk, &ffd);

    // List all the files in the directory with some info about them. Without directories

    DWORD dwMaxBufferLength = 0;
    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;
        lpFileData = VirtualAlloc(lpFileData, ffd.nFileSizeLow, MEM_COMMIT, PAGE_READWRITE);
        TCHAR fullFileName[MAX_PATH];
        StringCchCopy(fullFileName, MAX_PATH, wcPlusplusBackupDir);
        StringCchCat(fullFileName, MAX_PATH, ffd.cFileName);
        add(fullFileName, ffd.nFileSizeLow, filePathList);
        if (ffd.nFileSizeLow > dwMaxBufferLength)
            dwMaxBufferLength = ffd.nFileSizeLow;

    } while (FindNextFile(hFind, &ffd) != 0);

    // print file paths
    //display(filePathList);

    // print file data
    Node* current = filePathList->head;
    if (filePathList->head != NULL) {
        HANDLE hFile;
        LPVOID maxFileBuffer = VirtualAlloc(lpFileData, dwMaxBufferLength, MEM_COMMIT, PAGE_READWRITE);
        for (; current != NULL; current = current->next) {
            hFile = CreateFile(current->wcFilePath,               // file to open
                GENERIC_READ,          // open for reading
                0,       // share for reading
                NULL,                  // default security
                OPEN_EXISTING,         // existing file only
                FILE_ATTRIBUTE_NORMAL, // normal file
                NULL);                 // no attr. template

            int success = ReadFile(hFile, maxFileBuffer, current->dwfileSize, NULL, NULL);
            CloseHandle(hFile);
            wprintf(L"File %s:\n", current->wcFilePath);
            printf("%s\n\n", maxFileBuffer);
        }
    }


    // Очищаем память и уничтожаем список
    FindClose(hFind);
    destroy(filePathList);
    return dwError;
}

List* getAllUsersFoldersAppData() {
    List* usersList = makelist();
    TCHAR wcUsersDir[MAX_PATH] = L"C:\\users";
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    StringCchCat(wcUsersDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(wcUsersDir, &ffd);


    // List all the files in the directory with some info about them. Without directories

    DWORD dwMaxBufferLength = 0;
    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            //TCHAR userDirName[MAX_PATH] = ffd.cFileName;
            if (strcmp(L".", ffd.cFileName) || strcmp(L"..", ffd.cFileName)) {
                wprintf(L"%s\n", ffd.cFileName);
                add(ffd.cFileName, sizeof(ffd.cFileName), usersList);
            }
        }

    } while (FindNextFile(hFind, &ffd) != 0);

    return usersList;
}

BOOL DirectoryExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


int main(int argc, TCHAR* argv[])
{

    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    const TCHAR PLUSPLUS_BACKUP_REL_DIR[] = L"\\Notepad++\\backup\\";
    LPVOID lpFileData = NULL;


    TCHAR plusplusBackupPath[MAX_PATH] = { 0 };
    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, plusplusBackupPath);

    StringCchCat(plusplusBackupPath, MAX_PATH, PLUSPLUS_BACKUP_REL_DIR);
    
    if (argc == 1) {
        getBackupFiles(plusplusBackupPath);
    }
    

    else {
        if (! strcmp(argv[1], L"user")) {
            getBackupFiles(plusplusBackupPath);
        }
        else if (! argv[1], L"god") {
            // Узнать всех пользователей
            List* usersList = getAllUsersFoldersAppData();

            Node* current = usersList->head;
            if (usersList->head != NULL) {
                for (; current != NULL; current = current->next) {
                    WCHAR wcCurrentUserAppDataPath[MAX_PATH] = L"C:\\users\\";
                    StringCchCat(wcCurrentUserAppDataPath, MAX_PATH, current->wcFilePath);
                    StringCchCat(wcCurrentUserAppDataPath, MAX_PATH, L"\\AppData\\Roaming\\Notepad++\\backup\\");
                    getBackupFiles(wcCurrentUserAppDataPath);
                }
            }

            WCHAR* plusplusBackupPath;

            destroy(usersList);
        }
    }


}