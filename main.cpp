#include <iostream>
#include <algorithm> 


struct point
{
    float x, y;
};
struct node
{
    int heightR=0,heightL=0;
    struct node* right;
    struct node* left;
    struct point* dataL;
    struct point* dataR;
    bool tuple=false;
    bool side=false;
    node(point &p)
    {
        dataL=&p;
        dataR=&p;
    }
    node(point* p)
    {
        dataL=p;
        dataR=p;
    }
    float getMidPointX(){
        if(tuple==true){

            return dataL->x+0.5*(dataR->x-dataL->x);
            /*if(dataL->x<dataR->x){
                return dataL->x+0.5*std::abs(dataR->x-dataL->x);
            }
            else{
                return dataR->x+0.5*std::abs(dataL->x-dataR->x);
            }*/
        }
        else{
            return dataL->x;
        }
    }
    node(point &p1,point &p2)
    {
        tuple=true;
        dataL=&p1;
        dataR=&p2;
    }
    node(point* p1,point* p2)
    {
        tuple=true;
        dataL=p1;
        dataR=p2;
    }
    
    void editData(point &p1,point &p2)
    {
        tuple=true;
        dataL=&p1;
        dataR=&p2;
    }
};




class beachline
{
private:
    
    
public:
    struct node* root;

    node* insert(node* n,point &p, float x){
        if(n==NULL)
        {
            return new node(p);
        }
        if(n->tuple)
        {
            //i think we have to have an extra input here, that means the current position of the beach line
            //this will be used to assert the relative position of each tuple in comparison to the new site that is to be added
            
            /*if(p.x>n->dataR->x||(p.x-n->dataL->x)>(n->dataR->x-p.x))
            {
                n->right=insert(n->right,p);
                n->heightR=std::max(n->right->heightR,n->right->heightL)+1;
            }*/
            if(p.x>(*n).getMidPointX()){
                n->right=insert(n->right,p, x);
                n->heightR=std::max(n->right->heightR,n->right->heightL)+1;
            }
            /*else if((p.x-n->dataL->x)==(n->dataR->x-p.x)){

            }*/
            else
            {
                n->left=insert(n->left,p, x);
                n->heightL=std::max(n->left->heightR,n->left->heightL)+1;
            }
            if(n->heightL-n->heightR>=2)
            {
                n=rightRotation(n);
            }
            else if(n->heightL-n->heightR<=-2){
                n=leftRotation(n);
            }
            return n;
        }
        else
        {
            if(p.x>n->dataL->x)
            {
                node* temp=new node(p,*(n->dataL));
                temp->left=new node(p);
                temp->right=new node(*(n->dataL));
                temp->heightL=1;
                temp->heightR=1;
                n->editData(*(n->dataL),p);
                n->left=new node(n->dataL);
                n->right=temp;
                n->heightL=1;
                n->heightR=2;
            }
            else
            {
                node* temp=new node(*(n->dataL),p);
                temp->left=new node(*(n->dataL));
                temp->right=new node(p);
                temp->heightL=1;
                temp->heightR=1;
                n->editData(p,*(n->dataL));
                n->right=new node(n->dataR);
                n->left=temp;
                n->heightL=2;
                n->heightR=1;
            }
            return n;
        }
    }

    node* find(node* current, node* target){
        if(current==nullptr){
            return NULL;
        }
        if((*target).getMidPointX()==(*current).getMidPointX()){
            /*if(target->dataL==current->dataL && target->dataR==current->dataR){
                return current;
            }
            else{
                if(target->dataL->)

            }*/
            return current;
        }
        else if((*target).getMidPointX()>(*current).getMidPointX()){
            return find(current->right,target);
        }
        else{
            return find(current->left,target);
        }
        return NULL;
    }
    node* getleftmostchildParent(node* a, node* target){
        if(a->left->tuple==false){
            return a;
        } 
        else{
            return getleftmostchildParent(a->left, target);
        }
    }

    node* getrightmostchildParent(node* a, node* target){
        if(a->right->tuple==false){
            return a;
        } 
        else{
            return getrightmostchildParent(a->left, target);
        }
    }


    /*
            A           B
           / \         / \
          B   C  -->  D   A
         / \             / \
        D   E           E   C
    
    */
    node* rightRotation(node* a){
        node* b=a->left;
        node* e=b->right;
        a->left=e;
        a->heightL=std::max(e->heightL,e->heightR)+1;
        b->right=a;
        b->heightR=std::max(a->heightL,a->heightR)+1;
        return b;
    }
    /*
            A           B
           / \         / \
          B   C  <--  D   A
         / \             / \
        D   E           E   C
    
    */
    node* leftRotation(node* b){
        node* a=b->right;
        node* e=a->left;
        b->right=e;
        b->heightR=std::max(e->heightL,e->heightR)+1;
        a->left=b;
        a->heightL=std::max(b->heightL,b->heightR)+1;
        return a;
    }
    
};
int main()
{
    point a={1,1};
    point b={2,1.5};
    point c={1.5,2};
    point d={1,3};
    beachline Beachline=beachline();
    Beachline.root=Beachline.insert(Beachline.root,a,1);
    Beachline.root=Beachline.insert(Beachline.root,b,1.5);
    Beachline.root=Beachline.insert(Beachline.root,c,2);
    Beachline.root=Beachline.insert(Beachline.root,d,3);
    node* found=Beachline.find(Beachline.root,new node(c,a));
    std::cout << "Hello World" << 4 << std::endl;
}