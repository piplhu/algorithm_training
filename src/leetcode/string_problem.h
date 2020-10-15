#pragma once
#include <string>
#include <algorithm>
using namespace std;

//��ȡ�ַ����в������ظ��ַ��Ӵ�����󳤶�
static int lengthOfLongestSubstring(std::string s) {
	if (s.empty())
		return 0;
	if (s.size() == 1)
		return 1;
	int result = 1;
	int tmpresult = 1;
	std::string reStr;
	char tmp = s.at(0);
	reStr.push_back(tmp);
	int size = s.size();
	for (int i = 1; i < size; i++) {
		size_t index = reStr.rfind(s[i]);
		if (tmp != s[i] && string::npos == index) {
				reStr.push_back(s[i]);
				tmpresult++;
		}
		else {
			tmp = s[i];
			reStr.erase(0, index + 1);
			reStr.push_back(tmp);
			result = tmpresult > result ? tmpresult : result;
			tmpresult = reStr.size();
			
		}
	}
	return  tmpresult > result ? tmpresult : result;;
}

static int lengthOfLongestSubstringEx(string s) {
	int n = s.length(), ans = 0;
	int index[256] = {0}; // current index of character
	// try to extend the range [i, j]
	for (int j = 0, i = 0; j < n; j++) {
		i = std::max(index[s[j]], i);
		ans = std::max(ans, j - i + 1);
		index[s[j]] = j +1;
	}
	return ans;
}

static bool isPalindrome(const string s) {
	int size = s.length();
	for (int i = 0 ;i <size/2;i++){
		if (s[i] != s[size-1 - i])
			return false;
	}
	return true;
}

//��ȡ��Ļ����ִ�
static string longestPalindrome(string s) {
	if (s.empty())
		return "";
	int size = s.length();
	int maxLenght = 0;
	int startindex = 0;
	for (int i = 0; i < size ;i++)
	{
		for (int  j =i+1; j<= size;j++)
		{
			string tmp = s.substr(i, j-i);
			if (isPalindrome(tmp) && tmp.size() > maxLenght) {
				maxLenght = tmp.size();
				startindex = i;
			}
		}
	}
	if (maxLenght == 0)
		return s.substr(0,1);
	return s.substr(startindex, maxLenght);
}

static string longestPalindromeEx(string s) {
	if (s.empty())
		return "";
	int size = s.length();
	int maxLenght = 0;
	int startIndex = 0;
	vector<vector<int>> tmpVector(size,vector<int>(size));
	string tmp;
	tmp.append(s.rbegin(), s.rend());
	for (int i = 0; i < size; i++) {
		for(int j =0 ; j< size;j++)
		if (tmp[j] == s[i]) {
			if (i == 0 || j == 0)
				tmpVector[i][j] = 1;
			else
				tmpVector[i][j] = tmpVector[i - 1][j - 1] + 1;
		
			if (tmpVector[i][j] > maxLenght) {
				int beforeRev = size - 1 - j;
				if (beforeRev + tmpVector[i][j] - 1 == i) { 
					maxLenght = tmpVector[i][j];
					startIndex = i;
				}
			
			}
		}
	}
	return s.substr(startIndex -maxLenght +1, maxLenght);
}

//�������㷨
static string longestPalindromeEx2(string s)
{
	//����"#"
	string t = "$#";
	for (auto c : s) {
		t += c;
		t += '#';
	}

	vector<int> p(t.size(), 0);

	//mx��ʾĳ�����Ĵ����������Ҷ˰뾶���±꣬id��ʾ��������Ӵ����м�λ���±�
	//resLen��ʾ��Ӧ��s�е�����ӻ��Ĵ��İ뾶��resCenter��ʾ����ӻ��Ĵ����м�λ��
	int mx = 0, id = 0, resLen = 0, resCenter = 0;

	//����p����
	for (int i = 1; i < t.size(); ++i)
	{
		//����p[i]��ֵ
		p[i] = mx > i ? min(p[2 * id - i], mx - i) : 1;

		//��������������������Ҫ����������չ��
		while (t[i + p[i]] == t[i - p[i]])p[i]++;

		//�뾶�±�i+p[i]�����߽�mx����Ҫ����
		if (mx < i + p[i]) {
			mx = i + p[i];
			id = i;
		}

		//�����������Ӵ�����Ϣ
		if (resLen < p[i]) {
			resLen = p[i];
			resCenter = i;
		}
	}

	//������Ӵ�����Ϊ�뾶-1����ʼλ��Ϊ�м�λ�ü�ȥ�뾶�ٳ���2
	return s.substr((resCenter - resLen) / 2, resLen - 1);
}