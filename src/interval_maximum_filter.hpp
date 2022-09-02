#pragma once

#include "includes/definitions.hpp"
#include "src/datastructures/priority_queue.hpp"
#include "src/datastructures/pairing_head.hpp"
#include "algorithm"
#include "cmath"
#include "random"
#include "iostream"

namespace kv_intervall_maximum_filter {
  // This is an implementation of the I-Max-filter algorithm presented in this paper https://link.springer.com/chapter/10.1007/978-3-540-39658-1_61
  // Implemented by Jens Kienle and Simon Vögele
  class IntervallMaximumFilter {

  public:
    algen::WEdgeList operator()(const algen::WEdgeList& edge_list,
                                const algen::VertexId num_vertices) {

      std::vector<algen::Weight> weights;
      algen::VertexArray renumbering(num_vertices, algen::VERTEXID_UNDEFINED);

      algen::WEdgeList sample_edges = random_subset(edge_list);
      // Push reverse edges. if both e = (a,b,w) and its reverse edge (b,a,w) are already part of the random set, this will create duplicates.
      // In theory these duplicates shouldn't cause any issues
      long edge_list_size = sample_edges.size();
      for (long i = 0; i < edge_list_size; i++) {
        sample_edges.push_back(algen::WEdge(sample_edges[i].head, sample_edges[i].tail, sample_edges[i].weight));
      }

      //prints the random set for testing purposes.
      /*std::cout << "The random sample is: ";
      for (int i = 0; i < sample_edges.size(); i++) {
      std::cout << sample_edges[i] << ", ";
      }
      std::cout << "\n";*/

      algen::WEdgeList sample_mst = jarnik_prim(sample_edges, num_vertices, true, weights, renumbering);

      //prints the mst of the random set for testing purposes.
      /*std::cout << "The mst of the random sample is: ";
      for (int i = 0; i < sample_mst.size(); i++) {
      std::cout << sample_mst[i] << ", ";
      }
      std::cout << "\n";*/

      construct_prefix_suffix_binary_tree(weights, num_vertices);

      // Filtering all edges to remove edges that are the heaviest edge on a cycle
      for (long i = 0; i < edge_list.size(); i++) {
        algen::WEdge cur = edge_list[i];
        // Calculate which layer of the binary tree we must look at
        uint64_t l = msb(renumbering[cur.tail] ^ renumbering[cur.head]);
        // Add current edge if it is not the heaviest edge on a cycle
        if ((cur.weight < prefix_suffix_binary_tree[l][renumbering[cur.tail]])
              || (cur.weight < prefix_suffix_binary_tree[l][renumbering[cur.head]])) {
          sample_mst.push_back(cur);
        }
      }

      algen::WEdgeList mst = jarnik_prim(sample_mst, num_vertices, false, weights , renumbering);

      return mst;
    }

  private:
    // Implementation of the Jarnik-Prim Algorithm
    // If need_filtering_data is set the algorithm will collect the weights of all edges that are added to the MST in
    // mst_edge_weights and for every vertex the renumbering array will contain in which step the vertex was added to
    // the MST
    algen::WEdgeList jarnik_prim(const algen::WEdgeList& edge_list,
                                 const algen::VertexId num_vertices,
                                 const bool need_filtering_data,
                                 algen::WeightArray& mst_edge_weights,
                                 algen::VertexArray& renumbering) {
      construct_adjacency_array(edge_list, num_vertices);
      algen::WEdgeList mst;

      // initialise priority queue
      PriorityQueue q;
      q.push({0, 0});
      for (long i = 1; i < num_vertices; i++) {
        q.push({algen::WEIGHT_MAX, i});
      }
      std::pair<algen::Weight, algen::VertexId> current;
      std::vector<bool> added_to_mst(num_vertices, false);

      long counter = 0; // to count the JP order. 0 being the first vertex added to the mst, num_vertices being the last
      algen::VertexArray parent(num_vertices, algen::VERTEXID_UNDEFINED); // saves the parent from which the vertex is currently reached
      while(!q.empty()) {
        current = q.pop();
        added_to_mst[current.second] = true;
        //add the edge that reaches current to the mst unless current is the root of a tree in the MSF
        if(parent[current.second] != algen::VERTEXID_UNDEFINED) {
          mst.push_back(algen::WEdge(parent[current.second], current.second, current.first));
          mst.push_back(algen::WEdge(current.second, parent[current.second], current.first));
        }
        //document the JP order
        if(need_filtering_data) {
          mst_edge_weights.push_back(current.first);
          renumbering[current.second] = counter;
          counter ++;
        }
        //update parent vertices and edges if needed
        for (long i = adjacency_array_borders[current.second]; i < adjacency_array_borders[current.second + 1]; i++) {
          if(!added_to_mst[adjacency_array[i].head] && adjacency_array[i].weight < q.get_key(adjacency_array[i].head)) {
            q.set_key(adjacency_array[i].head, adjacency_array[i].weight);
            parent[adjacency_array[i].head] = current.second;
          }
        }
      }
      return mst;
    }

