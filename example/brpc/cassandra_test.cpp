#include "brpc/cassandra.h"
#include "brpc/server.h"
#include "brpc/channel.h"
#include "brpc/authenticator.h"
#include "brpc/policy/cassandra_authenticator.h"

#include <memory>

DEFINE_int32(dmp_timeout_ms, 13,  "dmp cass request timeout(ms)");
DEFINE_string(dmp_addrs, "list://172.31.16.245:9042",  "dmp address list");

bool GetDmpDataFromCassandra(const butil::StringPiece& query) {
    brpc::CassandraRequest creq;
    creq.ExecuteCql3Query(query);

    brpc::Controller     cntl;
    brpc::CassandraResponse resp;

    butil::Timer tm(butil::Timer::STARTED);
    
    

    brpc::policy::CassandraAuthenticator* m_dmp_auth_ = new brpc::policy::CassandraAuthenticator("cassandra", "cassandra", "rs_test_space");

    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_CQL;
    options.timeout_ms = FLAGS_dmp_timeout_ms;
    options.auth = static_cast<brpc::Authenticator*>(m_dmp_auth_);

    brpc::Channel m_dmp_channel_;
    if (m_dmp_channel_.Init(FLAGS_dmp_addrs.c_str(), "rr", &options) != 0) {
            return false;
        }


    m_dmp_channel_.CallMethod(NULL, &cntl, &creq, &resp, NULL);

    tm.stop();
    

    if (cntl.Failed()) {
        //TODO metrics
        LOG(ERROR) << "Fail to access dmp cass, " << cntl.ErrorText();
        return false;
    }

    LOG(INFO) << "test dmp sql `" << query << "`, row count is " << resp.GetRowsCount() << ", remote = " << cntl.remote_side();
    if (const int row_count = resp.GetRowsCount(); row_count > 0) {
        for (int r = 0; r < row_count; ++r) {
            std::string pkg_data;
            if (const int row_status = resp.GetColumnValueOfRow("top_tag", r, &pkg_data); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = pkg, status = " << row_status;
            } else {
                LOG(INFO) << "pkg_data:" << pkg_data;
            }

            std::string adtype_pkg_data;
            if (const int row_status = resp.GetColumnValueOfRow("dev_install_pkg", r, &adtype_pkg_data); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = adtype_pkg, status = " << row_status;
            } else {
                LOG(INFO) << "dev_install_pkg:" << adtype_pkg_data;
            }

            std::string install_pkg_info_by_tw;
            if (const int row_status = resp.GetColumnValueOfRow("install_pkg_info_by_tw", r, &install_pkg_info_by_tw); row_status != 0) {
                LOG(WARNING) << "row=" << r << ", col = adtype_pkg, status = " << row_status;
            } else {
                LOG(INFO) << "install_pkg_info_by_tw:" << install_pkg_info_by_tw;
            }


        }
    }

    return true;
}

int main() {
    std::string query = "SELECT top_tag, dev_install_pkg, install_pkg_info_by_tw FROM user_action.realtime_user_action WHERE oneid = 'd20f1d4b3e15b3db71c64ba78ab6e1fe'";
    bool result = GetDmpDataFromCassandra(query);
}






