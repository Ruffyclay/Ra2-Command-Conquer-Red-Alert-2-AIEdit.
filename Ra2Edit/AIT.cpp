// AIT.cpp : 实现文件
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "AIT.h"
#include "afxdialogex.h"
#include "OtherTools.h"
#include "Army.h"

CAIT AITWind;
CString flag_fresh_te_title;
// CAIT 对话框
// 是否执行增加内容操作
bool flag_add_ait = false;
CString add_ait_title;
int count_ait_num;
bool fresh_ait_list = false;

// 读取值得哈希表
const std::unordered_map<std::string, int> chineseToNumber{
	{ "无条件", -1 },
	{ "敌方拥有(前提)指定单位", 0 },
	{ "我方拥有(前提)指定单位", 1 },
	{ "敌方电力为黄", 2 },
	{ "敌方电力为红", 3 },
	{ "敌方金钱大于或等于", 4 },
	{ "本方铁幕装置就绪", 5 },
	{ "本方超时空传送就绪", 6 },
	{ "中立方拥有(前提)指定单位", 7 }
};

int getmode(const std::string& chinese) {
	auto iter = chineseToNumber.find(chinese);
	if (iter != chineseToNumber.end()) {
		return iter->second;
	}
	else {
		return -1; // 如果不指定 默认无条件
	}
}

std::unordered_map<std::string, int> Ruffylistmod{
	{ "小于", 0 },
	{ "小于等于", 1 },
	{ "等于", 2 },
	{ "大于等于", 3 },
	{ "大于", 4 },
	{ "不等于", 5 }
};

int Ruffyfind(const std::string& key) {
	if (Ruffylistmod.count(key) > 0) {
		return Ruffylistmod[key];
	}
	return 5; // 没有找到对应的值，返回默认值
}


// 读取AITT内容
void ReadAIT(){
	int poss, pose, len, len_temp_txt, psN = 0;
	char* temp, *temp_txt, *Line_num_txt = NULL;
	CString num, title, content, note, txt;
	while (psPos[psN].pos != NULL){
		poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, "[AITriggerTypes]", C_END);
		if (poss == -1){ psN += 1; continue; }
		pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
		if (pose == -1){ // 如果找不到内容,直接尝试找末尾
			pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, '\0', C_HEAD);
		}
		if (poss == pose){ psN += 1; continue; }
		// 读取区域中间的值
		temp = ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
		len = strlen(temp);
		// 除去中间空格并保存
		temp_txt = GetMemLines(temp, len); // 与加载进来的源文件无关
		len_temp_txt = GetMemLinesNum(temp);
		
		// 开始载入触发内容
		struct AITriggerTypes temp_t;
		for (int i = 0; i < len_temp_txt; i++)
		{
			// 循环读取每一行内容然后加入到内存
			Line_num_txt = GetMemLine(temp_txt, i);
			// 检索内容是否有错误
			if (CheckAITedit(Line_num_txt)){
				title = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BF);
				txt = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BH);

				// 保存所有的内容,动态变量
				temp_t.title = CStringToChar(title);
				temp_t.txt = CStringToChar(txt);
				aitriggertypes.push_back(temp_t);
			}
		}
	psN += 1;
	}
	main_aitt.reserve(strlen("[AITriggerTypes]\n"));
	main_aitt.insert(main_aitt.end(), "[AITriggerTypes]\n", "[AITriggerTypes]\n" + std::strlen("[AITriggerTypes]\n"));
	int lenCount = 0;
	count_ait_num = aitriggertypes.size() - 1;
	for (int i = 0; i < aitriggertypes.size(); i++) {
		std::stringstream ss;
		ss << aitriggertypes[i].title << "=" << aitriggertypes[i].txt;
		std::string temp_str = ss.str();
		lenCount += temp_str.length();
		main_aitt.reserve(lenCount + 1); // 为换行符预留一个字节
		main_aitt.insert(main_aitt.end(), temp_str.c_str(), temp_str.c_str() + temp_str.length());
		main_aitt.push_back('\n');
		aitriggertypes[i].title = NULL;
		aitriggertypes[i].txt = NULL;
	}
	main_aitt.push_back('\n');
}

