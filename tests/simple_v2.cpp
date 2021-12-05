#include <cmrc/cmrc.hpp>
#include <iostream>
#include <istream>
#include <streambuf>

CMRC_DECLARE(simple_v2);

// https://stackoverflow.com/questions/13059091/creating-an-input-stream-from-constant-memory
struct membuf : std::streambuf {
  membuf(const char* cbegin, const char* cend) {
    char* begin = const_cast<char*>(cbegin);
    char* end = const_cast<char*>(cend);
    this->setg(begin, begin, end);
  }
};

struct imemstream : virtual membuf, std::istream {
  imemstream(const char* cbegin, const char* cend)
      : membuf(cbegin, cend),
        std::istream(static_cast<std::streambuf*>(this)) {}
};

int main() {
  auto fs = cmrc::simple_v2::get_filesystem();
  auto data = fs.open("hello.txt");

  imemstream in(data.begin(), data.end());
  std::string line;
  while (std::getline(in, line)) {
    std::cout << line << '\n';
  }

  return 0;
}
