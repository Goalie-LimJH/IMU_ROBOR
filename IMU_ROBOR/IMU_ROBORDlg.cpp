
// IMU_ROBORDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMU_ROBOR.h"
#include "IMU_ROBORDlg.h"
#include "afxdialogex.h"
#include <conio.h>

#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_SIZE	1024
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
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


// CIMU_ROBORDlg dialog

CIMU_ROBORDlg::CIMU_ROBORDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMU_ROBOR_DIALOG, pParent)
	, m_str_sendmsg(_T(""))
	, m_CheckLogging(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMU_ROBORDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SEND_DATA, m_str_sendmsg);
	DDX_Control(pDX, IDC_LIST_CMD, m_list_cmd);
	DDX_Check(pDX, IDC_CHECK_Logging, m_CheckLogging);
}

BEGIN_MESSAGE_MAP(CIMU_ROBORDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SERIAL_READ_MESSAGE, OnSerialMessage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CIMU_ROBORDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_Close, &CIMU_ROBORDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CIMU_ROBORDlg::OnBnClickedButtonSend)
	ON_LBN_DBLCLK(IDC_LIST_CMD, &CIMU_ROBORDlg::OnLbnDblclkListCmd)
	ON_BN_CLICKED(IDC_CHECK_Logging, &CIMU_ROBORDlg::OnBnClickedCheckLogging)
	ON_BN_CLICKED(IDC_BUTTON_RA, &CIMU_ROBORDlg::OnBnClickedButtonRa)
	ON_BN_CLICKED(IDC_BUTTON_RP, &CIMU_ROBORDlg::OnBnClickedButtonRp)
	ON_BN_CLICKED(IDC_BUTTON_ZA, &CIMU_ROBORDlg::OnBnClickedButtonZa)
END_MESSAGE_MAP()


// CIMU_ROBORDlg message handlers

