#include <iostream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include "gtest/gtest.h"

//邻接表或者邻接矩阵
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> mygraph;

int graph_test() {
    mygraph g;
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 5, g);
    add_edge(5, 1, g);
    add_edge(1, 4, g);
    add_edge(2, 3, g);
    add_edge(3, 4, g);
    mygraph::vertex_iterator begin, end;
    boost::tie(begin, end) = vertices(g);

    mygraph::adjacency_iterator start, finish;

    while(begin != end) {
        std::cout << *begin << " yes ";
        boost::tie(start, finish) = adjacent_vertices(*begin, g);
        while(start != finish) {
            std::cout << *start << " ";
            ++start;
        }
        std::cout << std::endl;

        begin++;
    }
}
TEST(MYGRAPHTEST, GRAPHTEST) {
    graph_test();
}