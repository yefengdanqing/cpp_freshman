#include "brpc/cassandra.h"
#include "brpc/server.h"
#include "brpc/channel.h"
#include "brpc/authenticator.h"
#include "brpc/policy/cassandra_authenticator.h"

#include <memory>

DEFINE_int32(dmp_timeout_ms, 13,  "dmp cass request timeout(ms)");
DEFINE_string(dmp_addrs, "list://127.0.0.1",  "dmp address list");

bool GetDmpDataFromCassandra(const butil::StringPiece& query, brpc::Channel& m_dmp_channel_) {
    brpc::CassandraRequest creq;
    creq.ExecuteCql3Query(query);

    brpc::Controller     cntl;
    brpc::CassandraResponse resp;

    butil::Timer tm(butil::Timer::STARTED);

    m_dmp_channel_.CallMethod(NULL, &cntl, &creq, &resp, NULL);

    tm.stop();
    

    if (cntl.Failed()) {
        //TODO metrics
        LOG(ERROR) << "Fail to access dmp cass, " << cntl.ErrorText();
        return false;
    }

    LOG(INFO) << "test dmp sql `" << query  << std::endl;
    // << "`, row count is " << resp.GetRowsCount() << ", remote = " << cntl.remote_side();
    if (const int row_count = resp.GetRowsCount(); row_count > 0) {
        for (int r = 0; r < row_count; ++r) {
            int emp_id;
            if (const int row_status = resp.GetColumnValueOfRow<int>("emp_id", r, &emp_id); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = emp_id, status = " << row_status;
            } else {
                // LOG(INFO) << "emp_id:" << emp_id;
            }

            std::string emp_name;
            if (const int row_status = resp.GetColumnValueOfRow("emp_name", r, &emp_name); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = emp_name, status = " << row_status;
            } else {
                // LOG(INFO) << "emp_name:" << emp_name;
            }

            std::string emp_city;
            if (const int row_status = resp.GetColumnValueOfRow("emp_city", r, &emp_city); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = emp_city, status = " << row_status;
            } else {
                // LOG(INFO) << "emp_city:" << emp_city;
            }

            std::cout << "length:" << row_count
                                << ", emp_name:" << emp_name
                                << ", id:" << emp_id
                                << ", emp_city:" << emp_city
                                << std::endl;

            // double emp_phone;
            // if (const int row_status = resp.GetColumnValueOfRow<double>("emp_phone", r, &emp_phone); row_status != 0) {
            //     LOG(WARNING) << "row=" << r << ", col = emp_phone, status = " << row_status;
            // } else {
            //     LOG(INFO) << "emp_phone:" << emp_phone;
            // }
            // int emp_sal;
            // if (const int row_status = resp.GetColumnValueOfRow<int>("emp_sal", r, &emp_sal); row_status != 0) {
            //     LOG(WARNING) << "row=" << r << ", col = emp_phone, status = " << row_status;
            // } else {
            //     LOG(INFO) << "emp_sal:" << emp_sal;
            // }
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    brpc::Channel m_dmp_channel_;

     brpc::policy::CassandraAuthenticator* m_dmp_auth_ = new brpc::policy::CassandraAuthenticator("cassandra", "cassandra", "rs_test_space");

    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_CQL;
    options.timeout_ms = FLAGS_dmp_timeout_ms;
    options.auth = static_cast<brpc::Authenticator*>(m_dmp_auth_);

    if (m_dmp_channel_.Init(FLAGS_dmp_addrs.c_str(), "rr", &options) != 0) {
            return false;
    }
    std::string query = "SELECT emp_id,emp_name,emp_city,emp_phone,emp_sal FROM rs_test_space.emp WHERE emp_id = ";
    auto startTime = std::chrono::high_resolution_clock::now();
    int total_req_count = 1000;
    if (argc > 1) {
        total_req_count = std::stoi(argv[1]);
    }

    for(int i = 0; i < total_req_count; i++) {
        int j = (i % 11);
        std::string query_tmp = query;
        query_tmp.append(std::to_string(j)).append(";");

        std::cout << query_tmp << std::endl;

        bool result = GetDmpDataFromCassandra(query_tmp, m_dmp_channel_);
    }
    if (m_dmp_auth_ != nullptr) delete m_dmp_auth_;
    // 结束计时并计算耗时
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    // 输出耗时结果
    std::cout << "brpc total_req_count:" << total_req_count << ",Execution time: " << duration.count() << " microseconds" << std::endl;
    
}
