#include <iostream>
#include <gflags/gflags.h>

DEFINE_string(host, "localhost", "Server host address");
DEFINE_int32(port, 8080, "Server port");

int main(int argc, char **argv)
{
        gflags::ParseCommandLineFlags(&argc, &argv, true);
        std::cout << "Got '" << FLAGS_host << ":" << FLAGS_port << "'." << std::endl;
        return 0;
}