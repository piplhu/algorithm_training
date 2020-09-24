#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

void DFS(vector<string> &strPath, string subPath, TreeNode *node) {
  if (node == NULL)
    return;
  subPath += std::to_string(node->val);
  if (node->left == NULL && node->right == NULL) { //遍历到最后的一个节点
    strPath.push_back(subPath);
  } else {
    subPath += "->";
    DFS(strPath, subPath, node->left);
    DFS(strPath, subPath, node->right);
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
vector<string> binaryTreePaths(TreeNode *root) {
  vector<string> strPath;
  DFS(strPath, string(), root);
  return strPath;
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
  TreeNode *newNode = new TreeNode(val);
  (tmp->val > val) ? (tmp->left = newNode) : (tmp->right = newNode);
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
    cout << node->val << "->";
  Print(node->right);
}

/**
 * @brief 构建num个随机分布在0-max内的二叉树
 *
 * @param root
 * @param num
 * @param max
 */
void CreateBinaryTree(TreeNode *root, int num, int max = 100) {
  for (int i = 0; i < num; i++) {
    int val = rand() % max;
    BinaryTreeAddNode(root, val);
  }
}

int main() {

#if defined(SEARCH_ALL_PATH)
  //遍历BST所用从跟节点出发的路径
  TreeNode *root = new TreeNode(50);
  CreateBinaryTree(root, 20);
  Print(root);
  printf("\n");
  vector<string> path = binaryTreePaths(root);
  for (auto item : path)
    cout << item << endl;
#elif defined(FIND_MODE)

  //找出BST中出现频率最高的元素
  cout << "100" << endl;
#endif
  return 0;
}
