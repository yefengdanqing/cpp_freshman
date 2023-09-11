#include "graph_test.h"


using namespace std;
using namespace boost;


typedef boost::adjacency_list<listS, vecS, undirectedS> mygraph;


void build_test_graph() {
    boost::adjacency_list<> g;
 
    boost::adjacency_list<>::vertex_descriptor v1 = boost::add_vertex(g);
    boost::adjacency_list<>::vertex_descriptor v2 = boost::add_vertex(g);
    boost::adjacency_list<>::vertex_descriptor v3 = boost::add_vertex(g);
    boost::adjacency_list<>::vertex_descriptor v4 = boost::add_vertex(g);
 
    std::cout << v1 << ", " << v2 << ", " << v3 << ", " << v4 << '\n';
}
void build_test_graph1() {
    mygraph g; 
    add_edge (0, 1, g); 
    add_edge (0, 3, g);
    add_edge (1, 2, g);
    add_edge (2, 3, g);
    mygraph::vertex_iterator vertexIt, vertexEnd;
    mygraph::adjacency_iterator neighbourIt, neighbourEnd;
    tie(vertexIt, vertexEnd) = vertices(g);
    for (; vertexIt != vertexEnd; ++vertexIt) 
    { 
    cout << *vertexIt << " is connected with "; 
    tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, g); 
    for (; neighbourIt != neighbourEnd; ++neighbourIt) 
    cout << *neighbourIt << " "; 
    cout << "\n"; 
    }
}
void build_adjacency_list() {
    typedef adjacency_list< vecS, listS, undirectedS, VertexProperties,
        EdgeProperties >
        Graph;

    const int V = 5;
    Graph g(V);

    property_map< Graph, std::size_t VertexProperties::* >::type id
        = get(&VertexProperties::index, g);
    property_map< Graph, std::string EdgeProperties::* >::type name
        = get(&EdgeProperties::name, g);

    boost::graph_traits< Graph >::vertex_iterator vi, viend;
    int vnum = 0;

    for (boost::tie(vi, viend) = vertices(g); vi != viend; ++vi)
        id[*vi] = vnum++;

    add_edge(vertex(0, g), vertex(1, g), EdgeProperties("joe"), g);
    add_edge(vertex(1, g), vertex(2, g), EdgeProperties("curly"), g);
    add_edge(vertex(1, g), vertex(3, g), EdgeProperties("dick"), g);
    add_edge(vertex(2, g), vertex(4, g), EdgeProperties("tom"), g);
    add_edge(vertex(3, g), vertex(4, g), EdgeProperties("harry"), g);

    graph_traits< Graph >::vertex_iterator i, end;
    graph_traits< Graph >::out_edge_iterator ei, edge_end;
    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
        cout << id[*i] << " ";
        for (boost::tie(ei, edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
            cout << " --" << name[*ei] << "--> " << id[target(*ei, g)] << "  ";
        cout << endl;
    }
    print_edges(g, id);

    cout << endl << "removing edge (1,3): " << endl;
    remove_edge(vertex(1, g), vertex(3, g), g);

    ei = out_edges(vertex(1, g), g).first;
    cout << "removing edge (" << id[source(*ei, g)] << "," << id[target(*ei, g)]
         << ")" << endl;
    remove_edge(ei, g);

    for (boost::tie(i, end) = vertices(g); i != end; ++i)
    {
        cout << id[*i] << " ";
        for (boost::tie(ei, edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
            cout << " --" << name[*ei] << "--> " << id[target(*ei, g)] << "  ";
        cout << endl;
    }

    print_edges(g, id);
}