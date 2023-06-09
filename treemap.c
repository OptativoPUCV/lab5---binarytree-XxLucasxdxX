#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * map = (TreeMap*)malloc(sizeof(TreeMap));
  map->root=NULL;
  map->current=NULL;
  map->lower_than=lower_than;
  return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode * padre=NULL;
  TreeNode * actual=tree->root;
  while(actual!=NULL)
    {
      if(tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==0){
        return;
      } else if (tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==1){
        padre=actual;
        actual=actual->left;
      } else{
        padre=actual;
        actual=actual->right;
      }
    }
  TreeNode * nuevoNodo = createTreeNode(key, value);
  nuevoNodo->parent = padre;
  if(tree->lower_than(padre->pair->key, key) == 1) {
        padre->right = nuevoNodo;
    } else {
        padre->left = nuevoNodo;
    }
    tree->current=nuevoNodo;
}

TreeNode * minimum(TreeNode * x){
  while(x->left!=NULL){
    x=x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode * padre=node->parent;
  if(node->left == NULL && node->right==NULL){
    if(padre->left==node){
        padre->left=NULL;
    } else {
        padre->right=NULL;
    }
  } else if(node->left==NULL || node->right==NULL){
    TreeNode * hijo;
    if(node->left!=NULL){
      hijo=node->left;
    } else {
      hijo=node->right;
    }
    if(padre->right==node){
      padre->right=hijo;
    } else {
      padre->left=hijo;
    }
    hijo->parent=padre;
  } else{
  if(node->left!=NULL || node->right!=NULL){
    TreeNode * minimo=minimum(node->right);
    node->pair->key=minimo->pair->key;
    node->pair->value=minimo->pair->value;
    removeNode(tree,minimo);
   }
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode * actual= tree->root;
  while(actual!=NULL)
    {
      
      if(tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==0){
        tree->current=actual;
        return actual->pair;
      } else if (tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==1){
        actual=actual->left;
      }
      if((tree->lower_than(actual->pair->key, key)==1 && tree->lower_than(key, actual->pair->key)==0)){
        actual=actual->right;
      }
    }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
  TreeNode *actual= tree->root;
  TreeNode *ub_node= NULL;

  while(actual != NULL){
    if(tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==0){
      return actual->pair;
    } else if(tree->lower_than(actual->pair->key, key)==0 && tree->lower_than(key, actual->pair->key)==1){
      ub_node=actual;
      actual=actual->left;
    } else {
      actual=actual->right;
    }
  }
  if(ub_node!=NULL){
    return ub_node->pair;
  } else {
    return NULL;
  }
}

Pair * firstTreeMap(TreeMap * tree) {
    return minimum(tree->root)->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux = tree->current;
  if(aux->right !=NULL){
  aux = minimum(aux->right);
  } else {
    TreeNode * parent = aux->parent;
        while (parent != NULL && aux == parent->right) {
            aux = parent;
            parent = parent->parent;
        }
        aux = parent;
    }
    if (aux == NULL) return NULL;
    tree->current = aux;
    return aux->pair;
}
