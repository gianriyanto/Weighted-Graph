cmake_minimum_required(VERSION 3.9)
project(WeightedGraph2)

set(CMAKE_CXX_STANDARD 11)

add_executable(WeightedGraph2 main.cpp graph_algorithms.cpp graph_algorithms.h easy_weighted_graph_algorithms.cpp easy_weighted_graph_algorithms.h weighted_graph.cpp weighted_graph.h)