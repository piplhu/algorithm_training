/******************************************************************************
* File:             dichotomy.cpp
*
* Created:          03/03/21 
* Description:      二分法系列题解
* 如何避免空间缩到最小时候死循环? 左闭右开 左闭右闭 
*****************************************************************************/
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int test69(int x) {
  if (x == 0)
    return x;
  int l = 1, r = x, sqrt = 0, mid = 0;
  while (l <= r) {//
    mid = l + (r - l) / 2;//(l+r)/2会溢出
    sqrt = x / mid; //使用mid*mid 跟x比较会溢出
    if (sqrt == mid)
      return mid;
    else if (sqrt < mid) {
      r = mid - 1;
    } else {
      l = mid + 1;
    }
  }
  return r;
}

int LowerBound(const vector<int> &nums, int target) {
  int l = 0, r = nums.size(), mid = 0;
  while (l < r) {
    mid = l + (r - l) / 2;
    if (nums[mid] >= target) {
      r = mid;
    } else
      l = mid + 1;
  }
  return l;
}

int UpperBound(const vector<int> &nums, int target) {
  int l = 0, r = nums.size(), mid = 0;
  while (l < r) {
    mid = l + (r - l) / 2;
    if (nums[mid] > target)
      r = mid;
    else
      l = mid + 1;
  }
  return l;
}

vector<int> test34(vector<int> &nums, int target) {
  int low = LowerBound(nums, target);
  if (low == nums.size() || nums[low] != target)
    return vector<int>({-1, -1});
  int up = UpperBound(nums, target) - 1;//放在后面在left不符合条件时就不用再找right了
  return vector<int>({low, up});
}

bool test81(vector<int> &nums, int target) {
  int l = 0, r = nums.size() - 1, mid;
  while (l < r) {
    mid = l + (r - l +1) / 2;
    if (nums[mid] == nums[r]) {
      if (nums[r] == target)
        return true;
      r = r - 1;
    } else if (nums[mid] < nums[r]) { // mid,r右边是有序的
      if (nums[mid] <= target && target <= nums[r]) {
        l = mid;
      } else
        r = mid - 1;
    } else {//l,mid有序
      if (nums[l] <= target && target < nums[mid]) {
        r = mid - 1;
      } else
        l = mid;
    }
  }
  return nums[r] == target;
}

int test154(vector<int> &nums) {
  int res = nums.front(), l = 0, r = nums.size() - 1, mid;
  while (l < r) {
    mid = l + (r - l + 1) / 2;
    if (nums[mid] < nums[r]) { // mid,r 有序
      if (nums[mid] > res) {
        r = mid - 1;
      } else {
        res = nums[mid];
        r = mid - 1;
      }
    } else if (nums[mid] > nums[r]) {
      if (nums[mid] >= res && res < nums[r])
        return nums[l];
      else {
        res = nums[mid];
        l = mid;
      }
    } else {
      if (nums[r] <= res) {
        res = nums[r];
      }
      r--;
    }
  }
  return res;
}

int test540(vector<int> &nums) {
    int mid, l = 0, r = nums.size() - 1;
    while (l < r) {
      mid = l + (r - l) / 2;
      if (nums[mid] == nums[mid - 1]) {
        if (mid % 2 == 0) {
          r = mid;
        } else {
          l = mid + 1;
        }
      } else if (nums[mid] == nums[mid + 1]) {
        if (mid % 2 != 0) {
          r = mid - 1;
        } else {
          l = mid;
        }
      } else
        return nums[mid];
    }
    return nums[r];
}

double test4(const vector<int> &num1, const vector<int> &num2) {
  int m = num1.size();
  int n = num2.size();
  if (m > n) //确保 0<=j<=n
   return test4(num2, num1);
  int l = 0, r = m;
  while (l <= r) {
    int i = l + (r - l) / 2;
    int j = (m + n + 1) / 2 - i;
    if (j != 0 && i != m && num2[j - 1] > num1[i]) { // i前移动
      l = i + 1;
    } else if (i != 0 && j != n && num1[i - 1] > num2[j]) { // i后移
      r = i - 1;
    } else {
      int maxLeft = 0;
      if (i == 0) {
        maxLeft = num2[j - 1];
      } else if (j == 0) {
        maxLeft = num1[i - 1];
      } else
        maxLeft = std::max(num1[i - 1], num2[j - 1]);
      if ((m + n) % 2 != 0) //总数为奇数
        return maxLeft;

      int minRight = 0;
      if (i == m) {
        minRight = num2[j];
      } else if (j == n) {
        minRight = num1[i];
      } else
        minRight = std::min(num1[i], num2[j]);

      return (minRight + maxLeft) / 2.0;
    }
  }
  return 0.0;
}

std::ostream& operator<<(ostream& out,const vector<int>& data){
    for(int i : data){
        cout << i <<";";
    }
    cout << endl;
    return out;
}

int main(int argc, char *argv[]) {
  cout << test69(10) << endl;

  vector<int> nums({5, 7, 7, 8, 8, 10});
  cout << test34(nums, 7);

  vector<int> num({2, 5, 6, 0, 0, 1, 2});
  cout << (test81(num, 0) ? "true" : "false") << endl;

  vector<int> num1({10, 10, 10, 1, 10});
  cout << test154(num1) << endl;

  vector<int> num2({3, 3, 7, 7, 10, 11, 11});
  vector<int> num3({1, 1, 2, 3, 3, 4, 4, 8, 8});
  cout << test540(num3) << endl;

  vector<int> num4({1, 2});
  vector<int> num5({2, 5, 6,7});
  cout << test4(num5, num4) << endl;
  return 0;
}
