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
    int m_size;
    Node<T>* m_root;
public:
    bool (*m_comparator)(T& t1, T& t2);

    AVLTree(bool (*comparator)(T& t1, T& t2));

    int getSize();

    bool isBigger(T& t1, T& t2);

    Node<T>* find(const T& target);

    int calheight(Node<T> *p);

    int bf( Node<T> *n);

    Node<T> * llrotation(Node<T> *n);

    Node<T> * rrrotation(Node<T> *n);

    Node<T> * rlrotation(Node<T> *n);

    Node<T> * lrrotation(Node<T> *n);

    Node<T>* insert(Node<T>* prevNode,Node<T> *r,T* data);

    Node<T> * deleteNode(Node<T> *p,T* data);

    Node<T>* inpre(Node<T>* p);

    Node<T>* insuc(Node<T>* p);

    void inOrder(Node<T> *p, Node<T>* arr, int* index);

    void fillInOrder(Node<T> *p, Node<T>* arr, int* index, int sizeOfArr);

    void printInOrder(Node<T> *p);

    Node<T>* getRoot();

    Node<T>* createEmptyTree(Node<T>* r,int height);
};

template<class T>
AVLTree<T> *merge(AVLTree<T> *t1, AVLTree<T> *t2);

#endif //WET1DS_AVLTREE_H
