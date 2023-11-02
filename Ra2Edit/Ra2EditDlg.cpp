
// Ra2EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "Ra2EditDlg.h"
#include "string.h"
#include <iostream>
#include "OtherTools.h"
#include "TaskForces.h"
#include "AIT.h"
#include "Army.h"
#include "Cscripts.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::vector<TeamTypes> teamtypes;
std::vector<Scripts> scripts;
std::vector<TaskForces> taskforces;
std::vector<AITriggerTypes> aitriggertypes;

std::string main_team_num;
std::string main_task_num;
std::string main_script_num;
std::string main_team;
std::string main_task;
std::string main_script;
std::string main_aitt;

// 新增内容类型以及名称声明
std::string main_add_txt;

//std::vector<cahr> 
// 函数声明区
CString FileFuction_Open();
CString FileFuction_Create();

int start_regnum = 0;

bool flag_updata_ta = false;
bool flag_updata_sc = false;
bool flag_updata_te = false;

bool flag_fresh_sc = false;
bool flag_fresh_ta = false;
bool flag_fresh_te = false;

bool flag_auto_check = false;

// 全局变量声明区
// 全程记录打开的主文件记录
CString FilePath_main = (CString)"None";
// 分文件指针结构体 Max=16，唯一指定
struct postion psPos[16];
//std::vector<TeamTypes> teamTypes;


// 根目录存储
CString FilePath_root;

// 变量声明区

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRa2EditDlg 对话框



CRa2EditDlg::CRa2EditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRa2EditDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRa2EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CHECK1, m_check_ck);
	//DDX_Control(pDX, IDC_EDIT1, m_txt_c);
	//DDX_Control(pDX, IDC_CHECK2, m_autoc);
	//DDX_Control(pDX, IDC_EDIT2, m_tip_txt);
	DDX_Control(pDX, IDC_CHECK3, m_check_regnum);
}

