//
// Created by User on 11/15/2022.
//

#include "AVLTree.h"
#include "stdio.h"
template<class T>
AVLTree<T>::AVLTree(bool (*comparator)(T& t1, T& t2)){
    m_comparator=comparator;
    m_root=nullptr;
    m_size=0;
}

template<class T>
Node<T> *AVLTree<T>::find(const T& target) {
    Node<T> *temp = m_root;
    while (temp != nullptr) {
        if (!m_comparator(temp,target)&&!m_comparator(target,temp))
            return m_root->data;
        if (m_comparator(target,temp))
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
        r->left = r->right = nullptr;
        r->height = 0;
        if (r->father== nullptr) {
            this->m_root = r;
        }
        m_size++;
        return r;
    }

    if(!m_comparator(*data,*r->data)&&!m_comparator(*r->data,*data))
        return nullptr;
    if(!m_comparator(*data,*r->data))
        r->left = insert(r,r->left,data);
    else
        r->right = insert(r,r->right,data);

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

    if(p->left == nullptr && p->right == nullptr){
        if(p==this->m_root)
            this->m_root = nullptr;
        m_size--;
        delete p;
        return nullptr;
    }

    Node<T> *t;
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
void AVLTree<T>::inOrder(Node<T> *p, Node<T>* arr, int* index) {
    if (p->left== nullptr&&p->right== nullptr){
        arr[*index]=*p;
        *index=*index+1;
        return;
    }
    if (p->left!= nullptr) {
        this->inOrder(p->left, arr, index);
    }
    arr[*index]=*p;
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
    //std::cout<<"value:"<<p->data->getId()<<std::endl;
    //std::cout<<"height:"<<p->height<< std::endl;
    this->printInOrder(p->right);
}


template<class T>
void AVLTree<T>::fillInOrder(Node<T> *p, Node<T>* arr, int* index, int sizeOfArr) {
    if (p->left== nullptr&&p->right== nullptr){
        if (*index>=sizeOfArr){
            delete p;
            p= nullptr;
            *index=*index+1;
            return;
        }
        p->data=arr[*index].data;
        p->info=arr[*index].info;
        *index=*index+1;
        p->height= calheight(p);
        return;
    }
    this->fillInOrder(p->left,arr,index,sizeOfArr);
    p->data=arr[*index].data;
    p->info=arr[*index].info;
    *index=*index+1;
    this->fillInOrder(p->right,arr,index,sizeOfArr);
    p->height= calheight(p);
}

template<class T>
Node<T> *AVLTree<T>::getRoot() {
    return m_root;
}


template<class T>
AVLTree<T> *merge(AVLTree<T> *t1, AVLTree<T> *t2) {
    int sum=t1->getSize()+t2->getSize();
    Node<T>* arr1 = new Node<T>[t1->getSize()];
    Node<T>* arr2 = new Node<T>[t2->getSize()];
    int index=0;
    t1->inOrder(t1->getRoot(),arr1,&index);
    index=0;
    t2->inOrder(t2->getRoot(),arr2,&index);
    Node<T>* finalArr=new Node<T>[sum];
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
        else if (t1->m_comparator(*(arr1[j].data),*(arr2[k].data))){
            finalArr[i]=arr2[k];
            k++;
        }
        else{
            finalArr[i]=arr1[j];
            j++;
        }
    }
    int height=1;
    int temp=sum;
    while (temp>1){
        temp=temp/2;
        height++;
    }
    AVLTree<T>* t= new AVLTree<T>(t1->m_comparator);
    Node<T>* root = t->getRoot();
    root=t->createEmptyTree(t->getRoot(),height);
    int finalIndex=0;
    t->fillInOrder(t->getRoot(),finalArr,&finalIndex,sum);
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