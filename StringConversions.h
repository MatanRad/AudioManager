#ifndef STRING_CONVERSIONS_H
#define STRING_CONVERSIONS_H

#include<string>

std::string wstrtostr(const std::wstring& wstr);

std::wstring strtowstr(const std::string& str);

#endif