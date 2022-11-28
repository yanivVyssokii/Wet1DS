//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_NODE_H
#define WET1DS_NODE_H

template <class T>
struct Node {
    int info;
    int height;
    T* data;
    struct Node* father;
    struct Node* left;
    struct Node* right;
    Node(int info, T* data, struct Node* father, struct Node* leftSon, struct Node* rightSon);
    Node();
};

template<class T>
Node<T>::Node(int info, T *data, struct Node *father, struct Node *left, struct Node *right):
        info(info),height(0), data(data), father(father),left(left),right(right){

}

template<class T>
Node<T>::Node():
        info(0),height(0), data(nullptr), father(nullptr),left(nullptr),right(nullptr){

}


#endif //WET1DS_NODE_H