BOOL CIMU_ROBORDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//PortSearch1();
	////PortSearch2();
	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	PortSearch1();
	//PortSearch2();
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

	GetDlgItem(IDC_EDIT_SEND_DATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST_CMD)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_Logging)->EnableWindow(FALSE);

	//m_list_cmd.AddString("log version once");
	//m_list_cmd.AddString("log ncom1 gpgga ontime 5");
	//m_list_cmd.AddString("log USB1 bestposa ontime 1");
	//m_list_cmd.AddString("log USB1 inspvaxa ontime 0.05");
	//m_list_cmd.AddString("log USB1 dualantennaheadinga ontime 0.1");
	//m_list_cmd.AddString("unlogall true");
	
	//m_list_cmd.AddString("Reset angle (ra)");

	m_flag_connect = 0;
	m_flag_send = 0;
	m_flag_cmd = 0;

	m_CheckLogging = 0;
	m_flag_logfile = 1;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIMU_ROBORDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIMU_ROBORDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIMU_ROBORDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CIMU_ROBORDlg::OnSerialMessage(WPARAM wParam, LPARAM lParam)
{
	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));

	char nReadBuffer[MAX_SIZE];
	memset(nReadBuffer, 0, MAX_SIZE);
	
	DWORD nReadCount;
	DWORD i, idx, newdata;
	CString strTemp = _T("");
	newdata = 1;
	//CString pre_strTemp, new_strTemp;

	if (eEvent & CSerial::EEventRecv)
	{
		//while (TRUE)
		while(m_flag_connect)
		{
			m_Serial.Read(&nReadBuffer, MAX_SIZE, &nReadCount);
			if (nReadCount == 0)
			{
				break;
			}
//#if _DEBUG
//			//_cprintf("Received = ");
//			for (i = 0; i < nReadCount; i++)
//			{
//				_cprintf("%c", nReadBuffer[i]);
//			}
//			//_cprintf("\r\n");
//#endif
			/*if(nReadCount < MAX_SIZE)
				nReadBuffer[nReadCount] = '\0';*/

			strTemp.Format(_T("%s"), nReadBuffer);
			
			GetDlgItem(IDC_EDIT_RECEIVED_DATA)->SetWindowText(strTemp);
			GetLocalTime(&lt);
			//TRACE2("pre_lt.wHour = %02d / lt.wHour = %02d\n", pre_lt.wHour, lt.wHour);
			//for (i = 0; i < nReadCount; i++)
			//{
			//	TRACE1("%c", nReadBuffer[i]);
			//}
			if (m_CheckLogging)
			{
				if (m_flag_logfile)
				{
					m_flag_logfile = 0;
					pre_lt = lt;
					sprintf(str_filename, "..\\Log\\log_%02d%02d%02d%02d.txt", lt.wMonth, lt.wDay, lt.wHour, lt.wMinute);
					fp_log = fopen(str_filename, "a+");
		
					//fprintf(fp_log,"%s", strTemp);
					idx = 0;
					if (newdata)
					{
						for (int t = 0; t < nReadCount; t++)
						{
							if (nReadBuffer[t] == 13)
							{
								idx = t + 3;
								break;
								//if (nReadBuffer[t + 1] == 10)
								//{
									//idx = t + 2;
									//break;
								//}
							}
						}
						newdata = 0;
					}
					//_cprintf("\n%d\n", idx);
					strTemp = _T("");
					for (i = idx; i < nReadCount; i++)
					{
						//fprintf(fp_log, "%c", nReadBuffer[i]);
						//_cprintf("%c", nReadBuffer[i]);
						strTemp = strTemp + nReadBuffer[i];
					}
					fprintf(fp_log, "%s", strTemp);
					_cprintf("%s", strTemp);

				}
				else
				{
					if (pre_lt.wHour != lt.wHour)
					{
						pre_lt = lt;
						fclose(fp_log);

						sprintf(str_filename, "..\\Log\\log_%02d%02d%02d%02d.txt", lt.wMonth, lt.wDay, lt.wHour, lt.wMinute);
						fp_log = fopen(str_filename, "a+");
					}

					//fprintf(fp_log, "%s", strTemp);
					idx = 0;
					/*if (newdata)
					{
						for (int t = 0; t < nReadCount; t++)
						{
							if (nReadBuffer[t] == 13)
							{
								if (nReadBuffer[t + 1] == 10)
								{
									idx = t + 2;
									break;
								}
							}
						}
						newdata = 0;
					}*/
					//_cprintf("\n%d\n", idx);
					strTemp = _T("");
					for (i = idx; i < nReadCount; i++)
					{
						//fprintf(fp_log, "%c", nReadBuffer[i]);
						//_cprintf("%c", nReadBuffer[i]);
						strTemp = strTemp + nReadBuffer[i];
					}
					fprintf(fp_log, "%s", strTemp);
					_cprintf("%s", strTemp);
				}
			}
			else
			{
				if (fp_log != NULL)
				{
					m_flag_logfile = 1;
					fclose(fp_log);
				}	
			}
		}

		//Sleep(1);

	}
	return 0;
}

