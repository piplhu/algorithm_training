#include <stdio.h>
#include <stdlib.h>

typedef struct _TreeNode {
  int val;
  struct _TreeNode *left;
  struct _TreeNode *right;
} TreeNode;

/**
 * @brief 获取二叉树的从根节点到叶子节点的路径
 *
 * @param root
 * @param returnSize 路径数量
 *
 * @return 1->4->5字符串路径数组
 */
char **binaryTreePaths(TreeNode *root, int *returnSize) {}

TreeNode *CreateNode(int val) {
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
  node->val = val;
  node->left = NULL;
  node->right = NULL;
  return node;
}

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

void Print(TreeNode *root) {

  TreeNode *node = root;
  if (node != NULL)
    printf("%d ->", node->val);
  else
    return;
  Print(node->left);
  Print(node->right);
}

void CreateBinaryTree(TreeNode *root) {
  for (int i = 0; i < 20; i++) {
    int val = rand() % 100;
    BinaryTreeAddNode(root, val);
  }
}

int main() {
  TreeNode *root = CreateNode(50);
  CreateBinaryTree(root);
  Print(root);
  return 0;
}
