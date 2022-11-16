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

template<class T>
int AVLTree<T>::height(Node<T>* temp)
{
    int h = 0;
    if (temp != NULL) {
        int l_height = height(temp->left);
        int r_height = height(temp->right);
        int max_height = std::max(l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

template<class T>
int AVLTree<T>::diff(Node<T>* temp)
{
    int l_height = height(temp->left);
    int r_height = height(temp->right);
    int b_factor = l_height - r_height;

    return b_factor;
}
template<class T>
Node<T>* AVLTree<T>::rr_rotation(Node<T>* parent)
{
    Node<T>* temp;
    temp = parent->right;
    parent->right = temp->left;

    temp->left = parent;

    return temp;
}

template<class T>
Node<T>* AVLTree<T>::ll_rotation(Node<T>* parent)
{

    Node<T>* temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;

    return temp;
}

template<class T>
Node<T>* AVLTree<T>::lr_rotation(Node<T>* parent)
{
    Node<T>* temp;
    temp = parent->left;
    parent->left = rr_rotation(temp);
    return ll_rotation(parent);
}

template<class T>
Node<T>* AVLTree<T>::rl_rotation(Node<T>* parent)
{
    Node<T>* temp;
    temp = parent->right;
    parent->right = ll_rotation(temp);
    return rr_rotation(parent);
}

template<class T>
Node<T>* AVLTree<T>::balance(Node<T>* temp)
{
    int bal_factor = diff(temp);

    if (bal_factor > 1) {
        if (diff(temp->left) > 0) {

            temp = ll_rotation(temp);
        }

        else {
            temp = lr_rotation(temp);
        }
    }
    else if (bal_factor < -1) {
        if (diff(temp->right) > 0) {
            temp = rl_rotation(temp);
        }

        else

        {
            temp = rr_rotation(temp);
        }
    }

    return temp;
}
