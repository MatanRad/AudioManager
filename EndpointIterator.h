#ifndef ENDPOINT_ITERATOR_H
#define ENDPOINT_ITERATOR_H

#include <vector>

#include "Endpoint.h"

using namespace std;

namespace Endpoints
{
	class EndpointIterator
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