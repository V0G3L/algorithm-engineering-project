#include "src/interval_maximum_filter.hpp"
#include "includes/binary_includes.hpp"
#include "iostream"

void mst_test(algen::WEdgeList graph, int num_vertex) {
  //printing the used data
  std::cout << "The example edge list is: ";
  for (int i = 0; i < graph.size(); i++) {
    std::cout << graph[i] << ", ";
  }
  std::cout << "\n";

  //calculating and printing the correct mst
  algen::WEdgeList correct_mst = fast_kruskal(graph, num_vertex);

  std::cout << "The correct MST is: ";
  for (int i = 0; i < correct_mst.size(); i++) {
    std::cout << correct_mst[i] << ", ";
  }
  std::cout << "\n";

  //calculating and printing the mst produced by our algorithm
  //std::vector<algen::Weight> weights;
  //algen::VertexArray renumbering(graph.size(), algen::VERTEXID_UNDEFINED);
  kv_intervall_maximum_filter::IntervallMaximumFilter filter;
  algen::WEdgeList maximum_filter_mst = filter.operator()(graph, num_vertex);

  std::cout << "The calculated MST is: ";
  for (int i = 0; i < maximum_filter_mst.size(); i++) {
    std::cout << maximum_filter_mst[i] << ", ";
  }
  std::cout << "\n\n";
}

int main() {
  //first example
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

  mst_test(simple_graph, 6);

  //second example
  algen::WEdgeList bigger_is_better;
  bigger_is_better.push_back(algen::WEdge(0, 1, 10));
  bigger_is_better.push_back(algen::WEdge(0, 2, 1));
  bigger_is_better.push_back(algen::WEdge(0, 3, 3));
  bigger_is_better.push_back(algen::WEdge(1, 2, 15));
  bigger_is_better.push_back(algen::WEdge(1, 4, 3));
  bigger_is_better.push_back(algen::WEdge(1, 6, 9));
  bigger_is_better.push_back(algen::WEdge(2, 3, 8));
  bigger_is_better.push_back(algen::WEdge(2, 4, 1));
  bigger_is_better.push_back(algen::WEdge(2, 5, 3));
  bigger_is_better.push_back(algen::WEdge(3, 5, 4));
  bigger_is_better.push_back(algen::WEdge(3, 6, 2));
  bigger_is_better.push_back(algen::WEdge(4, 5, 6));
  bigger_is_better.push_back(algen::WEdge(4, 6, 1));
  bigger_is_better.push_back(algen::WEdge(5, 6, 7));
  bigger_is_better.push_back(algen::WEdge(6, 7, 20));
  bigger_is_better.push_back(algen::WEdge(7, 8, 4));
  bigger_is_better.push_back(algen::WEdge(7, 9, 6));
  bigger_is_better.push_back(algen::WEdge(7, 10, 1));
  bigger_is_better.push_back(algen::WEdge(8, 10, 2));
  bigger_is_better.push_back(algen::WEdge(9, 10, 5));
  long size = bigger_is_better.size();
  for (long i = 0; i < size; i++) {
    bigger_is_better.push_back(algen::WEdge(bigger_is_better[i].head, bigger_is_better[i].tail, bigger_is_better[i].weight));
  }
  mst_test(bigger_is_better, 11);
}