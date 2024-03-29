﻿
// MFCADC0714Dlg.h: 헤더 파일
//

#pragma once
#include "SerialComm.h"

// CMFCADC0714Dlg 대화 상자
class CMFCADC0714Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCADC0714Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCADC0714_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_combo_comport_list;
	CString m_str_comport;
	CComboBox m_combo_baudrate_list;
	CString m_combo_baudrate;
	CListCtrl m_DataList;

	//kdh 추가
	BOOL comport_state;
	afx_msg void OnBnClickedBtConnect();
	CSerialComm* m_comm;
	LRESULT		OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT		OnReceive(WPARAM length, LPARAM lpara);
	afx_msg void OnCbnSelchangeComboComport();
	afx_msg void OnCbnSelchangeComboBaudrate();
	CListCtrl m_DataList2;
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);

	struct SORTPARAM
	{
		int iSortColumn;
		bool bSortDirect;
		CListCtrl* pList;
		int flag = -1;  // 클릭한 header에 따라서 정렬할 값이 다르기 때문에 구분해주기위한 변수
	};
	BOOL m_bAscending;
	afx_msg void OnHdnItemclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	static int CALLBACK CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};
