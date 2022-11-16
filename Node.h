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
    struct Node* leftSon;
    struct Node* rightSon;
    Node(int info, T* data, struct Node* father, struct Node* leftSon, struct Node* rightSon);
};

template<class T>
Node<T>::Node(int info, T *data, struct Node *father, struct Node *leftSon, struct Node *rightSon):
        info(info), data(data), father(father),leftSon(leftSon),rightSon(rightSon){

}


#endif //WET1DS_NODE_H
