/******************************************************************************
 * File:             dfs.cpp
 *
 * Created:          06/29/21
 * Description:      leetcode深度优先搜索
 *****************************************************************************/

#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

const std::vector<int> g_direction = {-1, 0, -1, 0, 1}; //方向左上右下

int dfs(vector<vector<int>> &grid, int x, int y) {
  grid[y][x] = 0;
  int Count = 1;
  int maxY = grid.size();
  int maxX = grid[y].size();
  for (int tmp = 0; tmp < g_direction.size(); tmp++) {
    int tmpX = g_direction[tmp] + x;
    int tmpy = g_direction[tmp + 1] + y;
    if (tmpX >= 0 && tmpX < maxX && tmpy >= 0 && tmpy < maxY &&
        grid[tmpy][tmpX] == 1) {
      Count += dfs(grid, tmpX, tmpy);
    }
  }
  return Count;
}

int test_695(vector<vector<int>> &grid) {
  int y = grid.size();
  int maxArea = 0;
  for (int i = 0; i < y; i++) {
    int x = grid[i].size();
    for (int j = 0; j < x; j++) {
      if (grid[i][j] == 1) {
        maxArea = max(dfs(grid, j, i), maxArea);
      }
    }
  }
  return maxArea;
}


int main() {
  std::vector<vector<int>> grid = {{0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                                   {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0},
                                   {0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0}};
  std::cout << test_695(grid)<<std::endl;
  return 0;
}
