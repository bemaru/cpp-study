// ppltask_practice.cpp
// Build (VS Developer Command Prompt):
//   cl /EHsc /std:c++17 ppltask_practice.cpp
//
// This file is for practicing PPL Tasks (concurrency::task / ppltasks.h).
#include <ppltasks.h>

#include <windows.h>

#include <chrono>
#include <exception>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace concurrency;

static int slow_fibonacci(int n)
{
    if (n < 0) {
        throw std::invalid_argument("n must be >= 0");
    }
    if (n < 2) {
        return n;
    }

    int a = 0;
    int b = 1;
    for (int i = 2; i <= n; ++i) {
        int next = a + b;
        a = b;
        b = next;
        ::Sleep(10);
    }
    return b;
}

static task<void> example_then()
{
    std::cout << "[then] computing fib(30)..." << std::endl;
    return create_task([] { return slow_fibonacci(30); }).then([](int value) {
        std::cout << "[then] result = " << value << std::endl;
    });
}

static task<void> example_when_all()
{
    std::cout << "[when_all] computing fib(26..30) in parallel..." << std::endl;
    std::vector<task<int>> tasks;
    for (int n = 26; n <= 30; ++n) {
        tasks.push_back(create_task([n] { return slow_fibonacci(n); }));
    }

    return when_all(tasks.begin(), tasks.end()).then([](std::vector<int> results) {
        const int sum = std::accumulate(results.begin(), results.end(), 0);
        std::cout << "[when_all] sum = " << sum << std::endl;
    });
}

static task<void> example_cancellation()
{
    std::cout << "[cancel] start a cancellable task..." << std::endl;

    cancellation_token_source cts;
    cancellation_token token = cts.get_token();

    auto work = create_task([token] {
        for (int i = 0; i < 50; ++i) {
            if (token.is_canceled()) {
                cancel_current_task();
            }
            ::Sleep(20);
        }
        return 123;
    }, token);

    ::Sleep(200);
    cts.cancel();

    return work.then([](task<int> previous) {
        try {
            std::cout << "[cancel] result = " << previous.get() << std::endl;
        }
        catch (const task_canceled&) {
            std::cout << "[cancel] canceled" << std::endl;
        }
    });
}

static task<void> example_task_completion_event()
{
    std::cout << "[tce] turning callback-style completion into task..." << std::endl;

    task_completion_event<int> tce;
    std::thread([tce]() mutable {
        ::Sleep(150);
        tce.set(777);
    }).detach();

    return create_task(tce).then([](int value) {
        std::cout << "[tce] value = " << value << std::endl;
    });
}

int main()
{
    try {
        example_then().wait();
        example_when_all().wait();
        example_cancellation().wait();
        example_task_completion_event().wait();
    }
    catch (const std::exception& e) {
        std::cerr << "fatal: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
