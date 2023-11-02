
// Ra2EditDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>
#include "afxwin.h"

#define Max_Size 1024

// ָ��ṹ��
struct postion{
	char* pos = NULL;
	FILE* fp = NULL;
	int size = 0;
};

// ������������
struct TeamTypes{
	int num = -1;
	char* title = NULL;
	char* note = NULL;
	char* txt = NULL;
};

// �ű���������
struct Scripts{
	int num = -1;
	char* title = NULL;
	char* note = NULL;
	char* txt = NULL;
};

// ��ǲ������������
struct TaskForces{
	int num = -1;
	char* title= NULL; // ����
	char* note = NULL; // ע��
	char* txt = NULL; // ����
};

struct AITriggerTypes{
	char* title = NULL; // ����
	char* txt = NULL; // ����
};

// 

// CRa2EditDlg �Ի���
class CRa2EditDlg : public CDialogEx
{
// ����
public:
	CRa2EditDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_RA2EDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CMenu menu_parent;

public:
	afx_msg void MenuFileOpen();
	afx_msg void On32794();
	afx_msg void HIDEWINDOWS();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void CreateNewFile();
	afx_msg void SaveFile();
	afx_msg void ExitProgram();
	afx_msg void RefreshControl(UINT uCtlID);
	//UINT_PTR m_nTimerID;
	afx_msg void OnBnClickedCheck1();
private:
	// ����Ƿ��Զ����
	CButton m_check_ck;
	CEdit m_txt_c;
public:
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck2();
private:
	// �Զ���⿪ʼע���	// �Զ���⿪ʼע���
	CButton m_check_regnum;
public:
	afx_msg void OnBnClickedCheck3();
};

extern CString FilePath_main;
extern struct postion psPos[16];
extern CString FilePath_root;

// ��Ҫ�����ļ�ר��
extern std::string main_team_num;
extern std::string main_task_num;
extern std::string main_script_num;
extern std::string main_team;
extern std::string main_task;
extern std::string main_script;
extern std::string main_aitt;

// �������� Ĭ��ΪNone;
extern std::string main_add_txt;


extern std::vector<TeamTypes> teamtypes;
extern std::vector<Scripts> scripts;
extern std::vector<TaskForces> taskforces;
extern std::vector<AITriggerTypes> aitriggertypes;

// �����������־�� ��Ա �ű� ����
extern bool flag_updata_ta;
extern bool flag_updata_sc;
extern bool flag_updata_te;

// ˢ��ҳ�� �� ˢ�µ����ݱ���
extern bool flag_fresh_sc;
extern bool flag_fresh_ta;
extern bool flag_fresh_te;

extern int start_regnum;
extern bool flag_auto_check;
