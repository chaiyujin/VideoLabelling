#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define StringAlignLeft		1
#define StringAlignRight	2
#define StringAlignCenter	3

class MyString {
private:
	string str;
public:
	MyString(string str) : str(str) {}
	~MyString() {}

	static string space(int length = 0) {
		string ret = "";
		for (int i = 0; i < length; ++i) {
			ret += ' ';
		}
		return ret;
	}

	static void align(string &x, string &y, int mode) {
		int xl = (int)x.length();
		int yl = (int)y.length();
		int len = (xl > yl) ? xl : yl;
		xl = len - xl;
		yl = len - yl;
		if (mode == StringAlignLeft) {
			x = x + MyString::space(xl);
			y = y + MyString::space(yl);
		}
		else if (mode == StringAlignRight) {
			x = MyString::space(xl) + x;
			y = MyString::space(yl) + y;
		}
		else if (mode == StringAlignCenter) {
			x = MyString::space(xl / 2) + x + MyString::space(xl - xl / 2);
			y = MyString::space(yl / 2) + y + MyString::space(yl - yl / 2);
		}
	}

	static string int2string(int x) {
		char tmp[100];
		itoa(x, tmp, 10);
		return tmp;
	}

	static int string2int(string str) {
		int i = 0;
		int j = 0;
		while (i < (int)str.length() && (str[i] < '0' || str[i] > '9')) ++i;
		if (i >= (int)str.length())
			return -1e6;
		j = i;
		while (j < (int)str.length() && ('0' <= str[j] && str[j] <= '9')) ++j;
		str = str.substr(i, j - i);
		return atoi(str.c_str());
	}

	static string divideString(int x, int y) {
		string xx = MyString::int2string(x);
		string yy = MyString::int2string(y);
		string prefix = "";
		for (int i = 0; i < (int)yy.length() - (int)xx.length(); ++i)
			prefix += ' ';
		xx = prefix + xx;
		return xx + "/" + yy;
	}

	vector<string> split(char split_char) {
		vector<int> pos;
		pos.push_back(0);
		for (int i = 0; i < (int)str.length(); ) {
			int k = str.find(split_char, (size_t)i);
			if (k > -1) {
				pos.push_back(k + 1);
				i = k + 1;
			}
			else break;
		}
		pos.push_back((int)str.length() + 1);
		vector<string> list;
		for (int i = 0; i < pos.size() - 1; ++i) {
			list.push_back(str.substr(pos[i], pos[i + 1] - pos[i] - 1));
		}
		return list;
	}

	string join(vector<string> &str_list) {
		if ((int)str_list.size() == 0)
			return " ";
		string res = str_list[0];
		for (int i = 1; i < str_list.size(); ++i) {
			res += str + str_list[i];
		}
		return res;
	}
};
