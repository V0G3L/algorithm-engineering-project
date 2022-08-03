#pragma once

#include "includes/definitions.hpp"
#include "src/datastructures/priority_queue.hpp"

namespace kv_intervall_maximum_filter {
  // This is an implementation of the I-Max-filter algorithm presented in this paper https://link.springer.com/chapter/10.1007/978-3-540-39658-1_61
  // Implemented by Jens Kienle and Simon Vögele
  class IntervallMaximumFilter {

  public:
    algen::WEdgeList operator()(const algen::WEdgeList& edge_list,
                                const algen::VertexId num_vertices) {

      algen::WeightArray weights;
      algen::VertexArray renumbering(num_vertices, algen::VERTEXID_UNDEFINED);

      //TODO sampling here
      algen::WEdgeList sample_edges;

      algen::WEdgeList sample_mst = jarnik_prim(sample_edges, num_vertices, true, weights, renumbering);

      construct_prefix_suffix_binary_tree(weights, num_vertices);

      //TODO filtering here
      algen::WEdgeList filtered_edge_list;

      algen::WEdgeList mst = jarnik_prim(filtered_edge_list, num_vertices, false, algen::WeightArray() , algen::VertexArray());
      //TODO push reverse edges
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
      PriorityQueue q; //TODO implement a priority queue in src/datastructures/priority_queue.hpp
      q.push(0, 0);
      for (long i = 1; i < num_vertices; i++) {
        q.push(i, algen::VERTEXID_UNDEFINED);
      }
      algen::VertexId current;

      long counter = 0; // to count the JP order. 0 being the first vertex added to the mst, num_vertices being the last
      algen::VertexArray parent(num_vertices, algen::VERTEXID_UNDEFINED); // saves the parent from which the vertex is currently reached
      while(!q.empty()) {
          current = q.top();
          //add the vertex that reaches current to the mst unless current is the root of a tree in the MSF
          if(parent[current] != algen::VERTEXID_UNDEFINED) {
            mst.push_back(algen::WEdge(parent[current], current, q.get_key(current)));
          }
          q.pop();
          //document the JP order
          if(need_filtering_data) {
            renumbering[current] = counter;
            counter ++;
          }
          //update parent vertices and edges if needed
          for (long i = adjacency_array_borders[current]; i < adjacency_array_borders[current + 1]; i++) {
            if(edge_list[i].weight < q.get_key(edge_list[i].head)) {
              q.set_key(edge_list[i].head, edge_list[i].weight);
              parent[edge_list[i].head] = current;
            }
          }
      }
      return mst;
    }

    // Constructs an adjanceny array from a given list of edges
    void construct_adjacency_array(const algen::WEdgeList& edge_list,
                                   const algen::VertexId num_vertices) {
      adjacency_array = edge_list;
      adjacency_array_borders= std::vector<algen::VertexId>(num_vertices+1,0);

      std::sort(adjacency_array.begin(), adjacency_array.end(), algen::TailHeadOrder<algen::WEdge>());

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
    void construct_prefix_suffix_binary_tree(const algen::WeightArray& weights,
                                        const algen::VertexId num_vertices) {
      //TODO construct the binary tree for the interval maximum search here
    }

    algen::WEdgeList adjacency_array;
    std::vector<algen::VertexId> adjacency_array_borders;
    std::vector<std::vector<algen::Weight>> prefix_suffix_binary_tree;
  };
}