void CAIT::RefreshControl(UINT uCtlID)
{
	CRect rc;
	GetDlgItem(uCtlID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
	UpdateWindow();
}

IMPLEMENT_DYNAMIC(CAIT, CDialogEx)

CAIT::CAIT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAIT::IDD, pParent)
{

}

CAIT::~CAIT()
{
}

void CAIT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AIT_LIST, m_AIT_list);
	DDX_Control(pDX, IDC_STIP, m_tip_txt);
	DDX_Control(pDX, IDC_EDIT1, m_name_edit);
	DDX_Control(pDX, IDC_CHECK1, m_EasyEnemy_check);
	DDX_Control(pDX, IDC_CHECK2, m_NormalEnemy_check);
	DDX_Control(pDX, IDC_CHECK3, m_HardEnemy_check);
	DDX_Control(pDX, IDC_COMBO1, m_Country_ccb);
	DDX_Control(pDX, IDC_EDIT2, m_Tech_edit);
	DDX_Control(pDX, IDC_COMBO2, m_atttype_ccb);
	DDX_Control(pDX, IDC_COMBO7, m_aittunit_ccb);
	DDX_Control(pDX, IDC_EDIT6, m_value_edit);
	DDX_Control(pDX, IDC_COMBO6, m_Unity_ccb);
	DDX_Control(pDX, IDC_COMBO3, m_lowaitt_ccb);
	DDX_Control(pDX, IDC_COMBO4, m_normalaitt_ccb);
	DDX_Control(pDX, IDC_COMBO5, m_heightaitt_ccb);
	DDX_Control(pDX, IDC_COMBO8, m_teamA_ccb);
	DDX_Control(pDX, IDC_COMBO9, m_aittmod_ccb);
	DDX_Control(pDX, IDC_CHECK4, m_autoturn_check);
	DDX_Control(pDX, IDC_BUTTON5, c_turn);
	//DDX_Control(pDX, IDC_CHECK5, m_autosave_ck);
}


BEGIN_MESSAGE_MAP(CAIT, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CAIT::OnBnClickedButton2)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_SETFOCUS, IDC_AIT_LIST, &CAIT::OnNMSetfocusAitList)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CAIT::OnHdnItemclickAitList)
	ON_NOTIFY(NM_CLICK, IDC_AIT_LIST, &CAIT::OnNMClickAitList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_AIT_LIST, &CAIT::OnLvnKeydownAitList)
	ON_BN_CLICKED(IDC_BUTTON3, &CAIT::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CAIT::OnBnClickedButton1)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CAIT::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CAIT::OnCbnSelchangeCombo9)
	ON_CBN_EDITCHANGE(IDC_COMBO9, &CAIT::OnCbnEditchangeCombo9)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAIT::OnCbnSelchangeCombo2)
	ON_CBN_KILLFOCUS(IDC_COMBO2, &CAIT::OnCbnKillfocusCombo2)
	ON_CBN_DBLCLK(IDC_COMBO2, &CAIT::OnCbnDblclkCombo2)
	ON_CBN_EDITCHANGE(IDC_COMBO2, &CAIT::OnCbnEditchangeCombo2)
	ON_CBN_SETFOCUS(IDC_COMBO2, &CAIT::OnCbnSetfocusCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CAIT::OnCbnDropdownCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CAIT::OnCbnSelchangeCombo8)
	ON_BN_CLICKED(IDC_BUTTON5, &CAIT::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK4, &CAIT::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CAIT 消息处理程序


void CAIT::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	OnCancel();
}

bool AIT_List_init = true;
void CAIT::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	// 解决载入问题
	if (AIT_List_init&&flag){

		m_AIT_list.SetExtendedStyle(m_AIT_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		CString head[] = { TEXT("注册名称"), TEXT("注释") };
		for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++)
			m_AIT_list.InsertColumn(i, head[i], LVCFMT_LEFT, 100);

		AIT_List_init = false;
		CString title, note; // 注册名称，备注
		std::istringstream num_stream(main_aitt);
		std::string line, reg_title, reg_note;
		int team_idx = 0;
		while (std::getline(num_stream, line)) {
			// 分离注册编号和注册标题
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				reg_title = line.substr(0, pos);
				int posend = line.find(',', pos + 1); // 从pos+1位置开始搜索第一个逗号
				reg_note = line.substr(pos + 1, posend - (pos + 1));
			}
			else {
				continue;
			}

			// 插入列表
			title.Format(_T("%s"), CString(reg_title.c_str()));
			note.Format(_T("%s"), CString(reg_note.c_str()));
			m_AIT_list.InsertItem(team_idx, title);
			m_AIT_list.SetItemText(team_idx, 1, note);
			++team_idx;
		}

		m_AIT_list.RedrawWindow();
		AITWind.Invalidate();
		AITWind.UpdateWindow();
	}
}


