struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);

// Run the current process on a container, mount it on a certain file node
// @param char* path: the directory that this process wants to run in
// must be created beforehand
int runvc(char*);

// Pause the current container
// @param int: the id of container
// if id = -1, means the command the caller from container itself, pause the cur proc
// else the caller is root proc
// return 0 if success, else -1
int vcpause(int);

// Resume the specified container
// @param int: the id of container, must be greater than 0, the caller must be root proc
// return 0 if success, else -1
int vcresume(int);

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char*, char*);
char* strcat(char*, const char*);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
uint strlen(char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
