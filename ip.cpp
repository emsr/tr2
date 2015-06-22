// $HOME/bin/bin/g++ -std=c++1y -o ip ip.cpp

// LD_LIBRARY_PATH=$HOME/bin/lib64:$LD_LIBRARY_PATH ./ip

#include <array>
#include <iostream>
#include <iomanip>
#include <regex>

struct IPv4
{
  std::array<int,4> ip;
};

template<typename _CharT, typename _Traits>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& os, const IPv4& ip)
  {
    _CharT c = os.fill(_CharT('0'));
    os << '[' << std::setw(3) << ip.ip[0]
       << '.' << std::setw(3) << ip.ip[1]
       << '.' << std::setw(3) << ip.ip[2]
       << '.' << std::setw(3) << ip.ip[3] << ']';
    os.fill(c);
    return os;
  }

IPv4
operator""_ip(const char* nstr)
{
  std::cout << nstr << '\n';
  IPv4 ip;
  std::cmatch mres;
  std::regex re{R"((\d{1,3})'(\d{1,3})'(\d{1,3})'(\d{1,3}))"};
  bool ok = std::regex_match(nstr, mres, re);
  if (ok && mres.size() == 5)
  {
    for (int i = 1; i < mres.size(); ++i)
      ip.ip[i - 1] = std::stoi(mres[i].str());
  }
  return ip;
}

int
main()
{
  IPv4 eth0 = 172'20'70'115_ip;
  std::cout << eth0 << std::endl;

  IPv4 lo = 127'0'0'1_ip;
  std::cout << lo << std::endl;
}
