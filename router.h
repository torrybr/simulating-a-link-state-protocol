#ifndef ROUTER_H
#define ROUTER_H

#include "project3.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using std::vector;

class Router {

  
 public:
 Router(const int&  _router_number): // the c++ way .. ;p
  router_number( _router_number )
  ,port( MANAGER_PORT + router_number )
  {}
  void initNetworkMap(int number_nodes);
  void routerProcess();
  struct sockaddr_in getRouterSockAddr(int router_number);
  
 protected: 
  int router_number;
  int router_socket;
  int port;
  int createRouterSocket();
  int createManagerConnection();
  void checkError(int status);
  map<int, vector<LSP>> network_map;
  ROUTER_INFO router_info;
  vector<int> router_connections;
  
};

#endif // ROUTER_H
