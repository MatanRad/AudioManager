#ifndef ENDPOINT_ITERATOR_H
#define ENDPOINT_ITERATOR_H

#define DLL_EXPORT __declspec( dllexport )

#include <vector>

#include "Endpoint.h"

using namespace std;

namespace Endpoints
{
	class DLL_EXPORT EndpointIterator
	{
		IMMDeviceEnumerator* enumerator;

		IMMDeviceCollection* GetDeviceCollection() const;

	public:

		vector<Endpoint> GetEndpoints() const;

		Endpoint GetEndpointByName(const char* name) const;
		Endpoint GetEndpointByID(const char* id) const;
		Endpoint GetDefaultEndpoint(EndpointDefaultType defType) const;

		EndpointIterator();
		~EndpointIterator();
	};
}

#endif