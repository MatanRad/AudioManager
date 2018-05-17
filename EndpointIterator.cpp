#include "EndpointIterator.h"
#include "Defines.h"
#include "Exceptions/EndpointNotFoundException.h"
#include "StringConversions.h"

#include <cstring>


using namespace std;
using namespace Endpoints;

IMMDeviceCollection* Endpoints::EndpointIterator::GetDeviceCollection() const
{
	IMMDeviceCollection* collection;
	HRESULT res;

	res = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &collection);
	if (res != S_OK)
	{
		HANDLE_ERROR("Error retrieving IMMDeviceCollection.");
	}

	return collection;
}

vector<Endpoint> Endpoints::EndpointIterator::GetEndpoints() const
{
	auto endpoints = vector<Endpoint>();
	HRESULT res;
	size_t count;
	IMMDeviceCollection* collection = this->GetDeviceCollection();
	IMMDevice *currDevice;

	res = collection->GetCount(&count);
	if (res != S_OK)
	{
		collection->Release();
		HANDLE_ERROR("Error when getting collection count.");
	}

	for (int i = 0; i < count; i++)
	{
		res = collection->Item(i, &currDevice);
		if (res != S_OK)
		{
			collection->Release();
			HANDLE_ERROR("Error when getting collection item.");
		}

		endpoints.push_back(Endpoint(currDevice));
	}

	collection->Release();

	return endpoints;
}

Endpoint Endpoints::EndpointIterator::GetEndpointByName(const char* name) const
{
	auto endpoints = this->GetEndpoints();
	size_t count = endpoints.size();

	for (int i = 0; i < count; i++)
	{
		if (strcmp(name, endpoints[i].GetName()) == 0)
		{
			return endpoints[i];
		}
	}

	throw Exceptions::EndpointNotFoundException();
}

Endpoint Endpoints::EndpointIterator::GetEndpointByID(const char* id) const
{
	HRESULT res;
	wstring wid = strtowstr(string(id));
	IMMDevice* device;
	
	res = enumerator->GetDevice(wid.c_str(), &device);
	if (res != S_OK)
	{
		if (res == E_NOTFOUND) throw Exceptions::EndpointNotFoundException();
		else HANDLE_ERROR("Error occurred when requesting endpoint by ID.");
	}

	return Endpoint(device);
}

Endpoint Endpoints::EndpointIterator::GetDefaultEndpoint(EndpointDefaultType defType) const
{
	HRESULT res;
	IMMDevice *device;

	res = enumerator->GetDefaultAudioEndpoint(eRender, (defType == EndpointDefaultType::Comm ? eCommunications : eMultimedia), &device);
	if (res != S_OK)
	{
		HANDLE_ERROR("Error occurred when requesting default endpoint.");
	}
	
	return Endpoint(device);
}

EndpointIterator::EndpointIterator()
{
	HRESULT res;

	res = CoInitialize(NULL);
	if (res != S_OK && res != S_FALSE)
	{
		HANDLE_ERROR(COINITIALIZE_ERR);
	}

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	res = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)(&enumerator));
	if (res != S_OK)
	{
		HANDLE_ERROR("Error when creating IMMDeviceEnumerator.");
	}
}

EndpointIterator::~EndpointIterator()
{
	enumerator->Release();
	CoUninitialize();
}
