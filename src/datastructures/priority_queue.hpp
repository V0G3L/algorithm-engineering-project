#include <unordered_map>

//this is the implementation of priority queue using a binary heap
class PriorityQueue {
private:
  std::vector<std::pair<algen::Weight, algen::VertexId>> heap;  //the binary heap
  std::unordered_map<algen::VertexId, long> index_map;          //maps a value to it's index in the 'heap' vector

  long parent(long current) {
    return (current - 1) / 2;
  }

  long leftChild(long current) {
    return ((2 * current) + 1);
  }

  long rightChild(long current) {
    return ((2 * current) + 2);
  }

  void swap(long i, long j) {
    std::pair<algen::Weight, algen::VertexId> tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
    index_map[heap[i].second] = i;
    index_map[heap[j].second] = j;
  }

  void shiftUp(long target) {
    //as long as the target isn't already root and it's parent has a smaller key, swap them
    while (target > 0 && heap[parent(target)].first > heap[target].first) {
      swap(parent(target), target);
      target = parent(target);
    }
  }

  void shiftDown(long target) {
    long minIndex = target;

    //check if the left child of the target (if it exists) has a smaller key
    if(leftChild(target) <= heap.size() && heap[leftChild(target)].first < heap[minIndex].first) {
      minIndex = leftChild(target);
    }
    //check if right child of the target (if it exists) has the smallest key
    if(rightChild(target) <= heap.size() && heap[rightChild(target)].first < heap[minIndex].first) {
      minIndex = rightChild(target);
    }

    //swap target with the smaller child
    if(minIndex != target) {
      swap(minIndex, target);
      shiftDown(minIndex);
    }
  }

public:

  PriorityQueue() {
    heap = std::vector<std::pair<algen::Weight, algen::VertexId>>(0);
  }

  //returns true if the queue is empty, false otherwise
  bool empty() {
    return (heap.size() == 0);
  }

  //pushes a new element with value 'val' and priority 'key' to the priority queue
  void push(std::pair<algen::Weight, algen::VertexId> new_element) {
    heap.push_back(new_element);
    index_map.insert({new_element.second , heap.size() - 1});
    shiftUp(heap.size() - 1);
  }

  //returns the current front of the queue
  std::pair<algen::Weight, algen::VertexId> top() {
    return heap[0];
  }

  //removes the current front from the queue
  void pop() {
    swap(0, heap.size() - 1);
    heap.pop_back();
    shiftDown(0);
  }

  //returns the current priority of a given element with value 'val'
  //asserts that the given value is in the queue
  algen::Weight get_key(algen::VertexId val) {
    assert(index_map.find(val) != index_map.end());
    return heap[index_map[val]].first;
  }

  //changes the priority of a given element with value 'val' to the priority 'new_key'
  //asserts that the given value is in the queue
  void set_key(algen::VertexId val, algen::Weight new_key) {
    assert(index_map.find(val) != index_map.end());
    long pos = index_map[val]; //insert position of value here
    algen::Weight old_key = heap[pos].first;
    heap[pos].first = new_key;
    if (new_key < old_key) {
      shiftUp(pos);
    } else if (new_key > old_key) {
      shiftDown(pos);
    }
  }
};