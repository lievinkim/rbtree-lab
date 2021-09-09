#include "rbtree.h"
#include <malloc.h>

// 새로운 생성 함수
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void inorder(rbtree  *t, node_t *n);
void for_array(node_t *n, key_t *arr, key_t end_idx, int *i);

node_t *sibling(node_t *n);
void replace_node(node_t* n, node_t* child);
void delete_one_child(rbtree* t, node_t* n);
void delete_case1(rbtree* t, node_t *n);
void delete_case2(rbtree* t, node_t *n);
void delete_case3(rbtree* t, node_t *n);
void delete_case4(rbtree* t, node_t *n);
void delete_case5(rbtree* t, node_t *n);
void delete_case6(rbtree* t, node_t *n);

// 기존 생성 함수
rbtree *new_rbtree(void);
node_t *rbtree_insert(rbtree *t, const key_t key);
node_t *rbtree_find(const rbtree *t, const key_t key);
node_t *rbtree_min(const rbtree *t);
node_t *rbtree_max(const rbtree *t);
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n);
int rbtree_erase(rbtree *t, node_t *p);
void delete_rbtree(rbtree *t);

// 메인 함수
int main(void) {

  rbtree *t = new_rbtree();

  rbtree_insert(t, 10);
  rbtree_insert(t, 20);
  rbtree_insert(t, 30);
  rbtree_insert(t, 100);
  rbtree_insert(t, 90);

  inorder(t, t->root);

  key_t *res = (key_t *)calloc(5, sizeof(key_t));
  rbtree_to_array(t, res, 5);

  for (int i=0; i<5; i++) {
    printf("%d ", res[i]);
  }

  // insert_arr(t, arr, n);
  // qsort((void *)arr, n, sizeof(key_t), comp);

  // key_t *res = calloc(n, sizeof(key_t));
  // rbtree_to_array(t, res, n);
  // for (int i = 0; i < n; i++) {
  //   assert(arr[i] == res[i]);
  // }

  return 0;
}


// 새로운 생성 함수 구현
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != NULL) {
    y->left->parent = x;
  }

  y->parent = x->parent;
  if (x->parent == NULL) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != NULL) {
    y->right->parent = x;
  }

  y->parent = x->parent;
  if (x->parent == NULL) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  }

  y->right = x;
  x->parent = y;
}

void inorder(rbtree  *t, node_t *n) {
  if (n != NULL) {
    inorder(t, n->left);
    printf("NODE[%d] : %d\n", n->key, n->color);
    inorder(t, n->right);
  }
}

node_t *sibling(node_t *n) {
  if (n==n->parent->left){
    return n->parent->right;
  }
  else {
    return n->parent->left;
  }
}

void replace_node(node_t* n, node_t* child) {
  child->parent=n->parent;

  if (n->parent->left ==n) {
    n->parent->left=child;
  }
  else if (n->parent->right ==n) {
    n->parent->right=child;
  }
}

void delete_one_child(rbtree* t, node_t* n) {

  // 선제조건 n이 최대 하나의 non-null자식을 갖고 있음
  if(t->root==n) {
      t->root=NULL;
      free(n);
      return;
    }

  // 자식이 없는 경우,
  if (n->left==NULL && n->right==NULL) { 

    //RED일 경우, 바로 free
    //BLACK일 경우, 삭제 선 진행
    if (n->color==RBTREE_BLACK) {  
      delete_case1(t,n);
    }

    if (n==n->parent->left){
      n->parent->left=NULL;
    }
    else if(n==n->parent->right){
      n->parent->right=NULL;
    }

    free(n); 
    return;
  }

  // 자식이 하나인 경우
  node_t* child;  
  if (n->left==NULL){  
    child = n->right;
  }
  else {
    child = n->left;
  }
  
  replace_node(n,child);
  if (n->color==RBTREE_BLACK) {
    if(child->color==RBTREE_RED) {
      child->color=RBTREE_BLACK;
    }
    free(n);
  }
}

void delete_case1(rbtree *t, node_t *n) {
  if (n->parent != NULL) {
    delete_case2(t,n);
  }
   
}

void delete_case2(rbtree *t, node_t *n) {
  node_t *s = sibling(n);
  if (s->color == RBTREE_RED) {
    n->parent->color = RBTREE_RED;
    s->color =RBTREE_BLACK;
    if (n == n->parent->left) {
      left_rotate(t,n->parent);
    }
    else {
      right_rotate(t,n->parent);
    }
  }
  delete_case3(t, n);
}

void delete_case3(rbtree *t, node_t *n) {
  node_t *s = sibling(n);
  if ((n->parent->color ==RBTREE_BLACK) 
    && (s->color ==RBTREE_BLACK) 
    && (s->left==NULL || s->left->color ==RBTREE_BLACK) 
    && (s->right==NULL || s->right->color ==RBTREE_BLACK)) {
    s->color = RBTREE_RED;
    delete_case1(t,n->parent);
  } else {
    delete_case4(t,n);
  }
}

void delete_case4(rbtree *t, node_t *n) {
  node_t *s = sibling(n);
  if ((n->parent->color == RBTREE_RED) &&
    (s->color ==RBTREE_BLACK) &&
    (s->left==NULL || s->left->color ==RBTREE_BLACK) &&
    (s->right==NULL || s->right->color ==RBTREE_BLACK)) {
    s->color = RBTREE_RED;
    n->parent->color =RBTREE_BLACK;
  } else
    delete_case5(t,n);
}