void CAIT::OnNMSetfocusAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码

	*pResult = 0;
}

// 用不上
void CAIT::OnHdnItemclickAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	*pResult = 0;
}

// 仅查询内容部分
void CAIT::OnNMClickAitList(NMHDR *pNMHDR, LRESULT *pResult)
{

	if (flag_add_ait){
		count_ait_num += 1;
		m_AIT_list.InsertItem(count_ait_num, add_ait_title);
		m_AIT_list.SetItemText(count_ait_num, 1, (CString)"NoName");
		flag_add_ait = false;
		fresh_ait_list = true; // 刷新列表
	}

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码


	POSITION pos = m_AIT_list.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		char* posf = NULL;
		int nIndex = m_AIT_list.GetNextSelectedItem(pos);
		// 获取具体内容
		CString title;
		std::string content;
		title = m_AIT_list.GetItemText(nIndex, 0);
		content = FindMemSTDS(main_aitt, CStringToChar(title));
		m_tip_txt.SetWindowTextW(title);

		// 查找注释
		CString name;
		posf = ExtractText(content.data(), "=",",");
		name = (CString)posf;
		m_name_edit.SetWindowTextW(name);
		free(posf);

		// 查找作战小队A
		CString teamA;
		posf = SerchTargetString(content.data(), ",", 1, 2);
		teamA = (CString)posf;
		m_teamA_ccb.SetWindowTextW(teamA);
		free(posf);

		// 查找指定国家
		CString country;
		posf = SerchTargetString(content.data(), ",", 2, 3);
		country = (CString)posf;
		m_Country_ccb.SetWindowTextW(country);
		free(posf);

		// 查找条件类型
		CString tjtype;
		posf = SerchTargetString(content.data(), ",", 4, 5);
		tjtype = (CString)posf;
		switch (numtoi(tjtype)){
		case -1:m_atttype_ccb.SetWindowTextW((CString)"无条件"); break;
		case 0:m_atttype_ccb.SetWindowTextW((CString)"敌方拥有(前提)指定单位"); break;
		case 1:m_atttype_ccb.SetWindowTextW((CString)"我方拥有(前提)指定单位"); break;
		case 2:m_atttype_ccb.SetWindowTextW((CString)"敌方电力为黄"); break;
		case 3:m_atttype_ccb.SetWindowTextW((CString)"敌方电力为红"); break;
		case 4:m_atttype_ccb.SetWindowTextW((CString)"敌方金钱大于或等于"); break;
		case 5:m_atttype_ccb.SetWindowTextW((CString)"本方铁幕装置就绪"); break;
		case 6:m_atttype_ccb.SetWindowTextW((CString)"本方超时空传送就绪"); break;
		case 7:m_atttype_ccb.SetWindowTextW((CString)"中立方拥有(前提)指定单位"); break;
		}
		free(posf);

		// 查找条件模式
		CString aittmod;
		std::string tjmod, tjval,test;
		test = SerchTargetString(content.data(), ",", 6, 7);
		if (strlen(test.data()) != 64){
			CString temp;
			temp.Format(_T("%s,%s"), title, (CString)"此注册表有错误，已自动纠正");
			m_tip_txt.SetWindowTextW(temp);
			test = "0000000000000000000000000000000000000000000000000000000000000000";
		}
		tjmod = FindHexData((char*)test.data(), false); // 返回条件检测模式
		tjval = FindHexData((char*)test.data(), true); // 返回条件检测数值
		
		char* temp = "Ruffy";
		switch (atoi(tjmod.data())){
		case 0: temp = "小于"; break;
		case 1: temp = "小于等于"; break;
		case 2: temp = "等于"; break;
		case 3: temp = "大于等于"; break;
		case 4: temp = "大于"; break;
		case 5: temp = "不等于"; break;
		}
		m_aittmod_ccb.SetWindowTextW((CString)temp);
		m_value_edit.SetWindowTextW((CString)tjval.data());
		test.clear();
		
		// 查找条件单位
		CString tjunit;
		posf = SerchTargetString(content.data(), ",", 5, 6);
		m_aittunit_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 基本权重 7 8
		posf = inta(SerchTargetString(content.data(), ",", 7, 8));
		m_normalaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 最低权重 8 9
		posf = inta(SerchTargetString(content.data(), ",", 8, 9));
		m_lowaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 最高权重 9 10
		posf = inta(SerchTargetString(content.data(), ",", 9, 10));
		m_heightaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 检查阵营使用
		posf = SerchTargetString(content.data(), ",", 12, 13);
		m_Unity_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 检查科技等级
		posf = SerchTargetString(content.data(), ",", 4, 5);
		m_Tech_edit.SetWindowTextW((CString)posf);
		free(posf);

		// 检查等级
		posf = SerchTargetString(content.data(), ",", 15, 16);
		if ((CString)posf == (CString)"1"){
			m_EasyEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_EasyEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 16, 17);
		if ((CString)posf == (CString)"1"){
			m_NormalEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_NormalEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 17, 0);
		if ((CString)posf == (CString)"1"){
			m_HardEnemy_check.SetCheck(BST_CHECKED);
		}else m_HardEnemy_check.SetCheck(BST_UNCHECKED);

		// 自动更新列表
		m_teamA_ccb.GetWindowTextW(flag_fresh_te_title);
		if (m_autoturn_check.GetCheck() == BST_CHECKED)
			flag_fresh_te = true;
		if (flag_fresh_te){
			ArmyWinD.ShowWindow(SW_HIDE);
			ArmyWinD.ShowWindow(SW_NORMAL);
		}
		free(posf);

	}
	*pResult = 0;
}

