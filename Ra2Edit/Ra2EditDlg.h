
// Ra2EditDlg.h : 头文件
//

#pragma once

#include <vector>
#include "afxwin.h"

#define Max_Size 1024

// 指针结构体
struct postion{
	char* pos = NULL;
	FILE* fp = NULL;
	int size = 0;
};

// 队伍配置数据
struct TeamTypes{
	int num = -1;
	char* title = NULL;
	char* note = NULL;
	char* txt = NULL;
};

// 脚本配置数据
struct Scripts{
	int num = -1;
	char* title = NULL;
	char* note = NULL;
	char* txt = NULL;
};

// 派遣部队配置数据
struct TaskForces{
	int num = -1;
	char* title= NULL; // 标题
	char* note = NULL; // 注释
	char* txt = NULL; // 内容
};

struct AITriggerTypes{
	char* title = NULL; // 标题
	char* txt = NULL; // 内容
};

// 

// CRa2EditDlg 对话框
class CRa2EditDlg : public CDialogEx
{
// 构造
public:
	CRa2EditDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_RA2EDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 检查是否自动检查
	CButton m_check_ck;
	CEdit m_txt_c;
public:
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck2();
private:
	// 自动检测开始注册号	// 自动检测开始注册号
	CButton m_check_regnum;
public:
	afx_msg void OnBnClickedCheck3();
};

extern CString FilePath_main;
extern struct postion psPos[16];
extern CString FilePath_root;

// 主要保存文件专用
extern std::string main_team_num;
extern std::string main_task_num;
extern std::string main_script_num;
extern std::string main_team;
extern std::string main_task;
extern std::string main_script;
extern std::string main_aitt;

// 新增内容 默认为None;
extern std::string main_add_txt;


extern std::vector<TeamTypes> teamtypes;
extern std::vector<Scripts> scripts;
extern std::vector<TaskForces> taskforces;
extern std::vector<AITriggerTypes> aitriggertypes;

// 更新下拉框标志符 成员 脚本 队伍
extern bool flag_updata_ta;
extern bool flag_updata_sc;
extern bool flag_updata_te;

// 刷新页面 和 刷新的内容标题
extern bool flag_fresh_sc;
extern bool flag_fresh_ta;
extern bool flag_fresh_te;

extern int start_regnum;
extern bool flag_auto_check;
