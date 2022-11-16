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

    int calheight(Node<T> *p);

    int bf( Node<T> *n);

    Node<T> * llrotation(Node<T> *n);

    Node<T> * rrrotation(Node<T> *n);

    Node<T> * rlrotation(Node<T> *n);

    Node<T> * lrrotation(Node<T> *n);

    Node<T>* insert(Node<T> *r,int data);

    Node<T> * deleteNode(Node<T> *p,int data);

    Node<T> inpre(Node<T> p);

    Node<T> insuc(Node<T> p);
};


#endif //WET1DS_AVLTREE_H
