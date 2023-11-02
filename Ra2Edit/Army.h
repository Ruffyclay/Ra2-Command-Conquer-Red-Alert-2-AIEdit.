#pragma once

#include "Ra2EditDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// CArmy �Ի���


class CArmy : public CDialogEx
{
	DECLARE_DYNAMIC(CArmy)

public:
	CArmy(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CArmy();

// �Ի�������
	enum { IDD = IDD_ARMY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
private:
	CRa2EditDlg* pRa2EditDlg;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	// С�������б�
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
	// ���򲻻���
	CButton m_Suicide_check;
	// �ƶ��г�����������
	CButton m_Aggressive_check;
	// ���ȵȼ�
	CEdit m_Priority_edit;
	// ��󹲴���
	CEdit m_Max_edit;
	// ע��
	CEdit m_Name_edit;
	// Ԥ��
	CButton m_Prebuild_check;
	// ��Ա�ɱ����ȼ����ߵĶ������
	CButton m_AreTeamMembersRecruitable_check;
	// ���Ƚ���
	CButton m_Autocreate_check;
	// �رܻ�����
	CButton m_AvoidThreats_check;
	// ����������ʹ��
	CButton m_LooseRecruit_check;
	// ����
	CButton m_Whiner_check;
	// ʹ�ÿս�
	CButton m_Droppod_check;
	// ����Ч��
	CButton m_Annoyance_check;
	// ʹ���ؾ�
	CButton m_UseTransportOrigin_check;
	// ��Ա����
	CButton m_Reinforce_check;
	// ��������
	CButton m_IonImmune_check;
	// ֻ����ָ������
	CButton m_OnlyTargetHouseEnemy_check;
	// ����
	CButton m_Full_check;
	// ����
	CButton m_GuardSlower_check;
	// ��װ��
	CButton m_Loadable_check;
	// �ؾ߷���
	CButton m_TransportsReturnOnUnload_check;
	// ��������
	CButton m_IsBaseDefense_check;
	// ��ǲ�����б�ѡ��
	CComboBox m_task_ccb;
	// �ű�ѡ��
	CComboBox m_script_ccb;
	// ֻ����ǲ����������
	/*CEdit m_task_rd;*/
	// ֻ���ű�����
	/*CEdit m_script_rd;*/
	
	// �������������Ա
	CButton m_Recruiter_check;
public:
	//afx_msg void OnCbnEditupdateCombo1();
	afx_msg void OnBnClickedButton3();

private:
	// ��ʾ����
	CStatic Ctip_txt;
public:
	afx_msg void OnBnClickedButton5();
	//afx_msg void OnBnClickedButton4();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton4();
private:
	// �Զ���ת
	CButton m_autoturn_check;
public:
	afx_msg void OnBnClickedCheck20();
private:
	// ��ת��ť
	CButton m_turn_bt;
	// �Զ�����
	//CButton m_autosave_ck;
};
extern CArmy ArmyWinD;
extern void ReadTeam();
extern int main_team_num_index;
extern bool flag_add_team;
extern CString flag_fresh_sc_title, flag_fresh_ta_title;