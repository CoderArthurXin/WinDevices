#include "helper.h"
#include <cassert>
#include <vector>

std::string ConvertWideToUTF8(const wchar_t* wideString)
{
	if (nullptr == wideString)
		return "";

	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wideString, lstrlenW(wideString), nullptr, 0, 0, 0);
	std::string utf8String(requiredSize, '\0');

	if (WideCharToMultiByte(CP_UTF8, 0, wideString, lstrlenW(wideString), &utf8String[0], requiredSize, 0, 0) == 0)
	{
		return "";
	}

	return utf8String;
}

std::wstring ConvertUTF8ToWide(const char* utf8String)
{
	if (nullptr == utf8String)
		return L"";

	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, utf8String, strlen(utf8String), nullptr, 0);
	std::wstring wideString(requiredSize, L'\0');

	if (MultiByteToWideChar(CP_UTF8, 0, utf8String, strlen(utf8String), &wideString[0], requiredSize) == 0)
	{
		return L"";
	}

	return wideString;
}

std::optional<std::wstring> ReadGUID(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData)
{
	GUID containerId;
	DWORD bufferSize = sizeof(GUID);

	DEVPROPTYPE propType;
	std::wstring propertyValue;
	if (SetupDiGetDeviceProperty(
		hDevInfoSet,
		&deviceInfoData,
		key,
		&propType,
		reinterpret_cast<PBYTE>(&containerId),
		bufferSize,
		&bufferSize,
		0)) {
		assert(propType == DEVPROP_TYPE_GUID);
		wchar_t containerIdStr[40];
		if (auto r = StringFromGUID2(containerId, containerIdStr, ARRAYSIZE(containerIdStr)); r > 0) {
			propertyValue = containerIdStr;
		}

	}

	if (propertyValue.empty()) return std::nullopt;

	return propertyValue;
}

std::optional<std::wstring> ReadString(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData)
{
	DEVPROPTYPE propType;
	DWORD requiredSize = 0;

	SetupDiGetDeviceProperty(
		hDevInfoSet,
		&deviceInfoData,
		key,
		&propType,
		nullptr,
		0,
		&requiredSize,
		0);

	if (requiredSize == 0) return std::nullopt;
	assert(propType == DEVPROP_TYPE_STRING);

	std::wstring propertyValue;
	auto pDataBuffer = (BYTE*)malloc(requiredSize);
	if (SetupDiGetDeviceProperty(
		hDevInfoSet,
		&deviceInfoData,
		key,
		&propType,
		pDataBuffer,
		requiredSize,
		&requiredSize,
		0)) {
		propertyValue = reinterpret_cast<wchar_t*>(pDataBuffer);
	}

	free(pDataBuffer);

	if (propertyValue.empty()) return std::nullopt;

	return propertyValue;
}

std::optional<UINT32> ReadUint32(const DEVPROPKEY* key, HDEVINFO hDevInfoSet, SP_DEVINFO_DATA& deviceInfoData)
{
	DEVPROPTYPE propType;
	DWORD requiredSize = 0;

	SetupDiGetDeviceProperty(
		hDevInfoSet,
		&deviceInfoData,
		key,
		&propType,
		nullptr,
		0,
		&requiredSize,
		0);

	if (requiredSize == 0) return std::nullopt;
	assert(propType == DEVPROP_TYPE_UINT32);

	UINT32 propertyValue;
	if (std::vector<BYTE> buffer(requiredSize); SetupDiGetDeviceProperty(
		hDevInfoSet,
		&deviceInfoData,
		key,
		&propType,
		buffer.data(),
		buffer.size(),
		&requiredSize,
		0)) {
		propertyValue = *reinterpret_cast<UINT32*>(buffer.data());
	}

	return propertyValue;
}