void CIMU_ROBORDlg::PortSearch1()
{
#define MAX_VALUE_NAME 100
	{
		vector<CString> vecTempPort;
		HKEY hKey;
		DWORD dwResult;
		CString strTemp;
		CString strTemp1;

		int	nSerialControlID = 0;
		vector<CString>::iterator itr;


		dwResult = RegOpenKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

		if (dwResult == ERROR_SUCCESS)
		{
			TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
			DWORD    cchClassName = MAX_PATH;  // size of class string 
			DWORD    cSubKeys = 0;               // number of subkeys 

			DWORD    cValues;              // number of values for key 
			DWORD i, retCode;

			TCHAR  achValue[MAX_VALUE_NAME];
			DWORD cchValue;


			// Get the class name and the value count. 
			retCode = RegQueryInfoKey(hKey,				// key handle 
				NULL,				// buffer for class name 
				NULL,				// size of class string 
				NULL,				// reserved 
				NULL,             // number of subkeys 
				NULL,				// longest subkey size 
				NULL,				// longest class string 
				&cValues,         // number of values for this key 
				NULL,				// longest value name 
				NULL,				// longest value data 
				NULL,				// security descriptor
				NULL);			// last write time    

								// Enumerate the key values. 


			if (cValues)
			{
				for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++)
				{
					cchValue = MAX_VALUE_NAME;
					achValue[0] = '\0';
					retCode = RegEnumValue(hKey, i,
						achValue,
						&cchValue,
						NULL,
						NULL,
						NULL,
						NULL);
					if (retCode == ERROR_SUCCESS)
					{

						TCHAR chTemp[MAX_VALUE_NAME];
						DWORD dwBufSize;

						chTemp[0] = '\0';

						retCode = RegQueryValueEx(hKey, achValue, NULL, NULL, (PBYTE)chTemp, &dwBufSize);

						if (retCode == ERROR_SUCCESS)
						{
							//strTemp.Format(_T("%.3d"), _ttoi(chTemp+_tcslen(_T("COM"))));
							strTemp.Format(_T("%d"), _ttoi(chTemp + _tcslen(_T("COM"))));
							vecTempPort.push_back(strTemp);
						}
						else
						{
							retCode = RegQueryValueEx(hKey, achValue, NULL, NULL, (PBYTE)chTemp, &dwBufSize);

							if (ERROR_SUCCESS == retCode)
							{

								strTemp.Format(_T("%d"), _ttoi(chTemp + _tcslen(_T("COM"))));
								vecTempPort.push_back(strTemp);
							}
						}
					}
				}
			}
			RegCloseKey(hKey);
		}
		sort(vecTempPort.begin(), vecTempPort.end());

		itr = vecTempPort.begin();

		while (itr != vecTempPort.end())
		{
			SERIAL_PORT temp;
			strTemp.Format(_T("COM%s"), *itr);
			temp.strDeviceName = strTemp;
			temp.strPortName = strTemp;
			m_SerialPort.push_back(temp);

			((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->InsertString(((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCount(), strTemp);

			//m_ctrlComboSerial.InsertString(0, strTemp);
			itr++;
		}
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->SetCurSel(0);
}

int CIMU_ROBORDlg::PortSearch2()
{
	HDEVINFO        hDevInfo = 0L;
	SP_DEVINFO_DATA spDevInfoData = { 0 };
	short           wIndex = 0;
	HTREEITEM       hTreeChild = 0L;


	m_SerialPort.erase(m_SerialPort.begin(), m_SerialPort.end());

#if 1
	hDevInfo = SetupDiGetClassDevs(0L, 0L, this->GetParent()->GetSafeHwnd(), DIGCF_PRESENT | DIGCF_ALLCLASSES | DIGCF_PROFILE);
	if (hDevInfo == (void*)-1)
	{
		return 0;
	};

	wIndex = 0;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);


	while (1)
	{
		if (SetupDiEnumDeviceInfo(hDevInfo,
			wIndex,
			&spDevInfoData))
		{
			TCHAR  szBuf[MAX_PATH] = { 0 };
			short wImageIdx = 0;
			short wItem = 0;
			//
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo,
				&spDevInfoData,
				SPDRP_CLASS, //SPDRP_DEVICEDESC,
				0L,
				(PBYTE)szBuf,
				2048,
				0))
			{
				wIndex++;
				continue;
			};



			if (_tcscmp(szBuf, _T("Ports")) == 0)
			{
				TCHAR szName[64] = { 0 };
				TCHAR szID[LINE_LEN] = { 0 };
				TCHAR szPath[MAX_PATH] = { 0 };
				DWORD dwRequireSize;
				TCHAR* pFindCharFirst;
				TCHAR* pFindCharEnd;

				SERIAL_PORT stTempSerialPort;


				if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid, szBuf, MAX_PATH, &dwRequireSize))
				{
					wIndex++;
					continue;
				};

				if (!SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, szID, LINE_LEN, 0))
				{
					wIndex++;
					continue;
				}

				stTempSerialPort.strDeviceName = szName;

				pFindCharFirst = _tcsrchr(szName, '(');
				pFindCharFirst = (TCHAR*)(pFindCharFirst + 1);
				pFindCharEnd = _tcsrchr(szName, ')');
				*pFindCharEnd = '\0';
				stTempSerialPort.strPortName = pFindCharFirst;

				if (_tcsstr(pFindCharFirst, _T("COM")) != 0)
				{
					m_SerialPort.push_back(stTempSerialPort);
					((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->InsertString(((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCount(), stTempSerialPort.strDeviceName);
				}
			}
		}
		else
			break;

		wIndex++;
	};
	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->SetCurSel(0);
#endif	
}

void CIMU_ROBORDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	unsigned int nIndex;
	CString strTemp;
	nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCurSel();
	strTemp = m_SerialPort[nIndex].strPortName;

	if (m_Serial.IsOpen() == TRUE)
	{
		m_flag_connect = 0;
		m_CheckLogging = 0;
		if (fp_log != NULL)
		{
			m_flag_logfile = 1;
			fclose(fp_log);
		}
		CheckDlgButton(IDC_CHECK_Logging, false);

		m_Serial.Close();
		GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("Connect"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT_LIST))->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RECEIVED_DATA)->SetWindowText(" ");

		GetDlgItem(IDC_EDIT_SEND_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_CMD)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_Logging)->EnableWindow(FALSE);
	}
	else
	{
		if (ERROR_SUCCESS != m_Serial.Open(strTemp, this, WM_SERIAL_READ_MESSAGE))
		{
			MessageBox(_T("Serial Port Open Error"));
			return;
		}
		//m_Serial.Setup(CSerial::EBaud38400);
		//m_Serial.Setup(CSerialMFC::EBaud38400);
		m_Serial.Setup(CSerial::EBaud115200);
		//m_Serial.SetupHandshaking(CSerial::EHandshakeHardware);
		m_Serial.SetupHandshaking(CSerial::EHandshakeOff);
		
		
		m_Serial.Setup(CSerialMFC::EBaud115200);
		m_Serial.SetupHandshaking(CSerialMFC::EHandshakeOff);
		
		m_flag_connect = 1;

		GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("Disconnect"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT_LIST))->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_SEND_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_CMD)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_Logging)->EnableWindow(TRUE);
	}
}

