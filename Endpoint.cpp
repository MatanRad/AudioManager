#include <Endpointvolume.h>
#include <stdlib.h>
#include <atlstr.h>
#include <string>

#include "Endpoint.h"
#include "StringConversions.h"
#include "Defines.h"
#include "PolicyConfig.h"
using namespace Endpoints;
using namespace std;

// Taken from "FunctionDiscoveryKeys_devpkey.h" as it will not compile.
DEFINE_PROPERTYKEY(PKEY_Device_DeviceDesc, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 2);     // DEVPROP_TYPE_STRING
DEFINE_PROPERTYKEY(PKEY_DeviceInterface_FriendlyName, 0x026e516e, 0xb814, 0x414b, 0x83, 0xcd, 0x85, 0x6d, 0x6f, 0xef, 0x48, 0x22, 2); // DEVPROP_TYPE_STRING

string GetKeyFromPropStore(IPropertyStore *propstore, const PROPERTYKEY& key)
{
	PROPVARIANT val;
	HRESULT hr;

	PropVariantInit(&val);

	hr = propstore->GetValue(key, &val);
	if (hr != S_OK && hr != INPLACE_S_TRUNCATED)
	{
		HANDLE_ERROR("key not found in key store.");
	}

	return wstrtostr(wstring(val.pwszVal));
}


string GetNameFromPropStore(IPropertyStore *propstore)
{
	return GetKeyFromPropStore(propstore, PKEY_Device_DeviceDesc);
}


string GetControllerFromPropStore(IPropertyStore *propstore)
{
	return GetKeyFromPropStore(propstore, PKEY_DeviceInterface_FriendlyName);
}


Endpoint::Endpoint(IMMDevice* device) : device(device) 
{
	IPropertyStore* store;
	HRESULT res;

	string nname, ncontroller;

	this->device->AddRef();

	res = CoInitialize(NULL);
	if (res != S_OK && res != S_FALSE)
	{
		HANDLE_ERROR(COINITIALIZE_ERR);
	}

	res = device->OpenPropertyStore(STGM_READ, &store);
	if (res != S_OK)
	{
		HANDLE_ERROR("Can't open IPropertyStore.");
	}

	nname = GetNameFromPropStore(store);
	ncontroller = GetControllerFromPropStore(store);

	this->name = new char[nname.size()+1];
	this->controller = new char[ncontroller.size()+1];

	strncpy_s(this->name, nname.size() + 1, nname.c_str(), nname.size());
	strncpy_s(this->controller, ncontroller.size() + 1, ncontroller.c_str(), ncontroller.size());
}

Endpoints::Endpoint::Endpoint(const Endpoint& endpoint)
{
	CopyFrom(endpoint);
}

Endpoints::Endpoint::~Endpoint()
{
	Release();
}

IAudioEndpointVolume* Endpoint::GetAudioEndpointVolume() const
{
	IAudioEndpointVolume* endvol;
	HRESULT res;

	res = this->device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&endvol);
	if (res != S_OK)
	{
		HANDLE_ERROR("Cannot activate device.");
	}

	return endvol;
}

void Endpoints::Endpoint::Release()
{
	this->device->Release();

	delete[] this->name;
	delete[] this->controller;
	CoUninitialize();
}

void Endpoints::Endpoint::CopyFrom(const Endpoint & endpoint)
{
	string nname = string(endpoint.name);
	string ncontroller = string(endpoint.controller);
	HRESULT res;

	res = CoInitialize(NULL);
	if (res != S_OK && res != S_FALSE)
	{
		HANDLE_ERROR(COINITIALIZE_ERR);
	}

	endpoint.device->AddRef();

	this->device = endpoint.device;
	this->name = new char[nname.size() + 1];
	this->controller = new char[ncontroller.size() + 1];

	strncpy_s(this->name, nname.size() + 1, nname.c_str(), nname.size() + 1);
	strncpy_s(this->controller, ncontroller.size() + 1, ncontroller.c_str(), ncontroller.size() + 1);
}

LPWSTR Endpoints::Endpoint::GetID()
{
	LPWSTR id;
	HRESULT res;

	res = device->GetId(&id);
	if (res != S_OK)
	{
		HANDLE_ERROR("Can't get id of device");
	}

	return id;
}

const char * Endpoints::Endpoint::GetName() const
{
	return this->name;
}

const char * Endpoints::Endpoint::GetControllerName() const
{
	return this->controller;
}

void Endpoints::Endpoint::SetMute(bool mute)
{
	IAudioEndpointVolume* endvol = this->GetAudioEndpointVolume();
	
	endvol->SetMute(mute, NULL);
	
	endvol->Release();
}

bool Endpoints::Endpoint::GetMute() const
{
	IAudioEndpointVolume* endvol;
	HRESULT res;
	BOOL ismuted = 0;

	endvol = this->GetAudioEndpointVolume();
	res = endvol->GetMute(&ismuted);

	endvol->Release();

	if (res != S_OK)
	{
		HANDLE_ERROR("Error when getting device mute status.");
	}

	return ismuted;
}

void Endpoints::Endpoint::Mute()
{
	this->SetMute(true);
}

void Endpoints::Endpoint::Unmute()
{
	this->SetMute(false);
}

float Endpoints::Endpoint::GetVolumeFloat() const
{
	IAudioEndpointVolume* endvol;
	HRESULT res;
	float vol = 0;

	endvol = this->GetAudioEndpointVolume();
	res = endvol->GetMasterVolumeLevelScalar(&vol);

	endvol->Release();

	if (res != S_OK)
	{
		HANDLE_ERROR("Error when getting device volume.");
	}

	return vol;
}

int Endpoints::Endpoint::GetVolume() const
{
	return (int)(this->GetVolumeFloat() * 100);
}

void Endpoints::Endpoint::SetVolumeFloat(float vol)
{
	IAudioEndpointVolume* endvol;
	HRESULT res;

	endvol = this->GetAudioEndpointVolume();
	res = endvol->SetMasterVolumeLevelScalar(vol, NULL);

	endvol->Release();

	if (res != S_OK)
	{
		HANDLE_ERROR("Error when setting device volume.");
	}
}

void Endpoints::Endpoint::SetVolume(int vol)
{
	this->SetVolumeFloat(vol / 100.0f);
}

bool Endpoints::Endpoint::IsDefault(EndpointDefaultType defType) const
{
	return false;
}

void Endpoints::Endpoint::SetDefault(EndpointDefaultType defType)
{
	IPolicyConfigVista *con;
	HRESULT res;
	
	res = CoCreateInstance(__uuidof(CPolicyConfigVistaClient), NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&con);
	if (SUCCEEDED(res))
	{
		con->SetDefaultEndpoint(this->GetID(), (defType == EndpointDefaultType::Comm ? eCommunications : eMultimedia));
	}
	con->Release();
}

void Endpoints::Endpoint::SetDefaultMain()
{
	this->SetDefault(Endpoints::EndpointDefaultType::Main);
}

void Endpoints::Endpoint::SetDefaultComm()
{
	this->SetDefault(Endpoints::EndpointDefaultType::Comm);
}

Endpoint & Endpoints::Endpoint::operator=(const Endpoint & other)
{
	Release();
	CopyFrom(other);

	return *this;
}

