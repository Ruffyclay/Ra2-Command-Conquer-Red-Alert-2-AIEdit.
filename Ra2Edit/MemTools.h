#pragma once
// 内存字符操作工具
#include "stdafx.h"
#include <string.h>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include "PhyTools.h"
#include <cstring>
#include "OtherTools.h"

#define _CRT_SECURE_NO_WARNINGS

#define C_HEAD 0
#define C_END 1
#define R_BH 1  // 返回后面内容
#define R_BF 0  // 返回前面的内容

// 检查是否满足注册表条件
inline bool CheckRegedit(const char* txt){
	if ((txt == '\0'))return false;
	size_t len = strlen(txt);
	for (size_t i = 0; i < len; i++){
		if ((txt[i] >= 48) && (txt[i] <= 57)){
			for (size_t j = 0; j < len - i; j++){
				if (txt[i + j] == '='){
					return true;
				}
			}
		}
	}
	return false;
}

inline bool CheckAITedit(const char* txt){
	if ((txt == '\0'))return false;
	bool flag = false;
	size_t len = strlen(txt);
	for (size_t i = 0; i < len; i++){
		if (txt[i] != ';'){ // 开头非;第一点
			for (size_t j = 0; j < len - i; j++){
				if (txt[i + j] == '='){ // 有=第二点
					flag = true;
				}
				if (txt[i + j] == ';'){ // 分号不准出现在这一行 第三点
					return false;
				}
			}
		}
	}
	return flag;
}

// 查看返回值
inline void testCheck(const char a[]){
	CString ms;
	ms.Format(_T("%s"), (CString)a);
	AfxMessageBox(ms);
	return;
}

// 在内存指定区域搜索特征文字并返回头位置或尾位置, 修改让它包括'\0'这个字符
// 文件大小,开始位置，内容，目标，模式
inline int TxtMemFind(int filesize, int startPos, const char* str1, const char* str2, char mode) {
	if (str1 == NULL || str2 == NULL)return -1;
	bool flag = true; // 查找末尾标志符
	if (str2 == '\0'){ // 如果找的就是末尾
		flag = false;
	}
    int len1 = filesize;
    int len2 = strlen(str2);
    if (len1 < len2) {
        return -1;
    }
    int i = startPos;
    while ((i <= len1 - len2 )&& flag) {
        int j = 0;
        while (j < len2 && str1[i + j] != '\0' && str1[i + j] == str2[j]) {
            j++;
        }
        if ((j == len2) && (mode == 1)) {
            return i + strlen(str2) + 1;
        }
        else if ((j == len2) && (mode == 0)) {
            return i;
        }
        i++;
    }
	if (flag){
		while ((i <= len1 - len2)){
			int j = 0;
			while (j < len2 && str1[i + j] == str2[j]) {
				j++;
			}
			if ((j == len2) && (mode == 1)) {
				return i + strlen(str2) + 1;
			}
			i++;
		}
		return i;
	}

    return -1;
}



// 读取内存中指定内容区间，记得释放内存
// 内容 大小 开始位置 结束位置
inline char* ReadMemContent(const char* txt, int size, int poss, int pose) {
	if (poss >= pose || poss >= size) {
		return nullptr;
	}
	char* content = new char[pose - poss + 1];
	int endIndex = pose - poss;
	if (endIndex >= size - poss) {
		endIndex = size - poss;
	}
	for (int i = 0; i < endIndex; i++) {
		content[i] = *(txt + poss + i);
	}
	content[endIndex] = '\0';
	return content;
}

// 读取指定内容中的每一行，在每一行的末尾添加'\n'
inline char* GetMemLines(const char* txt, int size){
	int pos = 0;
	char* lines = (char*)malloc(size + 1);
	for (int i = 0; i < size; i++){ // 读取内容的指针
		lines[pos] = txt[i]; // 赋值内容
		if (txt[i] == '\n'){ // 跳过多换行内容
			pos += 1;
			lines[pos] = txt[i];
			for (int j = 0; j < 1024; j++){ // 用来跳过连续换行符号的
				if (txt[j + i] != '\n'){ // 判断连续换行符
					i += j;
					lines[pos] = txt[i];
					break;
				}
			}
		}
		pos += 1;
	}
	lines[pos] = '\0';
	return lines;
}


// 返回指定内容中第几行
inline char* GetMemLine(const char* txt, int LineNum) {
	if (txt == NULL) return NULL;
	const char* start = txt;
	int curLine = 1;
	while (*txt != '\0' && curLine != LineNum) {
		if (*txt == '\n') {
			curLine++;
			start = txt + 1;
		}
		txt++;
	}
	if (*txt == '\0' && curLine < LineNum) return NULL;
	const char* end = txt;
	while (*end != '\0' && *end != '\n') end++;
	char* result = (char*)malloc(end - start + 1);
	if (result == NULL) return NULL;
	size_t count = end - start;
	strncpy_s(result, count + 1, start, count);
	result[count] = '\0';
	return result;
}

// 返回多少行内容
inline int GetMemLinesNum(const char* txt){
	int ln = 1; // 至少有一行
	for (int i = 0; i < strlen(txt); i++){ // 指针递增
		if (txt[i] == '\n'){
			ln += 1;
		}
	}
	return ln;
}