// 键盘操作事件
void CAIT::OnLvnKeydownAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	POSITION pos = m_AIT_list.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		char* posf = NULL;
		int nIndex = m_AIT_list.GetNextSelectedItem(pos);
		// 获取具体内容
		CString title;
		std::string content;
		title = m_AIT_list.GetItemText(nIndex, 0);
		content = FindMemSTDS(main_aitt, CStringToChar(title));
		m_tip_txt.SetWindowTextW(title);

		// 查找注释
		CString name;
		posf = ExtractText(content.data(), "=", ",");
		name = (CString)posf;
		m_name_edit.SetWindowTextW(name);
		free(posf);

		// 查找作战小队A
		CString teamA;
		posf = SerchTargetString(content.data(), ",", 1, 2);
		teamA = (CString)posf;
		m_teamA_ccb.SetWindowTextW(teamA);
		free(posf);

		// 查找指定国家
		CString country;
		posf = SerchTargetString(content.data(), ",", 2, 3);
		country = (CString)posf;
		m_Country_ccb.SetWindowTextW(country);
		free(posf);

		// 查找条件类型
		CString tjtype;
		posf = SerchTargetString(content.data(), ",", 4, 5);
		tjtype = (CString)posf;
		switch (numtoi(tjtype)){
		case -1:m_atttype_ccb.SetWindowTextW((CString)"无条件"); break;
		case 0:m_atttype_ccb.SetWindowTextW((CString)"敌方拥有(前提)指定单位"); break;
		case 1:m_atttype_ccb.SetWindowTextW((CString)"我方拥有(前提)指定单位"); break;
		case 2:m_atttype_ccb.SetWindowTextW((CString)"敌方电力为黄"); break;
		case 3:m_atttype_ccb.SetWindowTextW((CString)"敌方电力为红"); break;
		case 4:m_atttype_ccb.SetWindowTextW((CString)"敌方金钱大于或等于"); break;
		case 5:m_atttype_ccb.SetWindowTextW((CString)"本方铁幕装置就绪"); break;
		case 6:m_atttype_ccb.SetWindowTextW((CString)"本方超时空传送就绪"); break;
		case 7:m_atttype_ccb.SetWindowTextW((CString)"中立方拥有(前提)指定单位"); break;
		}
		free(posf);

		// 查找条件模式
		CString aittmod;
		std::string tjmod, tjval, test;
		test = SerchTargetString(content.data(), ",", 6, 7);
		if (strlen(test.data()) != 64){
			CString temp;
			temp.Format(_T("%s,%s"), title, (CString)"此注册表有错误，已自动纠正");
			m_tip_txt.SetWindowTextW(temp);
			test = "0000000000000000000000000000000000000000000000000000000000000000";
		}
		tjmod = FindHexData((char*)test.data(), false); // 返回条件检测模式
		tjval = FindHexData((char*)test.data(), true); // 返回条件检测数值

		char* temp = "Ruffy";
		switch (atoi(tjmod.data())){
		case 0: temp = "小于"; break;
		case 1: temp = "小于等于"; break;
		case 2: temp = "等于"; break;
		case 3: temp = "大于等于"; break;
		case 4: temp = "大于"; break;
		case 5: temp = "不等于"; break;
		}
		m_aittmod_ccb.SetWindowTextW((CString)temp);
		m_value_edit.SetWindowTextW((CString)tjval.data());
		test.clear();

		// 查找条件单位
		CString tjunit;
		posf = SerchTargetString(content.data(), ",", 5, 6);
		m_aittunit_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 基本权重 7 8
		posf = inta(SerchTargetString(content.data(), ",", 7, 8));
		m_normalaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 最低权重 8 9
		posf = inta(SerchTargetString(content.data(), ",", 8, 9));
		m_lowaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 最高权重 9 10
		posf = inta(SerchTargetString(content.data(), ",", 9, 10));
		m_heightaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 检查阵营使用
		posf = SerchTargetString(content.data(), ",", 12, 13);
		m_Unity_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// 检查科技等级
		posf = SerchTargetString(content.data(), ",", 4, 5);
		m_Tech_edit.SetWindowTextW((CString)posf);
		free(posf);

		// 检查等级
		posf = SerchTargetString(content.data(), ",", 15, 16);
		if ((CString)posf == (CString)"1"){
			m_EasyEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_EasyEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 16, 17);
		if ((CString)posf == (CString)"1"){
			m_NormalEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_NormalEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 17, 0);
		if ((CString)posf == (CString)"1"){
			m_HardEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_HardEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);

	}
	*pResult = 0;
}

