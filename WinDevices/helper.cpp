#include "helper.h"
#include <cassert>
#include <vector>

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
