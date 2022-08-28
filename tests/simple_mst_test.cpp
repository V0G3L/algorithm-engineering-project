#include "src/interval_maximum_filter.hpp"
#include "iostream"

int main() {
  algen::WEdgeList simple_graph;
  simple_graph.push_back(algen::WEdge(0, 1, 5));
  simple_graph.push_back(algen::WEdge(1, 0, 5));

  simple_graph.push_back(algen::WEdge(0, 2, 2));
  simple_graph.push_back(algen::WEdge(2, 0, 2));

  simple_graph.push_back(algen::WEdge(1, 2, 8));
  simple_graph.push_back(algen::WEdge(2, 1, 8));

  simple_graph.push_back(algen::WEdge(1, 3, 4));
  simple_graph.push_back(algen::WEdge(3, 1, 4));

  simple_graph.push_back(algen::WEdge(1, 4, 2));
  simple_graph.push_back(algen::WEdge(4, 1, 2));

  simple_graph.push_back(algen::WEdge(2, 4, 7));
  simple_graph.push_back(algen::WEdge(4, 2, 7));

  simple_graph.push_back(algen::WEdge(3, 4, 6));
  simple_graph.push_back(algen::WEdge(4, 3, 6));

  simple_graph.push_back(algen::WEdge(3, 5, 3));
  simple_graph.push_back(algen::WEdge(5, 3, 3));

  simple_graph.push_back(algen::WEdge(4, 5, 1));
  simple_graph.push_back(algen::WEdge(5, 4, 1));

  std::cout << "The example edge list is: ";
  for (int i = 0; i < simple_graph.size(); i++) {
    std::cout << simple_graph[i] << ", ";
  }
  std::cout << "\n";

  std::cout << "The correct MST is: (0, 1, 5), (1, 0, 5), (0, 2, 5), (2, 0, 5), (1, 4, 2), (4, 1, 2), (4, 5, 1), (5, 4, 1), (5, 3, 3), (3, 5, 3)\n";

  std::vector<algen::Weight> weights;
  algen::VertexArray renumbering(6, algen::VERTEXID_UNDEFINED);
  kv_intervall_maximum_filter::IntervallMaximumFilter filter;
  algen::WEdgeList mst = filter.operator()(simple_graph, 6);

  std::cout << "The calculated MST is: ";
  for (int i = 0; i < mst.size(); i++) {
    std::cout << mst[i] << ", ";
  }
  std::cout << "\n";
}