// 保存当前修改
void CAIT::OnBnClickedButton3()
{
	int EasyEnemy, NormalEnemy, HardEnemy;
	CString title, aitt_txt,name,country,attmod,tech,teamA,atttype,aittunit,value,unity,lowaitt,normalaitt,heightaitt;
	int nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1)return;
	// 获取标题
	title = m_AIT_list.GetItemText(nIndex, 0);
	// 获取注释
	m_name_edit.GetWindowTextW(name);
	// 获取简单敌人可用
	EasyEnemy = m_EasyEnemy_check.GetCheck();
	// 获取中等敌人可用
	NormalEnemy = m_NormalEnemy_check.GetCheck();
	// 获取困难敌人可用
	HardEnemy = m_HardEnemy_check.GetCheck();
	// 获取国家状况
	m_Country_ccb.GetWindowTextW(country);
	// 获取科技状态
	m_Tech_edit.GetWindowTextW(tech);
	// 获取条件模式
	m_aittmod_ccb.GetWindowTextW(attmod);
	// 获取条件类型
	m_atttype_ccb.GetWindowTextW(atttype);
	// 获取条件单位
	m_aittunit_ccb.GetWindowTextW(aittunit);
	// 获取条件数值
	m_value_edit.GetWindowTextW(value);
	// 获取指定阵营
	m_Unity_ccb.GetWindowTextW(unity);
	// 获取最低权重
	m_lowaitt_ccb.GetWindowTextW(lowaitt);
	// 获取基本权重
	m_normalaitt_ccb.GetWindowTextW(normalaitt);
	// 获取最高权重
	m_heightaitt_ccb.GetWindowTextW(heightaitt);
	// 获取队伍A
	m_teamA_ccb.GetWindowTextW(teamA);

	// 获取标题前面部分
	std::string head;
	int headpos;
	headpos = TxtMemFind(main_aitt.size(),0,main_aitt.data(),CStringToChar(title),R_BF);
	head = main_aitt.substr(0,headpos);
	
	// 获取标题内容末尾的内容
	std::string content_end;
	content_end += CStringToChar(title);
	if (!Sac(FindMemSTDS(main_aitt, CStringToChar(title)), "None")){
		content_end += FindMemSTDS(main_aitt, CStringToChar(title));
	}
	else content_end = "";
	std::string end;
	int endpos;
	endpos = content_end.size() + headpos;
	end = main_aitt.substr(endpos);

	// 获取修改部分
	std::string replace_txt;
	replace_txt += CStringToChar(title);
	replace_txt += "=";
	replace_txt += CStringToChar(name);
	replace_txt += ",";
	replace_txt += CStringToChar(teamA);
	replace_txt += ",";
	replace_txt += CStringToChar(country);
	replace_txt += ",";
	replace_txt += CStringToChar(tech);
	replace_txt += ",";
	// 哈希值寻找
	char temp_value[20];
	sprintf_s(temp_value, sizeof(temp_value), "%d", getmode(CStringToChar(atttype)));
	replace_txt += temp_value;
	replace_txt += ",";

	// 添加单位
	replace_txt += CStringToChar(aittunit);
	replace_txt += ",";

	// 添加触发数值
	replace_txt += decimalToHexadecimal(CStringToChar(value));
	// 条件参数
	replace_txt += "0";

	char temp_tj[20];
	sprintf_s(temp_tj, sizeof(temp_tj), "%d", Ruffyfind(CStringToChar(attmod)));

	replace_txt += temp_tj;
	replace_txt += "000000000000000000000000000000000000000000000000000000,";
	// 基本最低最高触发
	replace_txt += CStringToChar(normalaitt);
	replace_txt += ".000000,";
	replace_txt += CStringToChar(lowaitt);
	replace_txt += ".000000,";
	replace_txt += CStringToChar(heightaitt);
	replace_txt += ".000000,1,0,";
	replace_txt += CStringToChar(unity);
	replace_txt += ",0,";
	replace_txt += "<none>,";

	// 中等 简单 困难是否可用
	if (EasyEnemy == BST_CHECKED)
		replace_txt += "1,";
	else replace_txt += "0,";
	if (NormalEnemy == BST_CHECKED)
		replace_txt += "1,";
	else replace_txt += "0,";
	if (HardEnemy == BST_CHECKED)
		replace_txt += "1";
	else replace_txt += "0";
	head += replace_txt;
	head += GetMemLines(end.data(), end.size());
	head += "\n";
	main_aitt.clear();
	main_aitt = head;
	head.clear();
	fresh_ait_list = true;

	std::string upnote;
	CString o_tit;
	upnote = CStringToChar(name);
	int count = m_AIT_list.GetItemCount();
	for (int i = 0; i < count; i++){
		o_tit = m_AIT_list.GetItemText(i, 0);
		if (o_tit == title){
			m_AIT_list.SetItemText(i, 1, (CString)upnote.data());
		}
	}

	return;
}

