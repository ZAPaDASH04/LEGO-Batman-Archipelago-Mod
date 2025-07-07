

// // injector.cpp
// #include <windows.h>
// #include <tlhelp32.h>
// #include <iostream>

// DWORD GetProcessIdByName(const char* name) {
//     PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
//     HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//     if (snapshot == INVALID_HANDLE_VALUE) return 0;

//     if (Process32First(snapshot, &pe)) {
//         do {
//             if (_stricmp(pe.szExeFile, name) == 0) {
//                 CloseHandle(snapshot);
//                 return pe.th32ProcessID;
//             }
//         } while (Process32Next(snapshot, &pe));
//     }

//     CloseHandle(snapshot);
//     return 0;
// }

// const char* GetDllFullPath(const char* dllName) {
//     char exePath[MAX_PATH];
//     GetModuleFileNameA(NULL, exePath, MAX_PATH);

//     // Strip filename from path
//     char* lastSlash = strrchr(exePath, '\\');
//     if (lastSlash) {
//         *(lastSlash + 1) = '\0'; // Keep trailing slash
//     }

//     return (std::string(exePath) + dllName).c_str();
// }

// int main() {
//     const char* dllPath = GetDllFullPath("mod.dll"); // dllPath = "C:\\stupid\\mod.dll"; // Use absolute path
//     std::cout << "Injecting DLL from path: " << dllPath << std::endl;
//     DWORD pid = GetProcessIdByName("LEGOBatman.exe");

//     if (!pid) {
//         std::cerr << "LEGOBatman.exe not found.\n";
//         return 1;
//     }

//     HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//     if (!hProcess) {
//         std::cerr << "Failed to open process.\n";
//         return 1;
//     }

//     LPVOID remoteMem = VirtualAllocEx(hProcess, nullptr, strlen(dllPath) + 1,
//                                       MEM_COMMIT, PAGE_READWRITE);
//     if (!remoteMem) {
//         std::cerr << "VirtualAllocEx failed. Error: " << GetLastError() << "\n";
//         return 1;
//     }

//     if (!WriteProcessMemory(hProcess, remoteMem, dllPath, strlen(dllPath) + 1, nullptr)) {
//         std::cerr << "WriteProcessMemory failed. Error: " << GetLastError() << "\n";
//         return 1;
//     }
//     // HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
//     //     (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteMem, 0, nullptr);
//     HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
//     if (!hKernel32) {
//         std::cerr << "Failed to get handle for kernel32.dll. Error: " << GetLastError() << "\n";
//         return 1;
//     }

//     FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");
//     if (!loadLibraryAddr) {
//         std::cerr << "Failed to get address of LoadLibraryA. Error: " << GetLastError() << "\n";
//         return 1;
//     }
//     //std::cout << "LoadLibraryA address: " << loadLibraryAddr << "\n";
//     printf("LoadLibraryA address: %p\n", loadLibraryAddr);

//     HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
//         (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteMem, 0, nullptr);

//     DWORD exitCode;
//     if (hThread) {
//         std::cout << "Injection thread created.\n";
//         WaitForSingleObject(hThread, INFINITE); // wait for DLL thread to complete
//         GetExitCodeThread(hThread, &exitCode);
//         std::cout << "DLL LoadLibraryA return code: " << std::hex << exitCode << "\n";
//         CloseHandle(hThread);
//     } else {
//         std::cerr << "Injection failed. Error: " << GetLastError() << "\n";
//     }

//     CloseHandle(hProcess);

//     //MessageBoxA(NULL, "Injected?", "mod.dll", MB_OK);

//     return 0;
// }


#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iomanip>
#include <filesystem>
#include <Shlwapi.h>

namespace filesystem = std::filesystem;

bool injectDLL(DWORD);

char dllName[] = "lb1mod.dll";
char ProcessName[] = "LEGOBatman.exe";
//typedef HINSTANCE (* fpLoadLibrary)(char *);



DWORD GetProcId(const char* pn, unsigned short int fi = 0b1101)
{
    DWORD procId = 0;
    while (true) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hSnap != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 pE;
            pE.dwSize = sizeof(pE);

            if (Process32First(hSnap, &pE))
            {
                if (!pE.th32ProcessID)
                    Process32Next(hSnap, &pE);
                do
                {
                    if (fi == 0b10100111001)
                        std::cout << pE.szExeFile << u8"\x09\x09\x09" << pE.th32ProcessID << std::endl;
                    if (!_stricmp(pE.szExeFile, pn))
                    {
                        procId = pE.th32ProcessID;
                        //printf("Process : 0x%lx\n", pE);
                        break;
                    }
                } while (Process32Next(hSnap, &pE));
            }
        }
        CloseHandle(hSnap);
        if (procId!=0) break;
        Sleep(1000);
    }
    return procId;
}


