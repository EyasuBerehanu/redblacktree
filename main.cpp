//Eyasu Berehanu
//4/18/2025
//This is a red black tree, a binary search tree that balances itselfwhich allows you to insert values, read values from a file, delte values, and search up values in the bianry search tree 
//Sources:
//Some help from my brother with getsuccessor, 
//Geeksforgeeks on helping how to learn how to do left and right, and fix insert
//Michal Sambol - Red Black tree under 5min explation
//Red black tree visualizer
//Mr Galbrith assistances on print from past heap project
//for fix delelte and update remove: https://www.geeksforgeeks.org/deletion-in-red-black-tree/#, 
//https://brilliant.org/wiki/red-black-tree/, wikpida red black tree explained
//https://www.youtube.com/watch?v=ZxCvM-9BaXE
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Node {
    int data;
    bool color; //true for red fals for black
    Node* left; 
    Node* right; 
    Node* parent;
    
    Node(int value) {
        data = value;
        color = true; // New nodes start red true
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* insert(Node* root, int value);
void printTree(Node* root, int depth);
void filess(Node*& root, int& size);
Node* remove(Node* root, int value);
Node* getSuccessor(Node* cur); 
Node* search(Node* root, int value);
void fixInsert(Node*& root, Node*& x);
void rotateLeft(Node*& root, Node*& x);
void rotateRight(Node*& root, Node*& x);
void fixRemove(Node*& root, Node*& x, Node* xParent);

void rotateLeft(Node*& root, Node*& x) { //does left rotation on a given node

    Node* y = x->right; // y to xs right child
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        root = y;  // x was the root
        
    } else if (x == x->parent->left) {
        x->parent->left = y; // x was left child so parent's left pointer should change
        
    } else {
        x->parent->right = y; // x was right child so parent's left pointer should change
        
    }
    
    y->left = x;
    x->parent = y;
}

void rotateRight(Node*& root, Node*& x) { //does right rotation on a given node
    Node* y = x->left; // y to xs lefty child
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        root = y;  // x was the root
        
    } else if (x == x->parent->right) {
        x->parent->right = y; // x was right child so parent's left pointer should change
        
    } else {
        x->parent->left = y; // x was left child so parent's left pointer should change
    }
    
    y->right = x;
    x->parent = y;
}

void fixInsert(Node*& root, Node*& x) {
   
    while (x != root && x->parent->color == true) { // While x is not the root and its parent's color is RED
        Node* grandparent = x->parent->parent;

        //x's parent is a left child
        if (x->parent == grandparent->left) {
            Node* uncle = grandparent->right;
	    //Uncle is RED
            if (uncle != nullptr && uncle->color == true) {
                x->parent->color = false; // Parent BLACK
                uncle->color = false;     // Uncle BLACK
                grandparent->color = true; // Grandparent RED
                x = grandparent; 
            }
            
        else { //Uncle BLACK or NULL
                if (x == x->parent->right) {
                    x = x->parent;
                    rotateLeft(root, x);
                }
                x->parent->color = false; 
                grandparent->color = true; 
                rotateRight(root, grandparent);
            }
        }
        
        else { //parent is a right child
              Node* uncle = grandparent->left;
            if (uncle != nullptr && uncle->color == true) { // Uncle is RED
                x->parent->color = false;
                uncle->color = false;
                grandparent->color = true;
                x = grandparent;
            }
            
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(root, x);
                }
                x->parent->color = false;
		grandparent->color = true;
                rotateLeft(root, grandparent);
            }
        }
    }

    root->color = false; 
}

