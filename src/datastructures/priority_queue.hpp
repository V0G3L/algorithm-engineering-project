class PriorityQueue {

public:

    PriorityQueue() {
        //TODO
    }

    //returns true if the queue is empty, false otherwise
    bool empty() {
        //TODO
    }

    //pushes a new element with value 'val' and priority 'key' to the priority queue
    void push(algen::Weight key, algen::VertexId val) {
        //TODO
    }

    //returns the current front of the queue
    algen::VertexId top() {
        //TODO
    }

    //removes the current front from the queue
    void pop() {
        //TODO
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

private:

};