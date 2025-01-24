#include <filesystem>
#include <thread>
#include <string>
#include <vector>
#include <iostream>

#include "brpc/server.h"
#include "brpc/reloadable_flags.h"
#include "gflags/gflags.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <chrono>


#define NUM_ITERATIONS 1000000  // 测试次数

#include "cl_test.pb.h"


DEFINE_uint64(concurrency, 1, "Concurrent logging thread num");
DEFINE_uint64(dummy_port, 8080, "port");

DEFINE_int32(mode, 1, "");
BRPC_VALIDATE_GFLAG(mode, brpc::PositiveInteger/*always true*/);
DEFINE_uint64(qps, 10000, "");
DEFINE_uint64(batch, 1, "");

// ChangeType enum definition
enum class SChangeType {
    FLUCTUATE = 1,  // Fluctuating between upper and lower bound
    SMOOTH = 2      // Smoothly rising from the lower bound to the upper bound
};

// Stage struct definition
struct Stage {
    int32_t lower_bound;     // Lower bound for the stage
    int32_t upper_bound;     // Upper bound for the stage
    int32_t duration_sec;    // Duration of the stage in seconds
    SChangeType type;         // Type of the change (FLUCTUATE or SMOOTH)
};

// TestCase struct definition
struct STestCase {
    std::string case_name;                     // Name of the test case
    std::string max_concurrency;               // Max concurrency for the test case
    std::vector<Stage> qps_stage_list;         // List of QPS stages
    std::vector<Stage> latency_stage_list;     // List of latency stages
};

// TestCaseSet struct definition
struct STestCaseSet {
    std::vector<STestCase> test_case;  // List of test cases
};

void set_struct_value(size_t x) {
    STestCaseSet test_case_set;
    test_case_set.test_case.reserve(1);

    // Create a TestCase
    STestCase test_case;
    test_case.case_name = "Test Case 1";
    test_case.max_concurrency = "100";

    test_case.qps_stage_list.reserve(2);
    test_case.latency_stage_list.reserve(1);
    // Add QPS stages
    Stage qps_stage_1;
    qps_stage_1.lower_bound = 100;
    qps_stage_1.upper_bound = 200;
    qps_stage_1.duration_sec = 30;
    qps_stage_1.type = SChangeType::FLUCTUATE;
    test_case.qps_stage_list.emplace_back(std::move(qps_stage_1));

    Stage qps_stage_2;
    qps_stage_2.lower_bound = 200;
    qps_stage_2.upper_bound = 300;
    qps_stage_2.duration_sec = 40;
    qps_stage_2.type = SChangeType::SMOOTH;
    test_case.qps_stage_list.emplace_back(std::move(qps_stage_2));

    // Add Latency stages
    Stage latency_stage_1;
    latency_stage_1.lower_bound = 10;
    latency_stage_1.upper_bound = 20;
    latency_stage_1.duration_sec = 15;
    latency_stage_1.type = SChangeType::FLUCTUATE;
    test_case.latency_stage_list.emplace_back(std::move(latency_stage_1));

    // Add the test case to the test case set
    test_case_set.test_case.emplace_back(std::move(test_case));

    // // Print the TestCaseSet content
    // std::cout << "TestCaseSet contains " << test_case_set.test_case.size() << " test case(s)." << std::endl;
    // for (const auto& tc : test_case_set.test_case) {
    //     std::cout << "Case Name: " << tc.case_name << std::endl;
    //     std::cout << "Max Concurrency: " << tc.max_concurrency << std::endl;
    //     std::cout << "QPS Stage Count: " << tc.qps_stage_list.size() << std::endl;
    //     std::cout << "Latency Stage Count: " << tc.latency_stage_list.size() << std::endl;
    // }

}

void set_pb_value(size_t x) {
    // 初始化一个 TestCaseSet 实例
    test::TestCaseSet test_case_set;

    // 创建一个 TestCase 实例
    test::TestCase* test_case = test_case_set.add_test_case();
    test_case->set_case_name("Test Case 1");
    test_case->set_max_concurrency("100");

    // 创建 qps_stage_list
    test::Stage* qps_stage_1 = test_case->add_qps_stage_list();
    qps_stage_1->set_lower_bound(100);
    qps_stage_1->set_upper_bound(200);
    qps_stage_1->set_duration_sec(30);
    qps_stage_1->set_type(test::ChangeType::FLUCTUATE);  // 假设 ChangeType 是一个枚举类型

    test::Stage* qps_stage_2 = test_case->add_qps_stage_list();
    qps_stage_2->set_lower_bound(200);
    qps_stage_2->set_upper_bound(300);
    qps_stage_2->set_duration_sec(40);
    qps_stage_2->set_type(test::ChangeType::SMOOTH);

    // 创建 latency_stage_list
    test::Stage* latency_stage_1 = test_case->add_latency_stage_list();
    latency_stage_1->set_lower_bound(10);
    latency_stage_1->set_upper_bound(20);
    latency_stage_1->set_duration_sec(15);
    latency_stage_1->set_type(test::ChangeType::FLUCTUATE);
    // 输出 TestCaseSet 中的所有信息
    // std::cout << test_case_set.DebugString() << std::endl;
}

void get_pb_value(const test::TestCaseSet& test_case_set) {
    // 遍历所有 TestCase
    for (const auto& test_case : test_case_set.test_case()) {
        const auto& case1 =  test_case.case_name();
        const auto& concurrentcy = test_case.max_concurrency();
        
        // 获取 QPS 阶段列表
        std::cout << "QPS Stages:" << std::endl;
        for (const auto& qps_stage : test_case.qps_stage_list()) {
            auto lower = qps_stage.lower_bound();
            auto upper = qps_stage.upper_bound();
            auto second = qps_stage.duration_sec();
            auto type = qps_stage.type();
        }
        
        for (const auto& latency_stage : test_case.latency_stage_list()) {
            auto lower = latency_stage.lower_bound();
            auto upper = latency_stage.upper_bound();
            auto second = latency_stage.duration_sec();
            auto type = latency_stage.type();
        }
    }
}

