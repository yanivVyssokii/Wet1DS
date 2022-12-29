//
// Created by User on 11/15/2022.
//

#ifndef WET1DS_AVLTREE_H
#define WET1DS_AVLTREE_H
#include "Node.h"
#include "stdio.h"
#include "wet1util.h"
#include "iostream"
class Team;
template <class T>
class AVLTree {
private:
    int m_size;
    Node<T>* m_root;
public:
    bool (*m_comparator)(T& t1, T& t2);

    AVLTree(bool (*comparator)(T& t1, T& t2));

    int getSize();

    Node<T>* find( T& target);

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

    void inOrder(Node<T> *p, Node<T>** arr, int* index);

    void fillInOrder(Node<T> *p, Node<T>** arr, int* index, int sizeOfArr);

    void printInOrder(Node<T> *p);

    Node<T>* getRoot();

    void setRoot(Node<T>* newRoot);

    void setSize(int size);

    Node<T>* createEmptyTree(Node<T>* r,int height);

    Node<T>* findClosestBigger(Node<T> *p);

    Node<T>* findClosestSmaller(Node<T> *p);

    void deleteData(Node<T>* r);

    void updateFathers(Node<T>* p);

    void updateTeam(Node<T>*p,Team* team,int id);

    ~AVLTree();
};

template<class T>
AVLTree<T> *merge(AVLTree<T> *t1, AVLTree<T> *t2);
template<class T>
AVLTree<T>::AVLTree(bool (*comparator)(T& t1, T& t2)){
    m_comparator=comparator;
    m_root=nullptr;
    m_size=0;
}

