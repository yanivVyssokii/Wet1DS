//
// Created by User on 11/15/2022.
//

#include "AVLTree.h"

template<class T>
AVLTree<T>::AVLTree() {
m_root= nullptr;
}

template<class T>
Node<T> *AVLTree<T>::find(int id) {
    Node<T> *temp = m_root;
    while (temp != nullptr) {
        if (temp->data->m_playerId == id)
            return m_root->data;
        if (temp->data->m_playerId > id)
            temp = temp->leftSon;
        else
            temp = temp->rightSon;
    }
    return nullptr;
}
template<class T>
    Node<T>* AVLTree<T>::insert(Node<T>* root, T* value)
    {

        if (root == NULL) {
            root = new Node<T>;
            root->data = value;
            root->left = NULL;
            root->right = NULL;

            return root;
        }

        if (value->getId() < root->getId()) {
            root->left = insert(root->left, value);
        }

        else if (value->getId() > root->getId()) {
            root->right = insert(root->right, value);
        }
        return root;
    }
