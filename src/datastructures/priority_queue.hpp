//this is the implementation of priority queue using a binary heap
class PriorityQueue {
private:
  std::vector<std::pair<algen::Weight, algen::VertexId>> heap;

  int parent(int current) {
    return (current - 1) / 2;
  }

  int leftChild(int current) {
    return ((2 * current) + 1);
  }

  int rightChild(int current) {
    return ((2 * current) + 2);
  }

  void swap(int i, int j) {
    std::pair<algen::Weight, algen::VertexId> tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
  }

  void shiftUp(int target) {
    //as long as the target isn't already root and it's parent has a smaller key, swap them
    while (target > 0 && heap[parent(target)].first > heap[target].first) {
      swap(parent(target), target);
      target = parent(target);
    }
  }

  void shiftDown(int target) {
    int minIndex = target;

    //check if the left child of the target (if it exists) has a smaller key
    if(leftChild(target) <= heap.size() && heap[leftChild(target)].first < heap[minIndex].first) {
      minIndex = leftChild(target);
    }
    //check if right child of the target (if it exists) has the smallest key
    if(rightChild(target) <= heap.size() && heap[rightChild(target)].first < heap[minIndex].first) {
      minIndex = rightChild(target);
    }

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
  //returns algen::VERTEXID_UNDEFINED if there is no such element in the queue
  algen::Weight get_key(algen::VertexId val) {
    //TODO
  }

  //changes the priority of a given element with value 'val' to the priority 'new_key'
  void set_key(algen::VertexId val, algen::Weight new_key) {
    //TODO
  }
};