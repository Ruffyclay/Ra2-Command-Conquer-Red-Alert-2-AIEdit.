// Addnew.cpp : 实现文件
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "Addnew.h"
#include "afxdialogex.h"
#include "OtherTools.h"
#include "Army.h"
#include "AIT.h"
#include "Cscripts.h"
#include "TaskForces.h"

CAddnew add_wind;
// 标题
CString add_new_title;
// 添加类型指定
char add_type;
bool Add_new_init = false;
// CAddnew 对话框

IMPLEMENT_DYNAMIC(CAddnew, CDialogEx)

CAddnew::CAddnew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddnew::IDD, pParent)
{

}

CAddnew::~CAddnew()
{
}

void CAddnew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_add_name_edit);
	DDX_Control(pDX, IDC_COMBO1, m_add_type_ccb);
}


BEGIN_MESSAGE_MAP(CAddnew, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// 在编辑框中按下回车键
void CAddnew::OnOK()
{
	// 临时变量
	CString temp_str, temp_nana,tt;
	m_add_name_edit.GetWindowTextW(temp_str);
	if ((CStringToChar(temp_str) == (CString)"Ruffy") && (FilePath_main == "None")){
		testCheck("很高兴您能使用我的软件!感谢使用!\nNice to you use my program!Thank you for use!");
		OnCancel();
		return;
	}

	if (FilePath_main == "None"){
		testCheck("ヽ(#`Д´)ﾉo(▼皿▼メ;)o你小子隔着干啥???");
		OnCancel();
		return;
	}


	//  会让程序内存占用大幅度上升下降但是更加稳定，使用，不计较那么点内存
	 std::string temp_std;
	m_add_name_edit.GetWindowTextW(tt);
	add_new_title = temp_str;
	temp_str.Format(_T("%s"), tt);
	if (temp_str != (CString)""){
		char* pStr = NULL;
		add_type = m_add_type_ccb.GetCurSel();
		switch (add_type){
		case 0:{ // 处理: 触发
				   flag_add_ait = true;
				   temp_std = main_aitt;
				   main_aitt.clear();
				   temp_std += CStringToChar(temp_str);
				   temp_std += "=NoName,NoTeam,<all>,0,1,NoCheck,0000000002000000000000000000000000000000000000000000000000000000,0.000000,0.000000,0.000000,1,0,0,0,<none>,0,0,0\0";
				   main_aitt = GetMemLines(temp_std.data(),temp_std.size());
				   main_aitt += "\n";
				   temp_std.clear();
				   add_ait_title = add_new_title;
				   OnCancel();
				   break;
				}
		case 1:{ // 队伍
				   temp_std = main_team_num;
				   main_team_num.clear();
				   main_team_num_index += 1;
				   // 注册标题名
				   temp_nana.Format(_T("%d=%s"), main_team_num_index+start_regnum, temp_str);
				   temp_std += CStringToChar(temp_nana);
				   temp_std += "\n";
				   main_team_num = temp_std;
				   temp_std.clear();

				   // 注册定义
				   temp_std = main_team;
				   main_team.clear();
				   temp_nana.Format(_T("[%s]"),temp_str);
				   temp_std += CStringToChar(temp_nana);
				   FILE* fp;
				   char* temp_mod_team = NULL;
				   CString file_mod_path;
				   file_mod_path = (CString)"LoadTempTeamData.txt";
				   fopen_s(&fp, CStringToChar(file_mod_path), "r");
				   temp_std += "\n";
				   if (fp != NULL){
					   char* temp_P = NULL;
					   int size = GetPhySize(fp);
					   temp_mod_team = (char*)malloc(size);
					   int n = fread(temp_mod_team, 1, size, fp);
					   temp_mod_team[n] = '\0';
					   temp_P = GetMemLines(temp_mod_team, strlen(temp_mod_team));
					   temp_std += temp_P;
					   temp_std += "\n\n";
					   temp_P = NULL;
					   free(temp_mod_team);
				   }
				   else
					   temp_std += "\n";

				   main_team = temp_std;
				   temp_std.clear();
				   flag_add_team = true;
				   fclose(fp);
				   flag_updata_te = true;
				   OnCancel();
			break;
		}
		case 2:{ // 脚本
				   CString title;
				   title.Format(_T("[%s]\nName=Ruffy\n\n"), tt);
				   main_script += CStringToChar(title);
				   title.Format(_T("%d=%s\n"), Count_sc+start_regnum,tt);
				   main_script_num += CStringToChar(title);
				   add_title_sc = tt;
				   flag_add_sc = true;
				   OnCancel();
				   break;
		}
		case 3:{ // 人员
				   CString title;
				   title.Format(_T("[%s]\nName=Ruffy\nGroup=-1\n\n"), tt);
				   main_task += CStringToChar(title);
				   title.Format(_T("%d=%s\n"), Count_ta+start_regnum, tt);
				   main_task_num += CStringToChar(title);
				   add_title_ta = tt;
				   flag_add_task = true;
				   flag_updata_ta = true;
				   OnCancel();
				   break;
		}
		}
	}
}


void CAddnew::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (Add_new_init)
	{ // 触发 队伍 小队 脚本 0 - 3
		m_add_type_ccb.SetCurSel(add_type);
		m_add_name_edit.SetWindowTextW(_T(""));
	}
}
