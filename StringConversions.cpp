#include "StringConversions.h"
#include "Defines.h"

using namespace std;

string wstrtostr(const wstring& wstr)
{
	size_t numConverted;
	errno_t err;
	string result;
	char* buff;

	buff = new char[wstr.length() + 1];

	err = wcstombs_s(&numConverted, buff, wstr.length() + 1, wstr.c_str(), wstr.length());

	if (err != 0)
	{
		HANDLE_ERROR("Error when converting from wide-char string.");
	}

	result = string(buff);

	delete[] buff;
	return result;
}

wstring strtowstr(const string& str)
{
	size_t size = str.size() + 1;
	size_t numConverted = 0;
	wchar_t* buff = new wchar_t[size];
	errno_t err;
	wstring wstr;

	err = mbstowcs_s(&numConverted, buff, size, str.c_str(), size - 1);
	if (err != 0)
	{
		HANDLE_ERROR("Error when converting from multi-byte string.");
	}
	
	wstr = wstring(buff);
	delete[] buff;

	return wstr;
}