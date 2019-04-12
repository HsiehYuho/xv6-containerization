#define MAX_VCS 3 // max number of containers

// // Container name has 3 names
// extern char* vcnames[MAX_VCS];

enum contstate { EMPTY, RUN, HALT}; 

// One process can run on a container
struct container{ 
  int id; 
  enum contstate state; 
  struct inode* root; // on which inode where container mounts on
};
