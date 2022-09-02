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

  //third example
  algen::WEdgeList non_planar;
  //K5
  non_planar.push_back(algen::WEdge(0, 1, 2));
  non_planar.push_back(algen::WEdge(0, 2, 1));
  non_planar.push_back(algen::WEdge(0, 3, 10));
  non_planar.push_back(algen::WEdge(0, 4, 10));
  non_planar.push_back(algen::WEdge(1, 0, 10));
  non_planar.push_back(algen::WEdge(1, 2, 10));
  non_planar.push_back(algen::WEdge(1, 3, 3));
  non_planar.push_back(algen::WEdge(1, 4, 10));
  non_planar.push_back(algen::WEdge(2, 0, 10));
  non_planar.push_back(algen::WEdge(2, 1, 10));
  non_planar.push_back(algen::WEdge(2, 3, 4));
  non_planar.push_back(algen::WEdge(2, 4, 10));
  non_planar.push_back(algen::WEdge(3, 0, 10));
  non_planar.push_back(algen::WEdge(3, 1, 10));
  non_planar.push_back(algen::WEdge(3, 2, 10));
  non_planar.push_back(algen::WEdge(3, 4, 5));
  non_planar.push_back(algen::WEdge(4, 0, 10));
  non_planar.push_back(algen::WEdge(4, 1, 10));
  non_planar.push_back(algen::WEdge(4, 2, 10));
  non_planar.push_back(algen::WEdge(4, 3, 10));
  //connection
  non_planar.push_back(algen::WEdge(4, 5, 1000));
  //k3,3
  non_planar.push_back(algen::WEdge(5, 8, 1));
  non_planar.push_back(algen::WEdge(5, 9, 10));
  non_planar.push_back(algen::WEdge(5, 10, 10));
  non_planar.push_back(algen::WEdge(6, 8, 10));
  non_planar.push_back(algen::WEdge(6, 9, 10));
  non_planar.push_back(algen::WEdge(6, 10, 10));
  non_planar.push_back(algen::WEdge(7, 8, 10));
  non_planar.push_back(algen::WEdge(7, 9, 1));
  non_planar.push_back(algen::WEdge(7, 10, 1));
  non_planar.push_back(algen::WEdge(8, 5, 10));
  non_planar.push_back(algen::WEdge(8, 6, 10));
  non_planar.push_back(algen::WEdge(8, 7, 1));
  non_planar.push_back(algen::WEdge(9, 5, 10));
  non_planar.push_back(algen::WEdge(9, 6, 1));
  non_planar.push_back(algen::WEdge(9, 7, 10));
  non_planar.push_back(algen::WEdge(10, 5, 10));
  non_planar.push_back(algen::WEdge(10, 6, 10));
  non_planar.push_back(algen::WEdge(10, 7, 10));
  size = non_planar.size();
  for (long i = 0; i < size; i++) {
    non_planar.push_back(algen::WEdge(non_planar[i].head, non_planar[i].tail, non_planar[i].weight));
  }
  mst_test(non_planar, 11);


  // Testing the pairing heap with random inputs
  auto gen = std::mt19937(std::random_device()());
  auto distr = std::uniform_int_distribution<algen::VertexId>(0, 255);  //adjust the range to change the size of the random sample
  const long sample_size = 100000;
  while (true) {
    kv_intervall_maximum_filter::PairingHeap q(sample_size);
    std::vector<algen::Weight> test(sample_size, algen::WEIGHT_UNDEFINED);
    std::vector<algen::Weight> test_unsorted(sample_size, algen::WEIGHT_UNDEFINED);
    for (algen::VertexId i = 0; i < sample_size; i++) {
      q.push({test[i], i});
    }
    std::cout << "{";
    for (algen::VertexId i = 0; i < sample_size; i++) {
      test[i] = distr(gen);
      test_unsorted[i] = test[i];
      std::cout << "," << test[i];
    }
    std::cout << "}\n";
    //test = {91,37,162,sample_size4,195,165,32,36,185,26,131,236,137,127,223};
    for (algen::VertexId i = 0; i < sample_size; i++) {
      q.set_key(i, test[i]);
    }

    auto gen2 = std::mt19937(std::random_device()());
    auto distr2 = std::uniform_int_distribution<algen::VertexId>(0, sample_size-1);  //adjust the range to change the size of the random sample
    for (int i = 0; i < 100000; i++) {
      long x = distr2(gen2);
      long w = distr(gen);
      if (w < test[x]) {
        test[x] = w;
        test_unsorted[x] = w;
        q.set_key(x, w);
      }
    }

    std::vector<std::pair<algen::Weight,algen::VertexId>> test2(0);
    while (!q.empty()) {
      auto x = q.pop();
      test2.push_back(x);
    }
    std::sort(test.begin(), test.end());
    std::cout << "{";
    for (algen::VertexId i = 0; i < sample_size; i++) {
      std::cout << "," << test[i];
    }
    std::cout << "}\n";
    std::cout << "{";
    for (algen::VertexId i = 0; i < sample_size; i++) {
      std::cout << "," << test2[i].first;
    }
    std::cout << "}\n";
    for (algen::VertexId i = 0; i < sample_size; i++) {
      if (test[i] != test2[i].first && test2[i].first == test[test2[i].second]) {
        std::cout << "This failed\n";
        return 0;
      }
    }
  }

}