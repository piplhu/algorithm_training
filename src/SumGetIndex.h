#pragma once
#include <vector>
#include <algorithm>
using namespace std;

//��������2������Ϊsum������
static  std::pair<int, int> sumTwo(std::vector<int>& vecdata, int sum) {
	std::pair<int, int> pair(-1, -1);
	std::vector<int>::iterator beginItr = vecdata.begin();
	for (int i =0;i < vecdata.size();i++)
	{
		int tmp = sum - vecdata[i];
		auto iter =  std::find(beginItr +i+1, vecdata.end(), tmp);
		if (iter != vecdata.end()) {
			pair.second =  iter - beginItr;
			pair.first = i;
			return pair;
		}
	}
	return pair;
}

static void  calcMedian(vector<int>& nums, double& num_median) {
	int index = nums.size() / 2;
	if (nums.size() % 2 != 0) {
		num_median = nums[index];
	}
	else {
		num_median = static_cast<double>(nums[index] + nums[index - 1]) / 2.0;
	}
}
//���2��������ϵ��������λ��
static double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	double num_median = 0;
	if (nums1.empty()) {
		calcMedian(nums2, num_median);
		return num_median;
	}
	if (nums2.empty()) {
		calcMedian(nums1, num_median);
		return num_median;
	}
	vector<int> tmp;
	tmp.reserve(nums1.size() + nums2.size());
	int i = 0, j = 0;
 	while (true){
		if (i < nums1.size()  && j < nums2.size() ) {
			if (nums1[i] <= nums2[j]) {
				tmp.push_back(nums1[i]);
				i++;
			}
			else {
				tmp.push_back(nums2[j]);
				j++;
			}
		}
		else {
			while (i < nums1.size() ) {
				tmp.push_back(nums1[i]);
				i++;
			}
			while (j < nums2.size() )
			{
				tmp.push_back(nums2[j]);
				j++;
			}
			break;
		}
	}
	calcMedian(tmp, num_median);
	return num_median;
}


static double findMedianSortedArraysEx1(vector<int>& nums1, vector<int>& nums2) {
	int m = nums1.size();
	int n = nums2.size();
	int len = m + n;
	int left = -1, right = -1;
	int start1 = 0, start2 = 0;
	for (int i = 0; i < len / 2 + 1; i++) {
		left = right;
		if (start1 < m && (start2 >= n || nums1[start1] < nums2[start2]))
			right = nums1[start1];
		else
			right = nums2[start2];
	}
	if ((len & 1) == 0)
		return (left + right) / 2.0;
	else
		return right;
}

int getKth(vector<int> nums1, int start1, int end1, vector<int> nums2, int start2, int end2, int k) {
	int len1 = end1 - start1 + 1;
	int len2 = end2 - start2 + 1;
	//�� len1 �ĳ���С�� len2���������ܱ�֤�����������ˣ�һ���� len1 
	if (len1 > len2) return getKth(nums2, start2, end2, nums1, start1, end1, k);
	if (len1 == 0) return nums2[start2 + k - 1];

	if (k == 1) return std::min(nums1[start1], nums2[start2]);

	int i = start1 + std::min(len1, k / 2) - 1;
	int j = start2 + std::min(len2, k / 2) - 1;

	if (nums1[i] > nums2[j]) {
		return getKth(nums1, start1, end1, nums2, j + 1, end2, k - (j - start2 + 1));
	}
	else {
		return getKth(nums1, i + 1, end1, nums2, start2, end2, k - (i - start1 + 1));
	}
}

static double findMedianSortedArraysEx2(vector<int>& nums1, vector<int>& nums2) {
	int n = nums1.size();

	int m = nums2.size();
	int left = (n + m + 1) / 2;
	int right = (n + m + 2) / 2;
	//��ż��������������ϲ����������������������ͬ���� k ��
	return (getKth(nums1, 0, n - 1, nums2, 0, m - 1, left) + getKth(nums1, 0, n - 1, nums2, 0, m - 1, right)) * 0.5;
}

static double findMedianSortedArraysEx3(vector<int>& nums1, vector<int>& nums2) {
	int n = nums1.size();
	int m = nums2.size();

	if (n > m)  //��֤����1һ�����
	{
		return findMedianSortedArrays(nums2, nums1);
	}

	// Ci Ϊ��i������ĸ�,����C1Ϊ2ʱ��ʾ��1������ֻ��2��Ԫ�ء�LMaxiΪ��i�����������Ԫ�ء�RMiniΪ��i�����������Ԫ�ء�
	int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0, hi = 2 * n;  //����Ŀǰ���������'#'��������1��2*n����

	while (lo <= hi)   //����
	{
		c1 = (lo + hi) / 2;  //c1�Ƕ��ֵĽ��
		c2 = m + n - c1;

		LMax1 = (c1 == 0) ? INT_MIN : nums1[(c1 - 1) / 2];
		RMin1 = (c1 == 2 * n) ? INT_MAX : nums1[c1 / 2];
		LMax2 = (c2 == 0) ? INT_MIN : nums2[(c2 - 1) / 2];
		RMin2 = (c2 == 2 * m) ? INT_MAX : nums2[c2 / 2];

		if (LMax1 > RMin2)
			hi = c1 - 1;
		else if (LMax2 > RMin1)
			lo = c1 + 1;
		else
			break;
	}
	return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0;
}

