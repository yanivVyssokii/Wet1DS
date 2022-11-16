//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_AVLTREE_H
#define WET1DS_AVLTREE_H
#include "Node.h"
#include "wet1util.h"
template <class T>
class AVLTree {
private:
    Node<T>* m_root;
public:
    AVLTree();

    Node<T>* find(int id);

   void insert(Node<T>& root,T& value);

    void remove(Node<T>& node);

    void merge(Node<T>& node1,Node<T>& node2 );

    Node<T>* insert(Node<T>* root, T* value);

    int height(Node<T>* temp);

    int diff(Node<T>* temp);

    Node<T>* rr_rotation(Node<T>* parent);

    Node<T>* ll_rotation(Node<T>* parent);

    Node<T>* lr_rotation(Node<T>* parent);

    Node<T>* rl_rotation(Node<T>* parent);

    Node<T>* balance(Node<T>* temp);
}


#endif //WET1DS_AVLTREE_H
