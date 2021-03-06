#include "tree.hpp"

#include <stack>
#include <cmath>

void tree_free(tree_node *root)
{
    if(root){
        tree_free(root->left);
        tree_free(root->right);
        delete root;
    }
}

int tree_size(const tree_node *node)
{
    if(!node){
        return 0;
    }else{
        return 1+tree_size(node->left) + tree_size(node->right);
    }
}

int tree_height(const tree_node *node) //height is distance to bottom-most node.
{
    if (node == nullptr) {
        return 0;
    }

    int l_height = tree_height(node->left) + 1;
    int r_height = tree_height(node->right) + 1;

    return max(l_height, r_height);
}

float tree_balance(const tree_node *node) //TODO:  b = ( h / h_opt - 1 ) / ( n / h_opt - 1 ),
{
    int n = tree_size(node);
    int h = tree_height(node);
    float h_opt = ceil(log2(n+1));

    float b = (h/h_opt-1)/(n/h_opt-1);

    return b;
}

bool tree_contains(const tree_node *root, const string &value)
{
    if(root==nullptr){
        return false;
    }
    if(value < root->value){
        return tree_contains(root->left, value);
    }else if(value > root->value){
        return tree_contains(root->right, value);
    }else{
        return true;
    }
}

tree_node *tree_insert(tree_node *root_node, const string &value)
{
    if(root_node==nullptr){
        tree_node *new_node=new tree_node;
        new_node->value=value;
        new_node->left=nullptr;
        new_node->right=nullptr;
        return new_node;
    }else if(value < root_node->value){
        root_node->left = tree_insert(root_node->left, value);
    }else if(value > root_node->value){
        root_node->right = tree_insert(root_node->right, value);
    }
    return root_node;
}

tree_node *tree_insert(tree_node *root_node, tree_node *new_node)
{
    if(root_node==nullptr){
        new_node->left=nullptr;
        new_node->right=nullptr;
        return new_node;
    }else if(new_node->value < root_node->value){
        root_node->left = tree_insert(root_node->left, new_node);
    }else if(new_node->value > root_node->value){
        root_node->right = tree_insert(root_node->right, new_node);
    }else{
        delete new_node; // Attempting to add node that already exists
    }
    return root_node;
}

tree_node *tree_insert(tree_node *root, const vector<tree_node*> &nodes)
{
    for(int i=0; i<nodes.size(); i++){
        root = tree_insert(root, nodes[i]);
    }
    return root;
}

void tree_collect_nodes(vector<tree_node*> &nodes, tree_node *root)
{
    // Note: this is **not** a useful template or basis for how to implement
    // tree_collect_nodes_in_order. It is deliberately written in an odd way.

    deque<tree_node*> todo;
    todo.push_back(root);

    while(!todo.empty()){
        tree_node *top=todo.back();
        todo.pop_back();
        if(!top){
            continue;
        }

        nodes.push_back(top);

        todo.push_back(top->left);
        todo.push_front(top->right);
    }
}

void tree_collect_nodes_in_order(vector<tree_node*> &nodes, tree_node *root)
{
    if(root != nullptr){
      tree_collect_nodes_in_order(nodes, root->left);
      nodes.push_back(root);
      tree_collect_nodes_in_order(nodes, root->right);
    }
}

bool are_nodes_ordered(const vector<tree_node*> &nodes)
{
    for(int i=0; i<nodes.size()-1; i++){
        if(nodes[i-1]->value > nodes[i]->value){
            return false;
        }
    }
    return true;
}

tree_node *tree_rebuild_random(tree_node *root, mt19937 &rng)
{
    vector<tree_node*> nodes;
    tree_collect_nodes(nodes, root);

    root=nullptr;
    while(!nodes.empty()){
        // Pick a random node and move to the end
        int uniform = rng();
        int index=uniform % nodes.size();
        swap(nodes[index], nodes[nodes.size()-1]);

        // Insert the last node in vector, then remove
        root = tree_insert(root, nodes.back());
        nodes.pop_back();
    }
    return root;
}

tree_node *tree_rebuild_balanced(vector<tree_node*> &nodes, int begin, int end)
{
    if(begin==end){
        return nullptr;
    }
    assert(begin<end);

    int mid=(begin+end)/2;
    assert(begin<=mid && mid<end);
    //cerr << "Middle index: " << mid << endl;

    tree_node *root=nodes[mid];
    root->left = tree_rebuild_balanced(nodes, begin, mid);
    root->right = tree_rebuild_balanced(nodes, mid+1, end);
    //already working???
}

tree_node *tree_rebuild_balanced(tree_node *root)
{
    vector<tree_node*> nodes;
    tree_collect_nodes_in_order(nodes, root);
    return tree_rebuild_balanced(nodes, 0, nodes.size());
}