// 新增内容
void CAIT::OnBnClickedButton1()
{
	add_type = 0;
	Add_new_init = true;
	add_wind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_NORMAL);
}


void CAIT::OnMouseMove(UINT nFlags, CPoint point)
{

	if (flag_add_ait){
		count_ait_num += 1;
		m_AIT_list.InsertItem(count_ait_num, add_ait_title);
		m_AIT_list.SetItemText(count_ait_num,1, (CString)"NoName");
		flag_add_ait = false;
		fresh_ait_list = true; // 刷新列表
	}
	
	if (fresh_ait_list){
		fresh_ait_list = false;
		CString title, name,namet;
		std::string content, nameStr;
		int nCount = m_AIT_list.GetItemCount();
		for (int i = 0; i < nCount; i++) {
			title = m_AIT_list.GetItemText(i, 0); // 获取标题
			namet = m_AIT_list.GetItemText(i, 1); // 获取第二列的name
			content = FindMemSTDS(main_aitt, CStringToChar(title));
			// 查找内容注释
			char* posf = ExtractText(content.data(), "=", ",");
			name = (CString)posf;
			free(posf);
			if (name!=namet){
				m_AIT_list.SetItemText(i, 1,name);
			}
		}
	}

	// 获取条件模式的内容
	CString tjmode;
	m_atttype_ccb.GetWindowTextW(tjmode);
	m_aittmod_ccb.EnableWindow(TRUE);
	m_aittunit_ccb.EnableWindow(TRUE);
	m_value_edit.EnableWindow(TRUE);
	// 获取条件模式的内容
	m_atttype_ccb.GetWindowTextW(tjmode);
	if (tjmode == (CString)"敌方金钱大于或等于"){
		m_aittmod_ccb.SetWindowTextW((CString)"大于或等于");
		if (m_aittmod_ccb.IsWindowEnabled())
			m_aittmod_ccb.EnableWindow(FALSE);
		m_aittunit_ccb.SetWindowTextW((CString)"<none>");
		if (m_aittunit_ccb.IsWindowEnabled())
			m_aittunit_ccb.EnableWindow(FALSE);
	}
	else if ((tjmode == (CString)"本方铁幕装置就绪") || (tjmode == (CString)"本方超时空传送就绪") || (tjmode == (CString)"敌方电力为红") || (tjmode == (CString)"敌方电力为黄") || (tjmode == (CString)"无条件")){
		m_aittmod_ccb.SetWindowTextW((CString)"小于");
		m_aittunit_ccb.SetWindowTextW((CString)"<none>");
		if (m_aittmod_ccb.IsWindowEnabled())
			m_aittmod_ccb.EnableWindow(FALSE);
		if (m_aittunit_ccb.IsWindowEnabled())
			m_aittunit_ccb.EnableWindow(FALSE);
		m_value_edit.SetWindowTextW((CString)"0");
		if (m_value_edit.IsWindowEnabled())
			m_value_edit.EnableWindow(FALSE);
	}

	if (flag_updata_te){	// 更新脚本下拉框
		std::string str_temp;
		int linenum = 0,nIndex;
		m_teamA_ccb.ResetContent();
		char* pStr = GetMemLines(main_team_num.data(), main_team_num.size());
		linenum = GetMemLinesNum(pStr);
		for (int i = 0; i < linenum; i++)
		{
			str_temp = GetMemLine(main_team_num.data(), i);
			CString temp_all;
			temp_all = (CString)FindMemSTDS(str_temp, "=").data();
			if (temp_all != (CString)"None")
			m_teamA_ccb.AddString(temp_all);
		}
		nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED);
		if (nIndex != -1){
			CString title, team_s;
			m_tip_txt.GetWindowTextW(title);
			m_teamA_ccb.SetWindowTextW(title);
		}
		flag_updata_te = false;
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

// 删除
void CAIT::OnBnClickedButton4()
{
	int nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED); // 获取当前高亮行的索引
	while (nIndex != -1){
		nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED); // 获取当前高亮行的索引
		if (nIndex != -1) {
			CString del_title;
			count_ait_num -= 1;
			del_title = m_AIT_list.GetItemText(nIndex, 0);
			m_AIT_list.DeleteItem(nIndex);
			DelMemLine(main_aitt, CStringToChar(del_title));
			fresh_ait_list = true;
		}
	}
	main_aitt = GetMemLines(main_aitt.data(), main_aitt.size());
}

