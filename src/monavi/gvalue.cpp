#include "gvalue.hpp"

GSValue::GSValue(const std::string &val) 
  : mVal(val) {

}

void GSValue::setValue(const std::string &newVal) {
  mVal = newVal;
}

bool GSValue::operator==(const GSValue &rhs) const {
  return mVal == rhs.mVal;
}

GSValue::operator int() const {
  return stoi(mVal);
}

GSValue::operator std::string() const {
  return mVal;
}

GSValue::operator double() const {
  return stod(mVal);
}

GSValue::operator float() const {
  return stof(mVal);
}

