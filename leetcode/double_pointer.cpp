/******************************************************************************
* File:             double_pointer.cpp
*
* Author:           leetcode 双指针系列题目解答  
* Created:          02/24/21 
* Description:      
*****************************************************************************/

#include <iostream>
#include <ostream>
#include <vector>
#include <math.h>

using namespace std;

std::vector<int> test167(vector<int> &numbers, int target) {
  int begin = 0;
  int end = numbers.size() - 1;
  int sum = 0;
  while (begin != end) {
    sum = numbers[begin] + numbers[end];
    if (sum > target)
      end--;
    else if (sum < target)
      begin++;
    else
      break;
  }
  return vector<int>({begin + 1, end + 1});
}

void test88(vector<int>& num1,int m,vector<int>& num2,int n){
  int pos = m-- + n-- - 1;
  while (n >= 0 && m >= 0) {
    num1[pos--] = num1[m] > num2[n] ? num1[m--] : num2[n--];
  }
  while (n >= 0) {
    num1[pos--] = num2[n--];
  }
}

struct ListNode{
    int val;
    ListNode* next;
    ListNode(int x):val(x),next(nullptr) {
    }   
};

/******************************************************************************
* f =2s;
* 第一次相遇时 f = s + nb;
* 即 s = nb;
* a + nb 时 回到起点
* 即s 再走a 回到起点
*****************************************************************************/
ListNode *test142(ListNode *head) {
  ListNode *fast = head, *slow = head;
  do {
    if (!fast || !fast->next)
      return nullptr;
    fast = fast->next->next;
    slow = slow->next;
  } while (fast != slow);
  fast = head;
  while (fast != slow) {
    fast = fast->next;
    slow = slow->next;
  }
  return slow;
}

/*need 表示每个字符缺少的数量 大于等于0表示还缺
 *flag 字符是否在t中出现
 *当s对应的索引的字符在t中出现过时,所需要数--
 *所需要数跟t数量一致时 判断最新长度更新l向右移动
 */
string test76(string s, string t) {
  vector<int> need(128, 0);
  vector<bool> flag(128, false);
  for (char i : t) {
    ++need[i];
    flag[i] = true;
  }
  int l = 0, r = 0, cnt = 0, minsize = s.size() + 1, min_l = 0;
  for (; r < s.size(); r++) {
    if (flag[s[r]]) {
      if (--need[s[r]] >= 0)
        cnt++;
      while (cnt == t.size()) {
        if (r - l + 1 < minsize) {
          min_l = l;
          minsize = r - l + 1;
        }
        if (flag[s[l]] && ++need[s[l]] > 0)//当是所需字符是l移动1 跳出循环找下一个字串
          cnt--;
        l++;
      }
    }
  }
  return minsize > s.size() ? "" : s.substr(min_l, minsize);
}

bool test633(int c) {
  if (c <= 2)
    return true;
  int l = 0;
  int r = sqrt(c);
  while (l <= r) {
    if (l*l == c-r*r)
      return true;
    else if (c - l*l > r*r)
      r--;
    else
      l++;
  }
  return false;
}

bool checkPalindrome(string s, int left, int right) {
  for (; left < right && s[left] == s[right]; ++left, --right) {
  }
  return left >= right;
}

bool test680(string s) {
  int l = 0;
  int r = s.size() - 1;
  while (l < r) {
    if (s[l] != s[r]) {//第一版使用if(l+1) else if(r-1) else false 有问题;需要一次全部校验完 
      return checkPalindrome(s, l + 1, r) || checkPalindrome(s, l, r - 1);
    } else {
      l++;
      r--;
    }
  }
  return true;
}

bool checkSubString(const string &s, const string &t) {
  int ls = 0, lt = 0;
  for (; ls < s.size() && lt < s.size();) {
    if (s[ls] == t[lt])
      lt++;
    ls++;
  }
  return lt == t.size();
}

string test524(string s, const vector<string> &dictionary) {
  int lenght = 0;
  string tmp;
  for (string t : dictionary) {
    int ssize = tmp.size();
    int tsize = t.size();
    if (ssize > tsize || (ssize == tsize && tmp.compare(t) < 0)) //提前过滤
      continue;
    if (checkSubString(s, t))  {
      lenght = t.size();
      tmp = t;
    }
  }
  return tmp;
}

std::ostream& operator<<(ostream& out,const vector<int>& data){
    for(int i : data){
        cout << i <<";";
    }
    cout << endl;
    return out;
}

int main(int argc, char *argv[]) {
  vector<int> numbers({2, 3, 4});
  numbers = test167(numbers, 6);
  cout << numbers << endl;
  vector<int> num1({1, 2, 3, 0, 0, 0});
  vector<int> num2({2, 5, 6});
  test88(num1, 3, num2, 3);
  cout << num1;
  string s("ADOBECODEBANC");
  string t("ABC");
  cout << test76(s, t) << endl;

  cout << (test633(100000) ? "true" : "false") << endl;

  string s1("aguokepatgbnvfqmgmlcupuufxoohdfpgjdmysgvhmvffcnqxjjxqncffvmhvgsymd"
            "jgpfdhooxfuupuculmgmqfvnbgtapekouga");
  cout << (test680(s1) ? "true" : "false") << endl;

  cout << test524("abpcplea",
                  vector<string>({"ale", "apple", "monkey", "plea"}))
       << endl;
  return 0;
}
