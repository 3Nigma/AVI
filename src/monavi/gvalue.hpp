#ifndef _GSVALUE_HPP_
#define _GSVALUE_HPP_

#include <string>

class GSValue {
public:
  GSValue(const std::string &val);

  void setValue(const std::string &newVal);
  bool operator==(const GSValue &rhs) const;

  operator int() const;
  operator std::string() const;
  operator double() const;
  operator float() const;
private:
  std::string mVal;
};

#endif