Node* insert(Node* root, int value) {
    Node* newNode = new Node(value); //new red node
    
    if (root == nullptr) {
        newNode->color = false;  // Black
        return newNode;
    }
    
    Node* current = root;
    Node* parent = nullptr;
    
    while (current != nullptr) { //traverese tree to find the right(as in correct) point
        parent = current;

        if (value < current->data) {
            current = current->left; //left for smaller
        } else {
            current = current->right; //right for bigger
        }
    }

    if (value < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    newNode->parent = parent; //connects new node to tree

    fixInsert(root, newNode);

    return root;
}

Node* getSuccessor(Node* cur) { //finds the next large node or the successor of a node or number
    if (cur == nullptr || cur->right == nullptr){ //whyen the current node or right clid is a null then there is no succesor
       return nullptr; 
    } 
    cur = cur->right;
    while (cur->left != nullptr) {
        cur = cur->left;
    }
    return cur;
}

void fixRemove(Node*& root, Node*& x, Node* xparent) { //fixes violations of the rbt after deletion so it can meet the rules of rbt
    while (x != root && (x == nullptr || x->color == false)) { //while x is root or is red
        
        if (x == xparent->left) { // x = left chikd
            Node* sib = xparent->right;
            
            if (sib != nullptr && sib->color == true) { //for when sibling is red
                sib->color = false;
                xparent->color = true;
                rotateLeft(root, xparent); //use the rotate left made for fixinsert is being used here then we updatre sib
                sib = xparent->right; 
            }

            if ((sib->left == nullptr || sib->left->color == false) && (sib->right == nullptr || sib->right->color == false)) { //sib is black and the sib childern is also black
                sib->color = true;
                x = xparent;
                xparent = x->parent;
            }
            else {
                if (sib->right == nullptr || sib->right->color == false) { //sib is black but now left child is red and right is black 
                    
                    if (sib->left != nullptr) {
                        sib->left->color = false;
                    }
                    sib->color = true;
                    rotateRight(root, sib);
                    sib = xparent->right;
                }
                
                
                sib->color = xparent->color; //when right child is red while sibling is blkack
                xparent->color = false;
                
                if (sib->right != nullptr) {
                    sib->right->color = false;
                }
                rotateLeft(root, xparent);
                sib = root;
                break;
            }
        }
        else { //now when x is right child
            Node* sib = xparent->left;
            if (sib != nullptr && sib->color == true) { //for when sibling is red
                sib->color = false;
                xparent->color = true;
                rotateRight(root, xparent);
                sib = xparent->left;
            }

            if ((sib->left == nullptr || sib->left->color == false) && (sib->right == nullptr || sib->right->color == false)) {  //sib is black and the sib childern is also black
                sib->color = true;
                x = xparent;
                xparent = x->parent;
            }
            else {
                if (sib->left == nullptr || sib->left->color == false) { //sib is black but now right child is red and left is black 
                    
                    if (sib->right != nullptr) {
                        sib->right->color = false;
                    }
                    sib->color = true;
                    rotateLeft(root, sib);
                    sib = xparent->left;
                }
                //when left child is red while sibling is black
                sib->color = xparent->color; 
                xparent->color = false;
                
                if (sib->left != nullptr) {
                    sib->left->color = false;
                }
                rotateRight(root, xparent);
                x = root;
                break;
            }
        }
    }
    if (x != nullptr) { //make sure x is black after fixRemove
        x->color = false;
    }
}

Node* remove(Node* root, int value) { //removes the chose number but now with the logic of rbt implemented, had to use succersor to do this
    Node* node = root;
    Node* x = nullptr;          // points to the child that replace deleted node
    Node* xParent = nullptr;    // need for fixRemove
    bool firstColor;

    while (node != nullptr && node->data != value) { //looks for node with the number we lookin to delete
        if (value < node->data) {
            node = node->left;   // left with smaller num
        } else {
            node = node->right;  // right with larger num
        }
    }

    if (node == nullptr) {
        return root;  
    }

    firstColor = node->color;

    Node* y = nullptr;  // the value that will be removed

    if (node->left == nullptr) { //theres no left child so it replaces it with right child
        x = node->right;
        xParent = node->parent;

        if (x != nullptr) {
            x->parent = node->parent;
        }

        if (node->parent == nullptr) {
            root = x;  
        } else if (node == node->parent->left) {
            node->parent->left = x;  // changes left pointer
        } else {
            node->parent->right = x; 
        }

        y = node;  
    } else if (node->right == nullptr) { //no right child so left child replaces
        x = node->left;
        xParent = node->parent;

        if (x != nullptr) {
            x->parent = node->parent;
        }

        if (node->parent == nullptr) {
            root = x;  
        } else if (node == node->parent->left) { //same idea as other case
            node->parent->left = x;  
        } else {
            node->parent->right = x; 
        }

        y = node;  
    } else { //when the node has both childern so we use successor
        Node* successor = getSuccessor(node);
        firstColor = successor->color;  
        x = successor->right;  

        if (successor->parent == node) { //succersor is child of the node we delete
            if (x != nullptr) {
                x->parent = successor;
            }
            xParent = successor;
        } else { 
            if (x != nullptr) {
                x->parent = successor->parent;
            }
            successor->parent->left = x;  // replace successor with  right child

            successor->right = node->right;
            if (successor->right != nullptr) {
                successor->right->parent = successor;
            }
        }

        if (node->parent == nullptr) { //replaces with succser in tree
            root = successor;  
        } else if (node == node->parent->left) {
            node->parent->left = successor;
        } else {
            node->parent->right = successor;
        }
        
        successor->parent = node->parent;
        successor->left = node->left;
        if (successor->left != nullptr) {
            successor->left->parent = successor;
        }

        successor->color = node->color; //copies og node color to the succesor

        y = node;  // Node to delete is original node
    }

    delete y;  

   
    if (firstColor == false) { //if node was black use fixRemove
        fixRemove(root, x, xParent);
    }

    return root;  
}


Node* search(Node* root, int value) { //searchs for a value in binary search tree
    if (root == nullptr || root->data == value) {
        return root;
    }

    if (value > root->data) {
        return search(root->right, value); //uses recurtyion to seatch the right if value we are searching for is larger
    }

    return search(root->left, value); //uses recurtyion to seatch the left if value we are searching for is smaller
}

void printTree(Node* root, int depth = 0) { //prints tree similar to heap
    if (root == nullptr){
       return; 
    } 

    printTree(root->right, depth + 1); //uses recurtion to print right sideof a tree
    for (int i = 0; i < depth; i++) {
        cout << "   ";
    }
    cout << root->data << " ("; //states the color a node is in in the program
        if (root->color == true) {
            cout << "R";
        } else {
            cout << "B";
        }
        if (root->parent != nullptr) {
        cout << " P:" << root->parent->data; //then states that nodes parents
    }
    
    cout << ")" << endl;
    
     
    printTree(root->left, depth + 1);  //uses recurtion to print left sideof a tree
}

void filess(Node*& root) {
    ifstream file("numbers.txt");
    int value;
    
    while (file >> value) {
        if (value >= 1 && value <= 999) {
            root = insert(root, value);
        }
    }
    file.close();
}

int main() {
    Node* root = nullptr;
    int choice;

    cout << "Choose:" << endl;
    cout << "1 for file input" << endl;
    cout << "2 for manual input" << endl;
    cin >> choice;

    if (choice == 1) { //if 1 is chosen then it goes to numbers.txt and prints those numbers
        filess(root);
    } else if (choice == 2) {  //if 2 is chosen then it lets u imput your own order of numbers
        cout << "Enter numbers and type done to finish it (only nums from 1-999 plz)" << endl;
        while (true) {
            char input[10];
            cin >> input;
            if (strcmp(input, "done") == 0){
               break; 
            } 
            int value = atoi(input);
            if (value >= 1 && value <= 999) {
                root = insert(root, value);
            }
        }
    }
    printTree(root);
    while (true) {
        cout << " " << endl;
        
        cout << " " << endl; 
        
        //prompts to seach insert remove and quit
        cout << "Type I to insert a number" << endl;
        cout << "Type R to remove a number" << endl;
        cout << "Type S to search for a number" << endl;
        cout << "Type Q to quit" << endl;

        int value;
        char input[10];
            cin >> input;
            
        if (strcmp(input, "I") == 0) { //if I is types it ask for a value to insert
            cout << "Enter number to insert ";
            cin >> value;
            if (value >= 1 && value <= 999) {
                root = insert(root, value);
                printTree(root);
            }
        } else if (strcmp(input, "R") == 0) { //if R is types it ask for a value to remove
            cout << "Enter number to remove: ";
            cin >> value;
            root = remove(root, value);
            printTree(root);
        } else if (strcmp(input, "S") == 0) { //if S is types it ask for a value to Search
            cout << "Enter number to search for ";
            cin >> value;
            
            Node* result = search(root, value);
            if (result != nullptr) {
                printTree(root);
                cout << value << " is in the tree" << endl;
            } else {
                printTree(root);
                cout << value << " is not in the tree" << endl;
            } 
        } else {
            break; //anything else it breaks
        }
    }

    return 0;
}
