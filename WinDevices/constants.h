#pragma once
#include <devpropdef.h> // DEVPROPKEY

using DevPropKeyItem = struct {
	const DEVPROPKEY* pKey;
	DEVPROPTYPE type;
	const wchar_t* pName;
};

using SetupClassItem = struct _SetupClassItem {
	std::wstring name;
	GUID guid;
};

using InterfaceClassItem = struct _InterfaceClassItem {
	std::wstring name;
	GUID guid;
};