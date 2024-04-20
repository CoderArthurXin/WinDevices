
// WinDevicesDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WinDevices.h"
#include "WinDevicesDlg.h"
#include "afxdialogex.h"
#include "helper.h"
#include "constants.h"
#include <WinBase.h>
#include <initguid.h>   // include before devpropdef.h
#include <Windows.h>
#include <devpropdef.h>
#include <devpkey.h>
#include <string>
#include <vector>
#include <devguid.h>
#include <variant>

#include <format>
#include <set>
#include "interfaceGuids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment (lib, "Setupapi.lib")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CWinDevicesDlg 对话框



CWinDevicesDlg::CWinDevicesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDEVICES_DIALOG, pParent)
	, m_strDevicesInfo(_T(""))
	, m_strSetupClass(_T(""))
	, m_strInterfaceClass(_T(""))
	, m_strEnumerator(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinDevicesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVICES_INFO, m_strDevicesInfo);
	DDX_CBString(pDX, IDC_COMBO_SETUP_CLASS, m_strSetupClass);
	DDX_CBString(pDX, IDC_COMBO_INTERFACE_CLASS, m_strInterfaceClass);
	DDX_CBString(pDX, IDC_COMBO_ENUMERATOR, m_strEnumerator);
	DDX_Control(pDX, IDC_COMBO_SETUP_CLASS, m_ComboSetupClass);
	DDX_Control(pDX, IDC_COMBO_INTERFACE_CLASS, m_ComboInterfaceClass);
	DDX_Control(pDX, IDC_COMBO_ENUMERATOR, m_ComboEnumerator);
}

BEGIN_MESSAGE_MAP(CWinDevicesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CLREAR, &CWinDevicesDlg::OnBnClickedBtnClrear)
	ON_BN_CLICKED(IDC_BTN_ENUM, &CWinDevicesDlg::OnBnClickedBtnEnum)
	ON_CBN_SELCHANGE(IDC_COMBO_SETUP_CLASS, &CWinDevicesDlg::OnSelchangeComboSetupClass)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE_CLASS, &CWinDevicesDlg::OnSelchangeComboInterfaceClass)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CWinDevicesDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CWinDevicesDlg 消息处理程序

