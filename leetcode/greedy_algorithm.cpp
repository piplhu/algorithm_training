#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int test605(vector<int> data) {
  int res = 0;
  for (int i = 0; i < data.size();) {
    if (data[i] == 1)
      i += 2;
    else if (i == data.size() - 1 || data[i + 1] == 0) {//原数组中不存在相邻种花的
      i += 2;
      res++;
    } else
      i += 3;
  }

  return res;
}

int test452(vector<vector<int>> &point) {
  if (point.empty())
    return 0;
  int res = 1;
  std::sort(point.begin(), point.end(), [](vector<int> &a, vector<int> &b) {
    if (a[0] != b[0])
      return a[0] < b[0];
    else
      return a[1] < b[1];
  });
  vector<int> tmp({point[0][0], point[0][1]});
  for (int i = 1; i < point.size(); i++) {
    vector<int> item = point[i];
    if (tmp[1] < item[0]) {
      res++;
      tmp[0] = item[0];
      tmp[1] = item[1];
    } else {
      tmp[0] = std::max(item[0], tmp[0]);
      tmp[1] = std::min(item[1], tmp[1]);
    }
  }
  return res;
}

vector<int> test763(string s) {
  std::vector<int> res;
  int map[26] = {-1};
  for (int i = 0; i < s.size(); i++) {
    map[s[i] - 'a'] = i;
  }
  int charMaxPosition = 0;
  int start = 0;

  for(int j =0;j<s.size();j++){
    int maxPosition = map[s[j] - 'a'];
    if (maxPosition > charMaxPosition)
      charMaxPosition = maxPosition;
    if (j == charMaxPosition){
      res.emplace_back(maxPosition - start + 1);
      start = maxPosition + 1;
    }
  }

  return res;
}

int test122(vector<int>& prices){
  if (prices.size() < 2)
    return 0;
  int res = 0;
  for (int i = 1; i < prices.size(); i++) {
    res += std::max(prices[i] - prices[i - 1], 0);
  }
  return res;
}

vector<vector<int>> test406(vector<vector<int>>& people){
  std::vector<vector<int>> res;
  std::sort(people.begin(), people.end(),
            [](const vector<int> &a, const vector<int> &b) {
              if (a[0] == b[0])
                return a[1] < b[1];
              else
                return a[0] > b[0];
            });
  res.reserve(people.size());
  for(auto item :people){
      res.insert(res.begin()+item[1],item);
  }
  return res;
}

bool test665(vector<int> &nums) {
  int num = 0;
  for (int i = 1; i < nums.size(); i++) {
      if (nums[i - 1] > nums[i]){
        if (i == 1 || nums[i] >= nums[i - 2])
          nums[i - 1] = nums[i];
        else
          nums[i] = nums[i - 1];
        num++;
      }
    if (num > 1)
      break;
  }
  return num < 2;
}

int main(int argc, char *argv[]) {
  vector<int> data({0, 1, 1, 0});
  std::cout << test605(data) << endl;
  vector<vector<int>> point({{3, 9},
                             {7, 12},
                             {3, 8},
                             {6, 8},
                             {9, 10},
                             {2, 9},
                             {0, 9},
                             {3, 9},
                             {0, 6},
                             {2, 8}});
  std::cout << test452(point) << endl;
  string s = "ababcbacadefegdehijhklij";
  vector<int> res = test763(s);
  cout << "[";
  for (int i : res)
    cout << i << ",";
  cout << "]" << endl;

  vector<int> prices({7, 1, 5, 3, 6, 4});
  cout << test122(prices)<<endl;

  vector<vector<int>> people({{7, 0}, {4, 4}, {7, 1}, {5, 0}, {6, 1}, {5, 2}});
  people =  test406(people);

  vector<int> nums({1,4,1,2});
  cout << (test665(nums) ? "true" :"flase") <<endl;
  return 0;
}