    template <typename EdgeType>
    struct TailHeadOrder {
      bool operator()(const EdgeType& lhs, const EdgeType& rhs) const {
        return std::tie(lhs.tail, lhs.head) < std::tie(rhs.tail, rhs.head);
      }
    };

    // Constructs an adjacency array from a given list of edges
    void construct_adjacency_array(const algen::WEdgeList& edge_list,
                                   const algen::VertexId num_vertices) {
      adjacency_array = edge_list;
      adjacency_array_borders= std::vector<algen::VertexId>(num_vertices+1,0);

      std::sort(adjacency_array.begin(), adjacency_array.end(), TailHeadOrder<algen::WEdge>());

      algen::VertexId current_vertex = 0;
      adjacency_array_borders[0] = 0;
      for (long i = 0; i < adjacency_array.size(); i++) {
        if (adjacency_array[i].tail != current_vertex) {
          algen::VertexId next_vertex = adjacency_array[i].tail;
          for (long j = 0; j < adjacency_array[i].tail - current_vertex; j++) {
            adjacency_array_borders[next_vertex-j] = i;
          }
          current_vertex = next_vertex;
        }
      }
      for (long j = 0; j < num_vertices - current_vertex; j++) {
        adjacency_array_borders[num_vertices-j] = adjacency_array.size();
      }
    }

    // Constructs a binary tree of prefix and suffix maxima that can be used to find interval maxima
    void construct_prefix_suffix_binary_tree(std::vector<algen::Weight>& weights,
                                        const algen::VertexId num_vertices) {
      uint64_t size = next_pow2(num_vertices);
      weights.insert(weights.end(), size - weights.size(), WEIGHT_MIN);
      uint64_t layers = log2(size);
      prefix_suffix_binary_tree = std::vector<std::vector<algen::Weight>>(layers);
      prefix_suffix_binary_tree[0] = weights;
      for (long l = 1; l < layers; l++) {
        prefix_suffix_binary_tree[l] = std::vector<algen::Weight>(size);
        for (long i = 0; i < size; i++) {
          long b = i/(pow(2,l));
          if (b % 2 == 1) {
            prefix_suffix_binary_tree[l][i] = find_maximum(weights ,pow(2,l) * b, i);
          } else {
            prefix_suffix_binary_tree[l][i] = find_maximum(weights , i,pow(2,l)*(b+1)-1);
          }
        }
      }
    }

    // Returns the maximum weight of a given array of weights
    algen::Weight find_maximum(const algen::WeightArray& weights, uint64_t l, uint64_t r) {
      int size = weights.size();

      long max = 0;
      for (long j = l; j <= r; j++) {
        if (weights[j] > max) {
          max = weights[j];
        }
      }
      return max;
    }

    // Returns tne minimum y where y>=x and y is a power of two
    uint64_t next_pow2(uint64_t x) {
      return x == 1 ? 1 : 1<<(64-__builtin_clzl(x-1));
    }

    //very naive and intuitive function to get a random subset of a given list of edges
    algen::WEdgeList random_subset(const algen::WEdgeList& source) {
      auto gen = std::mt19937(std::random_device()());
      auto distr = std::uniform_int_distribution<algen::VertexId>(0, 99);  //adjust the range to modify the divider of the random chance e.g. (0, 6) results in a x/7 chance
      algen::WEdgeList output;
      for (long i = 0; i < source.size(); i++) {
        if(distr(gen) <= 8) {                                     //adjust upper bound to modify the denominator of the random chance e.g. <= 3 results in a 4/y chance
          output.push_back(source[i]);
        }
      }
      return output;
    }

    // Returns the position of the most significant non zero bit
    uint64_t msb(uint64_t x) {
      return (63-__builtin_clzl(x));
    }


