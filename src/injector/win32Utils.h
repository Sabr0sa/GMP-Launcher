#pragma once
#include <windows.h>
#include <memory>
#include <string>
#include <string_view>

inline bool isSlash(const wchar_t c) {
    return c == L'\\' || c == L'/';
}

inline std::wstring parentPath(const std::wstring_view path) {
    const wchar_t *const first = path.data();
    const wchar_t *last = first + path.size();
    // Remove everything until the first slash
    while (first != last && !isSlash(last[-1])) {
        --last;
    }
    // Now remove the slashes
    while (first != last && isSlash(last[-1])) {
        --last;
    }

    return {first, last};
}

inline std::wstring absolutePath(const std::wstring& path) {
    const DWORD size = GetFullPathNameW(path.c_str(), 0, nullptr, nullptr);
    std::wstring absolutePath(size, '\0');
    GetFullPathNameW(path.c_str(), size, absolutePath.data(), nullptr);
    return absolutePath;
}

inline bool fileExists(const std::wstring& path) {
    DWORD result = GetFileAttributesW(path.c_str());
    return result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY) == 0U;
}

inline bool isProcessRunning(const std::wstring_view process) {
    struct handle_deleter
    {
        using pointer = HANDLE;

        void operator()(pointer handle)
        {
            CloseHandle(handle);
        }
    };
    using unique_handle = std::unique_ptr<HANDLE, handle_deleter>;

    const unique_handle snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (snapshot.get() == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Couldn't create Toolhelp32Snapshot. GetLastError: %lu", GetLastError());
        return false;
    }
    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snapshot.get(), &pe)) {
        do {
            if (pe.szExeFile == process) {
                return true;
            }
        } while (Process32NextW(snapshot.get(), &pe));
    }
    return false;
}
