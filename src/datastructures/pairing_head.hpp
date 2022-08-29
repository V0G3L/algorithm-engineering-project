#include "includes/definitions.hpp"
#include "includes/utils.hpp"
#include "unordered_map"
#include "random"
#include "assert.h"

// An Implementation of a pairing heap
namespace kv_intervall_maximum_filter {

  class Element{
  public:
    algen::Weight key;
    algen::VertexId value;
    Element* left;
    Element* right;
    Element* child;
    Element* parent;

    Element(algen::Weight key, algen::VertexId value) {
      this->key = key;
      this->value = value;
      left = NULL;
      right = NULL;
      child = NULL;
      parent = NULL;
    }
  };

  class PairingHeap {

  public:

    PairingHeap() {
      size = 0;
      head = NULL;
    }

    //returns true if the queue is empty, false otherwise
    bool empty() {
      return (size == 0);
    }

    //pushes a new element with value 'val' and priority 'key' to the priority queue
    void push(algen::Weight key, algen::VertexId val) {
      Element* new_elem = new Element(key,val);
      head = meld(head, new_elem);
    }

    //returns the current front of the queue
    algen::VertexId top() {
      return head->value;
    }

    //removes the current front from the queue
    void pop() {
      head = (merge_siblings(head->child));
    }

    //returns the current priority of a given element with value 'val'
    //returns algen::VERTEXID_UNDEFINED if there is no such element in the queue
    algen::Weight get_key(algen::VertexId val) {
      assert(index_map.find(val) != index_map.end());
      return index_map.at(val).key;
    }

    //changes the priority of a given element with value 'val' to the priority 'new_key'
    void set_key(algen::VertexId val, algen::Weight new_key) {
      assert(index_map.find(val) != index_map.end());
      Element* elem = &index_map.at(val);
      assert(new_key < elem->key);
      elem->key = new_key;
      if (elem->value != head->value) {
        Element* left_sibling = elem->left;
        Element* right_sibling = elem->right;
        if (left_sibling == NULL) {
          elem->parent->child = elem->right;
        }
        if (left_sibling != NULL) {
          left_sibling->right = right_sibling;
        }
        if (right_sibling != NULL) {
          right_sibling->left = left_sibling;
        }
        elem->parent = NULL;
        elem->right = NULL;
        elem->left = NULL;
        //TODO test not cutting the whole subtree
        meld(head, elem);
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
        if (a->child != NULL) {
          a->child->left = b;
        }
        a->child = b;
      } else {
        a->right = b->child;
        if (b->child != NULL) {
          b->child->left = a;
        }
        b->child = a;
      }
    }

    //Merges all siblings by melding them from left to right in pairs, then merging the pairs from right to left
    Element* merge_siblings(Element* first_sibling) {
      first_sibling->parent = NULL;
      if (first_sibling == NULL) {
        return first_sibling;
      } else if(first_sibling->right == NULL) {
        return first_sibling;
      } else {
        Element* second_sibling = first_sibling->right;
        Element* third_siblimg = second_sibling->right;
        first_sibling->right = NULL;
        second_sibling->left = NULL;
        second_sibling->parent = NULL;
        second_sibling->right = NULL;
        third_siblimg->left = NULL;
        return meld(meld(first_sibling, second_sibling), merge_siblings(third_siblimg));
      }
    }

    uint64_t size;                                                    //size of heap
    Element* head;                                                     //root of the heap
    std::unordered_map<algen::VertexId, Element> index_map;           //maps a value to the corresponding element
  };
}