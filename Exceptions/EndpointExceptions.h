#ifndef ENDPOINT_FOUND_EXCEPTION_H
#define ENDPOINT_FOUND_EXCEPTION_H

#define DLL_EXPORT __declspec( dllexport )


//
// Further modifications required for this file.
//


namespace Endpoints
{
	namespace Exceptions
	{
		// Todo: add more info and context
		class DLL_EXPORT EndpointNotFoundException
		{
		public:
			const char* what() const throw ()
			{
				return "Requested endpoint not found while iterating!";
			}
		};

		class DLL_EXPORT InvalidEndpointIdException
		{
		public:
			const char* what() const throw ()
			{
				return "Requested endpoint ID is invalid!";
			}
		};
	}
}


#endif