#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>
#include <vector>

namespace WindowsInfo {
    enum class Family {
        Unknown,
        Client,     // Desktop Windows
        Server      // Windows Server
    };

    // 클라이언트/서버 버전 모두 포함
    enum class Version {
        Unknown,
        // 클라이언트 버전
        Windows11,          // Build 22000+
        Windows10,          // Build 10240-19045
        Windows8_1,         // Build 9600
        Windows8,           // Build 9200
        Windows7,           // Build 7600-7601
        WindowsVista,       // Build 6000-6002
        WindowsXP,          // Build 2600
        // 서버 버전
        WindowsServer2022,  // Build 20348
        WindowsServer2019,  // Build 17763
        WindowsServer2016,  // Build 14393
        WindowsServer2012R2,// Build 9600
        WindowsServer2012,  // Build 9200
        WindowsServer2008R2,// Build 7600-7601
        WindowsServer2008,  // Build 6001-6002
        WindowsServer2003   // Build 3790
    };

    // 이전과 동일...
    enum class Architecture {
        Unknown,
        x86,    // 32-bit
        x64,    // 64-bit
        ARM,
        ARM64
    };

    struct Info {
        Family family;
        Version version;
        Architecture architecture;
        std::string edition;
        std::string displayVersion;   // 22H2, 23H2 등
        std::string buildNumber;      // 10.0.22621.2861
        std::uint32_t majorVersion;   // 10
        std::uint32_t minorVersion;   // 0
        std::uint32_t buildVersion;   // 22621
        std::uint32_t revisionVersion;// 2861

        bool isServer() const { return family == Family::Server; }
        bool is64Bit() const {
            return architecture == Architecture::x64 ||
                architecture == Architecture::ARM64;
        }

        // 버전 체크 편의 함수들
        bool isWindows11OrGreater() const {
            return version == Version::Windows11;
        }

        bool isWindows10OrGreater() const {
            return version == Version::Windows10 || isWindows11OrGreater();
        }

        bool isWindows8_1OrGreater() const {
            return version == Version::Windows8_1 || isWindows10OrGreater();
        }

        bool isWindows8OrGreater() const {
            return version == Version::Windows8 || isWindows8_1OrGreater();
        }

        bool isWindows7OrGreater() const {
            return version == Version::Windows7 || isWindows8OrGreater();
        }

        bool isAtLeast(Version compareVersion) const {
            // 서버와 클라이언트 버전을 따로 비교
            if (isServer()) {
                if (version == Version::WindowsServer2022) return true;
                if (compareVersion == Version::WindowsServer2022) return false;

                if (version == Version::WindowsServer2019) return compareVersion <= Version::WindowsServer2019;
                if (compareVersion == Version::WindowsServer2019) return false;

                if (version == Version::WindowsServer2016) return compareVersion <= Version::WindowsServer2016;
                if (compareVersion == Version::WindowsServer2016) return false;

                if (version == Version::WindowsServer2012R2) return compareVersion <= Version::WindowsServer2012R2;
                if (compareVersion == Version::WindowsServer2012R2) return false;

                if (version == Version::WindowsServer2012) return compareVersion <= Version::WindowsServer2012;
                if (compareVersion == Version::WindowsServer2012) return false;

                if (version == Version::WindowsServer2008R2) return compareVersion <= Version::WindowsServer2008R2;
                if (compareVersion == Version::WindowsServer2008R2) return false;

                if (version == Version::WindowsServer2008) return compareVersion <= Version::WindowsServer2008;
                if (compareVersion == Version::WindowsServer2008) return false;

                if (version == Version::WindowsServer2003) return compareVersion <= Version::WindowsServer2003;
            }
            else {
                if (version == Version::Windows11) return true;
                if (compareVersion == Version::Windows11) return false;

                if (version == Version::Windows10) return compareVersion <= Version::Windows10;
                if (compareVersion == Version::Windows10) return false;

                if (version == Version::Windows8_1) return compareVersion <= Version::Windows8_1;
                if (compareVersion == Version::Windows8_1) return false;

                if (version == Version::Windows8) return compareVersion <= Version::Windows8;
                if (compareVersion == Version::Windows8) return false;

                if (version == Version::Windows7) return compareVersion <= Version::Windows7;
                if (compareVersion == Version::Windows7) return false;

                if (version == Version::WindowsVista) return compareVersion <= Version::WindowsVista;
                if (compareVersion == Version::WindowsVista) return false;

                if (version == Version::WindowsXP) return compareVersion <= Version::WindowsXP;
            }
            return false;
        }

