#pragma once

// 编码转换工具箱
#include "stdafx.h"
#include "Ra2EditDlg.h"
#include "afxdialogex.h"
#include "FileTools.h"
#include "CharCode.h"
#include "PhyTools.h"
#include "MemTools.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm> // 包含 sort 函数的头文件
#include <windows.h>
#include <sstream>
#include "afxrich.h"
#include <richedit.h>
#include "Addnew.h"
#include <iostream>
#include <string>
#include <cstring>
#include <inttypes.h>
#include <iostream>
#include <string>
#include <unordered_map>


inline bool TeamByNum(const TeamTypes& a, const TeamTypes& b) {
	return a.num < b.num;
}

inline void TeamSortByNum(std::vector<TeamTypes>& teams) {
	std::sort(teams.begin(), teams.end(), TeamByNum);
}

inline bool ScriptByNum(const Scripts& a, const Scripts& b) {
	return a.num < b.num;
}

inline void ScriptSortByNum(std::vector<Scripts>& teams) {
	std::sort(teams.begin(), teams.end(), ScriptByNum);
}

inline bool TaskByNum(const TaskForces& a, const TaskForces& b) {
	return a.num < b.num;
}

inline void TaskSortByNum(std::vector<TaskForces>& teams) {
	std::sort(teams.begin(), teams.end(), TaskByNum);
}

inline bool Sac(const std::string& str, char const* c)
{
	std::string c_str(c);
	if (str.size() < c_str.size())
	{
		return false;
	}
	return std::equal(c_str.begin(), c_str.end(), str.begin());
}

// 查找注册表内容
inline std::string FindiItem(std::string txt, const char* title){
	std::string content = "";
	size_t pos = txt.find("[" + std::string(title) + "]");
	if (pos != std::string::npos) {
		size_t start_pos = txt.rfind("\n", pos) + 1;
		size_t end_pos = txt.find("[", start_pos + 1) - 1;
		size_t lentcontent = end_pos - start_pos;
		content = txt.substr(start_pos, lentcontent);
		return content; // 查找不到的时候自动返回末尾的内容("\0")帮助断文
	}
	else{
		return "None";
	}
}


inline char* SerchTargetString(const char* text, const char* target, int startPos, int endPos) {
	std::string result;

	// 首先判断内容中是否有这个符号，然后再累加总数
	const char* current = text;
	int targetCount = 0;
	while (current != nullptr) {
		current = strstr(current, target);
		if (current != nullptr) {
			targetCount++;
			current += strlen(target);
		}
	}

	if ((startPos == -2) && (endPos == -2))
	{
		startPos = targetCount;
		endPos = 0;
	}

	// 判断输入的开始位置和结束位置是否合法
	if (startPos <= 0 || endPos > targetCount || endPos < 0) {
		result = "None";
	}
	else {
		// 进行查找，将分隔符开始位置之后，结束位置之前的内容保存并返回
		current = text;
		for (int i = 1; i <= targetCount; i++) {
			if (i == startPos) {
				current = strstr(current, target) + strlen(target);
			}
			else if (i > startPos && i <= endPos) {
				const char* next = strstr(current, target);
				if (next == nullptr) {
					result = current;
					break;
				}
				result.append(current, next - current);
				current = next + strlen(target);
			}
			else {
				current = strstr(current, target);
				if (current == nullptr) {
					break;
				}
				current += strlen(target);
			}
		}

		if (endPos == 0) {
			result = current;
		}
	}

	// 使用安全版本函数返回结果
	char* res = (char*)malloc((result.size() + 1) * sizeof(char));
	strcpy_s(res, result.size() + 1, result.c_str());
	return res;
}

