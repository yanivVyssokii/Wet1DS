//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_AVLTREE_H
#define WET1DS_AVLTREE_H

template <class T>
class AVLTree {
private:
    Node<T> head;
public:

    Node<T>* find(int id);

    void add(Node<T>& node);

    void remove(Node<T>& node);

    void merge(Node<T>& node1,Node<T>& node2 );
};


#endif //WET1DS_AVLTREE_H
