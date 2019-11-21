

#include <stack>

//////////////////////////////////////////////////////////////////////////
////ÖÂÃüÈ±Ïİ£º²îÖµ´æÔÚÒç³ö·çÏÕ
//////////////////////////////////////////////////////////////////////////
class MinStack
{
	std::stack<int> diff_stack_;
	int minValue;
public:
	MinStack() {};
	void push(int value) {
		if (diff_stack_.empty()) {
			diff_stack_.push(0);
			minValue = value;
		}
		else {
			int tmp = value - minValue;
			diff_stack_.push(tmp);
			minValue = tmp < 0 ? value : minValue;
		}
	};
	int pop() {
		int tmp = diff_stack_.top();
		minValue = tmp < 0 ? tmp - minValue : minValue;
		diff_stack_.pop();
	};

	int top() {
		int tmp = diff_stack_.top();
		return tmp > 0 ? tmp + minValue : minValue;
	}

	int getMin() {
		return minValue;
	}
};


class MinStackEx
{
private:
	/* data */
	std::stack<int> data_stack_;
	std::stack<int> min_stack_;
public:
	MinStackEx(/* args */){};
	void push(int i){
		if(data_stack_.empty())
			min_stack_.push(i);
		data_stack_.push(i);
		if(i < min_stack_.top())
			min_stack_.push(i);
	}

	int pop(){
		int tmp = data_stack_.top();
		if(tmp == min_stack_.top())
			min_stack_.pop();
		data_stack_.pop();
	}

	int top(){
		return data_stack_.top();
	}

	int getMin(){
		return min_stack_.top();
	}
};