BOOL InjectDLL(DWORD procID, const char* dllPath)
{
    BOOL WPM = 0;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
    if (hProc == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WPM = WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
    if (!WPM)
    {
        CloseHandle(hProc);
        return -1;
    }
    printf("DLL Injected Succesfully 0x%X\n", WPM);
    HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)(void*)LoadLibraryA, loc, 0, 0);
    if (!hThread)
    {
        VirtualFree(loc, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProc);
        return -1;
    }
    printf("Thread Created Succesfully 0x%p\n", hThread);
    CloseHandle(hProc);
    VirtualFree(loc, strlen(dllPath) + 1, MEM_RELEASE);
    CloseHandle(hThread);
    return 0;
}

int main(void)
{
    //std::string pname, dllpath;
    const char* pname = "LEGOBatman.exe";
    //const char* dllpath = "C:\\Users\\Joseph Howard\\Desktop\\REPOSITORIES\\Lego Archepelago\\LB1hooking\\test\\lb1mod.dll";
    const char* dllpath = "C:\\Users\\Public\\stupid\\lb1mod.dll";

    // printf("process name (The name of process to inject ) :");
    // std::cin >> pname;
    // printf("dll path (Full path to the desired dll ) : ");
    // std::cin >> dllpath;
    // system("cls");

    
    if (filesystem::exists(dllpath) == FALSE)
    {
        printf("dll does not exist.\n");
        //wprintf(L"%s DLL File does NOT exist!\n", wide);
        return EXIT_FAILURE;
    }
    DWORD procID = 0;
    procID = GetProcId(pname);
    if (procID == 0)
    {
        printf("Process Not found (0x%lX)\n", GetLastError());
        printf("Here is a list of available process %lu\n", GetLastError());
        Sleep(3500);
        //GetProcId("skinjbir", 0b10100111001);
    }
    else
        InjectDLL(procID, dllpath);

    
    return EXIT_SUCCESS;
}


// int main() 
// {
//     DWORD processID = 0;

//     PROCESSENTRY32 pe32 = {};
//     pe32.dwSize = sizeof(PROCESSENTRY32);

//     HANDLE hProcSnap;

//     std::printf("Waiting for game.\n");

//     while(!processID) {
        
//         hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

//         // run through processes.
//         if (Process32First(hProcSnap, &pe32)) {
//             do {
//                 // TODO: test size of processes found. to switch to wcsncmp. actually reverse string cmp?
//                 //printf("proc: %s | count: 14 %lld\n", pe32.szExeFile, strlen(pe32.szExeFile));

//                 if (!_stricmp(pe32.szExeFile, ProcessName)) {
//                     processID = pe32.th32ProcessID; // get id
//                     break;
//                 }
//             } while (Process32Next(hProcSnap, &pe32));

//         }
//         Sleep(1000);
//     }
//     // process found
//     printf("Process Found.\nInjecting now...\n");

//     while(!injectDLL(processID))
//     {
//         Sleep(500);
//     }

//     // dll injected
//     printf("DLL injected sucessfully.\n\n");

//     CloseHandle(hProcSnap);
//     Sleep(1000); // TODO: unecesary?
//     return 0;
// }


// bool injectDLL(DWORD processID) {
//     HANDLE hProc, hThread;
//     LPVOID paramAddr;

//     // HINSTANCE hDll = LoadLibrary((L"KERNEL32"));

//     // fpLoadLibrary LoadLibraryAddr = (fpLoadLibrary) GetProcAddress(hDll, "LoadLibraryA");

//     hProc = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
//     if (hProc == INVALID_HANDLE_VALUE) {
//         std::printf("Could not open Process\n");
//         return false;
//     }

//     // TODO: temporary. requires relative path.
//     char dllPath[250] = "C:\\Users\\Joseph Howard\\Desktop\\REPOSITORIES\\Lego Archepelago\\LB1hooking\\test\\";
//     //char dllPath[250] = filesystem::absolute(".\\");
//     //char dllPath[250] = ".\\";
//     DWORD pathSize = strlen(dllPath) + strlen(dllName);
//     // 250 is assumed
//     strncat(dllPath, dllName, strlen(dllPath)); //TODO: perfect str___ choices
//     std::printf("%s\n",dllPath);

//     paramAddr = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//     std::printf("Writing %p\n", paramAddr);
//     bool memoryWritten = WriteProcessMemory(hProc, paramAddr, dllPath, pathSize+1, NULL);
//     if (memoryWritten == false) {
//         std::printf("VirtualAllocEx failed: %lu\n", GetLastError());
//         std::printf("Could not write to Process\n");
//         perror("dange");
//         CloseHandle(hProc);
//         return memoryWritten;
//     }

//     //CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE) LoadLibraryAddr, paramAddr, 0, 0);  
//     hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)(void*)LoadLibraryA, paramAddr, 0, 0);
//     if (!hThread) {
//         std::printf("Could not create thread.\n");
//         VirtualFree(paramAddr, pathSize + 1, MEM_RELEASE);
//         CloseHandle(hProc);
//         return false;
//     }

//     //printf("Thread Created Succesfully 0x%lX\n", hThread);
//     CloseHandle(hProc);
//     VirtualFree(paramAddr, pathSize + 1, MEM_RELEASE);
//     CloseHandle(hThread);
//     return memoryWritten;
// }