inline char* ExtractText(const char* text, const char* start, const char* end) {
	if (!text || !start || !end) {
		return _strdup("None"); // 使用_strdup创建动态内存，需要手动释放
	}

	size_t startLen = strlen(start);
	size_t endLen = strlen(end);
	size_t textLen = strlen(text);

	const char* startPtr = strstr(text, start); // 找到第一个匹配开始符号的位置
	int count = 0; // 记录匹配到的目标字符串个数
	while (startPtr) {
		count++;
		startPtr += startLen;
		const char* endPtr = strstr(startPtr, end); // 找到匹配的结束符号
		if (endPtr) {
			size_t length = endPtr - startPtr; // 计算开始符号之后，结束符号之前的内容长度
			char* result = new char[length + 1]; // 创建一个新的字符串来存储结果
			strncpy_s(result, length + 1, startPtr, length); // 使用strncpy_s安全复制目标字符串到新字符串中
			result[length] = '\0'; // 添加字符串结束符
			if (count == 1) {
				return result; // 如果是第一个匹配的目标字符串，直接返回
			}
			else {
				delete[] result; // 如果不是第一个匹配的目标字符串，释放内存并继续搜索
			}
			startPtr = strstr(endPtr, start); // 找到下一个匹配开始符号的位置
		}
		else {
			break;
		}
	}

	if (count > 0) {
		// 如果搜索到目标字符串但无法匹配结束符号，返回最后一个开始符号之后的内容
		size_t length = text + textLen - startPtr;
		char* result = new char[length + 1];
		strcpy_s(result, length + 1, startPtr);
		return result;
	}
	else {
		// 如果未搜索到目标字符串，返回"None"
		return _strdup("None"); // 使用_strdup创建动态内存，需要手动释放
	}
}

// 转类型
inline int numtoi(CString value) {
	char* num = CStringToChar(value);
	return atoi(num);
}


// 两位一组小端转大端
inline std::string reverse_hex_string(const std::string& str) {
	// 1. 分割字符串为长度为2的子字符串
	std::vector<std::string> substrings;
	for (size_t i = 0; i < str.length(); i += 2) {
		substrings.push_back(str.substr(i, 2));
	}

	// 2. 逆序排列字符串数组
	std::reverse(substrings.begin(), substrings.end());

	// 3. 拼接字符串数组为一个字符串
	std::string result;
	for (const auto& s : substrings) {
		result += s;
	}

	return result;
}

//错误	25	error LNK2005 : "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl reverse_hex_string(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &)" (? reverse_hex_string@@YA ? AV ? $basic_string@DU ? $char_traits@D@std@@V ? $allocator@D@2@@std@@ABV12@@Z) 已经在 Addnew.obj 中定义	C : \Users\30446\Desktop\学习\汇编\工程\Ra2Edit\Ra2Edit\Cscripts.obj	Ra2Edit


inline char* FindHexData(char* value, bool rval) {
    // 验证传入的参数是否为 64 位十六进制的内容
    if (strlen(value) != 64 || strspn(value, "0123456789abcdefABCDEF") != 64) {
        return "None";
    }
    else {
        // 获取数值位和标志位
		std::string data = std::string(value, 8);
		std::string flag = std::string(value + 8, 2);
		// 将数值位两个一组进行逆序排序
		data = reverse_hex_string(data);
        std::string result;
        // 如果 rval 为 true 则返回逆序排序后的数值位，否则返回标志位
        if (rval) {
            result = data;
        }
        else {
            result = flag;
        }
        // 将十六进制转换为十进制
        std::stringstream ss;
        ss << std::hex << result;
        int decValue;
        ss >> decValue;
        // 将转换后的十进制转换为字符串并返回
        char* res = new char[20];
        sprintf_s(res, 20, "%d", decValue);
        return res;
    }
}

// 取整
inline char* inta(const char* ch) {
	char* res = new char[20]; // 定义一个足够大的字符数组，用于存放结果
	int i = 0;
	while (ch[i] != '.' && ch[i] != '\0') { // 遍历字符串，找到小数点或字符串末尾
		res[i] = ch[i];
		i++;
	}
	res[i] = '\0'; // 字符串末尾要添加'\0'字符
	return res;
}

inline std::string decimalToHexadecimal(const char* decimalStr) {
	int decimalValue = atoi(decimalStr); // 将字符转为十进制整数
	char hexadecimalChars[9]; // 申请存储十六进制字符的数组空间，注意要多留出一位存放结束符'\0'
	sprintf_s(hexadecimalChars, sizeof(hexadecimalChars), "%08X", decimalValue); // 使用sprintf_s函数将十进制整数转为长度为8的十六进制字符
	
	return reverse_hex_string(hexadecimalChars);
}


inline std::string removeExtraNewlines(const std::string& input) {
	std::string result;
	bool previousNewline = false;

	for (char c : input) {
		if (c == '\n') {
			if (!previousNewline) {
				result += c;
			}
			previousNewline = true;
		}
		else if (c == '\r') {
			// 忽略连续的回车符
			continue;
		}
		else {
			result += c;
			previousNewline = false;
		}
	}

	// 如果末尾不是换行符，则添加一个换行符
	if (!result.empty() && result.back() != '\n') {
		result += '\n';
	}

	return result;
}