BEGIN_MESSAGE_MAP(CRa2EditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32773, &CRa2EditDlg::MenuFileOpen)
	ON_COMMAND(ID_32794, &CRa2EditDlg::On32794)
	ON_COMMAND(ID_32795, &CRa2EditDlg::HIDEWINDOWS)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32774, &CRa2EditDlg::SaveFile)
	ON_COMMAND(ID_32776, &CRa2EditDlg::ExitProgram)
	ON_BN_CLICKED(IDC_CHECK1, &CRa2EditDlg::OnBnClickedCheck1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CRa2EditDlg::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDC_CHECK3, &CRa2EditDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()

// CRa2EditDlg 消息处理程序

BOOL CRa2EditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码

	// 初始化父窗口菜单栏和悬浮提示
	menu_parent.LoadMenuW(IDR_MENU2);
	SetMenu(&menu_parent);
	
	

	// 获取客户区矩形
	CRect parentRect;
	GetClientRect(&parentRect);

	add_wind.Create(IDD_NEWADD, this);

	// 初始化小队配置窗口，显示在左上角
	ArmyWinD.Create(IDD_ARMY, this);
	CRect childRect;
	ArmyWinD.GetWindowRect(&childRect);
	int x = parentRect.left;
	int y = parentRect.top + 45;
	ArmyWinD.SetWindowPos(nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// 初始化脚本配置窗口，显示在右上角
	ScriptsWind.Create(IDD_SCRIPTS, this);
	ScriptsWind.GetWindowRect(&childRect);
	x = parentRect.right - childRect.Width();
	y = parentRect.top + 45;
	ScriptsWind.SetWindowPos(nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// 初始化派遣部队配置窗口，显示在左下角
	TaskForceWind.Create(IDD_TASK_DLG, this);
	TaskForceWind.GetWindowRect(&childRect);
	x = parentRect.left;
	y = parentRect.bottom - childRect.Height();
	TaskForceWind.SetWindowPos(nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// 初始化触发配置创库，显示在右下角
	AITWind.Create(IDD_AIT_DLG, this);
	AITWind.GetWindowRect(&childRect);
	x = parentRect.right - childRect.Width();
	y = parentRect.bottom - childRect.Height();
	AITWind.SetWindowPos(nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRa2EditDlg::RefreshControl(UINT uCtlID)
{
	CRect rc;
	GetDlgItem(uCtlID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
	UpdateWindow();
}

void CRa2EditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRa2EditDlg::OnPaint()
{
	m_check_regnum.SetCheck(BST_CHECKED);
	flag_auto_check = true;
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRa2EditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 菜单打开文件
void CRa2EditDlg::MenuFileOpen()
{
	if ((FilePath_main != (CString)"None") && (FilePath_main != (CString)"Nones")){
		AfxMessageBox(_T("不准重复打开文件，每次启动只能打开一次"));
	}
	else{
		main_aitt.clear();
		main_script.clear();
		main_script_num.clear();
		main_team.clear();
		main_team_num.clear();
		main_aitt.clear();
		CString Cfile_p;
		FilePath_main = FileFuction_Open();
		if (FilePath_main != (CString)"None"){
			// 获取目录根目录
			char* temp_path_main = GetMemPathLastRoot(CStringToChar(FilePath_main));
			FilePath_root = temp_path_main;
			// 检查文件是否能否正常打开
			fopen_s(&psPos[0].fp, CStringToChar(FilePath_main), "r");
			if (psPos[0].fp != NULL){ // 此时psPos[0].pos内容指向了正确的头地址
				// 将文件中的内容全部读取到内存中
				psPos[0].size = GetPhySize(psPos[0].fp);
				psPos[0].pos = (char*)malloc(psPos[0].size + 1); // 刷新指针指向的地址确保不会越界
				psPos[0].size = fread(psPos[0].pos, 1, psPos[0].size, psPos[0].fp);
				psPos[0].pos[psPos[0].size] = '\0'; // 字符串结束符
				fclose(psPos[0].fp);

				// 搜索包含其他文件的声明的内容
				int poss = TxtMemFind(psPos[0].size, 0, psPos[0].pos, "[#include]", C_END);
				if (poss != -1){
					int pose = TxtMemFind(psPos[0].size, poss, psPos[0].pos, "[", C_HEAD);
					if (pose == -1)pose = TxtMemFind(psPos[0].size, poss, psPos[0].pos, ";", C_HEAD);
					char* temp = ReadMemContent(psPos[0].pos, psPos[0].size, poss, pose);
					int len = strlen(temp);
					char* tempss = GetMemLines(temp, len); // 与加载进来的源文件无关
					int linenum = GetMemLinesNum(tempss);
					char* temp_check = NULL;

					// 将读取到的包含路径的文件依次读取到结构体指针中去
					for (int i = 1; i < linenum; i++) {
						temp_check = GetMemLine(tempss, i); // 时时刻刻变换的不稳定的临时指针
						if (CheckRegedit(temp_check)) {
							psPos[i].pos = RemoveMemPre((GetTargetString(temp_check, strlen(temp_check), "=", R_BH)));
							ReplaceMemChr(psPos[i].pos, '/', '\\'); // 纠正符号
							// 拼接路径
							Cfile_p.Format(_T("%s%s"), (CString)temp_path_main, (CString)psPos[i].pos);
							CString tip = Cfile_p;
							fopen_s(&psPos[i].fp, CStringToChar(Cfile_p), "r");
							psPos[i].pos = "None";
							if (psPos[i].fp != NULL) {
								psPos[i].size = GetPhySize(psPos[i].fp);
								psPos[i].pos = (char*)malloc(psPos[i].size + 1);
								int n = fread(psPos[i].pos, 1, psPos[i].size, psPos[i].fp);
								psPos[i].pos[n] = '\0';
								// 释放使用权
								fclose(psPos[i].fp);
							}
							if (psPos[i].pos == "None"){
								if (psPos[i].pos == "None"){
									tip.Format(_T("%s\n%s"), tip, (CString)"读取失败!请检查:\n文件是否存在\n编码是否为ANSI格式\n包含文件名是否输入错误等信息");
									AfxMessageBox(tip);
								}
							}
						}
					}
				}
			}
			else{
				testCheck("文件无法打开，请检查是否已被占用!");
			}
			ReadTeam(); // 读取队伍配置
			ReadScript(); // 读取脚本配置
			ReadTask(); // 读取派遣队伍配置
			ReadAIT(); // 读取触发配置
			ArmyWinD.ShowWindow(SW_HIDE);
			ScriptsWind.ShowWindow(SW_HIDE);
			TaskForceWind.ShowWindow(SW_HIDE);
			AITWind.ShowWindow(SW_HIDE);
			for (int i = 0; i < 16; i++){
				if (psPos[i].pos != "None"){
					free(psPos[i].pos);
					psPos[i].size = 0;
				}
			}


			// 打开分窗口
			ArmyWinD.ShowWindow(SW_NORMAL);
			ScriptsWind.ShowWindow(SW_NORMAL);
			TaskForceWind.ShowWindow(SW_NORMAL);
			AITWind.ShowWindow(SW_NORMAL);
		}
	}
}

// 显示所有窗口
void CRa2EditDlg::On32794()
{
	// TODO:  在此添加命令处理程序代码
	ArmyWinD.ShowWindow(SW_NORMAL);
	ScriptsWind.ShowWindow(SW_NORMAL);
	TaskForceWind.ShowWindow(SW_NORMAL);
	AITWind.ShowWindow(SW_NORMAL);
}


void CRa2EditDlg::HIDEWINDOWS()
{
	// TODO:  在此添加命令处理程序代码
	ArmyWinD.ShowWindow(SW_HIDE);
	ScriptsWind.ShowWindow(SW_HIDE);
	TaskForceWind.ShowWindow(SW_HIDE);
	AITWind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_HIDE);
}


void CRa2EditDlg::SaveFile()
{
	// TODO:  在此添加命令处理程序代码
	if (FilePath_main == "None" || FilePath_main == "Nones"){
		AfxMessageBox(_T("没检测到任何内容X_X"));
		return;
	}
	FILE* fp;
	fopen_s(&fp,CStringToChar(FilePath_main), "r");
	CString dir_root;
	if (fp != NULL){
		fclose(fp);
		dir_root = (CString)GetMemPathLastRoot(CStringToChar(FilePath_main));
		dir_root = dir_root + _T("+AI\\");
		int nRet = MessageBox(_T("是否分文件保存?⊙(・◇・)？"), _T("保存提示"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nRet == IDYES)
		{
			if (!CreateDirectory((CString)dir_root, NULL))
			{
				DWORD dwError = GetLastError();
				if (dwError != ERROR_ALREADY_EXISTS)
				{
					AfxMessageBox(_T("保存文件失败!T_T"));
					return;
				}
				// 保存文件代码
				CString temp_path;
				char* content = "[#include]\n0=./+AI/TaskForce.ini\n1=./+AI/TeamTypes.ini\n2=./+AI/Scripts.ini\n3=./+AI/AITriggerTypes.ini\n";
				fopen_s(&fp, CStringToChar(FilePath_main), "w");
				fwrite(content, 1, strlen(content), fp);
				fclose(fp);

				temp_path=dir_root+_T("TeamTypes.ini");
				fopen_s(&fp, CStringToChar(temp_path), "w");
				fwrite(main_team_num.data(), 1, main_team_num.size(), fp);
				fwrite(main_team.data(), 1, main_team.size(), fp);
				fclose(fp);

				temp_path = dir_root + _T("TaskForce.ini");
				fopen_s(&fp, CStringToChar(temp_path), "w");
				fwrite(main_task_num.data(), 1, main_task_num.size(), fp);
				fwrite(main_task.data(), 1, main_task.size(), fp);
				fclose(fp);

				temp_path = dir_root + _T("Scripts.ini");
				fopen_s(&fp, CStringToChar(temp_path), "w");
				fwrite(main_script_num.data(), 1, main_script_num.size(), fp);
				fwrite(main_script.data(), 1, main_script.size(), fp);
				fclose(fp);

				temp_path = dir_root + _T("AITriggerTypes.ini");
				fopen_s(&fp, CStringToChar(temp_path), "w");
				fwrite(main_aitt.data(), 1, main_aitt.size(), fp);
				fclose(fp);

			}
		}
		else if(nRet == IDNO)
		{
			fopen_s(&fp, CStringToChar(FilePath_main), "w");
			fwrite(main_task_num.data(), 1, main_task_num.size(), fp);
			fwrite(main_task.data(), 1, main_task.size(), fp);
			fwrite(main_script_num.data(), 1, main_script_num.size(), fp);
			fwrite(main_script.data(), 1, main_script.size(), fp);
			fwrite(main_team_num.data(), 1, main_team_num.size(), fp);
			fwrite(main_team.data(), 1, main_team.size(), fp);
			fwrite("\n", 1, 1, fp);
			fwrite(main_aitt.data(), 1, main_aitt.size(), fp);
			fclose(fp);
		}
		else return;
	}
}


void CRa2EditDlg::ExitProgram()
{
	if (FilePath_main != "None"&&FilePath_main != "Nones"){
		int nRet = MessageBox(_T("请注意，有文件未保存∑(っ°Д°;)っ\n是否继续退出(✪ω✪)"), _T("Fight!!(ｏ^-^)~''☆ミ☆ミ"), MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nRet == IDYES) {
			exit(0);
		}
	}
	else{
		exit(0);
	}
}

void CRa2EditDlg::OnClose()
{
	if (FilePath_main != "None"&&FilePath_main != "Nones"){
		if (AfxMessageBox(_T("请确保文件已经做了正确的操作\n本人很懒没做这块\no(╥﹏╥)o"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			exit(0);
		}
		else{
			return;
		}
	}
	exit(0);
}

void CRa2EditDlg::OnBnClickedCheck1()
{

}


void CRa2EditDlg::OnEnKillfocusEdit1()
{
	CString temp;
	m_txt_c.GetWindowTextW(temp);
	start_regnum = numtoi(temp);
}




void CRa2EditDlg::OnBnClickedCheck3()
{
	if (m_check_regnum.GetCheck() == BST_CHECKED){
		flag_auto_check = true;
	}
	else flag_auto_check = false;
}
