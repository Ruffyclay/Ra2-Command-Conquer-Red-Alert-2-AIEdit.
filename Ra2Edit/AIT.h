#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CAIT 对话框

class CAIT : public CDialogEx
{
	DECLARE_DYNAMIC(CAIT)

public:
	CAIT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAIT();

// 对话框数据
	enum { IDD = IDD_AIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
	CListCtrl m_AIT_list;
	// 触发注释
	CStatic m_tip_txt;
	// Name内容
	CEdit m_name_edit;
	// 简单敌人可用
	CButton m_EasyEnemy_check;
	// 中等敌人可用
	CButton m_NormalEnemy_check;
	// 困难敌人可用
	CButton m_HardEnemy_check;
	// 指定国家
	CComboBox m_Country_ccb;
	// 科技等级
	CEdit m_Tech_edit;
	// 条件类型
	CComboBox m_atttype_ccb;
	// 条件单位
	CComboBox m_aittunit_ccb;
	// 条件数值
	CEdit m_value_edit;
	// 指定阵营
	CComboBox m_Unity_ccb;
	// 最低权重
	CComboBox m_lowaitt_ccb;
	// 基本权重
	CComboBox m_normalaitt_ccb;
	// 最高权重
	CComboBox m_heightaitt_ccb;
public:
	afx_msg void OnNMSetfocusAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAitList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// 作战队伍A
	CComboBox m_teamA_ccb;
	// 条件模式
	CComboBox m_aittmod_ccb;
public:
	afx_msg void OnLvnKeydownAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnCbnEditchangeCombo9();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnKillfocusCombo2();
	afx_msg void OnCbnDblclkCombo2();
	afx_msg void OnCbnEditchangeCombo2();
	afx_msg void OnCbnSetfocusCombo2();
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void RefreshControl(UINT uCtlID);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck4();
	// 自动跳转
	CButton m_autoturn_check;
private:
	// 跳转按钮
	CButton c_turn;
	// 自动保存
	//CButton m_autosave_ck;
};
extern CAIT AITWind;
extern void ReadAIT();
extern bool flag_add_ait;
extern CString add_ait_title;
extern CString flag_fresh_te_title;