#include "stdafx.h"

#include <iostream>

#include "minstack.h"
#include "SortPrinf.h"
#include "SumGetIndex.h"
#include "ReverseBitAdd.h"
#include "string_problem.h"

void one() { std::cout <<("one"); }
void two() { std::cout <<("two"); }
void three() { std::cout <<("three"); }


int main() {
	MinStack minstack;
	minstack.push(3); 
	minstack.push(1);
	minstack.push(4);
	minstack.push(2);
	
	MinStackEx minstackex;
	minstackex.push(3); 
	minstackex.push(1);
	minstackex.push(4);
	minstackex.push(2);

	std::cout << minstack.getMin() << std::endl;
	std::cout << minstack.top() << std::endl;

	std::cout << minstackex.getMin() << std::endl;
	std::cout << minstackex.top() << std::endl;

	int target = 6;
	std::vector<int> vecdata{ 3,2,4 };
	std::pair<int,int> result = sumTwo(vecdata, target);
	std::cout << "SumIndex:" << result.first << "," << result.second << std::endl;



	ListNode* l1 = new ListNode(1);
	l1->pushpack(8);
	l1->pushpack(3);

	ListNode* l2 = new ListNode(0);
	/*l2->next = new ListNode(6);
	l2->next->next = new ListNode(4);*/

	ListNode* resultsum = addTwoNumbers(l1, l2);
	while (resultsum != nullptr) {
		std::cout << resultsum->val;
		resultsum = resultsum->next;
	}
	std::cout << std::endl;
	std::cout << "reserveList-----" << std::endl;

	ListNode* l3 = new ListNode(1);
	l3->pushpack(2);
	l3->pushpack(3);
	l3->pushpack(4);
	ListNode* resultsum1 = reverselist(l3);
	while (resultsum1 != nullptr) {
		std::cout << resultsum1->val;
		resultsum1 = resultsum1->next;
	}
	std::cout << std::endl;

	std::cout << "GetSubstringLenght----" << std::endl;
	std::cout << lengthOfLongestSubstringEx("dvdf");

	std::cout << std::endl;
	std::cout << "findMedianSortedArrays----" << std::endl;
	vector<int> nums1 = { 1,3,4,9 };
	vector<int>	nums2 = { 1,2,3,4,5,6,7,8,9,10 };
	std::cout << findMedianSortedArrays(nums1,nums2);
	std::cout << findMedianSortedArraysEx2(nums1, nums2);

	std::cout << std::endl;
	std::cout << "Palindrome----" << std::endl;
	string s("anugnxshgonmqydttcvmtsoaprxnhpmpovdolbidqiyqubirkvhwppcdyeouvgedccipsvnobrccbndzjdbgxkzdbcjsjjovnhpnbkurxqfupiprpbiwqdnwaqvjbqoaqzkqgdxkfczdkznqxvupdmnyiidqpnbvgjraszbvvztpapxmomnghfaywkzlrupvjpcvascgvstqmvuveiiixjmdofdwyvhgkydrnfuojhzulhobyhtsxmcovwmamjwljioevhafdlpjpmqstguqhrhvsdvinphejfbdvrvabthpyyphyqharjvzriosrdnwmaxtgriivdqlmugtagvsoylqfwhjpmjxcysfujdvcqovxabjdbvyvembfpahvyoybdhweikcgnzrdqlzusgoobysfmlzifwjzlazuepimhbgkrfimmemhayxeqxynewcnynmgyjcwrpqnayvxoebgyjusppfpsfeonfwnbsdonucaipoafavmlrrlplnnbsaghbawooabsjndqnvruuwvllpvvhuepmqtprgktnwxmflmmbifbbsfthbeafseqrgwnwjxkkcqgbucwusjdipxuekanzwimuizqynaxrvicyzjhulqjshtsqswehnozehmbsdmacciflcgsrlyhjukpvosptmsjfteoimtewkrivdllqiotvtrubgkfcacvgqzxjmhmmqlikrtfrurltgtcreafcgisjpvasiwmhcofqkcteudgjoqqmtucnwcocsoiqtfuoazxdayricnmwcg");
	//string s("abb");
	std::cout << longestPalindromeEx(s)<<endl;
	std::cout << longestPalindromeEx2(s) << endl;
	s = "cbbd";
	std::cout << longestPalindrome(s) << endl;

	std::cout << std::endl;
	std::cout << "SortPritf----" << std::endl;
	Foo fo;
	std::function<void(std::function<void()>)> fn1 = std::bind(&Foo::first, &fo, std::placeholders::_1);
	std::function<void(std::function<void()>)> fn2 = std::bind(&Foo::second, &fo, std::placeholders::_1);
	std::function<void(std::function<void()>)> fn3 = std::bind(&Foo::third, &fo, std::placeholders::_1);
	std::thread th1(fn1, one);
	std::thread th3(fn3, three);
	std::thread th2(fn2, two);

	system("pause");
	return 0;
}
