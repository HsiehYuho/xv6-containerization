// The entry point of containerization

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define CPY_CMDS_NUM 3

char *shargv[] = { "sh", 0 };
char *vcnames[] = {"vc0", "vc1", "vc2"};
int vcidx = 0;

// Before chdir the current process, we need to copy necessary files command for demo
// Assumptions: ls mkdir files are locates and after /
char *cpycmds[CPY_CMDS_NUM] = { "ls", "mkdir", "sh"};

// Call in init, so that we have basic cmds in the container
// @param char* srcf : source file
// @param char* destdir: destination directory
// @return: success 0, else -1
int copyfiles(char* srcf, char* destdir){
	// normalize destdir "x/y/z", "x/y/z/"  to "x/y/z/"
	int len = strlen(destdir);
	if(destdir[len-1] != '/'){
		destdir = strcat(destdir, "/");
		len += 1;
	}

	char destf[32];
	strcpy(&destf[0], destdir);
	strcpy(&destf[len], srcf);

	int tofd = open(destf, O_CREATE | O_RDWR);
	if(tofd < 0){
		printf(1, "Invalid file destination %s. \n", destf);
		return -1;
	}

	int fromfd = open(srcf, O_RDONLY);
	if(fromfd < 0){
		printf(1, "Invalid file source %s.\n", srcf);
		return -1;
	}

	int bytes;
	char buf[512];

	while((bytes = read(fromfd, buf, sizeof(buf))) > 0){
		if(bytes != write(tofd, buf, bytes)){
			printf(1, "Copy file %s to %s failed \n",srcf, destf);
			close(tofd);
			close(fromfd);
			return -1;
		}
	}
	close(tofd);
	close(fromfd);
	return 0;
}

void attachvc(){
	int fd, id;
	if(vcidx >= MAX_VCS){
		printf(1, "Run out of containers \n");
		exit();
	}
	char* vcname = vcnames[vcidx++];
	fd = open(vcname, O_RDWR);

	// Copy the necessary cmds to destination
	for(int i = 0; i < CPY_CMDS_NUM; i++){
		char* cmd = cpycmds[i];
		if(copyfiles(cmd, "./testdir") < 0){
			printf(1, "Fail to copy cmds to vc.");
			exit();
		}
	}

	// Need to copy necessary files [ls, mkdir] to the newly environment
	id = fork();

	if (id == 0){
		close(0);
		close(1);
		close(2);
		dup(fd);
		dup(fd);
		dup(fd);
		if(runvc("./testdir") < 0){
			printf(1, "Fail to run vc. Did you mkdir ? \n");
			exit();
		}
		exec("/sh",shargv);
		exit();
	}

}

// run current processor
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