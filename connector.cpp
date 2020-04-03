#include <stdexcept>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "connector.hpp"

void Connector::setup_my_socket()
{
  int sockfd;
  struct addrinfo hints, *myinfo, *p;
  memset(&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  int rv;
  rv = getaddrinfo(NULL, this->my_port.c_str(), &hints, &myinfo);
  if ( rv != 0) {
    throw std::runtime_error( std::string("getadrinfo() ~my_info~ failed:") + gai_strerror(rv) );
  }


  for(p = myinfo; p != NULL; p = p->ai_next)
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      std::cerr<<"server: socket()";
      continue;
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      std::cerr<<"setsockopt()\n";
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      std::cerr<<"server: bind()";
      continue;
    }
    break;
  }

  freeaddrinfo(myinfo); // all done with this structure

  if (p == NULL)  {
    throw std::runtime_error(std::string("Error, my socket is NULL after setsockopt and bind"));
  }
  this->the_socket = sockfd;
}

void Connector::connect_to_target()
{
  struct addrinfo hints, *theirinfo, *p;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int rv;
  rv = getaddrinfo(this->their_host.c_str(), this->their_port.c_str(), &hints, &theirinfo);
  if (rv != 0) {
    throw std::runtime_error( std::string("getadrinfo() ~their_info~ failed:") + gai_strerror(rv) );
  }

  for (p = theirinfo; p != NULL; p=p->ai_next)
  {
    if (connect(this->the_socket, p->ai_addr, p->ai_addrlen) == -1) {
      close(this->the_socket);
      throw std::runtime_error("connect() failed");
    }
    break;
  }
}

void Connector::do_communication()
{
  this->send(std::string("hi"));
}

void Connector::safe_exit()
{
  ::close(this->the_socket);
  this->the_socket = -1;
}

void Connector::send(std::string msg)
{
   if (!::send(this->the_socket, msg.c_str(), msg.length(), 0))
   {
     throw std::runtime_error(std::string("Error send() -> message :") + msg);
   }
}

void Connector::receive()
{

}



// PUBLIC

Connector::Connector(int t_thread_id, std::string t_my_port, std::string t_their_host, std::string t_their_port)
: the_socket(-1), thread_id(t_thread_id), my_port(t_my_port), their_host(t_their_host), their_port(t_their_port)
{

}

void Connector::run_main()
{
  try
  {
    this->setup_my_socket();
    this->connect_to_target();
    this->do_communication();
    this->safe_exit();
  }
  catch (const std::exception &e)
  {
    // catch anything thrown within try block that derives from std::exception
    std::cerr << e.what()<<std::endl;
    this->safe_exit();
  }
  catch (...)
  {
    std::cerr << "caught some unknown stuff\n";
    this->safe_exit();
  }
}

void Connector::fake_main()
{
    std::cout<<"Thread <"<<thread_id<<">\n";
}