// 文件符号分割，自动提取有意义的内容，以行为单位
// 文本，文本大小，目标，R_BF|R_BH
inline char* GetTargetString(const char* txt, long size, const char* target, int mod) {
	if (!txt || !target) return NULL;

	char* buffer = (char*)malloc(size + 1);
	if (!buffer) return NULL;

	buffer[size] = '\0';

	long pos = TxtMemFind(size, 0, txt, target, C_HEAD);
	if (pos < 0) {
		free(buffer);
		return NULL;
	}

	if (mod == R_BF) { // 返回前面
		for (int i = 0; i < pos; i++) {
			buffer[i] = txt[i];
		}
		buffer[pos] = '\0';
	}
	else if (mod == R_BH) {
		pos += 1;
		int i = 0;
		while (pos < size) {
			if (txt[pos] == ';') {
				buffer[i-1] = '\0'; // 自动取消保存中间的空格，以及分号后面的内容
				break;
			}
			else if (txt[pos] == ' ' || txt[pos] == '\t') {
				if (i > 0 && buffer[i - 1] != ' ') {
					buffer[i] = ' ';
					i++;
				}
			}
			else {
				buffer[i] = txt[pos];
				i++;
			}
			pos++;
		}
		buffer[i] = '\0';
	}

	return buffer;
}


// 获取所选文件根目录
// 目录路径
inline char* GetMemPathLastRoot(const char* path){
	int pos = 0;
	int posE = -1; // 最终确定值
	int len = strlen(path);
	char* temp = (char*)malloc(len + 2); // 申请空间时需要加1存储结束符，再加1存储根目录
	if (temp == nullptr) { // 判断是否申请空间失败
		return nullptr;
	}
	while (path[pos] != '\0'){
		if (path[pos] == '\\' || path[pos] == '/') { // 判断是否为路径分隔符
			posE = pos;
		}
		pos += 1;
	}
	if (posE == -1) { // 判断是否找到路径分隔符
		temp[0] = '\0'; // 如果没找到，则返回空字符串
	}
	else {
		for (int i = 0; i <= posE; i++){ // 需要复制路径分隔符所在位置之前的字符，包括路径分隔符
			temp[i] = path[i];
		}
		temp[posE + 1] = '\0'; // 将根目录后的字符置为结束符
	}
	return temp;
}

// 去除注册表前缀
// path
inline char* RemoveMemPre(const char* path) {
	if (strncmp(path, "./", 2) == 0) { // 如果前缀是"./"
		path += 2; // 指针后移两位，跳过"./"
	}
	int len = strlen(path);
	char* result = (char*)malloc(len + 1); // 申请空间存储返回结果
	strncpy_s(result, len + 1, path, len); // 使用 strncpy_s 复制剩余的字符串到新数组中
	return result;
}

// 在指定内容后面新增内容
// 原地址，指针位置，指定内容
inline char* InsertMemCon(const char* originAddr, int pos, const char* newContent) {
	int len1 = strlen(originAddr);
	int len2 = strlen(newContent);
	char* result = new char[len1 + len2 + 1];
	for (int i = 0; i < pos; i++) {
		result[i] = originAddr[i];
	}
	for (int i = pos; i < pos + len2; i++) {
		result[i] = newContent[i - pos];
	}
	for (int i = pos + len2; i <= len1 + len2; i++) {
		result[i] = originAddr[i - len2];
	}
	delete[] originAddr;
	return result;
}

// 删除指定内容区间的内容
// 内容，开始位置，结束位置
inline char* RemoveMemStr(char* str, int start, int end) {
	int len = strlen(str);
	if (start >= end || start >= len) return str;

	int remove_len = end - start + 1;
	int remain_len = len - remove_len;

	// 分配新的内存空间
	char* new_str = (char*)malloc(remain_len + 1);

	// 复制前半部分
	if (start > 0) {
		memcpy(new_str, str, start);
	}
	new_str[start] = '\0';

	// 复制后半部分
	if (end < len - 1) {
		memcpy(new_str + start, str + end + 1, len - end - 1);
	}
	new_str[remain_len] = '\0';

	// 释放原来的内存空间
	free(str);

	return new_str;
}

// 修改地址，被替换字符，替换字符
inline char* ReplaceMemChr(char* str, char oldChar, char newChar) {
	if (str == NULL) return NULL;
	size_t length = strlen(str);
	for (size_t i = 0; i < length; i++) {
		if (str[i] == oldChar) {
			str[i] = newChar;
		}
	}
	return str;
}

// 查找一行的内容
inline std::string FindMemSTDS(std::string txt, char* target){
	size_t start_pos = 0, end_pos = 0;
	std::string rtxt = "None";
	start_pos = txt.find(target,0);
	if (start_pos != std::string::npos){ // 如果找到了
		start_pos += strlen(target);
		end_pos = txt.find("\n", start_pos);
		if (end_pos == std::string::npos){
			end_pos = txt.find("\0", start_pos);
			if (end_pos == start_pos){ // 如果还找不到就只找当前行内容
				end_pos = txt.find("\n", start_pos);
			}
		}
		rtxt = txt.substr(start_pos, end_pos - start_pos);
	}
	return rtxt;
}

// 删除一行内容
inline bool DelMemLine(std::string& txt, char* target) {
	size_t start_pos = 0, end_pos = 0;
	start_pos = txt.find(target, 0);
	if (start_pos != std::string::npos) { // 如果找到了
		end_pos = txt.find("\n", start_pos);
		if (end_pos == std::string::npos) {
			end_pos = txt.find("\0", start_pos);
		}
		end_pos++; // 将end_pos调整到下一行开头
		size_t delete_len = end_pos - start_pos;
		if (start_pos > 0) { // 如果不是第一行
			start_pos--; // 将start_pos调整到上一行末尾的\n
		}
		txt.erase(start_pos, delete_len); // 删除该行
		return true;
	}
	return false;
}