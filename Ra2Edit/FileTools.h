#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHECK_HEAD 0
#define CHECK_END 1

/*
* �ı��ļ��Ĳ�����Ҫ�ǲ����ļ�ָ��
* FILE *fp; // �ṹ��
* fopen_s(&fp, filename, "r+"); ���ļ�
* fseek(�ļ�ָ��,ƫ����,ƫ�Ʒ�ʽ)
* �ļ�ָ���������ƫ�Ʒ�ʽSEEK_CUR,SEEK_SET,SEEK_END�ֱ��ǵ�ǰ����ͷ����β
* fwrite(����, ÿ��д��Ĵ�С(һ��Ϊ1)��д�����(һ����strlen()��sizeof())
* , �ļ�ָ��);
* fread(������ݵ�����,ÿ��д��Ĵ�С(һ��Ϊsizeof(����[0])
* ����ȡ�Ĵ���(һ��Ϊsizeof(����)),�ļ�ָ��))
* fputs��fgetc����fputc��fgetc�ֱ����ڵ�ǰָ��ĺ���д����ȡ�ַ���
* ��������1���ַ���һ���ȡ����һ���ַ�����
* ftell()�������Ի�ȡ�ļ��Ĵ�С�Ӷ����Զ�̬�����ڴ�,malloc()��ʹ�������Ҫ��free(����)�ͷű�������ڴ�
*/

/*
* str2��str1������λ�� CHECK_HEAD ��ǰ CHECK_END �ں�
*/
inline int strinstr(const char* str1, const char* str2, int mode) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 < len2) {
		return -1;
	}
	for (int i = 0; i <= len1 - len2; i++) {
		int j = 0;
		while (j < len2 && str1[i + j] == str2[j]) {
			j++;
		}
		if ((j == len2)&&(mode == 1)) {
			return i + strlen(str2) + 1;
		}
		else if ((j == len2) && (mode == 0)){
			return i;
		}
	}
	return -1;
}
inline int count_valid_elements(char *arr, int len, char null) { // ͳ��������Чλ�ĸ���,null��ʡ��ֵ
	int count = 0;
	CString message;
	for (int i = 0; i < len; i++) {
		if (arr[i] != null) {  // �ж�
			count++;
		}
	}
	message.Format(_T("%c,%d"),arr,count);
	AfxMessageBox(message);
	return count;
}

// �ĳ��ڴ��ȡ��ʽ
// ��ȡָ����������� fp,posStart posEnd
inline CString GetFileContent(const char* text, long int posStart, long int posEnd) {
	if (text=="" || posStart > posEnd) {
		return (CStringW)"";
	}

	// ��ȡ�ļ���С
	long fileSize = strlen(text);

	// У���ļ�ָ����ʼ��ͽ��������Ч��
	if (posStart >= fileSize || posEnd >= fileSize) {
		return (CString)"";
	}

	// ������Ҫ��ȡ���ֽ���
	int numBytes = posEnd - posStart;

	// �����㹻���ڴ�洢�ļ�����
	char* content = (char*)malloc(numBytes + 1);
	if (!content) {
		return (CString)"";
	}

	// ��ȡ�ļ�����
	for (long i = posStart; i < posEnd; i++)
	{
		content[i] = text[posStart + i];
	}

	// ����ַ���������
	content[numBytes] = '\0';

	// ת��ΪCString����
	CString str(content);

	// �ͷ��ڴ�
	//free(content);

	return str;
}



// ���ļ�����ָ�������������׷������ �򿪵��ļ�ָ�� , ָ�������ݣ�׷�ӵ�����
inline void file_append_below(FILE* fp, long int pos, const char data[]) {
	char line[1024] = { 0 };
	char* buffer = NULL;
	long file_size;
	long countpos = 0; // ͳ����ƫ�ƴ���

	fseek(fp, 0L, SEEK_END); // ����궨λ��ĩβ
	file_size = ftell(fp); // ��ȡ�ļ��Ĵ�С
	rewind(fp); // �����ļ�ָ��
	if (pos != -1) { // ����ҵ���ָ��������
		// ������ƶ���ָ��λ�ã�Ȼ����ʣ�µ����ݱ��浽������
		fseek(fp, pos, SEEK_SET);
		buffer = (char*)malloc(file_size - pos + 1); // ��̬�����ڴ�
		int n = fread(buffer, sizeof(char), file_size - pos, fp);
		buffer[n] = '\0'; // ��ĩβ���'\0'��Ϊ�ַ�����β

		// ���µ����ļ�ָ�룬д�����ݵ��ļ�
		fseek(fp, pos, SEEK_SET);
		fwrite(data, sizeof(char), strlen(data), fp);

		// ��������󣬽���ʧ�����ݲ���ĩβ
		fwrite(buffer, 1, n, fp);
	}
	free(buffer);
}

// ����ָ���ַ�������������ָ��λ��
// ָ���ļ�ָ��(�Ѵ�), ָ��Ŀ��, ָ����ʼλ��,����ģʽCHECK_HEAD|CHECK_END
inline long file_find_target(FILE* fp, const char* target, long int pos, int mode){
	bool flag = false;
	long countpos = pos; // ָ�뿪ʼ��λ��
	if (fp != NULL){ // ����ļ�ָ�����
		long file_size;
		int len = strlen(target); // Ŀ�곤��
		fseek(fp, 0L, SEEK_END);
		file_size = ftell(fp); // �����ļ��ĳ���
		rewind(fp);
		fseek(fp, 0L, SEEK_SET);

		CString mes;

		// ���ļ����ݼ��ص��ڴ���ж�д����
		char* buffer = NULL;
		buffer = (char*)malloc(file_size + 1);
		int n = fread(buffer, sizeof(char), file_size, fp);
		buffer[n] = '\0'; // ĩβ��������,��ʱbuffer��С�������ļ���С��Ϊ�յ�����
		fwrite(buffer, 1, n, fp); // buffer����ָ��ȫ�����ݣ���ʱָ��λ��Ϊ0

		//testCheck(get_last_dir(CStringToChar(FilePath_main)));

		if (strlen(target) > file_size) return -1;
		while (len + countpos <= file_size){ // �����������ܿ�ʼ
			if (buffer[countpos] == target[0]){ // ��һ���ַ���ȷ
				flag = true;
				for (int i = 0; i < len; i++){ // ���ʣ�µ��ַ�
					// �ڴ��ڼ䣬ָ�벻�仯��ֻƫ��
					mes.Format(_T("%d,%c,%c"), countpos,buffer[countpos + i],target[i]);
					if (buffer[countpos + i] != target[i]){
						flag = false;
						break; // ����ѭ��
					}
					AfxMessageBox(mes);
				}
			}
			if (flag){
				break;
			}
			countpos += 1;
		}
		free(buffer);

		if ((flag) && (mode == 1)){ // ����β��
			return (long int)countpos + strlen(target) + 1;
		}
		else if ((flag) && (mode == 0)){ // ����ͷ��
			return (long int)countpos - 1;
		}
		else{ // ���ش���
			return -1;
		}
	}
}

inline CString file_checkinclude_con(FILE* fp){
	long poss, pose,file_size;
	char* txt = NULL;
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp); // �����ļ��ĳ���
	txt = (char*)malloc(file_size + 1);
	rewind(fp);
	fread(txt, sizeof(char), file_size, fp);
	txt[file_size] = '\0';
	poss = file_find_target(fp, "[#include]", 0, CHECK_END);
	pose = file_find_target(fp, "[", poss, CHECK_HEAD);
	return GetFileContent(txt,poss,pose);
}

