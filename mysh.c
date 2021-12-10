#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <signal.h>

int arg=0;
char buf[1024];
char *com[100];
int pid;
char *f = "data1.txt";
void sigcat() {
kill(pid,SIGINT);
}

void read_command() {
char *tmp = strtok(buf, " ");
int i=0;
while(tmp) { 
com[i++]=tmp;
tmp=strtok(NULL, " ");
}
arg=i;
com[i]=0;
}

int flag[100][2];
char *file[100][2]={0};
char *argv[100][100];
int ar=0;

void analazy_command() {
ar=0;
for(int i=0;i<100;i++){
flag[i][0] = flag[i][1]=0;
file[i][0]=file[i][1]=0;
for(int j=0;j<100;j++) {
argv[i][j]=0;
}
}
for(int i=0;i<arg;i++) argv[0][i]=com[i];
argv[0][arg]=NULL;
int a=0;
for(int i=0;i<arg;i++)
   if(strcmp(com[i]," | ")==0){
   argv[ar][a++]=NULL;
   ar++;
   int a=0;
   }
   else  if(strcmp(com[i]," < ")==0){
   flag[ar][0]=1;
   flag[ar][0]=com[i+1];
   argv[ar][a++]=NULL;
   
   }
   else  if(strcmp(com[i]," > ")==0){
   flag[ar][0]=1;
   flag[ar][0]=com[i+1];
   argv[ar][a++]=NULL;  
   }
   else {
   argv[ar][a++]=com[i];
   }
}

int do_command(){
pid=fork();
if(pid<0){
perror("fork error\n");
exit(0);
}

else if(pid==0){
if(!ar){
      if(flag[0][0]){
      close(0);
      int fd=open(file[0][0],O_RDONLY);
}

if(flag[0][1]){
      close(1);
      int fd2=open(file[0][1],O_WRONLY|O_TRUNC,0666);
      }
      execvp(argv[0][0],argv[0]);
      }
else 
     {
     int m;
     for(int m=0;m<ar;m++){
     int pid2=fork();
     if(pid2<0){
     perror("fork error\n");
     exit(0);
     }
     else if(pid2==0);{
     if(m){
     close(0);
     int fd=open(f,O_RDONLY);
     }
     if(flag[m][0]){
     close(0);
     int fd=open(file[m][0],O_RDONLY);
     }
     if(flag[m][1]){
     close(1);
     int fd=open(file[m][1],O_WRONLY|O_CREAT|O_TRUNC,0666);
     }
     close(1);
     remove(f);
     int fd=open(f,O_WRONLY|O_CREAT|O_TRUNC,0666);
     if(execvp(argv[m][0],argv[m])==-1){
     perror("execvp is error!\n");
     exit(0);
     }
}
  
}
     close(0);
     int fd=open(f,O_RDONLY);
     if(flag[m][1]){
     close(1);
     int fd=open(file[m][1],O_WRONLY|O_CREAT|O_TRUNC,0666);
     }
     execvp(argv[m][0],argv[m]);
     }
}
    else{
    waitpid(pid,NULL,0);
    }
    return 1;
}
int main(int argc,char *argv[]){
signal(SIGINT, &sigcat);
while(buf){
for(int i=0;i<100;i++)
com[i]=0;
arg=0;
read_command();
analazy_command();
do_command();
}