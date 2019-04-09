// The entry point of containerization

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *shargv[] = { "sh", 0 };

void attachvc(){
	int fd, id;
	fd = open("vc0", O_RDWR);
	id = fork();

	if (id == 0){
		close(0);
		close(1);
		close(2);
		dup(fd);
		dup(fd);
		dup(fd);
		exec("sh",shargv);
		printf(1, "Failure to attach VC.");
		exit();
	}

}

void start(char *s_args[]){
	attachvc();
	exit();
}


int
main(int argc, char *argv[]){
	if(strcmp(argv[1], "start") == 0){
		start(&argv[2]);
	}
}