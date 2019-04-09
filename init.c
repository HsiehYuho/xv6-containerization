// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "container.h"

char *argv[] = { "sh", 0 };

// Create containers devices
// ["vc0","vc1","vc2"]
void create_vcs(void){
  char* vcname = "vc0";
  int fd;
  for(int i = 0; i < MAX_VCS; i++){
    vcname[2] = '0' + i;
    if ((fd = open(vcname, O_RDWR)) < 0){
      // 1 is ip->major, the same as console, i+2 (means from 2 to 4) is ip->minor
      // console is (1,1), others is (1,2), (1,3), (1,4)
      mknod(vcname, 1, i+2); 
    }
    else{
      close(fd);
    }
  }
}

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  // create containers devices
  create_vcs();

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
