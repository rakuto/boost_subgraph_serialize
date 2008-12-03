/**
 *  Serialization of boost::subgraph
 *
 *  Created by Rakuto Furutani <xri://rakuto/> (rakuto+nospam@gmail.com)
 *
 * output:
 *	original:
 *	0 <--> 1 2 
 *	1 <--> 0 4 
 *	2 <--> 0 
 *	3 <--> 
 *	4 <--> 5 5 1 
 *	5 <--> 4 4 
 *	children num: 1
 *
 *	from file:
 *	0 <--> 1 2 
 *	1 <--> 0 4 
 *	2 <--> 0 
 *	3 <--> 
 *	4 <--> 5 5 1 
 *	5 <--> 4 4 
 *	children num: 1
 */
#include <fstream>
#include <string>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graph_utility.hpp>
// arhivers
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "subgraph_serialize.h"

using namespace boost;

typedef property<vertex_name_t, char> VertexProperty;
typedef property<edge_index_t, uint32_t, 
	property<edge_weight_t, uint32_t> > EdgeProperty;
typedef subgraph<adjacency_list<listS, vecS, undirectedS, VertexProperty, EdgeProperty> > Graph;

static const char* DAT_FILE_NAME = "subgraph.dat";

int main()
{
	enum {A, B, C, D, E, F, N};
	const char* node_names = "ABCDEF";
	
	// create root graph
	Graph root(N);
	property_map<Graph, vertex_name_t>::type vertex_names = get(vertex_name_t(), root);
	for(uint32_t i = 0; i < N; ++i) vertex_names[i] = node_names[i];	
	add_edge(A, B, EdgeProperty(1), root);
	add_edge(A, C, EdgeProperty(2), root);
	add_edge(E, F, EdgeProperty(3), root);
	
	// create sub graph
	enum {B1, E1, F1}; // refering to vertices in sub_g1
	Graph& sub_g1 = root.create_subgraph();
	add_vertex(B, sub_g1);
	add_vertex(E, sub_g1);
	add_vertex(F, sub_g1);	
	add_edge(E1, F1, sub_g1);
	add_edge(B1, E1, sub_g1);
	
	std::cout << "original:" << std::endl;
	print_graph(root);
	std::cout << "children num: " << root.num_children() << std::endl;
	
	// serialize and save graph
	std::ofstream ofs(DAT_FILE_NAME, std::ios::out | std::ios::binary);
	if(!ofs.is_open()) {
		std::cerr << "Can't open " << DAT_FILE_NAME << " file." << std::endl;
		return EXIT_FAILURE;
	}
	archive::binary_oarchive oa(ofs);
	oa << root;
	ofs.close();
	
	// Try to restore saved graph
	Graph g;
	std::ifstream ifs(DAT_FILE_NAME, std::ios::in | std::ios::binary);
	if(!ifs.is_open()) {
		std::cerr << "Can't open " << DAT_FILE_NAME << " file." << std::endl;
		return EXIT_FAILURE;		
	}
	archive::binary_iarchive ia(ifs);
	ia >> g;
	
	std::cout << "\nfrom file:" << std::endl;
	print_graph(g);	
	std::cout << "children num: " << g.num_children() << std::endl;
	
	return EXIT_SUCCESS;
}
