#pragma once

#include "Ra2EditDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// CArmy 对话框


class CArmy : public CDialogEx
{
	DECLARE_DYNAMIC(CArmy)

public:
	CArmy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArmy();

// 对话框数据
	enum { IDD = IDD_ARMY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	CRa2EditDlg* pRa2EditDlg;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	// 小队内容列表
	CListCtrl m_Army_list;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLvnColumnclickArmyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeName();
	virtual void OnOK();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMSetfocusArmyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusArmyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickArmyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownArmyList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// 被打不还击
	CButton m_Suicide_check;
	// 移动中尝试主动攻击
	CButton m_Aggressive_check;
	// 优先等级
	CEdit m_Priority_edit;
	// 最大共存数
	CEdit m_Max_edit;
	// 注释
	CEdit m_Name_edit;
	// 预造
	CButton m_Prebuild_check;
	// 成员可被优先级更高的队伍调用
	CButton m_AreTeamMembersRecruitable_check;
	// 优先建造
	CButton m_Autocreate_check;
	// 回避火力点
	CButton m_AvoidThreats_check;
	// 完成任务后不再使用
	CButton m_LooseRecruit_check;
	// 哀叫
	CButton m_Whiner_check;
	// 使用空降
	CButton m_Droppod_check;
	// 烦恼效果
	CButton m_Annoyance_check;
	// 使用载具
	CButton m_UseTransportOrigin_check;
	// 增员部队
	CButton m_Reinforce_check;
	// 离子免疫
	CButton m_IonImmune_check;
	// 只攻击指定对手
	CButton m_OnlyTargetHouseEnemy_check;
	// 运载
	CButton m_Full_check;
	// 减速
	CButton m_GuardSlower_check;
	// 可装载
	CButton m_Loadable_check;
	// 载具返回
	CButton m_TransportsReturnOnUnload_check;
	// 防御部队
	CButton m_IsBaseDefense_check;
	// 派遣部队列表选择
	CComboBox m_task_ccb;
	// 脚本选择
	CComboBox m_script_ccb;
	// 只读派遣队伍大概内容
	/*CEdit m_task_rd;*/
	// 只读脚本内容
	/*CEdit m_script_rd;*/
	
	// 允许调用其队伍成员
	CButton m_Recruiter_check;
public:
	//afx_msg void OnCbnEditupdateCombo1();
	afx_msg void OnBnClickedButton3();

private:
	// 提示内容
	CStatic Ctip_txt;
public:
	afx_msg void OnBnClickedButton5();
	//afx_msg void OnBnClickedButton4();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton4();
private:
	// 自动跳转
	CButton m_autoturn_check;
public:
	afx_msg void OnBnClickedCheck20();
private:
	// 跳转按钮
	CButton m_turn_bt;
	// 自动保存
	//CButton m_autosave_ck;
};
extern CArmy ArmyWinD;
extern void ReadTeam();
extern int main_team_num_index;
extern bool flag_add_team;
extern CString flag_fresh_sc_title, flag_fresh_ta_title;