#include "router.h"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "Incorrect program arguments. Example: ./router 1" << endl; 
    exit(1);
  }
  cout << "Router: argv:" << argv[1] << endl;
  int router_number = atoi(argv[1]);
  Router router(router_number);
  router.routerProcess();
  //cout << "This is the end!!" << endl;
  return 0;
}

void Router::initNetworkMap(int number_nodes) {
  for(int idx = 0; idx < number_nodes; idx++) {
    vector<LSP> nodes; 
    network_map.insert(pair<int, vector<LSP>>(idx, nodes));
  }
}

struct sockaddr_in Router::getRouterSockAddr(int router_number) {
    struct sockaddr_in tempAddr; 
    tempAddr.sin_family = AF_INET;
    tempAddr.sin_port = htons(MANAGER_PORT + router_number);
    inet_pton(AF_INET, "127.0.0.1", &tempAddr.sin_addr.s_addr);
    return tempAddr; 
}

void Router::routerProcess() {
  router_socket = createRouterSocket();
  cout << "Router: Created a UDP socket on " << MANAGER_PORT+router_number << " with the sock_filedescriptor " << router_socket << endl;
  int manager_socket = createManagerConnection();
  cout << "Router: router number: " << router_number << endl;
    
  int status = send(manager_socket, &router_number, sizeof(router_number), 0);
  if(status == -1) {
    cout << "Failed to send the manager my router number :(" << endl;
    exit(1);
  }
    
    
  // Recive the size of many many pairs it has in the table
  status = recv(manager_socket, &router_info, sizeof(router_info), 0);
  if(status == -1) {
    cout << "Failed to recv the router info :(" << endl;
    exit(1);
  }
  if(DEBUG) {
            cout << "Router: Debug: number_incoming_connections:" << router_info.number_incoming_connections << endl;
            cout << "Router: Debug: number_nodes:" << router_info.number_nodes << endl;
            cout << "Router: Debug: number_edges:" << router_info.number_edges << endl;
    }
    
  initNetworkMap(router_info.number_nodes);
  int size = 0;
  status = recv(manager_socket, &size, sizeof(size), 0);

  if(status == -1) {
        cout << "Failed to recv the size :(" << endl;
        exit(1);
  }
  vector<LSP> table = network_map.find(router_number)->second;
  table.resize(size);
  if(size != 0) {
    status = recv(manager_socket, &table[0], sizeof(LSP)*size, 0);
    if(status == -1) {
        cout << "Failed to recv the LSP vector :(" << endl;
        exit(1);
    }
  }

    status = send(manager_socket, "Ready!", sizeof("Ready!"), 0); 
    if(status == -1) {
        cout << "Failed to send the ready message :(" << endl;
        exit(1);
    }
    
    char startCommand[sizeof("Go!")];
    status = recv(manager_socket, &startCommand, sizeof(startCommand), 0);
    if(status == -1) {
        cout << "Failed to recv the start UDP connetion process :(" << endl;
        perror("Error: ");
        exit(1);
    }
    cout << startCommand << endl;
    cout << "Router: Starting to setup UDP connections" << endl;
    
    /**
     * 
     * ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
     * 
     */
    
    for(int idx = 0; idx < static_cast<int>( table.size() ); idx++) {
        struct sockaddr_in sendToAddr =getRouterSockAddr(table.at(idx).destination);
        int send_val = 1;
        sendto(router_socket, &send_val, sizeof(int), 0, (struct sockaddr*)&sendToAddr, sizeof(sendToAddr));
    }
    
    for(int idx = 0; idx < router_info.number_incoming_connections; idx++) {
        // byte_count = recvfrom(sockfd, buf, sizeof buf, 0, &addr, &fromlen);
        int recv_val = 0;
        struct sockaddr recvFromAddr;
        socklen_t recvFromAddrSize;
        recvfrom(router_socket, &recv_val, sizeof(int), 0, &recvFromAddr, &recvFromAddrSize);
        if(DEBUG) cout << "Router["<< router_number << "]: recv_val: " << recv_val << endl; 
    }
    
    int setup_complete = DISTRIBUTION_PHASE;
    status = send(manager_socket, &setup_complete, sizeof(router_number), 0);
    
    
    /**
         char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &table[0], sizeof(table));
     */
  
}

int Router::createManagerConnection() {
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in client;
  int one = 1;
    
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
  client.sin_family = AF_INET;
  client.sin_port = htons(MANAGER_PORT);
  inet_pton(AF_INET, "127.0.0.1", &client.sin_addr.s_addr);
    
  int result = connect(socket_desc, (struct sockaddr*)(&client), sizeof(client));
  if(result == -1) {
    cout << "Couldn't connect to the manager!" << endl;
    exit(1);
  }
    
  return socket_desc;
}

int Router::createRouterSocket() {
  int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server;
  int one = 1;

  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);
    
  if( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) == -1 ){ // means it failed to bind... 0 on success, -1 if not
    cout << "Couldn't bind the port to the socket!" << endl;
    exit(1);
  }
    
  return socket_desc;
}
