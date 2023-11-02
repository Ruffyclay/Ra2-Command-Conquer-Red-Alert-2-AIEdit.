#pragma once
// �ڴ��ַ���������
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
#define R_BH 1  // ���غ�������
#define R_BF 0  // ����ǰ�������

// ����Ƿ�����ע�������
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
		if (txt[i] != ';'){ // ��ͷ��;��һ��
			for (size_t j = 0; j < len - i; j++){
				if (txt[i + j] == '='){ // ��=�ڶ���
					flag = true;
				}
				if (txt[i + j] == ';'){ // �ֺŲ�׼��������һ�� ������
					return false;
				}
			}
		}
	}
	return flag;
}

// �鿴����ֵ
inline void testCheck(const char a[]){
	CString ms;
	ms.Format(_T("%s"), (CString)a);
	AfxMessageBox(ms);
	return;
}

// ���ڴ�ָ�����������������ֲ�����ͷλ�û�βλ��, �޸���������'\0'����ַ�
// �ļ���С,��ʼλ�ã����ݣ�Ŀ�꣬ģʽ
inline int TxtMemFind(int filesize, int startPos, const char* str1, const char* str2, char mode) {
	if (str1 == NULL || str2 == NULL)return -1;
	bool flag = true; // ����ĩβ��־��
	if (str2 == '\0'){ // ����ҵľ���ĩβ
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



// ��ȡ�ڴ���ָ���������䣬�ǵ��ͷ��ڴ�
// ���� ��С ��ʼλ�� ����λ��
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

// ��ȡָ�������е�ÿһ�У���ÿһ�е�ĩβ���'\n'
inline char* GetMemLines(const char* txt, int size){
	int pos = 0;
	char* lines = (char*)malloc(size + 1);
	for (int i = 0; i < size; i++){ // ��ȡ���ݵ�ָ��
		lines[pos] = txt[i]; // ��ֵ����
		if (txt[i] == '\n'){ // �����໻������
			pos += 1;
			lines[pos] = txt[i];
			for (int j = 0; j < 1024; j++){ // ���������������з��ŵ�
				if (txt[j + i] != '\n'){ // �ж��������з�
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


// ����ָ�������еڼ���
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

// ���ض���������
inline int GetMemLinesNum(const char* txt){
	int ln = 1; // ������һ��
	for (int i = 0; i < strlen(txt); i++){ // ָ�����
		if (txt[i] == '\n'){
			ln += 1;
		}
	}
	return ln;
}


// �ļ����ŷָ�Զ���ȡ����������ݣ�����Ϊ��λ
// �ı����ı���С��Ŀ�꣬R_BF|R_BH
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

	if (mod == R_BF) { // ����ǰ��
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
				buffer[i-1] = '\0'; // �Զ�ȡ�������м�Ŀո��Լ��ֺź��������
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


// ��ȡ��ѡ�ļ���Ŀ¼
// Ŀ¼·��
inline char* GetMemPathLastRoot(const char* path){
	int pos = 0;
	int posE = -1; // ����ȷ��ֵ
	int len = strlen(path);
	char* temp = (char*)malloc(len + 2); // ����ռ�ʱ��Ҫ��1�洢���������ټ�1�洢��Ŀ¼
	if (temp == nullptr) { // �ж��Ƿ�����ռ�ʧ��
		return nullptr;
	}
	while (path[pos] != '\0'){
		if (path[pos] == '\\' || path[pos] == '/') { // �ж��Ƿ�Ϊ·���ָ���
			posE = pos;
		}
		pos += 1;
	}
	if (posE == -1) { // �ж��Ƿ��ҵ�·���ָ���
		temp[0] = '\0'; // ���û�ҵ����򷵻ؿ��ַ���
	}
	else {
		for (int i = 0; i <= posE; i++){ // ��Ҫ����·���ָ�������λ��֮ǰ���ַ�������·���ָ���
			temp[i] = path[i];
		}
		temp[posE + 1] = '\0'; // ����Ŀ¼����ַ���Ϊ������
	}
	return temp;
}

// ȥ��ע���ǰ׺
// path
inline char* RemoveMemPre(const char* path) {
	if (strncmp(path, "./", 2) == 0) { // ���ǰ׺��"./"
		path += 2; // ָ�������λ������"./"
	}
	int len = strlen(path);
	char* result = (char*)malloc(len + 1); // ����ռ�洢���ؽ��
	strncpy_s(result, len + 1, path, len); // ʹ�� strncpy_s ����ʣ����ַ�������������
	return result;
}

// ��ָ�����ݺ�����������
// ԭ��ַ��ָ��λ�ã�ָ������
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

// ɾ��ָ���������������
// ���ݣ���ʼλ�ã�����λ��
inline char* RemoveMemStr(char* str, int start, int end) {
	int len = strlen(str);
	if (start >= end || start >= len) return str;

	int remove_len = end - start + 1;
	int remain_len = len - remove_len;

	// �����µ��ڴ�ռ�
	char* new_str = (char*)malloc(remain_len + 1);

	// ����ǰ�벿��
	if (start > 0) {
		memcpy(new_str, str, start);
	}
	new_str[start] = '\0';

	// ���ƺ�벿��
	if (end < len - 1) {
		memcpy(new_str + start, str + end + 1, len - end - 1);
	}
	new_str[remain_len] = '\0';

	// �ͷ�ԭ�����ڴ�ռ�
	free(str);

	return new_str;
}

// �޸ĵ�ַ�����滻�ַ����滻�ַ�
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

// ����һ�е�����
inline std::string FindMemSTDS(std::string txt, char* target){
	size_t start_pos = 0, end_pos = 0;
	std::string rtxt = "None";
	start_pos = txt.find(target,0);
	if (start_pos != std::string::npos){ // ����ҵ���
		start_pos += strlen(target);
		end_pos = txt.find("\n", start_pos);
		if (end_pos == std::string::npos){
			end_pos = txt.find("\0", start_pos);
			if (end_pos == start_pos){ // ������Ҳ�����ֻ�ҵ�ǰ������
				end_pos = txt.find("\n", start_pos);
			}
		}
		rtxt = txt.substr(start_pos, end_pos - start_pos);
	}
	return rtxt;
}

// ɾ��һ������
inline bool DelMemLine(std::string& txt, char* target) {
	size_t start_pos = 0, end_pos = 0;
	start_pos = txt.find(target, 0);
	if (start_pos != std::string::npos) { // ����ҵ���
		end_pos = txt.find("\n", start_pos);
		if (end_pos == std::string::npos) {
			end_pos = txt.find("\0", start_pos);
		}
		end_pos++; // ��end_pos��������һ�п�ͷ
		size_t delete_len = end_pos - start_pos;
		if (start_pos > 0) { // ������ǵ�һ��
			start_pos--; // ��start_pos��������һ��ĩβ��\n
		}
		txt.erase(start_pos, delete_len); // ɾ������
		return true;
	}
	return false;
}