#include <iostream>

#include "settings.hpp"

using namespace std;

int main(int argc, char *argv[]){
  Settings &set = Settings::instance();
  set.loadFromFile("config.lua");
  cout << set.get<int>(std::string("serial_baud"));
  cout << set.get<std::string>(std::string("serial_parity"));
  return 0;
}
