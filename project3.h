#ifndef PROJECT3_H
#define PROJECT3_H
#define MANAGER_PORT 28000
#define DEBUG true
#define SETUP_PHASE 1
#define LINK_PHASE 2
#define DISTRIBUTION_PHASE 3
#define FINAL_PHASE 4

#include <map>
using std::map;
using std::pair;
typedef struct _LSP {
   int source;
   int destination; 
   int cost;
} LSP; 

typedef struct _ROUTER_INFO {
 int number_incoming_connections;
 int number_nodes;
 int number_edges; 
} ROUTER_INFO;
#endif
