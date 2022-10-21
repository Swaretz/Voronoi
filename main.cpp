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
        /*point* highest;
        if(dataL->y>dataR->y)highest=dataL;
        else highest=dataR;*/
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
            /*if(dataL==highest){
                returnvalue.x=(-b+sqrt(discriminant))/(2*a);
            }
            else{
                returnvalue.x=(-b-sqrt(discriminant))/(2*a);
            }*/
            returnvalue.x=(-b-sqrt(discriminant))/(2*a);
        }
        else if(discriminant==0){
            returnvalue.x=(-b)/(2*a);
        }
        return returnvalue;
    }

    float getMidPoint(){
        /*float distance=abs(dataL->x-dataR->x);
        if(dataL->y>dataR->y){
            return std::min(dataL->x,dataR->x)+0.75*distance;
        }
        else{
            return std::min(dataL->x,dataR->x)+0.25*distance;
        }*/
        return std::min(dataL->x,dataR->x)+abs(dataL->x-dataR->x)*0.5;
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
struct circle
{
    float radius,x,y;
    point* a;
    point* b;
    point* c;
};



class beachline
{
private:
    struct Storage
    {
        struct point* a;//first arc to be split
        struct point* b;//left arc
        struct point* c;//right arc
    };
    
public:
    //root of the tree
    struct node* root;
    struct Storage storage;
    void resetStorage(){
        storage.a=NULL;
        storage.b=NULL;
        storage.c=NULL;
    }
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
            /*
            supposed to check relative position of current tuple and new point to decide location in the tree
            does not work as intended at the moment
            */
           float testvalue = (*n).getRelativePos(x).x;
            if(p.x>testvalue){
                if(n->heightR==1)
                {
                    storage.a=n->dataR;
                    storage.b=n->dataL;
                }
                
                n->right=insert(n->right,p, x);
                if(storage.a!=NULL && (storage.c==NULL || storage.b==NULL))
                {
                    storage.b=n->dataL;
                }
                //here we should store the three point that neighbor the inserted sub tree
                //do this by checking height and then calculate circle event
                //if(n->heightR==1) dosomething with n->dataL;//this is to get the left neighbor
                n->heightR=std::max(n->right->heightR,n->right->heightL)+1;
            }
            else
            {
                if(n->heightL==1)
                {
                    storage.a=n->dataL;
                    storage.c=n->dataR;
                }
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
    /**
     * @brief given three input points, gives the centre of the circle which all three inputs lie 
     * 
     * @param a first point
     * @param b second point   
     * @param c third point
     * @return ** point centre of the circle
     */
    circle circleEvent(point &a, point &b, point &c){
        float value,rest, divval, radius;
        circle returnvalue;
        //equation 1:
        divval=(1/(2*b.y-2*a.y));
        value=(2*a.x-2*c.x)+(2*a.y-2*c.y)*(2*a.x-2*b.x)*divval;
        rest=(2*a.y-2*c.y)/(2*b.y-2*a.y)*(b.x*b.x-a.x*a.x+b.y*b.y-a.y*a.y)+(c.x*c.x-a.x*a.x+c.y*c.y-a.y*a.y);
        returnvalue.x=rest/(-value);
        returnvalue.y=((2*a.x-2*b.x)*returnvalue.x+(b.x*b.x-a.x*a.x+b.y*b.y-a.y*a.y))*divval;
        returnvalue.radius=std::sqrt((1-returnvalue.x)*(1-returnvalue.x)+(1-returnvalue.y)*(1-returnvalue.y));
        returnvalue.a=&a;
        returnvalue.b=&b;
        returnvalue.c=&c;
        return returnvalue;
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

struct PrioNode
{
    bool circleEvent;
    PrioNode* next;
    float ycoord;
    void* data;
    PrioNode(point &s){
        circleEvent=false;
        next=NULL;
        ycoord=s.y;
        data=&s;
    }
    PrioNode(point &s, PrioNode* p){
        circleEvent=false;
        next=p;
        ycoord=s.y;
        data=&s;
    }
    PrioNode(circle &c){
        circleEvent=true;
        next=NULL;
        ycoord=c.y+c.radius;
        data=&c;
    }
    PrioNode(circle &c, PrioNode* p){
        circleEvent=true;
        next=p;
        ycoord=c.y+c.radius;
        data=&c;
    }
};


class PrioQueue
{
private:
struct PrioNode* root;
PrioNode* insert(point &p,PrioNode* current){
    if(current==NULL){
        return new PrioNode(p);
    }
    else if(p.y>current->ycoord){
       current->next=insert(p,current->next);
       return current;
    }
    else{
        return new PrioNode(p,current);
    }
}
PrioNode* insert(circle &p,PrioNode* current){
    if(current==NULL){
        return new PrioNode(p);
    }
    else if(p.y+p.radius>current->ycoord){
       current->next=insert(p,current->next);
       return current;
    }
    else{
        return new PrioNode(p,current);
    }
}
public:
void insert(point &site){
    root=insert(site,root);
}
void insert(circle &event){
    root=insert(event,root);
}
bool topCircle(){
    return root->circleEvent;
}
void* pop(){
    void* returnvalue=root->data;
    PrioNode* temp=root;
    root=root->next;
    free(temp);
    return returnvalue;
}
};
int main()
{
    point a={1,1};
    point b={2,1.5};
    point c={1.5,2};
    point d={1,3};
    PrioQueue queue= PrioQueue();
    queue.insert(a);
    queue.insert(b);
    point* test=(point*)queue.pop();
    point* test2=(point*)queue.pop();
    beachline Beachline=beachline();
    circle circ=Beachline.circleEvent(a,b,c);
    queue.insert(circ);
    Beachline.root=Beachline.insert(Beachline.root,a,1);
    Beachline.root=Beachline.insert(Beachline.root,b,1.5);
    Beachline.root=Beachline.insert(Beachline.root,c,2);
    /*point bc=Beachline.root->getRelativePos(2);
    point ab=Beachline.root->left->getRelativePos(2);
    point cb=Beachline.root->right->getRelativePos(2);
    point ba=Beachline.root->right->right->getRelativePos(2);*/
    Beachline.root=Beachline.insert(Beachline.root,d,3);
    point ab=Beachline.root->getRelativePos(3);
    point cb=Beachline.root->right->getRelativePos(3);
    point ba=Beachline.root->right->right->getRelativePos(3);
    point bc=Beachline.root->right->left->getRelativePos(3);
    point da=Beachline.root->left->getRelativePos(3);
    point ad=Beachline.root->left->left->getRelativePos(3);
    std::cout << "Hello World" << 4 << std::endl;
}