        bool supportsFeature(const std::string& featureName) const {
            if (featureName == "DirectStorage") {
                return isWindows10OrGreater() && is64Bit();
            }
            else if (featureName == "WSL2") {
                return isWindows10OrGreater() && buildVersion >= 18362;
            }
            else if (featureName == "DirectX12") {
                return isWindows10OrGreater();
            }
            else if (featureName == "ModernStandby") {
                return isWindows8OrGreater();
            }
            // 추가 기능 체크는 여기에 구현
            return false;
        }
    };

    Info GetWindowsInfo() {
        Info info;
        info.family = Family::Unknown;
        info.version = Version::Unknown;
        info.architecture = Architecture::Unknown;
        info.majorVersion = 0;
        info.minorVersion = 0;
        info.buildVersion = 0;
        info.revisionVersion = 0;

        SYSTEM_INFO si;
        GetNativeSystemInfo(&si);

        // 아키텍처 확인
        switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            info.architecture = Architecture::x64;
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            info.architecture = Architecture::x86;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            info.architecture = Architecture::ARM;
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            info.architecture = Architecture::ARM64;
            break;
        default:
            info.architecture = Architecture::Unknown;
        }

        // RtlGetVersion 사용
        typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
        auto RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(
            GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");

        if (RtlGetVersion) {
            RTL_OSVERSIONINFOW osvi = { sizeof(osvi) };
            if (RtlGetVersion(&osvi) == 0) {
                info.majorVersion = osvi.dwMajorVersion;
                info.minorVersion = osvi.dwMinorVersion;
                info.buildVersion = osvi.dwBuildNumber;

                // 서버 체크
                OSVERSIONINFOEXW osvi_ex = { sizeof(OSVERSIONINFOEXW) };
                osvi_ex.dwMajorVersion = osvi.dwMajorVersion;
                osvi_ex.dwMinorVersion = osvi.dwMinorVersion;
                osvi_ex.dwBuildNumber = osvi.dwBuildNumber;

                DWORDLONG conditionMask = 0;
                VER_SET_CONDITION(conditionMask, VER_PRODUCT_TYPE, VER_EQUAL);
                osvi_ex.wProductType = VER_NT_SERVER;
                bool isServer = VerifyVersionInfoW(&osvi_ex, VER_PRODUCT_TYPE, conditionMask);

                info.family = isServer ? Family::Server : Family::Client;

                // 버전 판단
                if (osvi.dwMajorVersion == 10) {
                    if (isServer) {
                        if (osvi.dwBuildNumber >= 20348) {
                            info.version = Version::WindowsServer2022;
                        }
                        else if (osvi.dwBuildNumber >= 17763) {
                            info.version = Version::WindowsServer2019;
                        }
                        else {
                            info.version = Version::WindowsServer2016;
                        }
                    }
                    else {
                        if (osvi.dwBuildNumber >= 22000) {
                            info.version = Version::Windows11;
                        }
                        else {
                            info.version = Version::Windows10;
                        }
                    }
                }
                else if (osvi.dwMajorVersion == 6) {
                    if (isServer) {
                        switch (osvi.dwMinorVersion) {
                        case 3:
                            info.version = Version::WindowsServer2012R2;
                            break;
                        case 2:
                            info.version = Version::WindowsServer2012;
                            break;
                        case 1:
                            info.version = Version::WindowsServer2008R2;
                            break;
                        case 0:
                            info.version = Version::WindowsServer2008;
                            break;
                        }
                    }
                    else {
                        switch (osvi.dwMinorVersion) {
                        case 3:
                            info.version = Version::Windows8_1;
                            break;
                        case 2:
                            info.version = Version::Windows8;
                            break;
                        case 1:
                            info.version = Version::Windows7;
                            break;
                        case 0:
                            info.version = Version::WindowsVista;
                            break;
                        }
                    }
                }
                else if (osvi.dwMajorVersion == 5) {
                    if (isServer) {
                        if (osvi.dwMinorVersion == 2) {
                            info.version = Version::WindowsServer2003;
                        }
                    }
                    else {
                        if (osvi.dwMinorVersion == 1) {
                            info.version = Version::WindowsXP;
                        }
                    }
                }

                // 빌드 번호 문자열 생성
                info.buildNumber = std::to_string(osvi.dwMajorVersion) + "." +
                    std::to_string(osvi.dwMinorVersion) + "." +
                    std::to_string(osvi.dwBuildNumber);

                // UBR(업데이트 빌드 리비전) 가져오기
                DWORD ubr = 0;
                DWORD ubrSize = sizeof(DWORD);
                if (RegGetValueW(HKEY_LOCAL_MACHINE,
                    L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                    L"UBR",
                    RRF_RT_REG_DWORD,
                    nullptr,
                    &ubr,
                    &ubrSize) == ERROR_SUCCESS) {
                    info.revisionVersion = ubr;
                    info.buildNumber += "." + std::to_string(ubr);
                }
            }
        }

        // 에디션과 디스플레이 버전 정보 가져오기
        auto GetRegistryString = [](const wchar_t* valueName) -> std::string {
            wchar_t buffer[256] = { 0 };
            DWORD size = sizeof(buffer);
            if (RegGetValueW(HKEY_LOCAL_MACHINE,
                L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                valueName,
                RRF_RT_REG_SZ,
                nullptr,
                buffer,
                &size) == ERROR_SUCCESS) {
                int len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
                std::string result(len - 1, 0);
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], len, NULL, NULL);
                return result;
            }
            return "";
            };

        info.edition = GetRegistryString(L"EditionID");
        info.displayVersion = GetRegistryString(L"DisplayVersion");

        return info;
    }

    // 버전을 문자열로 변환하는 유틸리티 함수
    std::string VersionToString(Version version) {
        switch (version) {
        case Version::Windows11: return "Windows 11";
        case Version::Windows10: return "Windows 10";
        case Version::Windows8_1: return "Windows 8.1";
        case Version::Windows8: return "Windows 8";
        case Version::Windows7: return "Windows 7";
        case Version::WindowsVista: return "Windows Vista";
        case Version::WindowsXP: return "Windows XP";
        case Version::WindowsServer2022: return "Windows Server 2022";
        case Version::WindowsServer2019: return "Windows Server 2019";
        case Version::WindowsServer2016: return "Windows Server 2016";
        case Version::WindowsServer2012R2: return "Windows Server 2012 R2";
        case Version::WindowsServer2012: return "Windows Server 2012";
        case Version::WindowsServer2008R2: return "Windows Server 2008 R2";
        case Version::WindowsServer2008: return "Windows Server 2008";
        case Version::WindowsServer2003: return "Windows Server 2003";
        default: return "Unknown Windows Version";
        }
    }
}

