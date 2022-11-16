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
int AVLTree<T>::calheight(Node<T> *p){

    if(p->left && p->right){
        if (p->left->height < p->right->height)
            return p->right->height + 1;
        else return  p->left->height + 1;
    }
    else if(p->left && p->right == NULL){
        return p->left->height + 1;
    }
    else if(p->left ==NULL && p->right){
        return p->right->height + 1;
    }
    return 0;

}
template<class T>
int AVLTree<T>::bf( Node<T> *n){
    if(n->left && n->right){
        return n->left->height - n->right->height;
    }
    else if(n->left && n->right == NULL){
        return n->left->height;
    }
    else if(n->left== NULL && n->right ){
        return -n->right->height;
    }
}
template<class T>
 Node<T> * AVLTree<T>::llrotation(Node<T> *n){
    struct node *p;
    struct node *tp;
    p = n;
    tp = p->left;

    p->left = tp->right;
    tp->right = p;

    return tp;
}

template <class T>
Node<T> * AVLTree<T>::rrrotation(Node<T> *n){
    struct node *p;
    struct node *tp;
    p = n;
    tp = p->right;

    p->right = tp->left;
    tp->left = p;

    return tp;
}

template <class T>
Node<T> * AVLTree<T>::rlrotation(Node<T> *n){
    struct node *p;
    struct node *tp;
    struct node *tp2;
    p = n;
    tp = p->right;
    tp2 =p->right->left;

    p -> right = tp2->left;
    tp ->left = tp2->right;
    tp2 ->left = p;
    tp2->right = tp;

    return tp2;
}

template <class T>
Node<T> * AVLTree<T>::lrrotation(Node<T> *n){
    struct node *p;
    struct node *tp;
    struct node *tp2;
    p = n;
    tp = p->left;
    tp2 =p->left->right;

    p -> left = tp2->right;
    tp ->right = tp2->left;
    tp2 ->right = p;
    tp2->left = tp;

    return tp2;
}

template <class T>
Node<T>* AVLTree<T>::insert(Node<T> *r,int data){

    if(r==NULL){
        struct node *n;
        n = new struct node;
        n->data = data;
        r = n;
        r->left = r->right = NULL;
        r->height = 1;
        return r;
    }
    else{
        if(data < r->data)
            r->left = insert(r->left,data);
        else
            r->right = insert(r->right,data);
    }

    r->height = calheight(r);

    if(bf(r)==2 && bf(r->left)==1){
        r = llrotation(r);
    }
    else if(bf(r)==-2 && bf(r->right)==-1){
        r = rrrotation(r);
    }
    else if(bf(r)==-2 && bf(r->right)==1){
        r = rlrotation(r);
    }
    else if(bf(r)==2 && bf(r->left)==-1){
        r = lrrotation(r);
    }

    return r;

}

template <class T>
Node<T> * AVLTree<T>::deleteNode(Node<T> *p,int data){

    if(p->left == NULL && p->right == NULL){
        if(p==this->root)
            this->root = NULL;
        delete p;
        return NULL;
    }

    struct node *t;
    struct node *q;
    if(p->data < data){
        p->right = deleteNode(p->right,data);
    }
    else if(p->data > data){
        p->left = deleteNode(p->left,data);
    }
    else{
        if(p->left != NULL){
            q = inpre(p->left);
            p->data = q->data;
            p->left=deleteNode(p->left,q->data);
        }
        else{
            q = insuc(p->right);
            p->data = q->data;
            p->right = deleteNode(p->right,q->data);
        }
    }

    if(bf(p)==2 && bf(p->left)==1){ p = llrotation(p); }
    else if(bf(p)==2 && bf(p->left)==-1){ p = lrrotation(p); }
    else if(bf(p)==2 && bf(p->left)==0){ p = llrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==-1){ p = rrrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==1){ p = rlrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==0){ p = llrotation(p); }


    return p;
}

template <class T>
Node<T> AVLTree<T>::inpre(Node<T> p){
    while(p->right!=NULL)
        p = p->right;
    return p;
}

template <class T>
Node<T> AVLTree<T>::insuc(Node<T> p){
    while(p->left!=NULL)
        p = p->left;

    return p;
}