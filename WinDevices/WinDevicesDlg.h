
// WinDevicesDlg.h: 头文件
//

#pragma once
#include <SetupAPI.h>
#include <vector>
#include <map>
#include <string>
#include <any>
#include <variant>

using PropertyKeyValue = std::variant<std::wstring, UINT32>;
using DeviceProperties = std::map<std::wstring, PropertyKeyValue>;

// CWinDevicesDlg 对话框
class CWinDevicesDlg : public CDialogEx
{
	CString m_strDevicesInfo;
	CString m_strSetupClass;
	CString m_strInterfaceClass;
	CString m_strEnumerator;

	CComboBox m_ComboSetupClass;
	CComboBox m_ComboInterfaceClass;
	CComboBox m_ComboEnumerator;

// 构造
public:
	CWinDevicesDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDEVICES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnClrear();
	afx_msg void OnBnClickedBtnEnum();
	afx_msg void OnSelchangeComboSetupClass();
	afx_msg void OnSelchangeComboInterfaceClass();

	void InitSetupClassCombo();
	void InitInterfaceClassCombo();
	void InitEnumeratorCombo();
	
	void GetDevicesInfo(HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData, DeviceProperties& devInfo) const;
	bool GetClassGuid(GUID* guid, bool& isSetup) const;
	afx_msg void OnBnClickedBtnSave();
};
