#ifndef ENDPOINT_NOT_FOUND_EXCEPTION_H
#define ENDPOINT_NOT_FOUND_EXCEPTION_H

#include <exception>

//
// Further modifications required for this file.
// Maybe change to Exceptions.h
//


namespace Endpoints
{
	namespace Exceptions
	{
		// Todo: add more info and context
		class EndpointNotFoundException : public std::exception
		{
		public:
			const char* what() const throw ()
			{
				return "Requested endpoint not found while iterating!";
			}
		};
	}
}


#endif