//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_NODE_H
#define WET1DS_NODE_H

template <class T>
struct Node {
    int info;
    T* data;
    struct Node* father;
    struct Node* leftSon;
    struct Node* rightSon;
};


#endif //WET1DS_NODE_H
