#include <stdio.h>
#include <stdlib.h>

typedef struct _TreeNode {
  int val;
  struct _TreeNode *left;
  struct _TreeNode *right;
} TreeNode;

void DFS(char **strPath, int path[1000], int depth, int *size, TreeNode *node) {
  if (node == NULL)
    return;
  if (node->left == NULL && node->right == NULL) { //遍历到最后的一个节点
    char *subPath = malloc(1000);
    int len = 0;
    for (int i = 0; i < depth; i++) {
      len += sprintf(subPath + len, "%d->", path[i]);
    }
    sprintf(subPath + len, "%d", node->val);
    strPath[(*size)++] = subPath;
  } else {
    path[depth++] = node->val;
    DFS(strPath, path, depth, size, node->left);
    DFS(strPath, path, depth, size, node->right);
  }
}

/**
 * @brief 获取二叉树的从根节点到叶子节点的路径
 *
 * @param root
 * @param returnSize 路径数量
 *
 * @return 1->4->5字符串路径数组
 */
char **binaryTreePaths(TreeNode *root, int *returnSize) {
  char **strPath = malloc(1000 * sizeof(char *));
  int path[1000] = {0};
  int depth = 0;
  *returnSize = 0;
  DFS(strPath, path, depth, returnSize, root);
  return strPath;
}

/**
 * @brief 构建一个二叉树节点
 *
 * @param val
 *
 * @return 
 */
TreeNode *CreateNode(int val) {
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
  node->val = val;
  node->left = NULL;
  node->right = NULL;
  return node;
}

/**
 * @brief 往二叉树添加节点
 *
 * @param root
 * @param val
 */
void BinaryTreeAddNode(TreeNode *root, int val) {
  if (root == NULL)
    return;
  TreeNode *node = root;
  TreeNode *tmp = root;
  while (node != NULL) {
    tmp = node;
    if (val < node->val)
      node = node->left;
    else
      node = node->right;
  }
  TreeNode *new = CreateNode(val);
  (tmp->val > val) ? (tmp->left = new) : (tmp->right = new);
}

/**
 * @brief 中序遍历
 *
 * @param root
 */
void Print(TreeNode *root) {

  if (root == NULL)
    return;
  TreeNode *node = root;
  Print(node->left);
  if (node != NULL)
    printf("%d ->", node->val);
  Print(node->right);
}

/**
 * @brief 构建随机二叉树
 *
 * @param root
 * @param num：节点个数
 */
void CreateBinaryTree(TreeNode *root , int num) {
  for (int i = 0; i < num; i++) {
    int val = rand() % 100;
    BinaryTreeAddNode(root, val);
  }
}

int main() {
  TreeNode *root = CreateNode(50);
  CreateBinaryTree(root, 20);
  Print(root);
  printf("\n");
  int size = 0;
  char **path = binaryTreePaths(root, &size);
  for (int i = 0; i < size; i++) {
    printf(path[i]);
    printf("\n");
    free(path[i]);
  }
  free(path);
  return 0;
}
