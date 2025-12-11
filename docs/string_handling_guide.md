## C++ 문자/문자열 처리 요약

### 1. 문자 타입과 특징

| 타입 | 크기 | 인코딩 | 리터럴 | string 타입 |
|------|------|---------|---------|------------|
| char | 1바이트 | ASCII/MBCS | "hello" | std::string |
| wchar_t | 2/4바이트 | UTF-16/32 | L"hello" | std::wstring |
| char8_t | 1바이트 | UTF-8 | u8"hello" | std::u8string |
| char16_t | 2바이트 | UTF-16 | u"hello" | std::u16string |
| char32_t | 4바이트 | UTF-32 | U"hello" | std::u32string |

### 2. 플랫폼별 기본 문자 처리

| 플랫폼 | 기본 타입 | 기본 인코딩 | API 문자열 | TCHAR |
|--------|-----------|-------------|------------|--------|
| Windows | wchar_t | UTF-16 | UTF-16 | _UNICODE 시 wchar_t |
| Linux | char | UTF-8 | UTF-8 | 미지원 |
| macOS | char | UTF-8 | UTF-8 | 미지원 |

### 3. 문자열 변환 함수

| 변환 방향 | Windows | Linux/macOS |
|-----------|----------|-------------|
| UTF-8 → UTF-16 | MultiByteToWideChar() | std::wstring_convert |
| UTF-16 → UTF-8 | WideCharToMultiByte() | std::wstring_convert |
| MBCS → UTF-16 | MultiByteToWideChar() | iconv |
| UTF-16 → MBCS | WideCharToMultiByte() | iconv |

### 4. 주요 매크로

| 매크로 | 설명 | 영향 |
|--------|------|------|
| _UNICODE | 유니코드 사용 설정 | TCHAR = wchar_t |
| _MBCS | 멀티바이트 설정 | TCHAR = char |
| TEXT() | 문자열 리터럴 매크로 | 설정에 따라 변환 |

### 5. 권장 사항
- **신규 프로젝트: UTF-8 사용 (char8_t)**
- Windows: wchar_t/UTF-16 고려
- 크로스플랫폼: UTF-8 기반 설계
- 레거시 호환: TCHAR 사용

## 연결 문서
- [[FN-20250114-문자집합과 인코딩 체계]]