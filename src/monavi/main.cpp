#include <iostream>
#include <gtkmm.h>
#include "mainwindow.hpp"

using namespace std;

int main(int argc, char *argv[]){
  Gtk::Main app(argc, argv);

  Glib::RefPtr<Gtk::Builder> appBuilder = Gtk::Builder::create_from_file("res.glade");
  MainWindow *mwin = nullptr;
  appBuilder->get_widget("MainWindow", mwin);
  
  app.run(*mwin);
  //Gtk::Dialog *settFrm = nullptr;
  //settBuilder->get_widget("SettingsDialog", settFrm);
  //if(nullptr != settFrm)
  //  settFrm->run();
  /*Settings &set = Settings::instance();
  set.loadFromFile("config.lua");
  cout << set.get<int>(std::string("serial_baud"));
  cout << set.get<std::string>(std::string("serial_parity"));
  */
  return 0;
}
