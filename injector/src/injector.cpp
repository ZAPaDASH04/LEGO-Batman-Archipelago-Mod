
// /**
//  * @file injector.cpp
//  * @author UNCLEAR. given by: pasdalover on discord https://discord.com/channels/539431629718945793/765144687479750676/1381908151514697730 who claims 50% ai generated.
//  * @brief 
//  * @version 0.1
//  * @date 2025-06-11
//  * 
//  * @copyright Copyright (c) 2025
//  * 
//  */

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>

// Modify as needed
const wchar_t* P = L"LEGOBatman.exe";
std::wstring D = L"mod";//"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Lego Batman\\mod.dll"; // Use full absolute path!


DWORD FindProcessId(const wchar_t* procName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(entry);

    DWORD pid = 0;
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, procName) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}


bool IsMainModuleLoaded(DWORD pid, const wchar_t* moduleName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32W me;
    me.dwSize = sizeof(me);

    if (Module32FirstW(hSnapshot, &me)) {
        do {
            if (_wcsicmp(me.szModule, moduleName) == 0) {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Module32NextW(hSnapshot, &me));
    }

    CloseHandle(hSnapshot);
    return false;
}

int main() {
    std::wstring b1 = L"er";
    std::wcout << L"[*] Waiting for process: " << P << std::endl;

    DWORD pid = 0;
    while ((pid = FindProcessId(P)) == 0) {
        Sleep(500); // Wait until the game is running
    }

    std::wcout << L"[+] Found process PID: " << pid << std::endl;
    std::wcout << L"[+] Waiting for module: LEGOBatman.exe" << std::endl;
    std::wstring b3 = L"l32";

    // while (!IsMainModuleLoaded(pid, L"LEGOBatman.exe")) {
    //     std::wcout << L"[*] Waiting for main module...\n";
    //     Sleep(500);
    // }
    // std::wcout << L"[+] Main module is loaded" << std::endl;

    // TODO: Test this
    while (!IsMainModuleLoaded(pid, L"xinput1_3.dll")) {
        std::wcout << L"[*] Waiting for final module...\n";
        Sleep(500);
    }

    std::wstring b5 = L"l";
    std::string a1 = "ad"; 

    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid);

    if (!hProcess) {
        std::cerr << "[-] Failed to open target process." << std::endl;
        return 1;
    }


    std::wstring b4 = L".d";
    D = D + b4;
    std::string a2 = "Libra";
    D = D + b5;
    std::wstring b2 = L"ne";
    D = D + b5;


    size_t pathLen = (wcslen(D.c_str()) + 1) * sizeof(wchar_t);
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        std::cerr << "[-] Failed to allocate memory in target process." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, remoteMem, D.c_str(), pathLen, NULL)) {
        std::cerr << "[-] Failed to write DLL path to target process memory." << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }
    std::string a4 = "A";

    //HMODULE hKernel32 = GetModuleHandleW(L"Kernel32.dll");
    //std::cout << "[+] Kernel Handle is " << std::hex << (DWORD) hKernel32 << std::endl;
    std::string a3 = "ry";
    std::wstring b0 = L"K";
    std::string a0 = "Lo";
    FARPROC loadLibraryAddr = GetProcAddress(GetModuleHandleW((b0+b1+b2+b3+b4+b5+b5).c_str()), (a0+a1+a2+a3+a4).c_str());
    //std::cout << "[+] load lib is " << std::hex << (DWORD) loadLibraryAddr << std::endl;

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr,
        remoteMem, 0, NULL);

    if (!hThread) {
        std::cerr << "[-] Failed to create remote thread." << std::endl;
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] DLL injection thread created." << std::endl;

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);
    std::cout << "[*] LoadLibraryW return (HMODULE): 0x" << std::hex << exitCode << std::endl;

    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "[*] Done." << std::endl;
    return 0;
}