#include <iostream>
#include <algorithm> 

struct node{
    int pos, heightR, heightL;
    struct node* left;
    struct node* right;
    struct node* parent;


    node(int p){
        heightR=0, heightL=0;
        pos=p;
        left=NULL;
        right=NULL;
    }
    void setParent(node &n){
        parent = &n;
    }
};
class btree{
private:
    node* root;
    void insertHelp(node &n, int p){
        if(n.pos==p)return;
        if(p>n.pos){
            
            if(n.right==NULL){
                n.right=new node(p);
                (*n.right).setParent(n);
                n.heightR=1;
            }
            else {
                insertHelp(*n.right,p);
                n.heightR=std::max(n.right->heightR,n.right->heightL)+1;
            }
            
        }
        else{
            
            if(n.left==NULL){
                n.left=new node(p);
                (*n.left).setParent(n);
                n.heightL=1;
            }
            else {
                insertHelp(*n.left,p);
                n.heightL=std::max(n.left->heightR,n.left->heightL)+1;
            }
        }

        int balancefactor= n.heightL-n.heightR;
        
        if(balancefactor>=2){
            if(p<n.left->pos){
                rightRotation(n);
            }
            else{
                leftRotation(*(n.left));
                rightRotation(n);
            }
        }
        else if(balancefactor<=-2){
            if(p>n.right->pos){
                leftRotation(n);
            }
            else{
                rightRotation(*(n.right));
                leftRotation(n);
            }
        }
               
    }
    void printHelp(node n){
        std::cout << n.pos<<" ";
        if(n.left!=NULL){
            std::cout << "(" ;
            printHelp(*n.left);
            std::cout << ") " ;
        }
        if(n.right!=NULL){
            std::cout << "[" ;
            printHelp(*n.right);
            std::cout << "]" ;
        }
        
    }
    node* get(node &n, int p){
        if(n.pos==p)return &n;
        else{
            if(p>n.pos){
                if(n.right==NULL)return NULL;
                return get(*n.right,p);
            }
            else {
                if(n.left==NULL)return NULL;
                return get(*n.left,p);
            }
        }
    }
    
    node* getLeftMostChild(node &n){
        if(n.left==NULL)return &n;
        else return getLeftMostChild(*n.left);
    }
    void deleteHelp(node &n, int p){
        if(&n==NULL)return;
        if(p>n.pos){
            deleteHelp(*n.right,p);

        }
        else if(p<n.pos){
            deleteHelp(*n.left,p);
        }
        else{

            if(n.left==NULL && n.right==NULL){
                if(&n==(*(n.parent)).right){
                    (*(n.parent)).right=NULL;
                }
                else{
                    (*(n.parent)).left=NULL;
                }
                free(&n);
                return;
            }
            else if(n.left!=NULL && n.right!=NULL){
                node* replacement = getLeftMostChild(*n.right);
                n.pos=replacement->pos;
                deleteHelp(*n.right,n.pos);
                
            }
            else{
                if(n.left==NULL){
                    n.pos=(*n.right).pos;
                    deleteHelp(*n.right,n.pos);
                    
                }
                else{
                    n.pos=(*n.left).pos;
                    deleteHelp(*n.left,n.pos);
                   
                }
            }
            
        }
        if(n.left==NULL){
            n.heightL=0;
            
        }
        else{
            n.heightL=std::max((n.left)->heightL,(n.left)->heightR)+1;
        }

        if(n.right==NULL){
            n.heightR=0;
        }
        else{
            n.heightR=std::max((n.right)->heightL,(n.right)->heightR)+1;
        }
        
        int balanceFactor= n.heightL-n.heightR;
        if(balanceFactor>=2){
            if((n.left->heightL-n.left->heightR)>=0){
                rightRotation(n);
            }
            else{
                leftRotation(*(n.left));
                rightRotation(n);
            }
        }
        if(balanceFactor<=-2){
            if((n.right->heightL-n.right->heightR)<=0){
                leftRotation(n);
            }
            else{
                rightRotation(*(n.right));
                leftRotation(n);
            }
        }

    }

    
    /*
            A           B
           / \         / \
          B   C  -->  D   A
         / \             / \
        D   E           E   C
    
    */
    void rightRotation(node &a){
        if(&a==NULL)return;
        if(&a==root){
            node* b = a.left;
            if(b==NULL)return;
            node* e = (*a.left).right;
            (*b).right=&a;
            a.parent=b;
            a.left=e;
            if(e!=NULL)(*e).parent=&a;
            (*b).parent=NULL;
            root=b;
            //added newest for rebalancing purposes
            if(e!=NULL){
                a.heightL=std::max((*e).heightL,(*e).heightR)+1;
            }
            else{
                a.heightL=0;
            }
            (*b).heightR=std::max(a.heightL,a.heightR)+1;
            return;
        }
        if(a.left==NULL && a.right==NULL)return;
        node* rootLocal= a.parent;
        node* b = a.left;
        node* e = (*a.left).right;
        bool right = (*rootLocal).right==&a;
        (*b).parent=rootLocal;
        if(right){
            (*rootLocal).right=b;
        }
        else{
            (*rootLocal).left=b;
        }
        (*b).right=&a;
        a.parent=b;
        a.left=e;
        if(e!=NULL){
            
            (*e).parent=&a;
            a.heightL=std::max((*e).heightL,(*e).heightR)+1;
        }
        else{
                a.heightL=0;
            }
        (*b).heightR=std::max(a.heightL,a.heightR)+1;
        if(right){
            b->parent->heightR=std::max(b->heightR,b->heightL)+1;
        }
        else{
            b->parent->heightL=std::max(b->heightR,b->heightL)+1;
        }
    }
    
    void leftRotation(node &a){
        if(&a==NULL)return;
        if(&a==root){
            node* b = a.right;
            if(b==NULL)return;
            node* e = (*a.right).left;
            (*b).left=&a;
            a.parent=b;
            a.right=e;
            if(e!=NULL)(*e).parent=&a;
            (*b).parent=NULL;
            root=b;
            if(e!=NULL){
                a.heightR=std::max((*e).heightL,(*e).heightR)+1;
            }
            else{
                a.heightR=0;
            }
            (*b).heightL=std::max(a.heightL,a.heightR)+1;
            return;
        }
        if(a.left==NULL && a.right==NULL)return;
        node* rootLocal= a.parent;
        node* b = a.right;
        node* e = (*a.right).left;
        bool right = (*rootLocal).right==&a;
        (*b).parent=rootLocal;
        if(right){
            (*rootLocal).right=b;
        }
        else{
            (*rootLocal).left=b;
        }
        (*b).left=&a;
        a.parent=b;
        a.right=e;
        if(e!=NULL){
            
            (*e).parent=&a;
            a.heightR=std::max((*e).heightL,(*e).heightR)+1;
        }
        else{
            a.heightR=0;
        }
        (*b).heightL=std::max(a.heightL,a.heightR)+1;
        if(right){
            b->parent->heightR=std::max(b->heightR,b->heightL)+1;
        }
        else{
            b->parent->heightL=std::max(b->heightR,b->heightL)+1;
        }
    }
public:
    btree(int p){
        root=new node(p);
    }
    btree(){
    }
    void insert(int p){
        if(root=NULL)root=new node(p);
        else{
            insertHelp(*root,p);
        }
        
    }
    void del(int p){
        deleteHelp(*root, p);
    }
    void printTree(){
        printHelp(*root);
    }
    

};