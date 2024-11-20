#include "que.h"
#include <iostream>

Queue::Queue() : front(nullptr), rear(nullptr) {}

void Queue::enqueue(uint8_t value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->next = nullptr;

    if (rear == nullptr) {
        front = rear = newNode;  // Queue is empty
    } else {
        rear->next = newNode;    // Add new node at the end
        rear = newNode;          // Update rear
    }
}

uint8_t* Queue::dequeue() {
    if (front == nullptr) {
        return nullptr;  // Return nullptr for an empty queue
    }
    Node* temp = front;
    uint8_t* value = new uint8_t(front->data); // Allocate memory for value
    front = front->next;

    if (front == nullptr) {
        rear = nullptr;  // If the queue becomes empty, update rear
    }
    delete temp;
    return value;
}

Queue::~Queue() {
    while (front != nullptr) {
        dequeue();  // Clean up memory
    }
}