// 无效事件
void CAIT::OnCbnSelchangeCombo9()
{
	
}


void CAIT::OnCbnEditchangeCombo9()
{
	// TODO:  在此添加控件通知处理程序代码
}

// 使用键盘修改内容时代码
void CAIT::OnCbnSelchangeCombo2()
{
}

// 失去焦点时代码
void CAIT::OnCbnKillfocusCombo2()
{
}

// 点击了左键代码 无用
void CAIT::OnCbnDblclkCombo2()
{
}

// 鼠标点击下拉框事件
void CAIT::OnCbnEditchangeCombo2()
{
	// TODO:  在此添加控件通知处理程序代码
	
}

// 无用代码
void CAIT::OnCbnSetfocusCombo2()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CAIT::OnCbnDropdownCombo2()
{
	// 获取条件模式的内容
}


void CAIT::OnCbnSelchangeCombo8()
{
	// TODO:  在此添加控件通知处理程序代码
}

// 点击跳转时
void CAIT::OnBnClickedButton5()
{
	flag_fresh_te = true;
	ArmyWinD.ShowWindow(SW_HIDE);
	ArmyWinD.ShowWindow(SW_NORMAL);
}


// 跳转按钮是否可用判断
void CAIT::OnBnClickedCheck4()
{
	if (m_autoturn_check.GetCheck() == BST_CHECKED){
		c_turn.EnableWindow(FALSE);
	}
	else c_turn.EnableWindow(TRUE);
}