void CIMU_ROBORDlg::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmpmsg;

	if (m_flag_cmd)
		m_flag_cmd = 0;

	GetDlgItemTextA(IDC_EDIT_SEND_DATA, tmpmsg);
	tmpmsg = tmpmsg + "\r\n";

	m_Serial.Write(tmpmsg);
}

void CIMU_ROBORDlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Serial.IsOpen() == TRUE)
	{
		//m_Serial.Write("unlogall true\n");
		m_Serial.Close();
	}

	CDialogEx::OnCancel();
}

BOOL CIMU_ROBORDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN)// || pMsg->wParam == VK_ESCAPE)
	{
		//if (m_flag_send)
		//	OnBnClickedButtonSend();
		
		if (pMsg->wParam == VK_ESCAPE)
			OnBnClickedButtonClose();

		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CIMU_ROBORDlg::OnLbnDblclkListCmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_list_cmd.GetCurSel();

	//m_list_cmd.GetText(index, m_strSendCmd);
	//GetDlgItem(IDC_EDIT_RECEIVED_DATA)->SetWindowText(" ");
	//SetDlgItemTextA(IDC_EDIT_SEND_DATA, m_strSendCmd);

	switch (index)
	{
	case 1:
		SetDlgItemTextA(IDC_EDIT_SEND_DATA, m_strSendCmd);
	}


	m_flag_cmd = 1;
}

void CIMU_ROBORDlg::OnBnClickedCheckLogging()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CheckLogging)
		m_CheckLogging = 0;
	else
		m_CheckLogging = 1;
		
}


void CIMU_ROBORDlg::OnBnClickedButtonRa()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmpmsg;

	if (m_flag_cmd)
		m_flag_cmd = 0;

	tmpmsg = "ra";
	tmpmsg = tmpmsg + "\r\n";

	m_Serial.Write(tmpmsg);

}

void CIMU_ROBORDlg::OnBnClickedButtonRp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmpmsg;

	if (m_flag_cmd)
		m_flag_cmd = 0;

	tmpmsg = "rp";
	tmpmsg = tmpmsg + "\r\n";

	m_Serial.Write(tmpmsg);
}

void CIMU_ROBORDlg::OnBnClickedButtonZa()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmpmsg;

	if (m_flag_cmd)
		m_flag_cmd = 0;

	tmpmsg = "za";
	tmpmsg = tmpmsg + "\r\n";

	m_Serial.Write(tmpmsg);
}

