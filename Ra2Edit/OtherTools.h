#pragma once

// ����ת��������
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
#include <algorithm> // ���� sort ������ͷ�ļ�
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

// ����ע�������
inline std::string FindiItem(std::string txt, const char* title){
	std::string content = "";
	size_t pos = txt.find("[" + std::string(title) + "]");
	if (pos != std::string::npos) {
		size_t start_pos = txt.rfind("\n", pos) + 1;
		size_t end_pos = txt.find("[", start_pos + 1) - 1;
		size_t lentcontent = end_pos - start_pos;
		content = txt.substr(start_pos, lentcontent);
		return content; // ���Ҳ�����ʱ���Զ�����ĩβ������("\0")��������
	}
	else{
		return "None";
	}
}


inline char* SerchTargetString(const char* text, const char* target, int startPos, int endPos) {
	std::string result;

	// �����ж��������Ƿ���������ţ�Ȼ�����ۼ�����
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

	// �ж�����Ŀ�ʼλ�úͽ���λ���Ƿ�Ϸ�
	if (startPos <= 0 || endPos > targetCount || endPos < 0) {
		result = "None";
	}
	else {
		// ���в��ң����ָ�����ʼλ��֮�󣬽���λ��֮ǰ�����ݱ��沢����
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

	// ʹ�ð�ȫ�汾�������ؽ��
	char* res = (char*)malloc((result.size() + 1) * sizeof(char));
	strcpy_s(res, result.size() + 1, result.c_str());
	return res;
}

inline char* ExtractText(const char* text, const char* start, const char* end) {
	if (!text || !start || !end) {
		return _strdup("None"); // ʹ��_strdup������̬�ڴ棬��Ҫ�ֶ��ͷ�
	}

	size_t startLen = strlen(start);
	size_t endLen = strlen(end);
	size_t textLen = strlen(text);

	const char* startPtr = strstr(text, start); // �ҵ���һ��ƥ�俪ʼ���ŵ�λ��
	int count = 0; // ��¼ƥ�䵽��Ŀ���ַ�������
	while (startPtr) {
		count++;
		startPtr += startLen;
		const char* endPtr = strstr(startPtr, end); // �ҵ�ƥ��Ľ�������
		if (endPtr) {
			size_t length = endPtr - startPtr; // ���㿪ʼ����֮�󣬽�������֮ǰ�����ݳ���
			char* result = new char[length + 1]; // ����һ���µ��ַ������洢���
			strncpy_s(result, length + 1, startPtr, length); // ʹ��strncpy_s��ȫ����Ŀ���ַ��������ַ�����
			result[length] = '\0'; // ����ַ���������
			if (count == 1) {
				return result; // ����ǵ�һ��ƥ���Ŀ���ַ�����ֱ�ӷ���
			}
			else {
				delete[] result; // ������ǵ�һ��ƥ���Ŀ���ַ������ͷ��ڴ沢��������
			}
			startPtr = strstr(endPtr, start); // �ҵ���һ��ƥ�俪ʼ���ŵ�λ��
		}
		else {
			break;
		}
	}

	if (count > 0) {
		// ���������Ŀ���ַ������޷�ƥ��������ţ��������һ����ʼ����֮�������
		size_t length = text + textLen - startPtr;
		char* result = new char[length + 1];
		strcpy_s(result, length + 1, startPtr);
		return result;
	}
	else {
		// ���δ������Ŀ���ַ���������"None"
		return _strdup("None"); // ʹ��_strdup������̬�ڴ棬��Ҫ�ֶ��ͷ�
	}
}

// ת����
inline int numtoi(CString value) {
	char* num = CStringToChar(value);
	return atoi(num);
}


// ��λһ��С��ת���
inline std::string reverse_hex_string(const std::string& str) {
	// 1. �ָ��ַ���Ϊ����Ϊ2�����ַ���
	std::vector<std::string> substrings;
	for (size_t i = 0; i < str.length(); i += 2) {
		substrings.push_back(str.substr(i, 2));
	}

	// 2. ���������ַ�������
	std::reverse(substrings.begin(), substrings.end());

	// 3. ƴ���ַ�������Ϊһ���ַ���
	std::string result;
	for (const auto& s : substrings) {
		result += s;
	}

	return result;
}

//����	25	error LNK2005 : "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl reverse_hex_string(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &)" (? reverse_hex_string@@YA ? AV ? $basic_string@DU ? $char_traits@D@std@@V ? $allocator@D@2@@std@@ABV12@@Z) �Ѿ��� Addnew.obj �ж���	C : \Users\30446\Desktop\ѧϰ\���\����\Ra2Edit\Ra2Edit\Cscripts.obj	Ra2Edit


inline char* FindHexData(char* value, bool rval) {
    // ��֤����Ĳ����Ƿ�Ϊ 64 λʮ�����Ƶ�����
    if (strlen(value) != 64 || strspn(value, "0123456789abcdefABCDEF") != 64) {
        return "None";
    }
    else {
        // ��ȡ��ֵλ�ͱ�־λ
		std::string data = std::string(value, 8);
		std::string flag = std::string(value + 8, 2);
		// ����ֵλ����һ�������������
		data = reverse_hex_string(data);
        std::string result;
        // ��� rval Ϊ true �򷵻�������������ֵλ�����򷵻ر�־λ
        if (rval) {
            result = data;
        }
        else {
            result = flag;
        }
        // ��ʮ������ת��Ϊʮ����
        std::stringstream ss;
        ss << std::hex << result;
        int decValue;
        ss >> decValue;
        // ��ת�����ʮ����ת��Ϊ�ַ���������
        char* res = new char[20];
        sprintf_s(res, 20, "%d", decValue);
        return res;
    }
}

// ȡ��
inline char* inta(const char* ch) {
	char* res = new char[20]; // ����һ���㹻����ַ����飬���ڴ�Ž��
	int i = 0;
	while (ch[i] != '.' && ch[i] != '\0') { // �����ַ������ҵ�С������ַ���ĩβ
		res[i] = ch[i];
		i++;
	}
	res[i] = '\0'; // �ַ���ĩβҪ���'\0'�ַ�
	return res;
}

inline std::string decimalToHexadecimal(const char* decimalStr) {
	int decimalValue = atoi(decimalStr); // ���ַ�תΪʮ��������
	char hexadecimalChars[9]; // ����洢ʮ�������ַ�������ռ䣬ע��Ҫ������һλ��Ž�����'\0'
	sprintf_s(hexadecimalChars, sizeof(hexadecimalChars), "%08X", decimalValue); // ʹ��sprintf_s������ʮ��������תΪ����Ϊ8��ʮ�������ַ�
	
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
			// ���������Ļس���
			continue;
		}
		else {
			result += c;
			previousNewline = false;
		}
	}

	// ���ĩβ���ǻ��з��������һ�����з�
	if (!result.empty() && result.back() != '\n') {
		result += '\n';
	}

	return result;
}





