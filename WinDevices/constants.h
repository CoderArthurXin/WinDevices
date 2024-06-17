#pragma once
#include <devpropdef.h> // DEVPROPKEY

using AX_PropertyKeyValue = std::variant<std::wstring, UINT32>;
using AX_DeviceProperties = std::map<std::wstring, AX_PropertyKeyValue>;

using AX_DevPropKeyItem = struct {
	const DEVPROPKEY* pKey;
	DEVPROPTYPE type;
	const wchar_t* pName;
};

using AX_SetupClassItem = struct _SetupClassItem {
	std::wstring name;
	GUID guid;
};

using AX_InterfaceClassItem = struct _InterfaceClassItem {
	std::wstring name;
	GUID guid;
};

const std::vector<AX_DevPropKeyItem> _PropKeysPtr = {
	{&DEVPKEY_NAME,                          DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_AOrder_NAME"},
	{&DEVPKEY_Device_DeviceDesc,             DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DeviceDesc"},
	{&DEVPKEY_Device_Service,                DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Service"},
	{&DEVPKEY_Device_Class,                  DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Class"},
	{&DEVPKEY_Device_ClassGuid,              DEVPROP_TYPE_GUID,    L"DEVPKEY_Device_ClassGuid"},
	{&DEVPKEY_Device_Driver,                 DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Driver"},
	{&DEVPKEY_Device_Manufacturer,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_Manufacturer"},
	{&DEVPKEY_Device_FriendlyName,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FriendlyName"},
	{&DEVPKEY_Device_LocationInfo,           DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_LocationInfo"},
	{&DEVPKEY_Device_PDOName,                DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_PDOName"},
	{&DEVPKEY_Device_Capabilities,           DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Capabilities"},
	{&DEVPKEY_Device_BusTypeGuid,            DEVPROP_TYPE_GUID,    L"DEVPKEY_Device_BusTypeGuid"},
	{&DEVPKEY_Device_BusNumber,              DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_BusNumber"},
	{&DEVPKEY_Device_EnumeratorName,         DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_EnumeratorName"},
	{&DEVPKEY_Device_DevType,                DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_DevType"},
	{&DEVPKEY_Device_Characteristics,        DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Characteristics"},
	{&DEVPKEY_Device_Address,                DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_Address"},
	{&DEVPKEY_Device_InstallState,           DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_InstallState"},
	//{&DEVPKEY_Device_LocationPaths,          DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_LocationPaths"},
	{&DEVPKEY_Device_BaseContainerId,        DEVPROP_TYPE_GUID,         L"DEVPKEY_Device_BaseContainerId"},
	{&DEVPKEY_Device_InstanceId,             DEVPROP_TYPE_STRING,       L"DEVPKEY_Device_InstanceId"},
	{&DEVPKEY_Device_Parent,                 DEVPROP_TYPE_STRING,       L"DEVPKEY_Device_Parent"},
	//{&DEVPKEY_Device_Children,               DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_Children"},
	//{&DEVPKEY_Device_Siblings,               DEVPROP_TYPE_STRING_LIST,  L"DEVPKEY_Device_Siblings"},
	{&DEVPKEY_Device_ContainerId,            DEVPROP_TYPE_GUID,         L"DEVPKEY_Device_ContainerId"},
	{&DEVPKEY_Device_FirmwareDate,           DEVPROP_TYPE_FILETIME,     L"DEVPKEY_Device_FirmwareDate"},
	{&DEVPKEY_Device_FirmwareVersion,        DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FirmwareVersion"},
	{&DEVPKEY_Device_FirmwareRevision,       DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_FirmwareRevision"},
	{&DEVPKEY_Device_SessionId,              DEVPROP_TYPE_UINT32,  L"DEVPKEY_Device_SessionId"},
	{&DEVPKEY_Device_DriverVersion,          DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DriverVersion"},
	{&DEVPKEY_Device_DriverDesc,             DEVPROP_TYPE_STRING,  L"DEVPKEY_Device_DriverDesc"},
};

