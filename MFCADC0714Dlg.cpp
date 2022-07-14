
// MFCADC0714Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCADC0714.h"
#include "MFCADC0714Dlg.h"
#include "afxdialogex.h"


// kdh 추가
#include "CMysqlController.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <sstream> 
using namespace std;

vector<char>rxBuffer;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCADC0714Dlg 대화 상자



CMFCADC0714Dlg::CMFCADC0714Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCADC0714_DIALOG, pParent)
	, m_str_comport(_T(""))
	, m_combo_baudrate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCADC0714Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport_list);
	DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate);
	DDX_Control(pDX, IDC_LIST1, m_DataList);
}

BEGIN_MESSAGE_MAP(CMFCADC0714Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCADC0714Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCADC0714Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BT_CONNECT, &CMFCADC0714Dlg::OnBnClickedBtConnect)

	ON_MESSAGE(WM_MYRECEIVE, &CMFCADC0714Dlg::OnReceive)
END_MESSAGE_MAP()


// CMFCADC0714Dlg 메시지 처리기

BOOL CMFCADC0714Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_DataList.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 50);
	m_DataList.InsertColumn(1, _T("시간"), LVCFMT_CENTER, 200);
	m_DataList.InsertColumn(2, _T("ADC값"), LVCFMT_CENTER, 300);

	m_combo_comport_list.AddString(_T("COM1"));
	m_combo_comport_list.AddString(_T("COM2"));
	m_combo_comport_list.AddString(_T("COM3"));
	m_combo_comport_list.AddString(_T("COM4"));
	m_combo_comport_list.AddString(_T("COM5"));
	m_combo_comport_list.AddString(_T("COM6"));
	m_combo_comport_list.AddString(_T("COM7"));
	m_combo_comport_list.AddString(_T("COM8"));
	m_combo_comport_list.AddString(_T("COM9"));
	m_combo_comport_list.AddString(_T("COM10"));

	m_combo_baudrate_list.AddString(_T("9600"));
	m_combo_baudrate_list.AddString(_T("19200"));
	m_combo_baudrate_list.AddString(_T("115200"));


	comport_state = false;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM10");
	m_combo_baudrate = _T("115200");
	UpdateData(FALSE);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCADC0714Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCADC0714Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCADC0714Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCADC0714Dlg::OnBnClickedButton1() // Select 새로고침
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMysqlController conn;
	CString result;
	if (conn.SelectQuery("select * from tb_adc", result) == true)
	{
		AfxMessageBox(result);
	}
	else AfxMessageBox(_T("실패"));

}


void CMFCADC0714Dlg::OnBnClickedButton2()  // Insert
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMysqlController conn;

	UpdateData(TRUE);
	char* send_data;
	CString result;
	result = "insert into tb_adc(시간, ADC값) VALUES(";
	result += "NOW(),";
	result += "21500)";
	send_data = (LPSTR)(LPCSTR)result;
	if (conn.InsertQuery(send_data) == true)
	{
		AfxMessageBox(_T("성공"));
	}
	else
	{
		AfxMessageBox(_T("실패"));
	}

}


void CMFCADC0714Dlg::OnBnClickedBtConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (comport_state)
	{
		if (m_comm)        //컴포트가존재하면
		{
			m_comm->Close();
			m_comm = NULL;
			AfxMessageBox(_T("COM 포트닫힘"));
			comport_state = false;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm = new CSerialComm(_T("\\\\.\\") + m_str_comport, m_combo_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
		if (m_comm->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			AfxMessageBox(_T("COM 포트열림"));
			comport_state = true;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			//GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}

	}
}
void CMFCADC0714Dlg::OnCbnSelchangeComboComport()
{
	// TODO: Add your control notification handler code here
	UpdateData(); 
}


void CMFCADC0714Dlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
LRESULT	CMFCADC0714Dlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	((CSerialComm*)lpara)->HandleClose();
	delete ((CSerialComm*)lpara);

	return 0;
}


LRESULT CMFCADC0714Dlg::OnReceive(WPARAM length, LPARAM lpara)
{


	

	//CString str;
	char* data = new char[length + 1];

	if(m_comm)
	{
		
		m_comm->Receive(data, length);	// Length 길이만큼 데이터 받음.
		data[length] = _T('\0');
		//str += _T("\r\n");
		//for (int i = 0; i < length; i++)
		//{
		//	rxBuffer.push_back(data[i]);
		//}

		CMysqlController conn;
		UpdateData(TRUE);
		char* send_data;
		CString rxBuffer_temp;
		CString result;
		rxBuffer_temp = (LPSTR)data; // 시간 값 (char -> CString)

		result = "insert into tb_adc(시간, ADC값) VALUES(";
		result += "NOW(),";
		result += rxBuffer_temp + ")";
		send_data = (LPSTR)(LPCSTR)result;
		conn.InsertQuery("DELETE FROM tb_adc WHERE 시간< NOW() - INTERVAL 10 minute");  // 10분전 데이터 삭제
		conn.InsertQuery(send_data);  // DB로  ADC 값 데이터 보내기

		//UpdateData(FALSE);

		string result_str;
		string line;
		stringstream result_stream;


		// 화면리스트에 DB 불러오기
		m_DataList.DeleteAllItems();  // 화면 비우기
		if (conn.SelectQuery("select * from tb_adc", result) == true)
		{
			result_str = string(CT2CA(result));
			result_stream.str(result_str);
			while (getline(result_stream, line))
			{
				stringstream lineStream(line); //한줄의 데이터를 가지고 사용
				string cell;
				CString cell2;
				int count0 = m_DataList.GetItemCount();
				int count1 = 0;
				while (getline(lineStream, cell, ','))
				{
					cell2 = cell.c_str();

					if (count1 == 0)
					{
						m_DataList.InsertItem(count0, cell2);
					}
					else
					{
						m_DataList.SetItem(count0, count1, LVIF_TEXT, cell2, 0, 0,
							0, 0);
					}
					count1++;
				}

			}
			UpdateData(false);
			//AfxMessageBox(result);
		}

	}

	return 0;
}
