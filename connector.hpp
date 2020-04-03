#ifndef _CONNECTOR_HPP_
#define _CONNECTOR_HPP_


#include <string>

class Connector
{
private:
  void setup_my_socket();
  void connect_to_target();
  void do_communication();
  void safe_exit();

  void send(std::string);
  void receive();

  int the_socket;
  int thread_id;
  std::string my_port;
  std::string their_host;
  std::string their_port;

public:
  Connector(int t_thread_id, std::string t_my_port, std::string t_their_host, std::string t_their_port);
  void run_main();
  void fake_main();

};
#endif