template<class T>
Node<T> *AVLTree<T>::find( T& target) {
    Node<T> *temp = this->getRoot();
    while (temp != nullptr) {
        if (!m_comparator(*(temp->data),target)&&!m_comparator(target,*(temp->data)))
            return temp;
        if (m_comparator(target,*(temp->data)))
            temp = temp->right;
        else
            temp = temp->left;
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
    else if(p->left && p->right == nullptr){
        return p->left->height + 1;
    }
    else if(p->left ==nullptr && p->right){
        return p->right->height + 1;
    }
    return 0;

}
template<class T>
int AVLTree<T>::bf( Node<T> *n){
    if(n->left && n->right){
        return n->left->height - n->right->height;
    }
    else if(n->left && n->right == nullptr){
        return n->left->height+1;
    }
    else if(n->left== nullptr && n->right ){
        return -n->right->height-1;
    }
    else{
        return 0;
    }
}
template<class T>
Node<T> * AVLTree<T>::llrotation(Node<T> *n){
    Node<T> *p;
    Node<T> *tp;
    p = n;
    tp = p->left;

    p->left = tp->right;
    tp->right = p;

    tp->father=tp->right->father;
    tp->right->father=tp;
    tp->left->father=tp;
    p->left!=nullptr? p->left->father=p : p=p;

    tp->right->height= calheight(tp->right);
    tp->height= calheight(tp);
    if (tp->father== nullptr){
        m_root=tp;
    }
    return tp;
}

template <class T>
Node<T> * AVLTree<T>::rrrotation(Node<T> *n){
    Node<T> *p;
    Node<T> *tp;
    p = n;
    tp = p->right;

    p->right = tp->left;

    tp->left = p;

    tp->father=tp->left->father;
    tp->left->father=tp;
    tp->right->father=tp;
    p->right!=nullptr? p->right->father=p : p=p;

    tp->left->height= calheight(tp->left);
    tp->height= calheight(tp);
    if (tp->father== nullptr){
        m_root=tp;
    }
    return tp;
}

template <class T>
Node<T> * AVLTree<T>::rlrotation(Node<T> *n){
    Node<T> *p;
    Node<T> *tp;
    Node<T> *tp2;
    p = n;
    tp = p->right;
    tp2 =p->right->left;

    p -> right = tp2->left;
    tp ->left = tp2->right;
    tp2 ->left = p;
    tp2->right = tp;

    tp2->father=tp2->left->father;
    tp2->left->father=tp2;
    tp2->right->father=tp2;
    p->right!=nullptr ? p->right->father=p : p=p;
    tp->left!=nullptr ? tp->left->father=tp : p=p;

    tp2->left->height= calheight(tp2->left);
    tp2->right->height= calheight(tp2->right);
    tp2->height= calheight(tp2);
    if (tp2->father== nullptr){
        m_root=tp2;
    }
    return tp2;
}

template <class T>
Node<T> * AVLTree<T>::lrrotation(Node<T> *n){
    Node<T>*p;
    Node<T> *tp;
    Node<T>*tp2;
    p = n;
    tp = p->left;
    tp2 =p->left->right;

    p -> left = tp2->right;
    tp ->right = tp2->left;
    tp2 ->right = p;
    tp2->left = tp;

    tp2->father=tp2->right->father;
    tp2->left->father=tp2;
    tp2->right->father=tp2;
    p->left!=nullptr ? p->left->father=p : p=p;
    tp->right!=nullptr ? tp->right->father=tp : p=p;

    tp2->left->height= calheight(tp2->left);
    tp2->right->height= calheight(tp2->right);
    tp2->height= calheight(tp2);
    if (tp2->father== nullptr){
        m_root=tp2;
    }
    return tp2;
}

template <class T>
Node<T>* AVLTree<T>::insert(Node<T>* prevNode, Node<T> *r,T* data){
    if(r==nullptr){
        Node<T> *n;
        n = new Node<T>(data->getId(),data,prevNode, nullptr, nullptr);
        n->data = data;
        r = n;
        r->father = prevNode;
        r->left =nullptr;
        r->right = nullptr;
        r->height = 0;
        if (r->father== nullptr) {
            this->m_root = r;
        }
        m_size++;
        return r;
    }

    if(!m_comparator(*data,*r->data)&&!m_comparator(*r->data,*data)) {
        return r;
    }
    if(!m_comparator(*data,*r->data)) {
        r->left = insert(r, r->left, data);
    }
    else {
        r->right = insert(r, r->right, data);
    }

    r->height = calheight(r);

    if(bf(r)==2 && bf(r->left)>=0){
        r = llrotation(r);
    }
    else if(bf(r)==-2 && bf(r->right)<=0){
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
Node<T> * AVLTree<T>::deleteNode(Node<T> *p,T* data){
    if (!p){
        return nullptr;
    }
    if(p->left == nullptr && p->right == nullptr){
        if (m_comparator(*p->data,*data)||m_comparator(*data,*p->data)){
            return p;
        }
        if(p==this->m_root) {
            m_size--;
            delete p;
            this->m_root = nullptr;
            return nullptr;
        }
        m_size--;
        Node<T>* temp=p;
        if (temp->father->right!=nullptr&&temp->father->right->data==temp->data){
            temp->father->right= nullptr;
        }
        else{
            temp->father->left= nullptr;
        }
        delete p;
        return nullptr;
    }

    Node<T> *q;
    if(m_comparator(*data,*p->data)){
        p->right = deleteNode(p->right,data);
        p->height = calheight(p);
    }
    else if(m_comparator(*p->data,*data)){
        p->left = deleteNode(p->left,data);
        p->height = calheight(p);
    }
    else{
        if(p->left != nullptr){
            q = inpre(p->left);
            p->data = q->data;
            p->info = q->info;
            p->left=deleteNode(p->left,q->data);
            p->height = calheight(p);
        }
        else{
            q = insuc(p->right);
            p->data = q->data;
            p->info = q->info;
            p->right = deleteNode(p->right,q->data);
            p->height = calheight(p);
        }
    }

    if(bf(p)==2 && bf(p->left)==1){ p = llrotation(p); }
    else if(bf(p)==2 && bf(p->left)==-1){ p = lrrotation(p); }
    else if(bf(p)==2 && bf(p->left)==0){ p = llrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==-1){ p = rrrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==1){ p = rlrotation(p); }
    else if(bf(p)==-2 && bf(p->right)==0){ p = rrrotation(p); }


    return p;
}

template <class T>
Node<T>* AVLTree<T>::inpre(Node<T>* p){
    while(p->right!=nullptr)
        p = p->right;
    return p;
}

template <class T>
Node<T>* AVLTree<T>::insuc(Node<T>* p){
    while(p->left!=nullptr)
        p = p->left;

    return p;
}

template<class T>
void AVLTree<T>::inOrder(Node<T> *p, Node<T>** arr, int* index) {
    if (!p){
        return;
    }
    if (p->left== nullptr&&p->right== nullptr){
        arr[*index]=p;
        *index=*index+1;
        return;
    }
    if (p->left!= nullptr) {
        this->inOrder(p->left, arr, index);
    }
    arr[*index]=p;
    *index=*index+1;
    if (p->right!= nullptr) {
        this->inOrder(p->right, arr, index);
    }
}

template<class T>
void AVLTree<T>::printInOrder(Node<T> *p) {
    if (p== nullptr){
        return;
    }
    this->printInOrder(p->left);
    this->printInOrder(p->right);
}


template<class T>
void AVLTree<T>::fillInOrder(Node<T> *p, Node<T>** arr, int* index, int sizeOfArr) {
    if (p->left== nullptr&&p->right== nullptr){
        if (*index>=sizeOfArr){
            delete p;
            p= nullptr;
            *index=*index+1;
            return;
        }
        p->data=arr[*index]->data;
        p->info=arr[*index]->info;
        *index=*index+1;
        p->height= calheight(p);
        return;
    }
    this->fillInOrder(p->left,arr,index,sizeOfArr);
    p->data=arr[*index]->data;
    p->info=arr[*index]->info;
    *index=*index+1;
    this->fillInOrder(p->right,arr,index,sizeOfArr);
    p->height= calheight(p);
}

template<class T>
Node<T> *AVLTree<T>::getRoot() {
    return m_root;
}


template<class T>
Node<T>* sortedArrayToBST(Node<T>* arr[],int start, int end)
{
    if (start > end)
        return nullptr;

    int mid = (start + end)/2;
    Node<T> *root = new Node<T>(arr[mid]->data->getId(),arr[mid]->data, nullptr, nullptr, nullptr);

    root->left = sortedArrayToBST(arr, start,mid - 1);

    root->right = sortedArrayToBST(arr, mid + 1, end);

    return root;
}

template<class T>
AVLTree<T> *merge(AVLTree<T> *t1, AVLTree<T> *t2) {
    int sum=t1->getSize()+t2->getSize();
    if (sum==0){
        AVLTree<T>* t=new AVLTree<T>(t1->m_comparator);
        t->updateFathers(t->getRoot());
        return t;
    }
    if (t1->getSize()==0){
        Node<T>** arr2 = new Node<T>*[t2->getSize()];
        int index=0;
        t2->inOrder(t2->getRoot(),arr2,&index);
        AVLTree<T>* t= new AVLTree<T>(t2->m_comparator);
        t->setRoot(sortedArrayToBST(arr2,0,sum-1));
        t->updateFathers(t->getRoot());
        t->setSize(sum);
        delete []arr2;
        return t;
    }
    if (t2->getSize()==0){
        Node<T>** arr1 = new Node<T>*[t1->getSize()];
        int index=0;
        t1->inOrder(t1->getRoot(),arr1,&index);
        AVLTree<T>* t= new AVLTree<T>(t1->m_comparator);
        t->setRoot(sortedArrayToBST(arr1,0,sum-1));
        t->updateFathers(t->getRoot());
        t->setSize(sum);
        delete []arr1;
        return t;
    }
    Node<T>** arr1 = new Node<T>*[t1->getSize()];
    Node<T>** arr2 = new Node<T>*[t2->getSize()];
    int index=-0;
    t1->inOrder(t1->getRoot(),arr1,&index);
    index=-0;
    t2->inOrder(t2->getRoot(),arr2,&index);
    Node<T>** finalArr=new Node<T>*[sum];
    int j=0,k=0;
    for (int i=0;i<sum;i++){
        if (j>=t1->getSize()){
            finalArr[i]=arr2[k];
            k++;
        }
        else if  (k>=t2->getSize()){
            finalArr[i]=arr1[j];
            j++;
        }
        else if (t1->m_comparator(*(arr1[j]->data),*(arr2[k]->data))){
            finalArr[i]=arr2[k];
            k++;
        }
        else{
            finalArr[i]=arr1[j];
            j++;
        }
    }//hi
    int height=1;
    int temp=sum;
    while (temp>1){
        temp=temp/2;
        height++;
    }
    AVLTree<T>* t= new AVLTree<T>(t1->m_comparator);
    t->setRoot(sortedArrayToBST(finalArr,0,sum-1));
    t->updateFathers(t->getRoot());
    t->setSize(sum);
    delete []arr1;
    delete []arr2;
    delete []finalArr;
    return t;
}








template<class T>
Node<T>* AVLTree<T>::createEmptyTree(Node<T>* r,int height) {
    if (height==0) {
        return nullptr;
    }
    r=new Node<T>();
    if (m_root== nullptr){
        m_root=r;
    }
    r->left=createEmptyTree(r->left,height-1);
    r->right=createEmptyTree(r->right,height-1);
    return r;


}

template<class T>
int AVLTree<T>::getSize() {
    return m_size;
}

template<class T>
Node<T> *AVLTree<T>::findClosestBigger(Node<T> *p) {
    if (p->father== nullptr){
        if (p->right== nullptr){
            return nullptr;
        }
        return insuc(p->right);
    }
    if (!p->right){
        if (p->father->left==p){
            return p->father;
        }
        Node<T>* temp=p->father;
        while (!temp && temp->father!= nullptr){
            if (temp->father->left==temp){
                return temp->father;
            }
            temp=temp->father;
        }
        return nullptr;
    }
    if (p->right){
        Node<T>* temp=p->right;
        while (temp->left!= nullptr){
            temp=temp->left;
        }
        return temp;
    }
    return nullptr;
}

template<class T>
Node<T> *AVLTree<T>::findClosestSmaller(Node<T> *p) {
    if (p->father== nullptr){
        if (p->left== nullptr){
            return nullptr;
        }
        return inpre(p->left);
    }
    if (!p->left){
        if (p->father->right==p){
            return p->father;
        }
        Node<T>* temp=p->father;
        while (!temp && temp->father!= nullptr){
            if (temp->father->right==temp){
                return temp->father;
            }
            temp=temp->father;
        }
        return nullptr;
    }
    if (p->left){
        Node<T>* temp=p->left;
        while (temp->right!= nullptr){
            temp=temp->right;
        }
        return temp;
    }
    return nullptr;
}

template<class T>
AVLTree<T>::~AVLTree() {
    delete m_root;
}

template<class T>
void AVLTree<T>::setRoot(Node<T> *newRoot) {
    m_root=newRoot;
}

template<class T>
void AVLTree<T>::deleteData(Node<T>* r) {
    if (!r){
        return;
    }
    deleteData(r->left);
    deleteData(r->right);
    delete r->data;
}

template<class T>
void AVLTree<T>::updateFathers(Node<T>*p) {
    if (p->right) {
        p->right->father = p;
        updateFathers(p->right);
    }
    if (p->left) {
        p->left->father = p;
        updateFathers(p->left);
    }

}

template<class T>
void AVLTree<T>::setSize(int size) {
    m_size=size;

}

template<class T>
void AVLTree<T>::updateTeam(Node<T>*p,Team* team,int id) {
    if (!p){
        return;
    }
    p->data->setTeam(team);
    p->data->setTeamId(id);
    updateTeam(p->left,team,id);
    updateTeam(p->right,team,id);
}


#endif //WET1DS_AVLTREE_H
