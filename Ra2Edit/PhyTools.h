#pragma once
#include "stdafx.h"

inline long GetPhySize(FILE* fp){
	if (fp != NULL){
		long file_size;
		fseek(fp, 0L, SEEK_END);
		file_size = ftell(fp); // �����ļ��ĳ���
		rewind(fp);
		fseek(fp, 0L, SEEK_SET);
		return file_size;
	}
	return -1;
}

// ���������ļ� Դ�ļ�����, �����ļ�����
inline void ReadAndBackup(const char* resule, const char* filebackup){
	FILE* fp = NULL;
	FILE* bck = NULL;
	char* buffer = NULL;
	long file_size;

	fopen_s(&fp, resule, "r"); // Դ�ļ�
	fopen_s(&bck, filebackup, "w+"); // ���ݵ��ļ�
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(file_size + 1);
	int n = fread(buffer, sizeof(char), file_size, fp);
	buffer[n] = '\0';

	fwrite("\0", sizeof(char), strlen("\0"), bck);
	fwrite(buffer, 1, n, bck);

	fclose(fp);
	fclose(bck);
	free(buffer);
	return;

}