int main() {
    try {
        auto info = WindowsInfo::GetWindowsInfo();

        // 1. 기본 시스템 정보
        printf("\n=== Windows System Information ===\n");
        printf("OS Version: %s\n", WindowsInfo::VersionToString(info.version).c_str());
        printf("Family: %s\n", info.isServer() ? "Server" : "Client");
        printf("Edition: %s\n", info.edition.c_str());
        printf("Display Version: %s\n", info.displayVersion.c_str());
        printf("Architecture: %s\n", [&info]() {
            switch (info.architecture) {
            case WindowsInfo::Architecture::x86: return "32-bit (x86)";
            case WindowsInfo::Architecture::x64: return "64-bit (x64)";
            case WindowsInfo::Architecture::ARM: return "ARM";
            case WindowsInfo::Architecture::ARM64: return "ARM64";
            default: return "Unknown Architecture";
            }
            }());

        // 2. 상세 버전 정보
        printf("\n=== Detailed Version Information ===\n");
        printf("Version Number: %d.%d.%d.%d\n",
            info.majorVersion, info.minorVersion,
            info.buildVersion, info.revisionVersion);
        printf("Build String: %s\n", info.buildNumber.c_str());

        // 3. 버전 체크 예시
        printf("\n=== Version Checks ===\n");
        printf("Windows 11 or later: %s\n", info.isWindows11OrGreater() ? "Yes" : "No");
        printf("Windows 10 or later: %s\n", info.isWindows10OrGreater() ? "Yes" : "No");
        printf("Windows 8.1 or later: %s\n", info.isWindows8_1OrGreater() ? "Yes" : "No");
        printf("Windows 8 or later: %s\n", info.isWindows8OrGreater() ? "Yes" : "No");
        printf("Windows 7 or later: %s\n", info.isWindows7OrGreater() ? "Yes" : "No");

        // 4. 서버 버전 특화 체크
        if (info.isServer()) {
            printf("\n=== Server Version Specific ===\n");
            printf("Is Server 2022: %s\n",
                (info.version == WindowsInfo::Version::WindowsServer2022) ? "Yes" : "No");
            printf("Is Server 2019 or later: %s\n",
                info.isAtLeast(WindowsInfo::Version::WindowsServer2019) ? "Yes" : "No");
            printf("Is Server 2016 or later: %s\n",
                info.isAtLeast(WindowsInfo::Version::WindowsServer2016) ? "Yes" : "No");
        }

        // 5. 시스템 아키텍처 관련
        printf("\n=== Architecture Information ===\n");
        printf("64-bit System: %s\n", info.is64Bit() ? "Yes" : "No");
        if (info.is64Bit()) {
            printf("Architecture Type: %s\n",
                info.architecture == WindowsInfo::Architecture::ARM64 ? "ARM64" : "x64");
        }

        // 6. 기능 지원 체크
        printf("\n=== Feature Support ===\n");
        struct FeatureCheck {
            const char* name;
            const char* description;
        };

        FeatureCheck features[] = {
            {"DirectStorage", "DirectStorage API Support"},
            {"WSL2", "Windows Subsystem for Linux 2"},
            {"DirectX12", "DirectX 12 Support"},
            {"ModernStandby", "Modern Standby Support"}
        };

        for (const auto& feature : features) {
            printf("%s: %s\n", feature.description,
                info.supportsFeature(feature.name) ? "Supported" : "Not Supported");
        }

        // 7. 시스템 요구사항 체크 예시
        printf("\n=== System Requirements Check ===\n");
        struct RequirementCheck {
            std::string check;
            bool result;
        };

        std::vector<RequirementCheck> requirements = {
            {"64-bit System", info.is64Bit()},
            {"Windows 10 or later", info.isWindows10OrGreater()},
            {"Build 19041 or later", info.buildVersion >= 19041}
        };

        bool allRequirementsMet = true;
        for (const auto& req : requirements) {
            printf("%s: %s\n", req.check.c_str(),
                req.result ? "Pass" : "Fail");
            if (!req.result) allRequirementsMet = false;
        }
        printf("All Requirements Met: %s\n",
            allRequirementsMet ? "Yes" : "No");

        // 8. 조건부 코드 실행 예시
        printf("\n=== Conditional Code Examples ===\n");
        if (info.isServer()) {
            // 서버 버전별 특화 코드
            switch (info.version) {
            case WindowsInfo::Version::WindowsServer2022:
                printf("Running Server 2022 specific code\n");
                break;
            case WindowsInfo::Version::WindowsServer2019:
                printf("Running Server 2019 specific code\n");
                break;
            case WindowsInfo::Version::WindowsServer2016:
                printf("Running Server 2016 specific code\n");
                break;
            default:
                printf("Running older server version code\n");
            }
        }
        else {
            // 클라이언트 버전별 특화 코드
            if (info.isWindows11OrGreater()) {
                printf("Running Windows 11 specific code\n");
            }
            else if (info.isWindows10OrGreater()) {
                printf("Running Windows 10 specific code\n");
            }
            else {
                printf("Running legacy Windows version code\n");
            }
        }

    }
    catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
        return 1;
    }

    printf("\nPress any key to exit...");
    system("pause");
    return 0;
}
