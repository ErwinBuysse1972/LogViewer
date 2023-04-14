#pragma once
#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <queue>
#include <deque>
#include <algorithm>

template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};

template<typename T, typename Container = std::vector<T>>
bool Contains(Container v, T val)
{
    if( std::count(v.begin(), v.end(), val) > 0)
        return true;
    return false;
}

template<typename Container = std::vector<std::string>>
bool InverseContains(Container v, std::string val)
{
    bool bReturn = false;
    std::for_each(v.begin(), v.end(), [=, &bReturn](std::string elem){
        if (val.find(elem) != std::string::npos)
            bReturn = true;
    });
    return bReturn;
}

class CConversion
{
private:
    unsigned char hexchar_to_int(char const ch);

public:
    static std::vector<std::string> split(const std::string& s, char delim, std::vector<std::string>& elems, bool bNoEmpty = false);
    static std::vector<std::string> Split(const char* pattern, char delim);
    static std::vector<std::string> Getline(std::istream in, std::string delims);
    static std::string ltrim(std::string s);
    static std::string rtrim(std::string s);
    static std::string trim(std::string s);
    std::vector<unsigned char> hexstr_to_bytes(std::string str);
};
