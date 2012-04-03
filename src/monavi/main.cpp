#include <iostream>

#include "settings.hpp"

using namespace std;

int main(int argc, char *argv[]){
  Settings::instance().loadFromFile("config.lua");
  return 0;
}