BOOL CWinDevicesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitSetupClassCombo();
	InitInterfaceClassCombo();
	InitEnumeratorCombo();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWinDevicesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinDevicesDlg::OnPaint()
{
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
HCURSOR CWinDevicesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

const std::vector<DevPropKeyItem> _PropKeysPtr{
	{&DEVPKEY_NAME,                          DEVPROP_TYPE_STRING,  L"DEVPKEY_NAME"},
	{&DEVPKEY_Device_DeviceDesc,             DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DeviceDesc"},
	{&DEVPKEY_Device_Service,                DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Service"},
	{&DEVPKEY_Device_Class,                  DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Class"},
	{&DEVPKEY_Device_ClassGuid,              DEVPROP_TYPE_GUID,    L"DEVPKEY_Device_ClassGuid"},
	//{&DEVPKEY_Device_Driver,                 DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Driver"},
	//{&DEVPKEY_Device_Manufacturer,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Manufacturer"},
	{&DEVPKEY_Device_FriendlyName,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FriendlyName"},
	//{&DEVPKEY_Device_LocationInfo,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_LocationInfo"},
	//{&DEVPKEY_Device_PDOName,                DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_PDOName"},
	{&DEVPKEY_Device_Capabilities,           DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Capabilities"},
	//{&DEVPKEY_Device_BusTypeGuid,            DEVPROP_TYPE_GUID,    L"DEVPKEY_Device_BusTypeGuid"},
	//{&DEVPKEY_Device_BusNumber,              DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_BusNumber"},
	{&DEVPKEY_Device_EnumeratorName,         DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_EnumeratorName"},
	//{&DEVPKEY_Device_DevType,                DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_DevType"},
	//{&DEVPKEY_Device_Characteristics,        DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Characteristics"},
	//{&DEVPKEY_Device_Address,                DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Address"},
	//{&DEVPKEY_Device_InstallState,           DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_InstallState"},
	//{&DEVPKEY_Device_LocationPaths,          DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_LocationPaths"},
	//{&DEVPKEY_Device_BaseContainerId,        DEVPROP_TYPE_GUID,         L"DEVPKEY_Device_BaseContainerId"},
	{&DEVPKEY_Device_InstanceId,             DEVPROP_TYPE_STRING,       L"DEVPKEY_Device_InstanceId"},
	{&DEVPKEY_Device_Parent,                 DEVPROP_TYPE_STRING,       L"DEVPKEY_Device_Parent"},
	//{&DEVPKEY_Device_Children,               DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_Children"},
	//{&DEVPKEY_Device_Siblings,               DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_Siblings"},
	{&DEVPKEY_Device_ContainerId,            DEVPROP_TYPE_GUID,         L"DEVPKEY_Device_ContainerId"},
	//{&DEVPKEY_Device_FirmwareDate,           DEVPROP_TYPE_FILETIME,     L"DEVPKEY_Device_FirmwareDate"},
	//{&DEVPKEY_Device_FirmwareVersion,        DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FirmwareVersion"},
	//{&DEVPKEY_Device_FirmwareRevision,       DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FirmwareRevision"},
	//{&DEVPKEY_Device_SessionId,              DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_SessionId"},
	//{&DEVPKEY_Device_DriverVersion,          DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DriverVersion"},
	//{&DEVPKEY_Device_DriverDesc,             DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DriverDesc"},
};

void CWinDevicesDlg::GetDevicesInfo(HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData) {
	CString strKeyInfo;
	for (const auto& prop : _PropKeysPtr) {
		strKeyInfo.Empty();
		if (prop.type == DEVPROP_TYPE_STRING) {
			if (auto sValue = ReadString(prop.pKey, hDevInfoSet, deviceInfoData); sValue.has_value()) {
				strKeyInfo.Format(_T("%s: %s"), prop.pName, sValue.value().c_str());
			}
		}
		else if (prop.type == DEVPROP_TYPE_UINT32) {
			if (auto uValue = ReadUint32(prop.pKey, hDevInfoSet, deviceInfoData); uValue.has_value()) {
				strKeyInfo.Format(_T("%s: %d"), prop.pName, uValue.value());
			}
		}
		else if (prop.type == DEVPROP_TYPE_GUID) {
			if (auto guid = ReadGUID(prop.pKey, hDevInfoSet, deviceInfoData); guid.has_value()) {
				strKeyInfo.Format(_T("%s: %s"), prop.pName, guid.value().c_str());
			}

		}

		if (!strKeyInfo.IsEmpty()) m_strDevicesInfo += strKeyInfo + _T("\r\n");
	}

	m_strDevicesInfo += _T("\r\n\r\n");
}

void CWinDevicesDlg::OnBnClickedBtnClrear()
{
	m_strDevicesInfo.Empty();
	UpdateData(FALSE);
}

bool CWinDevicesDlg::GetClassGuid(GUID* guid, bool& isSetup) const
{
	auto selectedGUIDStr = m_strSetupClass.IsEmpty() ? CString(L"") : m_strSetupClass;
	selectedGUIDStr = m_strInterfaceClass.IsEmpty() ? selectedGUIDStr : m_strInterfaceClass;

	auto offset = selectedGUIDStr.Find(L"{");
	if (-1 == offset) return false;

	selectedGUIDStr = selectedGUIDStr.Mid(offset);
	if (selectedGUIDStr.CompareNoCase(_T("{00000000-0000-0000-0000-000000000000}")) == 0) {
		return false;
	}

	if (auto hr = CLSIDFromString(selectedGUIDStr, guid); SUCCEEDED(hr)) {
		isSetup = !m_strSetupClass.IsEmpty();
		return true;
	}

	return false;
}

void CWinDevicesDlg::OnBnClickedBtnEnum()
{
	UpdateData(TRUE);

	GUID guid;
	auto isSetup = false;
	auto hasClassGuid = GetClassGuid(&guid, isSetup);

	auto selectSetup = hasClassGuid && isSetup;
	auto selectInterface = hasClassGuid && !isSetup;
	auto selectEnumerator = !m_strEnumerator.IsEmpty();

	DWORD flags = DIGCF_PRESENT;
	if (selectSetup && !selectEnumerator) {
		flags = DIGCF_PRESENT;
	}
	else if (selectSetup && selectEnumerator) {
		flags = DIGCF_PRESENT;
	}
	else if (selectInterface && !selectEnumerator) {
		flags = DIGCF_PRESENT | DIGCF_DEVICEINTERFACE;
	}
	else if (selectInterface && selectEnumerator) {
		flags = DIGCF_PRESENT | DIGCF_DEVICEINTERFACE;
	}
	else {
		flags = DIGCF_PRESENT | DIGCF_ALLCLASSES;
	}

	auto hDeviceInfoSet = SetupDiGetClassDevs(
		hasClassGuid ? &guid : nullptr,
		m_strEnumerator.IsEmpty() ? nullptr : m_strEnumerator.GetString(),
		nullptr,
		flags);
	if (INVALID_HANDLE_VALUE == hDeviceInfoSet) {
		auto error = std::format(_T("Last Error: {}"), ::GetLastError());
		m_strDevicesInfo += error.c_str();
		UpdateData(FALSE);
		return;
	}

	DWORD deviceIndex = 0;
	SP_DEVINFO_DATA deviceInfoData;
	ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	while (SetupDiEnumDeviceInfo(
		hDeviceInfoSet,
		deviceIndex,
		&deviceInfoData)) {
		deviceIndex++;

		m_strDevicesInfo += std::format(_T("Index: {}\r\n"), deviceIndex).c_str();
		GetDevicesInfo(hDeviceInfoSet, deviceInfoData);
	}

	if (hDeviceInfoSet) {
		SetupDiDestroyDeviceInfoList(hDeviceInfoSet);
	}

	UpdateData(FALSE);
}

void CWinDevicesDlg::OnSelchangeComboSetupClass()
{
	UpdateData(TRUE);
	m_strInterfaceClass.Empty();
	m_ComboInterfaceClass.SetCurSel(-1);
	UpdateData(FALSE);
}

void CWinDevicesDlg::OnSelchangeComboInterfaceClass()
{
	UpdateData(TRUE);
	m_strSetupClass.Empty();
	m_ComboSetupClass.SetCurSel(-1);
	UpdateData(FALSE);
}

void CWinDevicesDlg::InitEnumeratorCombo()
{
	const std::vector<std::wstring> enumerators{
		L"",
		L"ACPI",
		L"ACPI_HAL",
		L"BTH",
		L"BTHENUM",
		L"BTHHFENUM",
		L"BTHLE",
		L"BTHLEDEVICE",
		L"DISPLAY",
		L"HID",
		L"HTREE",
		L"INDIRECTDSP",
		L"INTELAUDIO",
		L"PCI",
		L"ROOT",
		L"RZCONTROL",
		L"RZVIRTUAL",
		L"SCSI",
		L"STORAGE",
		L"SWD",
		L"UEFI",
		L"UMB",
		L"USB",
		L"{5D624F94-8850-40C3-A3FA-A4FD2080BAF3}",
	};
	for (const auto& enumerator : enumerators) {
		m_ComboEnumerator.AddString(enumerator.c_str());
	}

	UpdateData(FALSE);
}

void CWinDevicesDlg::InitSetupClassCombo()
{
	const std::vector<SetupClassItem> setups{
		 {L"Empty", GUID()},
		 {L"GUID_DEVCLASS_ADAPTER", GUID_DEVCLASS_ADAPTER},
		 {L"GUID_DEVCLASS_BATTERY", GUID_DEVCLASS_BATTERY},
		 {L"GUID_DEVCLASS_BLUETOOTH", GUID_DEVCLASS_BLUETOOTH},
		 {L"GUID_DEVCLASS_CAMERA", GUID_DEVCLASS_CAMERA},
		 {L"GUID_DEVCLASS_CDROM", GUID_DEVCLASS_CDROM},
		 {L"GUID_DEVCLASS_COMPUTEACCELERATOR", GUID_DEVCLASS_COMPUTEACCELERATOR},
		 {L"GUID_DEVCLASS_COMPUTER", GUID_DEVCLASS_COMPUTER},
		 {L"GUID_DEVCLASS_FIRMWARE", GUID_DEVCLASS_FIRMWARE},
		 {L"GUID_DEVCLASS_FLOPPYDISK", GUID_DEVCLASS_FLOPPYDISK},
		 {L"GUID_DEVCLASS_GPS", GUID_DEVCLASS_GPS},
		 {L"GUID_DEVCLASS_HDC", GUID_DEVCLASS_HDC},
		 {L"GUID_DEVCLASS_HIDCLASS", GUID_DEVCLASS_HIDCLASS},
		 {L"GUID_DEVCLASS_IMAGE", GUID_DEVCLASS_IMAGE},
		 {L"GUID_DEVCLASS_INFINIBAND", GUID_DEVCLASS_INFINIBAND},
		 {L"GUID_DEVCLASS_INFRARED", GUID_DEVCLASS_INFRARED},
		 {L"GUID_DEVCLASS_KEYBOARD", GUID_DEVCLASS_KEYBOARD},
		 {L"GUID_DEVCLASS_MEDIA", GUID_DEVCLASS_MEDIA},
		 {L"GUID_DEVCLASS_MEMORY", GUID_DEVCLASS_MEMORY},
		 {L"GUID_DEVCLASS_MODEM", GUID_DEVCLASS_MODEM},
		 {L"GUID_DEVCLASS_MONITOR", GUID_DEVCLASS_MONITOR},
		 {L"GUID_DEVCLASS_MOUSE", GUID_DEVCLASS_MOUSE},
		 {L"GUID_DEVCLASS_NET", GUID_DEVCLASS_NET},
		 {L"GUID_DEVCLASS_PORTS", GUID_DEVCLASS_PORTS},
		 {L"GUID_DEVCLASS_PRINTER", GUID_DEVCLASS_PRINTER},
		 {L"GUID_DEVCLASS_PROCESSOR", GUID_DEVCLASS_PROCESSOR},
		 {L"GUID_DEVCLASS_SOFTWARECOMPONENT", GUID_DEVCLASS_SOFTWARECOMPONENT},
		 {L"GUID_DEVCLASS_SOUND", GUID_DEVCLASS_SOUND},
		 {L"GUID_DEVCLASS_SYSTEM", GUID_DEVCLASS_SYSTEM},
		 {L"GUID_DEVCLASS_TAPEDRIVE", GUID_DEVCLASS_TAPEDRIVE},
		 {L"GUID_DEVCLASS_USB", GUID_DEVCLASS_USB},
		 {L"GUID_DEVCLASS_VOLUME", GUID_DEVCLASS_VOLUME}
	};

	wchar_t containerIdStr[40];
	for (const auto& stup : setups) {
		StringFromGUID2(stup.guid, containerIdStr, ARRAYSIZE(containerIdStr));

		auto item = std::format(L"{}: {}", stup.name, containerIdStr);
		m_ComboSetupClass.AddString(item.c_str());
	}

	UpdateData(FALSE);
}

void CWinDevicesDlg::InitInterfaceClassCombo()
{
	const std::vector<InterfaceClassItem> iterfaces{
		{L"Empty", GUID()},
		{L"GUID_DEVINTERFACE_MOUSE", GUID_DEVINTERFACE_MOUSE},
		{L"GUID_DEVINTERFACE_KEYBOARD", GUID_DEVINTERFACE_KEYBOARD},
		{L"GUID_DEVINTERFACE_USB_HUB", GUID_DEVINTERFACE_USB_HUB},
		{L"GUID_DEVINTERFACE_USB_DEVICE", GUID_DEVINTERFACE_USB_DEVICE},
		{L"GUID_DEVINTERFACE_MONITOR", GUID_DEVINTERFACE_MONITOR},
		{L"GUID_DEVINTERFACE_HID", GUID_DEVINTERFACE_HID},
		{L"GUID_DEVINTERFACE_I2C", GUID_DEVINTERFACE_I2C},
		{L"GUID_DEVINTERFACE_BRIGHTNESS", GUID_DEVINTERFACE_BRIGHTNESS},
		{L"GUID_DEVINTERFACE_DISK", GUID_DEVINTERFACE_DISK},
		{L"GUID_DEVINTERFACE_TAPE", GUID_DEVINTERFACE_TAPE},
		{L"GUID_DEVINTERFACE_VOLUME", GUID_DEVINTERFACE_VOLUME},
		{L"GUID_DEVINTERFACE_STORAGEPORT", GUID_DEVINTERFACE_STORAGEPORT},
		{L"GUID_DEVINTERFACE_CDROM", GUID_DEVINTERFACE_CDROM},
		{L"GUID_DEVINTERFACE_COMPORT", GUID_DEVINTERFACE_COMPORT},
		{L"GUID_DEVINTERFACE_IMAGE", GUID_DEVINTERFACE_IMAGE},
		{L"GUID_DEVINTERFACE_MODEM", GUID_DEVINTERFACE_MODEM},
		{L"GUID_DEVINTERFACE_NET", GUID_DEVINTERFACE_NET},
	};

	wchar_t containerIdStr[40];
	for (const auto& ifc : iterfaces) {
		StringFromGUID2(ifc.guid, containerIdStr, ARRAYSIZE(containerIdStr));

		auto item = std::format(L"{}: {}", ifc.name, containerIdStr);
		m_ComboInterfaceClass.AddString(item.c_str());
	}

	UpdateData(FALSE);
}


void CWinDevicesDlg::OnBnClickedBtnSave()
{
	UpdateData(TRUE);

	if (m_strDevicesInfo.IsEmpty()) {
		MessageBox(_T("No data to save."));
		return;
	}

	CString strFileName;
	auto strGuidClass = m_strSetupClass.IsEmpty() ? m_strInterfaceClass : m_strSetupClass;
	if (auto offset = strGuidClass.Find(_T(":")); -1 != offset) {
		strGuidClass = strGuidClass.Mid(0, offset);
		strGuidClass.Trim();

		strFileName += strGuidClass;
	}


	if (!m_strEnumerator.IsEmpty()) {
		strFileName += _T("___") + m_strEnumerator;
	}

	if (strFileName.IsEmpty()) {
		strFileName.Format(_T("%04d_%04d_%04d"), ::rand(), ::rand(), ::rand());
	}

	strFileName += _T(".txt");

	CFile file;
	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
		MessageBox(_T("Create file failed."));
		return;
	}

	file.Write(m_strDevicesInfo.GetString(), m_strDevicesInfo.GetLength() * sizeof(TCHAR));
	file.Close();
}
