#include <cstdlib>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

int qSortFunc(vector<int> &nums, int left, int right) {
  //哨兵随机化
  int pivot = rand() % (right - left) + left;
  swap(nums[pivot], nums[left]);
  pivot = nums[left];
  while (left < right) {
    while (left < right && nums[right] >= pivot)
      right--;
    nums[left] = nums[right];
    while (left < right && nums[left] < pivot)
      left++;
    nums[right] = nums[left];
  }
  nums[left] = pivot;
  return left;
}

int test215(vector<int> &nums, int k) {
  int res = nums.size() - k, l = 0, r = nums.size() - 1;
  while (l < r) {
    int mid = qSortFunc(nums, l, r);
    if (mid == res)
      return nums[mid];
    else if (mid < res)
      l = mid + 1;
    else
      r = mid - 1;
  }
  return nums[l];
}

vector<int> test347(vector<int> &nums, int k) {
  unordered_map<int, int> map;
  int maxCount = 0;
  for (int item : nums) {
    maxCount = max(maxCount, ++map[item]);
  }
  vector<vector<int>> bucket(maxCount + 1);
  for (auto d : map)
    bucket[d.second].push_back(d.first);
  vector<int> res;
  for (int i = maxCount; i >= 0 && res.size() < k; i--) {
    for (auto t : bucket[i]) {
      res.push_back(t);
      if (res.size() == k)
          break;
    }
  }
  return res;
}

string test451(string s) {
  unordered_map<char, int> map;
  int maxCount = 0;
  for (char c : s) {
    maxCount = max(maxCount, ++map[c]);
  }
  vector<vector<char>> bucket(maxCount + 1);
  for (auto d : map)
    bucket[d.second].push_back(d.first);
  string res;
  for (int i = maxCount; i >= 0; i--) {
    for (auto t : bucket[i]) {
      res.append(i, t);
    }
  }
  return res;
}

void test75(vector<int> &nums) {
  int index0 = 0, index1 = 0;
  for (int& item : nums) {
    int tmp = item;
    item = 2;
    if (tmp < 2)
      nums[index1++] = 1;
    if (tmp < 1)
      nums[index0++] = 0;
  }
}

std::ostream& operator<<(ostream& out,const vector<int>& data){
    for(int i : data){
        cout << i <<";";
    }
    cout << endl;
    return out;
}
int main(int argc, char *argv[]) {
  vector<int> nums({3, 2, 1, 5, 6, 4});
  cout << test215(nums, 4) << endl;
  vector<int> nums1({1,1,1,2,2,3});
  cout << test347(nums1, 2) << endl;
  string s("tree");
  cout << test451(s) << endl;
  vector<int> num2({2, 0, 2, 1, 1, 0});
  test75(num2);
  cout << num2 << endl;
  return 0;
}
