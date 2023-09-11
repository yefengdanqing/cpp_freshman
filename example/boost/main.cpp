#include <iostream>
#include <utility> 


#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "gtest/gtest.h"


#include "graph_test.h"
#include "function_test.h"
#include "my_graph_test.h"

 
using namespace std;
using namespace boost;


int main(int argc, char* argv[])
{
    // build_test_graph();
    // build_test_graph1();
    build_adjacency_list();
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();

    // return 0;
};
