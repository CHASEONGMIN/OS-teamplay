/*

*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32154579. 32153682
*	    Student name : 차성민, 이창민
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "lab2_sync_types.h"

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock;
//int rc = pthread_mutex_init(&lock, NULL);

//pthread_mutex_t lock;
//int rc = pthread_mutex_init(&lock, NULL);
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
//inorder로 출력=================
int inorder(lab2_node* node){
    if(node == NULL){
        return LAB2_ERROR;
    }
    inorder(node->left);
    printf("%d ", node->key);
    inorder(node->right);
    return LAB2_SUCCESS;

}
//==============================
int lab2_node_print_inorder(lab2_tree *tree) {
    //트리가 비어있으면
    if(tree->root == NULL){
        printf("Tree is Empty!\n");
        return LAB2_ERROR ;
    }
    //출력, 재귀함수로 구현
    return inorder(tree->root);
}


/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {    
    lab2_tree* tree = (lab2_tree*)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {    
    lab2_node* node = (lab2_node*)malloc(sizeof(lab2_node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    if(tree->root == NULL){
        tree->root = new_node;
        return LAB2_SUCCESS;
    }

    lab2_node* node = tree->root;
    //NULL일때까지
    while(node){

        //작으면 왼쪽
        if(new_node->key < node->key){
            node = node->left;
        }
        //크면 오른쪽
        else if(new_node->key > node->key){
            node = node->right;
        }
        //같으면 에러
        else{
            return LAB2_ERROR;
        }
    }
    //끝까지 내려가면 그 위치에 삽입
    node = new_node;

}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){	
       	
	pthread_mutex_lock(&lock);
	lab2_node* node = tree->root;
	if(tree->root == NULL){
        tree->root = new_node;
	pthread_mutex_unlock(&lock);
        return LAB2_SUCCESS;
    }

    pthread_mutex_unlock(&lock);
    
    //NULL일때까지
    while(node){

        //작으면 왼쪽
        if(new_node->key < node->key){
	    pthread_mutex_lock(&lock);
            node = node->left;
	    pthread_mutex_unlock(&lock);
        }
        //크면 오른쪽
        else if(new_node->key > node->key){
	    pthread_mutex_lock(&lock);
            node = node->right;
	    pthread_mutex_unlock(&lock);
        }
        //같으면 에러
        else{
            return LAB2_ERROR;
        }
    }
    pthread_mutex_lock(&lock);
    //끝까지 내려가면 그 위치에 삽입
    node = new_node; 
    pthread_mutex_unlock(&lock);
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
        pthread_mutex_lock(&lock); 
	lab2_node* node = tree->root;
	if(tree->root == NULL){
        tree->root = new_node;
	pthread_mutex_unlock(&lock);
        return LAB2_SUCCESS;
    }


    //lab2_node* node = tree->root;
    //NULL일때까지
    while(node){

        //작으면 왼쪽
        if(new_node->key < node->key){
            node = node->left;
        }
        //크면 오른쪽
        else if(new_node->key > node->key){
            node = node->right;
        }
        //같으면 에러
        else{
	    pthread_mutex_unlock(&lock);
            return LAB2_ERROR;
        }
    }

    //끝까지 내려가면 그 위치에 삽입
    node = new_node;
    pthread_mutex_unlock(&lock);
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
//===============================================
int leftSubtreeMax(lab2_node* node)
{
    int key;
    lab2_node* temp = node->left;

    while (temp->right != NULL){
        temp = temp->right;
    }
    //값만 빼내고
    key = temp->key;

    //말단 노드 삭제
    lab2_node_delete(temp);
    return key;
}
int rightSubtreeMin(lab2_node* node)
{
    int key;
    lab2_node* temp = node->right;
    while (temp->left != NULL){
        temp = temp->left;
    }
    key = temp->key;

    //말단 노드 삭제
    lab2_node_delete(temp);
    return key;
} 
//================================================
int lab2_node_remove(lab2_tree *tree, int key) {
   
    lab2_node* node = tree->root;
    if(tree->root == NULL){
        return LAB2_ERROR;
    }

    //lab2_node* node = tree->root;
    //lab2_node* node_parent;
    //값을 찾음
    while(1){
        if(node->key < key && node->left != NULL){
            //node_parent = node;
            node = node->left;
        }
        else if(node->key > key && node->right != NULL){
           //node_parent = node;
            node = node->right;
        }
        //찾음
        else if(node->key == key){
            break;
        }
        //찾는 값이 없음
        else{
            return LAB2_ERROR;
        }
    }

    //left, right 존재x=>그냥 삭제
    if(!(node->left) && !(node->right)){
        lab2_node_delete(node);
    }
    //left존재 => 가장 큰 값찾고 올림
    else if(node->left && !(node->right)){
        node->key = leftSubtreeMax(node);

    }
    //right 존재 => 가장 작은 값찾고 올림
    else if(!(node->left) && node->right){
        node->key = rightSubtreeMin(node);
    }
    //둘다 존재 => 왼쪽 subtree 작은값 or 오른쪽 subtree 큰값
    else if(node->left && node->right){
        node->key = leftSubtreeMax(node);
    }

    return LAB2_SUCCESS;

}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
   
    pthread_mutex_lock(&lock);
    lab2_node* node = tree->root;
    if(tree->root == NULL){
        pthread_mutex_unlock(&lock);
        return LAB2_ERROR;
    }

    //lab2_node* node = tree->root;
    //lab2_node* node_parent;
    //값을 찾음
    while(1){
        if(node->key < key && node->left != NULL){
            //node_parent = node;
            node = node->left;
	    pthread_mutex_unlock(&lock);
        }
        else if(node->key > key && node->right != NULL){
           //node_parent = node;
            node = node->right;
		   pthread_mutex_lock(&lock);
        }
        //찾음
        else if(node->key == key){
            break;
        }
        //찾는 값이 없음
        else{
		pthread_mutex_unlock(&lock);
            return LAB2_ERROR;
        }
    }

    //left, right 존재x=>그냥 삭제
    if(!(node->left) && !(node->right)){
        lab2_node_delete(node);
    }
    //left존재 => 가장 큰 값찾고 올림
    else if(node->left && !(node->right)){
        node->key = leftSubtreeMax(node);

    }
    //right 존재 => 가장 작은 값찾고 올림
    else if(!(node->left) && node->right){
        node->key = rightSubtreeMin(node);
    }
    //둘다 존재 => 왼쪽 subtree 작은값 or 오른쪽 subtree 큰값
    else if(node->left && node->right){
        node->key = leftSubtreeMax(node);
    }
     pthread_mutex_unlock(&lock);
    return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
      pthread_mutex_lock(&lock);  
	lab2_node* node = tree->root;
	if(tree->root == NULL){
		pthread_mutex_unlock(&lock);
        return LAB2_ERROR;
    }

    //lab2_node* node = tree->root;
    //lab2_node* node_parent;
    //값을 찾음
    while(1){
        if(node->key < key && node->left != NULL){
            //node_parent = node;
            node = node->left;
        }
        else if(node->key > key && node->right != NULL){
           //node_parent = node;
            node = node->right;
        }
        //찾음
        else if(node->key == key){
            break;
        }
        //찾는 값이 없음
        else{
		pthread_mutex_unlock(&lock);
            return LAB2_ERROR;
        }
    }

    //left, right 존재x=>그냥 삭제
    if(!(node->left) && !(node->right)){
        lab2_node_delete(node);
    }
    //left존재 => 가장 큰 값찾고 올림
    else if(node->left && !(node->right)){
        node->key = leftSubtreeMax(node);

    }
    //right 존재 => 가장 작은 값찾고 올림
    else if(!(node->left) && node->right){
        node->key = rightSubtreeMin(node);
    }
    //둘다 존재 => 왼쪽 subtree 작은값 or 오른쪽 subtree 큰값
    else if(node->left && node->right){
        node->key = leftSubtreeMax(node);
    }
	pthread_mutex_unlock(&lock);
    return LAB2_SUCCESS;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
    free(tree);
}
/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
    node = NULL;
    free(node);
}

