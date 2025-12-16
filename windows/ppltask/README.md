# windows/ppltask

PPL Tasks(`concurrency::task`, `ppltasks.h`) 연습용 코드 모음.

## 파일

- `ppltask_practice.cpp`: `then`, `when_all`, cancellation, `task_completion_event` 기본 패턴 연습

## 빌드 (VSCode)

- 기본 빌드 태스크: `C/C++: cl.exe build active file`
- 결과물: `windows/ppltask/build/` 아래에 `*.exe` 생성

## TODO

- `SchedulerPolicy`/커스텀 `Scheduler`를 `task_options`로 연결해서 특정 task를 해당 스케줄러에서 실행하는 예제 추가
- `when_any` + 타임아웃/경쟁(race) 패턴 예제 추가
- `then` 체이닝에서 예외 전파/복구(`task<T>`를 받아 `get()` 처리) 예제 추가
- cancellation 흐름(토큰 전달, 취소 전파, cleanup 패턴) 예제 보강
- 간단한 출력 예시/실행 로그 추가

## 메모: 요즘에도 쓰나?

- PPL Tasks는 신규 프로젝트의 “표준 선택지”는 아니지만, Windows/MSVC 기반 레거시 코드(데스크톱 앱/엔터프라이즈/UWP·WinRT 일부)에서는 여전히 만날 수 있음
- 신규/크로스플랫폼에서는 보통 **C++20 코루틴(async/await 스타일)** + (프로젝트에 맞는) 실행기/라이브러리(예: Boost.Asio 코루틴, cppcoro, oneTBB 등)를 더 선호
- 학습 가치는 “Windows 코드 유지보수 대비” 또는 “비동기/continuation·cancellation·when_all/any 개념 체득”에 유효함; 완전 신규라면 코루틴 기반 학습이 투자 효율이 더 큼
