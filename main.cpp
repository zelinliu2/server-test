#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "connector.hpp"







int main (int argc, char** argv)
{
  std::vector<std::string> options (argv+1, argv+argc);
  if(options.size() != 4)
  {
    return -1;
  }

  for (auto x : options)
  {
    std::cout<<x<<std::endl;
  }

  int thread_count = std::stoi(options[0]);
  std::string my_port = options[1];
  std::string their_host = options[2];
  std::string their_port = options[3];

  std::vector<std::shared_ptr<Connector>> connectors;

  for (int i = 0; i < thread_count; ++i)
  {
    std::string thread_port = std::to_string(std::stoi(my_port) + i);
    std::shared_ptr<Connector> tmp = std::make_shared<Connector>(i, thread_port, their_host, their_port);
    connectors.emplace_back(std::move(tmp));
  }

  std::vector<std::thread> threads;
  for (int j = 0; j < thread_count; ++j)
  {
    std::thread tmp (&Connector::run_main, connectors[j]);
    threads.emplace_back(std::move(tmp));
  }

  for (int k = 0; k< thread_count; ++k)
  {
    threads[k].join();
  }

  std::cout<<"All threads joined! nice!\n";
  return 0;
}
