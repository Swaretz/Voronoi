#include <iostream>
#include <algorithm> 
#include <cmath>


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
    point getRelativePos(float directrix){
        //determine highest y value of the tuple
        if(tuple==false)return *dataL;
        point* highest;
        if(dataL->y>dataR->y)highest=dataL;
        else highest=dataR;
        point returnvalue;
        float a1=dataR->y-directrix;
        float a2=dataL->y-directrix;
        float b1=-(2*dataL->x)*a1;
        float b2=-(2*dataR->x)*a2;
        float c1=a1*((dataL->x)*(dataL->x)+((dataL->y)*(dataL->y))-(directrix*directrix));
        float c2=a2*((dataR->x)*(dataR->x)+((dataR->y)*(dataR->y))-(directrix*directrix));
        float a=a1-a2;
        float b=b1-b2;
        float c=c1-c2;
        float discriminant = b*b - 4*a*c;
        if(discriminant>0){
            if(dataL==highest){
                returnvalue.x=(-b+sqrt(discriminant))/(2*a);
            }
            else{
                returnvalue.x=(-b-sqrt(discriminant))/(2*a);
            }
        }
        else if(discriminant==0){
            returnvalue.x=(-b)/(2*a);
        }
        return returnvalue;
    }
    float getMidPoint(){
        float distance=abs(dataL->x-dataR->x);
        if(dataL->y>dataR->y){
            return std::min(dataL->x,dataR->x)+0.75*distance;
        }
        else{
            return std::min(dataL->x,dataR->x)+0.25*distance;
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
    //root of the tree
    struct node* root;

    /**
     * @brief Insert function for arcs into an AVL tree structure
     * 
     * @param n current node
     * @param p point to add
     * @param x current position of beachline
     * @return ** node* pointer to current node with updated children
     */
    node* insert(node* n,point &p, float x){
        //if current node is NULL (only when tree is empty) set current node to point to add
        if(n==NULL)
        {
            return new node(p);
        }
        //if node is intersection of two arcs add new node to subtree
        if(n->tuple)
        {
            //i think we have to have an extra input here, that means the current position of the beach line
            //this will be used to assert the relative position of each tuple in comparison to the new site that is to be added
            
            /*if(p.x>n->dataR->x||(p.x-n->dataL->x)>(n->dataR->x-p.x))
            {
                n->right=insert(n->right,p);
                n->heightR=std::max(n->right->heightR,n->right->heightL)+1;
            }*/

            /*
            supposed to check relative position of current tuple and new point to decide location in the tree
            does not work as intended at the moment
            */
            if(p.x>(*n).getMidPoint()){
                n->right=insert(n->right,p, x);
                n->heightR=std::max(n->right->heightR,n->right->heightL)+1;
            }
            else
            {
                n->left=insert(n->left,p, x);
                n->heightL=std::max(n->left->heightR,n->left->heightL)+1;
            }
            /*
            rebalances the subtree to keep balance (ensures O(log n) insert)
            */
            if(n->heightL-n->heightR>=2)
            {
                /*if(n->left->heightR==n->heightL-1){
                    n->left=leftRotation(n->left);
                }*/
                n=rightRotation(n);
            }
            else if(n->heightL-n->heightR<=-2)
            {
                /*if(n->right->heightL==n->heightR-1){
                    n->right=rightRotation(n->right);
                }*/
                n=leftRotation(n);
            }
            return n;
        }
        /*
        case where current (A) node is a leaf, point to insert (B) is smaller than A 
        Reverse ballance if B is larger 
              A
              |
              V
             B,A
            /   \
          A,B    A
         /   \
        A     B
        */
        else
        {
            /*
            if point to add is larger
            */
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
            /*
            if point to add is smaller
            */
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



    /*
    rotates subtree according to the folloing schema
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
    rotates subtree according to the folloing schema
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
    point test=(Beachline.root->right->right)->getRelativePos(8);
    std::cout << "Hello World" << 4 << std::endl;
}