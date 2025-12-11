# [JsonCpp](https://github.com/open-source-parsers/jsoncpp) 세팅

## vcpkg 로 JsonCpp 설치
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install jsoncpp
.\vcpkg integrate install
```

## 환경 변수 세팅
- 변수 이름: VCPKG_ROOT
- 변수 값 : C:\vcpkg (vcpkg clone 경로)
- 
## VS프로젝트 설정
- 추가 포함 디렉터리: %VCPKG_ROOT%/installed/x64-windows/include
- 추가 라이브러리 디렉터리 : %VCPKG_ROOT%/installed/x64-windows/lib