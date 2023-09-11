#include <iostream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>

struct EdgeProperties
{
    EdgeProperties(const std::string& n) : name(n) {}
    std::string name;
};

struct VertexProperties
{
    std::size_t index;
    boost::default_color_type color;
};



void build_test_graph();
void build_test_graph1();
void build_adjacency_list();


