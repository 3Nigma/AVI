#ifndef _GVALUE_HPP_
#define _GVALUE_HPP_

#include <string>

class GValue {
public:
  GValue(const std::string &val);

  void setValue(const std::string &newVal);
  bool operator==(const GValue &rhs) const;

  operator int() const;
  operator std::string() const;
  operator double() const;
  operator float() const;
private:
  std::string mVal;
};

#endif