    static constexpr algen::Weight WEIGHT_MIN = std::numeric_limits<algen::Weight>::min();
    algen::WEdgeList adjacency_array;
    std::vector<algen::VertexId> adjacency_array_borders;
    std::vector<std::vector<algen::Weight>> prefix_suffix_binary_tree;
  };

  //algorithm just using our implementation of jarnik prim used as a contender to compare against
  class CustomJarnikPrim {

  public:
    algen::WEdgeList operator()(const algen::WEdgeList& edge_list,
                                const algen::VertexId num_vertices) {

      std::vector<algen::Weight> weights;
      algen::VertexArray renumbering(num_vertices, algen::VERTEXID_UNDEFINED);

      algen::WEdgeList mst = jarnik_prim(edge_list, num_vertices, false, weights , renumbering);

      return mst;
    }

  private:
    // Implementation of the Jarnik-Prim Algorithm
    // If need_filtering_data is set the algorithm will collect the weights of all edges that are added to the MST in
    // mst_edge_weights and for every vertex the renumbering array will contain in which step the vertex was added to
    // the MST
    algen::WEdgeList jarnik_prim(const algen::WEdgeList& edge_list,
                                 const algen::VertexId num_vertices,
                                 const bool need_filtering_data,
                                 algen::WeightArray& mst_edge_weights,
                                 algen::VertexArray& renumbering) {
      construct_adjacency_array(edge_list, num_vertices);
      algen::WEdgeList mst;

      // initialise priority queue
      PriorityQueue q;
      q.push({0, 0});
      for (long i = 1; i < num_vertices; i++) {
        q.push({algen::WEIGHT_MAX, i});
      }
      std::pair<algen::Weight, algen::VertexId> current;
      std::vector<bool> added_to_mst(num_vertices, false);

      long counter = 0; // to count the JP order. 0 being the first vertex added to the mst, num_vertices being the last
      algen::VertexArray parent(num_vertices, algen::VERTEXID_UNDEFINED); // saves the parent from which the vertex is currently reached
      while(!q.empty()) {
        current = q.pop();
        added_to_mst[current.second] = true;
        //add the edge that reaches current to the mst unless current is the root of a tree in the MSF
        if(parent[current.second] != algen::VERTEXID_UNDEFINED) {
          mst.push_back(algen::WEdge(parent[current.second], current.second, current.first));
          mst.push_back(algen::WEdge(current.second, parent[current.second], current.first));
        }
        //document the JP order
        if(need_filtering_data) {
          mst_edge_weights.push_back(current.first);
          renumbering[current.second] = counter;
          counter ++;
        }
        //update parent vertices and edges if needed
        for (long i = adjacency_array_borders[current.second]; i < adjacency_array_borders[current.second + 1]; i++) {
          if(!added_to_mst[adjacency_array[i].head] && adjacency_array[i].weight < q.get_key(adjacency_array[i].head)) {
            q.set_key(adjacency_array[i].head, adjacency_array[i].weight);
            parent[adjacency_array[i].head] = current.second;
          }
        }
      }
      return mst;
    }

    template <typename EdgeType>
    struct TailHeadOrder {
      bool operator()(const EdgeType& lhs, const EdgeType& rhs) const {
        return std::tie(lhs.tail, lhs.head) < std::tie(rhs.tail, rhs.head);
      }
    };

    // Constructs an adjacency array from a given list of edges
    void construct_adjacency_array(const algen::WEdgeList& edge_list,
                                   const algen::VertexId num_vertices) {
      adjacency_array = edge_list;
      adjacency_array_borders= std::vector<algen::VertexId>(num_vertices+1,0);

      std::sort(adjacency_array.begin(), adjacency_array.end(), TailHeadOrder<algen::WEdge>());

      algen::VertexId current_vertex = 0;
      adjacency_array_borders[0] = 0;
      for (long i = 0; i < adjacency_array.size(); i++) {
        if (adjacency_array[i].tail != current_vertex) {
          algen::VertexId next_vertex = adjacency_array[i].tail;
          for (long j = 0; j < adjacency_array[i].tail - current_vertex; j++) {
            adjacency_array_borders[next_vertex-j] = i;
          }
          current_vertex = next_vertex;
        }
      }
      for (long j = 0; j < num_vertices - current_vertex; j++) {
        adjacency_array_borders[num_vertices-j] = adjacency_array.size();
      }
    }

    algen::WEdgeList adjacency_array;
    std::vector<algen::VertexId> adjacency_array_borders;
  };
}
