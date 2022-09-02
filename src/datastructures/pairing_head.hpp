#include <iostream>
#include "includes/definitions.hpp"
#include "unordered_map"
#include "random"
#include "assert.h"

// An Implementation of a pairing heap
namespace kv_intervall_maximum_filter {

  class Element{
  public:
    algen::Weight key;
    algen::VertexId value;
    algen::VertexId left;
    algen::VertexId right;
    algen::VertexId child;
    algen::VertexId parent;

    Element(algen::Weight key, algen::VertexId value) {
      this->key = key;
      this->value = value;
      left = -1;
      right = -1;
      child = -1;
      parent = -1;
    }
  };

  class PairingHeap {

  public:

    PairingHeap(long size) {
      head = NULL;
      heap.reserve(size);
    }

    //returns true if the queue is empty, false otherwise
    bool empty() {
      return (heap.size() == 0);
    }

    //pushes a new element with value 'val' and priority 'key' to the priority queue
    void push(std::pair<algen::Weight, algen::VertexId> input) {
      //TODO test insert buffer
      long index = heap.size();
      index_map.insert({input.second, index});
      heap.emplace_back(input.first, input.second);
      head = meld(head, &heap[index]);
    }

    //removes the current front from the queue and returns it
    std::pair<algen::Weight, algen::VertexId> pop() {
      assert(!empty());
      std::pair<algen::Weight, algen::VertexId> output = {head->key,head->value};
      head = merge_siblings(&heap[index_map[head->child]]);
      long index = index_map[output.second];
      heap[index] = heap[heap.size()-1];
      index_map[heap[heap.size()-1].value] = index;
      heap.pop_back();
      index_map.erase(output.second);
      return output;
    }

    //returns the current priority of a given element with value 'val'
    //returns algen::VERTEXID_UNDEFINED if there is no such element in the queue
    algen::Weight get_key(algen::VertexId val) {
      assert(index_map.find(val) != index_map.end());
      return heap[index_map[val]].key;
    }

    //changes the priority of a given element with value 'val' to the priority 'new_key'
    void set_key(algen::VertexId val, algen::Weight new_key) {
      assert(index_map.find(val) != index_map.end());
      Element* elem = &heap[index_map[val]];
      assert(new_key < elem->key);
      elem->key = new_key;
      if (elem->value != head->value) {
        if (elem->left == -1) {
          (heap[index_map[elem->parent]]).child = elem->right;
        } else {
          (heap[index_map[elem->left]]).right = elem->right;
        }
        if (elem->right != -1) {
          (heap[index_map[elem->right]]).left = elem->left;
        }
        elem->parent = -1;
        elem->right = -1;
        elem->left = -1;
        //TODO test not cutting the whole subtree
        head = meld(head, elem);
      }
    }

  private:

    //Combines the heaps rooted at a and b to one heap
    Element* meld(Element* a, Element* b) {
      if (a == NULL) {
        return b;
      } else if (b == NULL) {
        return a;
      } if (a->key < b->key) {
        b->right = a->child;
        if (a->child != -1) {
          (heap[index_map[a->child]]).left = b->value;
        }
        a->child = b->value;
        b->parent = a->value;
        return a;
      } else {
        a->right = b->child;
        if (b->child != -1) {
          (heap[index_map[b->child]]).left = a->value;
        }
        b->child = a->value;
        a->parent = b->value;
        return b;
      }
    }

    //Merges all siblings by melding them from left to right in pairs, then merging the pairs from right to left
    Element* merge_siblings(Element* first_sibling) {
      if (first_sibling == NULL) {
        return first_sibling;
      } else if(first_sibling->right == -1) {
        first_sibling->parent = -1;
        return first_sibling;
      } else {
        Element* second_sibling = &heap[index_map[first_sibling->right]];
        Element *third_siblimg = NULL;
        if (second_sibling->right != -1) {
          third_siblimg = &heap[index_map[second_sibling->right]];
        }
        first_sibling->parent = -1;
        first_sibling->right = -1;
        second_sibling->left = -1;
        second_sibling->parent = -1;
        second_sibling->right = -1;
        if (third_siblimg != NULL) {
          third_siblimg->left = -1;
        }
        return meld(meld(first_sibling, second_sibling), merge_siblings(third_siblimg));
      }
    }

    Element* head;                                               //value of the root of the heap
    std::vector<Element> heap;                                          //stores the elements of the heap
    std::unordered_map<algen::VertexId, long> index_map;                //maps a value to the corresponding element
  };
}