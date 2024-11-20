#ifndef QUE_H
#define QUE_H

#include <stdint.h>  // For uint8_t

struct Node {
    uint8_t data;   // The value of the node
    Node* next;     // Pointer to the next node in the list
};

class Queue {
public:
    Node* front;    // Front of the queue
    Node* rear;     // Rear of the queue
    Queue();
    uint8_t* dequeue();  // Change return type to pointer
    void enqueue(uint8_t value);
    ~Queue();
};

#endif // QUE_H
