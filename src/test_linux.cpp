#if defined(__linux__)

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <vector>
#include "test.h"

extern bool app_running;


static void Help(int argc, char **argv)
{
    std::cerr << "Usage: " << argv[0] << " thread_cnt" << " sleep_ns" << std::endl;
    exit(0);
}

void TestThread(int sleep_ns, std::vector<std::list<int64_t>> &results)
{
    results.resize(2);
    auto &&sleep_for_results = results[0];
    auto &&select_results = results[1];
    while (app_running)
    {
        // chrono::sleep_for
        {
            auto start_time = std::chrono::steady_clock::now();

            std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_ns));

            auto after_time = std::chrono::steady_clock::now();
            sleep_for_results.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(after_time - start_time).count());
        }

        // select
        {
            auto start_time = std::chrono::steady_clock::now();

            timeval timeout_tv = { 0, static_cast<long>(sleep_ns / 1000) };
            select(0, nullptr, nullptr, nullptr, &timeout_tv);

            auto after_time = std::chrono::steady_clock::now();
            select_results.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(after_time - start_time).count());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Test(int argc, char **argv)
{
    if (argc < 3)
    {
        Help(argc, argv);
    }

    int thread_cnt = atoi(argv[1]);
    int sleep_ns = atoi(argv[2]);
    std::vector<std::thread> threads(thread_cnt);
    std::vector<std::vector<std::list<int64_t>>> thread_results(thread_cnt);
    for (int i = 0; i < thread_cnt; ++i)
    {
        threads[i] = std::thread(TestThread, sleep_ns, std::ref(thread_results[i]));
    }

    for (int i = 0; i < thread_cnt; ++i)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }

    // Print result
    std::string output_name = "output_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".log";
    std::ofstream fout(output_name);

    for (int i = 0; i < thread_cnt; ++i)
    {
        auto &&thread_result = thread_results[i];
        fout << "thread " << i << std::endl;

        {
            auto &&sleep_for_results = thread_result[0];
            fout << "sleep_for results:";

            for (auto item : sleep_for_results)
            {
                fout << ", " << item;
            }
            fout << std::endl;
        }

        {
            auto &&select_results = thread_result[0];
            fout << "select results:";

            for (auto item : select_results)
            {
                fout << ", " << item;
            }
            fout << std::endl;
        }
    }
}

#endif
