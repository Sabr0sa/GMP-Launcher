#pragma once
#include <string>

struct ProgramArgs {
    std::wstring gothic{L"System\\Gothic2.exe"};
    std::wstring dll{L"gmp\\gmp.dll"};
    std::wstring host{L"localhost|28960"};
    std::wstring nickname;
    std::wstring debugLevel{L"-1"};
    bool enableGothicException{false};
};

inline ProgramArgs parseProgramArgs(int argc, wchar_t *argv[]) {
    ProgramArgs programArgs{};

    constexpr wchar_t gothicArg[] = L"--gothic=";
    constexpr wchar_t dllArg[] = L"--dll=";
    constexpr wchar_t hostArg[] = L"--host=";
    constexpr wchar_t nicknameArg[] = L"--nickname=";
    constexpr wchar_t debugLevelArg[] = L"--debug=";
    constexpr wchar_t exceptionArg[] = L"--exception";
    constexpr wchar_t helpArg[] = L"--help";
    for (int i = 1; i < argc; i++) {
        std::wstring_view arg(argv[i]);
        if (arg.find(gothicArg) != std::wstring_view::npos) {
            programArgs.gothic = arg.substr((sizeof(gothicArg) - 1) / sizeof(wchar_t));
        } else if (arg.find(dllArg) != std::wstring_view::npos) {
            programArgs.dll = arg.substr((sizeof(dllArg) - 1) / sizeof(wchar_t));
        } else if (arg.find(hostArg) != std::wstring_view::npos) {
            programArgs.host = arg.substr((sizeof(hostArg) - 1) / sizeof(wchar_t));
        } else if (arg.find(nicknameArg) != std::wstring_view::npos) {
            programArgs.nickname = arg.substr((sizeof(nicknameArg) - 1) / sizeof(wchar_t));
        } else if (arg.find(debugLevelArg) != std::wstring_view::npos) {
            programArgs.debugLevel = arg.substr((sizeof(debugLevelArg) - 1) / sizeof(wchar_t));
        } else if (arg.find(exceptionArg) != std::wstring_view::npos) {
            programArgs.enableGothicException = true;
        } else if (arg.find(helpArg) != std::wstring_view::npos) {
            puts("Usage: gmpinjector [options]\nOptions:");
            printf("  %ls\tPath to the Gothic executable. Defaults to %ls\n", gothicArg, programArgs.gothic.c_str());
            printf("  %ls\tPath to the GMP client DLL. Defaults to %ls\n", dllArg, programArgs.dll.c_str());
            printf("  %ls\tHost and port of the server to connect to, seperated by '|'. Defaults to %ls\n", hostArg, programArgs.host.c_str());
            printf("  %ls\tNickname to use on the server. By default, the server will generate a nickname for you\n", nicknameArg);
            printf("  %ls<0-9>\tDebug level of the zSpy. By default, logging is deactivated (-1). If activated, will start the zSpy in the Gothic directory\n", debugLevelArg);
            printf("  %ls\tWhether to use the Gothic exception handler. Defaults to %s\n", exceptionArg, (programArgs.enableGothicException ? "true" : "false"));
            std::exit(EXIT_SUCCESS);
        }
    }
    return programArgs;
}
