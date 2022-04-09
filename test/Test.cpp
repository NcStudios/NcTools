#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <Windows.h>

constexpr const char* PassText = "\033[32mPass: \033[0m";
constexpr const char* FailText = "\033[31mFail: \033[0m";
constexpr const char* BadFileText = "\033[33mBadFile: \033[0m";

struct TestInput
{
    std::string assetType;
    std::filesystem::path rawPath;
    std::filesystem::path referencePath;
};

auto ReadTestInput(const std::filesystem::path& path) -> std::vector<TestInput>
{
    auto file = std::ifstream{path};
    if (!file)
    {
        std::cerr << "Bad input path: " << path << '\n';
        return {};
    }

    std::vector<TestInput> out;
    TestInput current;
    while(!file.eof())
    {
        file >> current.assetType >> current.rawPath >> current.referencePath;
        out.push_back(std::move(current));
    }

    return out;
}

bool RunCommand(std::string appName, std::string cmdLine)
{
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(processInfo));
    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = INVALID_HANDLE_VALUE;
    startupInfo.hStdInput = INVALID_HANDLE_VALUE;
    startupInfo.hStdOutput = INVALID_HANDLE_VALUE;
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    auto result = CreateProcessA(TEXT(appName.data()), TEXT(cmdLine.data()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo);

    if(!result)
    {
        auto error = GetLastError();
        std::cerr << "Failure creating nc-tools process: " << error << '\n';
        return false;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    DWORD exitCode;
    result = GetExitCodeProcess(processInfo.hProcess, &exitCode);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return true;
}

bool RunBuilder(const std::filesystem::path& in, const std::filesystem::path& outDir, const std::string& type)
{
    std::stringstream cmdStream;
    cmdStream << "../nc-tools.exe -a " << type << " -i " << in.lexically_normal() << " -o " << outDir.lexically_normal();

    if(!RunCommand("../nc-tools.exe", cmdStream.str()))
    {
        std::cerr << "Failure building asset file: " << in.string() << '\n';
        return false;
    }

    return true;
}

auto ReadFile(const std::filesystem::path& path) -> std::vector<char>
{
    constexpr auto flags = std::ios::in | std::ios::binary | std::ios::ate;
    auto file = std::ifstream{path, flags};
    const auto numBytes = file.tellg();
    file.seekg(0);
    auto data = std::vector<char>(numBytes);
    file.read(data.data(), numBytes);
    return data;
}

bool EqualFiles(const std::filesystem::path& first, const std::filesystem::path& second)
{
    auto data1 = ReadFile(first);
    auto data2 = ReadFile(second);
    return data1.size() == data2.size() && std::equal(data1.cbegin(), data1.cend(), data2.cbegin());
}

int RunTests(const std::vector<TestInput>& files)
{
    int failureCount = 0;
    for(const auto& [type, rawPath, referencePath] : files)
    {
        if (!std::filesystem::exists(rawPath) || !std::filesystem::exists(referencePath))
        {
            std::cerr << BadFileText << rawPath << " or " << referencePath << '\n';
            continue;
        }

        auto testPath = std::filesystem::path{"temp"} / rawPath.filename();
        testPath.replace_extension(".nca");

        auto buildResult = RunBuilder(rawPath, "temp", type);
        if(!buildResult)
        {
            std::cerr << "nc-tools failure: " << rawPath << '\n';
            continue;
        }

        auto result = EqualFiles(referencePath, testPath);
        if(!result) ++failureCount;
        std::cerr << (result ? PassText : FailText) << testPath.filename() << '\n';
    }

    return failureCount;
}

int main(int argc, const char** argv)
{
    std::cout << "Running tests for NcAssetTools\n"
              << "------------------------------\n";

    if (argc != 2)
    {
        std::cerr << "Error: arg count expected to be 2 but was " << argc << '\n';
        return 1;
    }

    auto files = ReadTestInput(argv[1]);
    auto failureCount = RunTests(files);

    std::cerr << "\nResults - Run: " << files.size() << ", Failed: " << failureCount << '\n';

    return failureCount;
}