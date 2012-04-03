#include "gvalue.hpp"

GValue::GValue(const std::string &val) 
  : mVal(val) {

}

void GValue::setValue(const std::string &newVal) {
  mVal = newVal;
}

bool GValue::operator==(const GValue &rhs) const {
  return mVal == rhs.mVal;
}

GValue::operator int() const {
  return stoi(mVal);
}

GValue::operator std::string() const {
  return mVal;
}

GValue::operator double() const {
  return stod(mVal);
}

GValue::operator float() const {
  return stof(mVal);
}

