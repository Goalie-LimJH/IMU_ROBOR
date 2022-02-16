
// IMU_ROBORDlg.h : header file
//

#pragma once

#include <vector>
#include <deque>
#include <algorithm>
#include <string.h>

#include "SerialMFC.h"
#include "afxwin.h"

using namespace std;

typedef struct _Serial_Port
{
	CString strDeviceName;
	CString strPortName;
}SERIAL_PORT;


// CIMU_ROBORDlg dialog
class CIMU_ROBORDlg : public CDialogEx
{
	// Construction
public:
	CIMU_ROBORDlg(CWnd* pParent = NULL);	// standard constructor

												// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMU_ROBOR_DIALOG };
#endif



protected:
	vector<SERIAL_PORT> m_SerialPort;

public:
	CSerialMFC	m_Serial;
	void PortSearch1();
	int PortSearch2();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support




														// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSerialMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:

	int m_flag_connect;
	CString m_str_sendmsg;
	int m_flag_send;

	CListBox m_list_cmd;
	CString m_strSendCmd;
	int m_flag_cmd;


	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnLbnDblclkListCmd();
	afx_msg void OnBnClickedCheckLogging();

	afx_msg void OnBnClickedButtonRa();
	afx_msg void OnBnClickedButtonRp();
	afx_msg void OnBnClickedButtonZa();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL m_CheckLogging;
	int m_flag_logfile;
	FILE *fp_log;
	SYSTEMTIME lt, pre_lt;
	char str_filename[56];

};