void get_struct_value(const STestCaseSet& test_case_set) {
    // 遍历所有 TestCase
    for (const auto& test_case : test_case_set.test_case) {
        // 提取 TestCase 的值
        std::string case_name = test_case.case_name;
        std::string max_concurrency = test_case.max_concurrency;
        size_t qps_stage_count = test_case.qps_stage_list.size();
        size_t latency_stage_count = test_case.latency_stage_list.size();
        
        // // 打印 TestCase 的值
        // std::cout << "Case Name: " << case_name << std::endl;
        // std::cout << "Max Concurrency: " << max_concurrency << std::endl;
        // std::cout << "QPS Stage Count: " << qps_stage_count << std::endl;
        // std::cout << "Latency Stage Count: " << latency_stage_count << std::endl;

        // 提取并打印 QPS 阶段列表的值
        for (size_t i = 0; i < qps_stage_count; ++i) {
            const Stage& qps_stage = test_case.qps_stage_list[i];
            int qps_lower_bound = qps_stage.lower_bound;
            int qps_upper_bound = qps_stage.upper_bound;
            int qps_duration_sec = qps_stage.duration_sec;
            auto qps_type = qps_stage.type;
            
            // std::cout << "QPS Stage " << i + 1 << " Lower Bound: " << qps_lower_bound
            //           << ", Upper Bound: " << qps_upper_bound
            //           << ", Duration: " << qps_duration_sec
            //           << " seconds, Type: " << qps_type << std::endl;
        }

        // 提取并打印 Latency 阶段列表的值
        for (size_t i = 0; i < latency_stage_count; ++i) {
            const Stage& latency_stage = test_case.latency_stage_list[i];
            int latency_lower_bound = latency_stage.lower_bound;
            int latency_upper_bound = latency_stage.upper_bound;
            int latency_duration_sec = latency_stage.duration_sec;
            auto type = latency_stage.type;

            // std::cout << "Latency Stage " << i + 1 << " Lower Bound: " << latency_lower_bound
            //           << ", Upper Bound: " << latency_upper_bound
            //           << ", Duration: " << latency_duration_sec
            //           << " seconds, Type: " << latency_type << std::endl;
        }
    }
}

void run_loop() {
  std::string mode_str = "pb";
  
  if (FLAGS_mode == 2 || FLAGS_mode == 4) {
    mode_str = "struct";
  }
  ::bvar::LatencyRecorder latency {"test-" + mode_str};
  

  int64_t expect_us =
      1000.0 * 1000 / FLAGS_qps * FLAGS_batch * FLAGS_concurrency;

  void (*run_once)(size_t);
  if (FLAGS_mode == 1) {
    run_once = set_pb_value;
  } else if (FLAGS_mode == 2) {
    run_once = set_struct_value;
  }
  ::std::vector<::std::thread> threads;
  for (size_t i = 0; i < FLAGS_concurrency; ++i) {
    threads.emplace_back([&] {
      size_t round = 0;
      // if (FLAGS_mode == 1 || FLAGS_mode == 3) {

      // } else if (FLAGS_mode == 2 || FLAGS_mode == 4) {
      //   auto case_object = set_struct_value(round);
      // }
      STestCaseSet s_test_case_set;
      test::TestCaseSet test_case_set;
      
      while (!::brpc::IsAskedToQuit()) {
        auto round_begin = ::butil::cpuwide_time_ns();
        for (size_t j = 0; j < FLAGS_batch; ++j) {
          auto begin = ::butil::cpuwide_time_ns();
          if (FLAGS_mode == 3) {
            get_pb_value(test_case_set);
          } else if (FLAGS_mode == 4) {
            get_struct_value(s_test_case_set);
          } else {
            run_once(round);
          }
          auto end = ::butil::cpuwide_time_ns();
          latency << (end - begin);
        }
        auto round_end = ::butil::cpuwide_time_ns();
        auto use_us = (round_end - round_begin) / 1000;
        if (use_us < expect_us) {
          ::usleep(expect_us - use_us);
        }
        round++;
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
}

int main(int argc, char* argv[]) {
  ::gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::SetCommandLineOption("mode", "struct");
  ::brpc::StartDummyServerAt(FLAGS_dummy_port);

  // ::std::filesystem::remove_all("log");
  if (FLAGS_mode == 1) {
    
    run_loop();
  } else if (FLAGS_mode == 2) {
   
    run_loop();
  } else if (FLAGS_mode == 3) {
    run_loop();
  } else if (FLAGS_mode == 4) {
    run_loop();
  }

  return 0;
}



// void test_protobuf_assignment() {
//     // 测量赋值时间
//     auto start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < NUM_ITERATIONS; ++i) {
//         set_pb_value(i);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;

//     std::cout << "Protobuf assignment took: " << duration.count() << " seconds" << std::endl;
// }

// void test_struct_assignment() {


//     // 测量赋值时间
//     auto start = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < NUM_ITERATIONS; ++i) {
//         set_struct_value(i);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duration = end - start;

//     std::cout << "Struct assignment took: " << duration.count() << " seconds" << std::endl;
// }

// int main() {
//     // 测试 protobuf 和 struct 赋值的性能
//     std::cout << "Starting protobuf assignment test..." << std::endl;
//     test_protobuf_assignment();

//     std::cout << "Starting struct assignment test..." << std::endl;
//     test_struct_assignment();

//     return 0;
// }