void delete_case5(rbtree *t, node_t *n)
{
  node_t *s = sibling(n);

  if  (s->color ==RBTREE_BLACK) {
    /* 이 문은 자명하다,
        case 2로 인해서(case 2에서 '''N'''의 형제 노드를 원래 형제 노드 '''S'''의 자식노드로 바꾸지만,
        빨강 부모노드는 빨강 자식 노드를 가질 수 없기 때문에 '''N'''의 새로운 형제노드는 빨강일 수 없다). */
    /* 다음의 문은 빨강을 '''N'''의 부모노드의 오른쪽 자식의 오른쪽 자식으로 두기 위함이다.
        혹은 '''N'''의 부모노드의 왼쪽 자식의 왼쪽 자식으로 두기 위함. case 6에 넘기기 위해 */
    if ((n == n->parent->left) &&
      (s->right==NULL || s->right->color ==RBTREE_BLACK) &&
      (s->left!=NULL && s->left->color == RBTREE_RED)) { /* this last test is trivial too due to cases 2-4. */
      s->color = RBTREE_RED;
      s->left->color =RBTREE_BLACK;
      right_rotate(t,s);
    }
    else if ((n == n->parent->right) &&
      (s->left==NULL || s->left->color ==RBTREE_BLACK) &&
      (s->right!=NULL && s->right->color == RBTREE_RED)) {/* this last test is trivial too due to cases 2-4. */
      s->color = RBTREE_RED;
      s->right->color =RBTREE_BLACK;
      left_rotate(t,s);
    }
  }
  delete_case6(t,n);
}

void delete_case6(rbtree* t, node_t *n)
{
  node_t *s = sibling(n);

  s->color = n->parent->color;
  n->parent->color =RBTREE_BLACK;

  if (n == n->parent->left) {
    if (s->right!=NULL){
      s->right->color =RBTREE_BLACK;}
      left_rotate(t,n->parent);
  }
  else {
      if (s->left!=NULL) {
      s->left->color =RBTREE_BLACK;}
      right_rotate(t,n->parent);
  }
}




// 기존 생성 함수 구현
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  p->root = NULL;
  return p;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  // 노드 생성
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  z->parent = NULL;
  z->left = NULL;
  z->right = NULL;
  z->color = RBTREE_RED;

  // 트리 루트가 NULL인 경우
  if (t->root == NULL) {
    z->color = RBTREE_BLACK;
    t->root = z;
    return t->root;
  }

  // 삽입할 위치 찾아보기
  node_t *tmp_parent = t->root;
  node_t *compared_node = NULL;

  while (tmp_parent != NULL) {
    compared_node = tmp_parent;
    if (z->key < tmp_parent->key) {
      tmp_parent = tmp_parent->left;
    } else {
      tmp_parent = tmp_parent->right;
    }
  }

  z->parent = compared_node;
  if (z->key < compared_node->key) {
    compared_node->left = z;
  } else if (z->key > compared_node->key) {
    compared_node->right = z;
  }

  // R-R Violation 해결 부분
  while (z->parent != NULL && z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *u = z->parent->parent->right;

      if (u != NULL && u->color == RBTREE_RED) {
        z->parent->parent->color = RBTREE_RED;
        z->parent->color = RBTREE_BLACK;
        u->color = RBTREE_BLACK;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }

    } else if (z->parent == z->parent->parent->right) {
      node_t *u = z->parent->parent->left;

      if (u != NULL && u->color == RBTREE_RED) {
        z->parent->parent->color = RBTREE_RED;
        z->parent->color = RBTREE_BLACK;
        u->color = RBTREE_BLACK;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  
  node_t *current = t->root;

  if (current == NULL) {
    return NULL;
  } 

  while (1) {

    if (key < current->key) {
      if (current->left != NULL) {
        current = current->left;
      }
      else {
        return NULL;
      }
    }
    else if (key > current->key){
      if (current->right != NULL) {
        current = current->right;
      } 
      else {
        return NULL;
      }
    }
    else {
      return current;
    }
  }

}

node_t *rbtree_min(const rbtree *t) {

  node_t *current = t->root;

  while (current->left != NULL) {
    current = current->left;
  }

  return current;
}

node_t *rbtree_max(const rbtree *t) {

  node_t *current = t->root;
  while (current->right != NULL) {
    current = current->right;
  }

  return current;
}

void for_array(node_t *n, key_t *arr, key_t end_idx, int *i) {

    if(n!=NULL) {
      for_array(n->left, arr, end_idx, i);
      if (*i<end_idx) {
        arr[*i]=n->key;
        (*i)++;
      }
      else {
        return;
      }
      for_array(n->right, arr, end_idx, i);
    }

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

  node_t *current = t->root;
  int idx = 0;

  if (current == NULL) {
    return 0;
  }

  for_array(current, arr, n, &idx);

  return 0;
}



int rbtree_erase(rbtree *t, node_t *p) {
  // p가 없거나, 트리가 없으면 종료.
  if (t->root==NULL || p==NULL) {
    return 0;
  } 

  if(p->left!=NULL && p->right!=NULL) {
    // successor 대상 찾기 (키 값 기준 오른쪽에서 가장 작은 값.)
    node_t* temp= p->left;
    // successor의 부모노드.
    node_t* parent =NULL;       
    while(temp->right!=NULL) {
      parent=temp;
      temp=temp->right;
    }

  if (parent!=NULL){  
    if (temp->left!=NULL){
      parent->right = temp->left;
    }
  }
  // succesor의 부모노드가 삭제 대상인 p인 경우
  // p 바로 오른쪽 노드가 successor이고 successor는 왼쪽자식이 없는 상황
  else { 
    if (temp->left!=NULL) { 
      p->left=temp->left;}
    }
    p->key=temp->key;
    delete_one_child(t,temp);
  }
  else {
    delete_one_child(t,p);
  }
  return 0;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}














