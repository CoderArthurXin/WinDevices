#pragma once
#include <optional>
#include <string>
#include <Windows.h>
#include <devpropdef.h> // DEVPROPKEY
#include <SetupAPI.h> // HDEVINFO, SP_DEVINFO_DATA

std::optional<std::wstring> ReadGUID(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData);

std::optional<std::wstring> ReadString(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData);

std::optional<UINT32> ReadUint32(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData);