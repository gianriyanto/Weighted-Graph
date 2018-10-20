#ifndef GRAPH_ALGS
#define GRAPH_ALGS

#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <algorithm>
#include "weighted_graph.cpp"
#include "easy_weighted_graph_algorithms.cpp"

#include <limits>

//Returns true if the graph is connected, false otherwise.
template <typename vertex>
bool is_connected(const weighted_graph<vertex>& g){

    if (g.num_vertices() != 0){
        std::vector<vertex> traversedVertices = depth_first(g, *g.begin());		// traverse the graph from a staring vertex
        return (traversedVertices.size() == g.num_vertices());					// the number of traversed vertices must equal to the total number of vertices to be connected
    }

    return true; //return true if the graph is empty
}

//Returns a vector of weighted graphs, where each weighted graph is a connected
//component of the input graph.
template <typename vertex>
std::vector<weighted_graph<vertex>> connected_components(const weighted_graph<vertex>& g){

    std::vector<weighted_graph<vertex> > connectedComponents;	//vector that contains weighted graph (subgraphs component)
    weighted_graph<vertex> component;		//weighted graph for every subgraph

    int all_vertices = g.num_vertices();
    //boolean to track unvisited vertices
    bool unvisited[all_vertices];

    //mark all vertices as unvisited
    for (vertex v : g)
        unvisited[v] = true;

    for (vertex v : g) {
        if (unvisited[v] == false)	{	//finish when all vertices are visited
            break;
        }
        else { 		//if not all vertices are visited yet proceed
            std::vector<vertex> connected_vertices = breadth_first(g, v);	//traverse the graph starting from every vertex 'v'. all connected vertices of 'v' can be traversed
            weighted_graph<vertex> component;		//weighted graph for every subgraph
            for (vertex u : connected_vertices) {
                component.add_vertex(u);			//add every traversed vertex to the subgraph

                for (auto neighbour_itr = g.cneighbours_begin(u); neighbour_itr != g.cneighbours_end(u); neighbour_itr++) {			//iterate through every neighbouring vertex
                    component.add_edge(u, neighbour_itr->first, neighbour_itr->second);												//add edge of every neigbouring vertex to the subgraph
                    unvisited[u] = false;			//mark vertex as visited //by now 'component' contains individual subgraph of 'g'
                }
            }
            connectedComponents.push_back(component);
        }
    }

    return connectedComponents;
}

//Returns a map of the vertices of the weighted graph g and their distances from
//the given starting vertex v.
template <typename vertex>
std::map<vertex, int> dijkstras(const weighted_graph<vertex>& g, const vertex& v){

    // map of vertex and distance to that vertex
    std::map<vertex,int> dijkstra_shortest_path;

    // set to represent V-S (set of vertices that we still need to process)
    std::unordered_set<vertex> v_minus_s;

    //while the graph is not empty, insert pair (of starting vertex) to the map
    if (g.num_vertices()!=0)
        dijkstra_shortest_path.insert(std::make_pair(v,0)); 	//distance to the starting vertex is 0

    for (vertex u : g){
        v_minus_s.insert(u);		//insert every vertex into set v_minus_s
        if (u != v) {
            int minimum_distance = std::numeric_limits<int>::max();
            dijkstra_shortest_path.insert(std::make_pair(u, minimum_distance));		//insert pair of every other vertex to the map with distance as maximum
        }
    }

    //while there is still vertices in v_minus_s
    while(v_minus_s.size() != 0){
        vertex z;	//intitalise vertex variable to use in second loop later
        int current_minimum_distance = std::numeric_limits<int>::max();
        for (vertex v : v_minus_s) {														//for every vertex 'v' in v_minus_s
            if ( dijkstra_shortest_path.find(v)->second < current_minimum_distance ){ 		//if distance to 'v' is less than current minimum distance
                z = v;																		//set 'v' to 'z'
                current_minimum_distance = dijkstra_shortest_path.find(z)->second;			//set the current minimum distance to the distance to 'z' (formerly 'v')
            }
        }

        for (auto neighbour = g.cneighbours_begin(z); neighbour != g.cneighbours_end(z); ++neighbour) {		//iteratre through neighbour of 'z'
            for (vertex u : v_minus_s) {
                if ((neighbour->first == u) && (neighbour->second + (dijkstra_shortest_path.find(z)->second) < (dijkstra_shortest_path.find(neighbour->first)->second) )){	//find if neighbouring vertex of 'z' still left in V-S	and //find for a shorter distance compared to the current distance (of the neighbour of 'z')
                    (dijkstra_shortest_path.find(neighbour->first)->second) = neighbour->second + (dijkstra_shortest_path.find(z)->second);		//if a shorter distance is found, set it as the new shortest distance
                }
            }
        }

        v_minus_s.erase(z); 	//remove the first vector and return to the loop until no more is left
    }

    return dijkstra_shortest_path;
}


//Returns a vector containing all the articulation points of the
//input weighted graph g.
template <typename vertex>
std::vector<vertex> articulation_points(const weighted_graph<vertex>& g){

    weighted_graph<vertex> duplicate_g;
    std::vector<vertex> articulation_points;

    //for every vertex, remove vertex from the graph
    for (vertex v : g) {
        duplicate_g = g;					//a copy of g to add and remove vertices
        duplicate_g.remove_vertex(v);		//for every vertex, try and remove it to do tests

        //test: if graph remains connected, re-add vertex to the graph
        if (is_connected(duplicate_g)) {
            duplicate_g.add_vertex(v);
        }
            //test: if graph gets disconnected, add vector to articulation_points vector
        else {
            articulation_points.push_back(v);
            duplicate_g.add_vertex(v);
        }
    }
    return articulation_points;
}

#endif