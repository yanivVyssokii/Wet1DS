//
// Created by User on 11/15/2022.
//

#include "AVLTree.h"

template<class T>
AVLTree<T>::AVLTree() {
m_head= nullptr;
}

template<class T>
Node<T> *AVLTree<T>::find(int id) {
    Node<T>* temp=m_head;
    while (temp!=nullptr)
    {
        if (temp->data->m_playerId == id)
            return m_head->data;
        if (temp->data->m_playerId > id)
            temp=temp->leftSon;
        else
            temp=temp->rightSon;
    }
    return nullptr;

}
