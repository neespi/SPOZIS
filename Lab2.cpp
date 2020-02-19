#include <conio.h>
#include <windows.h>
#include "winbase.h"
#include "tlhelp32.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define cout wcout
#define cin wcin

TCHAR IpFilename[MAX_PATH];
HMODULE hModule = GetModuleHandle(NULL); // NULL чтобы получить хэндл главного модуля (exe) 

void ShowProcessEntry(PROCESSENTRY32& entry)
{
	cout << "\n\nth32ProcessID = " << entry.th32ProcessID;
	cout << "\nth32DefaultHeapID = " << entry.th32DefaultHeapID;
	cout << "\nth32ModuleID = " << entry.th32ModuleID;
	cout << "\nth32ParentProcessID = " << entry.th32ParentProcessID;
	cout << "\ncntUsage = " << entry.cntUsage;
	cout << "\ncntThreads = " << entry.cntThreads;
	cout << "\npcPriClassBase = " << entry.pcPriClassBase;
	cout << "\ndwFlags = " << entry.dwFlags;
	cout << "\nszExeFile = " << entry.szExeFile;
}

void ShowThreadEntry(THREADENTRY32& entry)
{
	cout << "\n\nth32ThreadID = " << entry.th32ThreadID;
	cout << "\nth32OwnerProcessID = " << entry.th32OwnerProcessID;
	cout << "\ncntUsage = " << entry.cntUsage;
	cout << "\ntpBasePri = " << entry.tpBasePri;
	cout << "\ntpDeltaPri = " << entry.tpDeltaPri;
	cout << "\ndwFlags = " << entry.dwFlags;
}

void ShowModuleEntry(MODULEENTRY32& entry)

{
	cout << "\n\nth32ModuleID = " << entry.th32ModuleID;
	cout << "\nth32ProcessID = " << entry.th32ProcessID;
	cout << "\nGlblcntUsage = " << entry.GlblcntUsage;
	cout << "\nProccntUsage = " << entry.ProccntUsage;
	cout << "\nmodBaseAddr = " << entry.modBaseAddr;
	cout << "\nmodBaseSize = " << entry.modBaseSize;
	cout << "\nhModule = " << entry.hModule;
	cout << "\nszModule = " << entry.szModule;
	cout << "\nszExePath = " << entry.szExePath;
}


int fTask1() //принимая дескриптор, имя или полное имя модуля, возвращает другие два элемента в своих выходных параметрах 
{
	int m = 0;
	HANDLE handle;
	cout << GetCurrentProcessId();
	printf("\n\n\n0. Informaton\n1. Handle\n2. File Path\n3. FName\n\n");
	printf("Your selection?\n\n");
	cin >> m;
	switch (m)
	{
	case 0: {
		cout << "\nHandle file: " << hModule;
		GetModuleFileName(NULL, IpFilename, MAX_PATH);
		cout << "\nFile path: " << IpFilename;
		int n, i;
		for (i = 0; i <= 40; i++)
		{
			if (IpFilename[i] == IpFilename[29])
			{
				n = i;
			}
		}
		cout << "\nFName: ";
		for (i = n + 1; i <= 40; i++) { cout << "" <<IpFilename[i]; }
		break;
	}
	case 1: {
		printf("Mode 'Handle' selected\nInput Handle:\n");
		cin >> handle;
		cout << GetModuleFileName(NULL, IpFilename, MAX_PATH);
		break;

	}
	case 2: {
		printf("Mode 'File Path' selected\nInput File Path:\n");
		cout << GetModuleHandle(NULL);
		break;
	}
	case 3: {
		printf("Mode 'FName' selected\nInput FName:\n");
		cout << GetModuleFileName(NULL, IpFilename, MAX_PATH);
		cout << GetModuleHandle(NULL);
		break;
	}
	default: {
		printf("Invalid mode\n");
		break;
	}
	}
	cout << "\n\n\n\n";
	system("pause");
	return 0;
}

int fTask2()
{
	DWORD dCurrentId = GetCurrentProcessId();
	HANDLE hCurrentPseudo = GetCurrentProcess();
	HANDLE hCurrentIdDup;
	DuplicateHandle(hCurrentPseudo, hCurrentPseudo, hCurrentPseudo, &hCurrentIdDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
	HANDLE hCurrentIdOpen = OpenProcess(PROCESS_DUP_HANDLE, TRUE, dCurrentId);
	BOOL fClosedDup = CloseHandle(hCurrentIdDup);
	BOOL fClosedOpen = CloseHandle(hCurrentIdOpen);
	cout << "\nID of the current process: " << dCurrentId
		<< "\nPseudodescriptor of the current process: " << hCurrentPseudo
		<< "\nDescriptor recived by DuplicateHandle: " << hCurrentIdDup
		<< "\nDescriptor recived by OpenProcess: " << hCurrentIdOpen
		<< "\nClose descriptor recived by OpenProcess: " << fClosedOpen
		<< "\nClose descriptor recived by DuplicateHandle: " << fClosedDup << endl;
	system("pause");
	return 0;
}

int fTask3()
{
	cout << "\n\n\t\tProcesses\t\t\n";
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		cout << "\nSnapshot ID = " << hSnapshot;
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		BOOL fRepeat = Process32First(hSnapshot, &entry);
		int i = 0;
		while (fRepeat)
		{
			i++;
			ShowProcessEntry(entry);
			fRepeat = Process32Next(hSnapshot, &entry);
		}
		CloseHandle(hSnapshot);
		cout << "\n\t\t End of process data \t\t\n";
	}
	cout << "\n\n\t\t Streams \t\t\n";
	{
		{
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			cout << "\nSnapshot ID = " << hSnapshot;
			THREADENTRY32 entry;
			entry.dwSize = sizeof(THREADENTRY32);
			BOOL fRepeat = Thread32First(hSnapshot, &entry);
			DWORD dwCurentProcessId = GetCurrentProcessId();
			while (fRepeat)
			{
				if (entry.th32OwnerProcessID == dwCurentProcessId)
				{
				ShowThreadEntry(entry);
				}
			fRepeat = Thread32Next(hSnapshot, &entry);
			}
			CloseHandle(hSnapshot);
			cout << "\n\n\t\t End of stream data \t\t\n";
		}
	}
	cout << "\n\n\t\t Modules \t\t\n";
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
		cout << "\nSnapshot ID = " << hSnapshot;
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);
		BOOL fRepeat = Module32First(hSnapshot, &entry);
		while (fRepeat)
		{
			ShowModuleEntry(entry);
			fRepeat = Module32Next(hSnapshot, &entry);
		}
		CloseHandle(hSnapshot);
		cout << "\n\n\t\t End of module data \t\t\n";
	}
	system("pause");
	return 0;
}

int main()
{
	fTask1();
	fTask2();
	fTask3();
	cout << "\n\n\n";
	_getch();

}
