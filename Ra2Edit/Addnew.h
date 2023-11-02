#pragma once
#include "afxwin.h"


// CAddnew 对话框

class CAddnew : public CDialogEx
{
	DECLARE_DYNAMIC(CAddnew)

public:
	CAddnew(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddnew();

// 对话框数据
	enum { IDD = IDD_NEWADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 新增内容的名称
	CEdit m_add_name_edit;
	// 新增内容的类型
	CComboBox m_add_type_ccb;
	virtual void OnOK();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
extern CAddnew add_wind;
extern char add_type;
extern bool Add_new_init;
extern CString add_new_title;
