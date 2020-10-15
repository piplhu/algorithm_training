#pragma once

struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(nullptr) {}
	void pushpack(int value) {
		if (next == NULL) {
			next = new ListNode(value);
			return;
		}
			
		ListNode** tmp = &(next->next);
		while (*tmp != NULL)
			tmp = &((*tmp)->next);
		 *tmp= new ListNode(value);
	}
};

static  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
	ListNode* result = nullptr;
	ListNode* node = nullptr;
	int carry = 0;
	int sum = 0;
	while (l1!= nullptr || l2!= nullptr) {
		int x = l1 ? l1->val : 0 ;
		int y = l2 ? l2->val : 0;
		sum = carry + x + y;
		carry = sum / 10;
		if (result == nullptr) {
			result = new ListNode(sum%10);
			node = result;
		}
		else{
			node->next = new ListNode(sum%10);
			node = node->next;
		}	
		if(l1 !=nullptr)	l1 = l1->next;
		if(l2 !=nullptr)	l2 = l2->next;
	}
		
	/*while (l1 != nullptr) {
		int tmp = l1->val;
		if (isNeedCarryBit) {
			tmp++;
			isNeedCarryBit = false;
		}
		if (tmp >= 10) {
			tmp = tmp - 10;
			isNeedCarryBit = true;
		}
		node->next = new ListNode(tmp);
		node = node->next;
		l1 = l1->next;
	}
		
	while (l2 != nullptr) {
		int tmp = l2->val;
		if (isNeedCarryBit) {
			tmp++;
			isNeedCarryBit = false;
		}
		if (tmp >= 10) {
			tmp = tmp - 10;
			isNeedCarryBit = true;
		}
		node->next = new ListNode(tmp);
		node = node->next;
		l2 = l2->next;
	}
	if (isNeedCarryBit)
		node->next = new ListNode(1);*/
	return result;
}

static void SwitchNode(ListNode** head, ListNode** end) {

	ListNode* tmp = *head;
	*head = (*end)->next;
	(*end)->next = (*end)->next->next;
	(*head)->next = tmp;
	/*ListNode* tmp1 = end->next->next;
	head->next = tmp;
	tmp->next = tmp1;*/
	//*end = (*end)->next;
}

//·´×ªÁ´±í
static ListNode* reverselist(ListNode* head) {
	ListNode* end = head;
	/*ListNode* tmp = head;
	head = head->next;
	ListNode* tmp1 = head->next;
	head->next = tmp;
	tmp->next = tmp1;*/

	while (end->next!= NULL)
		SwitchNode(&head, &end);
	/*tmp = head;
	head = tmp1;
	head->next = tmp;
	
	tmp->next = tmp->next->next;
	head = tmp->next;
	head->next =*/ 
	return head;
}

