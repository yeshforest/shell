#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main(int argc,char* argv[]){
  char str[1024];
  char cmd[1024];
  int len;//str-1길이
  int amper=0;//background? 1:Y 0:N
  char *command0,*command1,*command2;
  int fd;
  int fd2[2];
  int status;

while(1){ 
  //finish
	printf("[shell]$ ");
	fgets(str,sizeof(str),stdin);// user input
	str[strlen(str)-1]='\0';
	if(strcmp(str,"logout")==0||strcmp(str,"exit")==0) {break;exit(0);}

 	 //if str has &(background)? 
	len=strlen(str)-1;//length except last char(&)
	if(strncmp(str+len,"&",1)==0){//str 시작주소+len ==’&’
		amper=1;
        command1=strtok(str,"&");// first cmd
        command2=strtok(NULL,"&");// last cmd
	 
	}
	else{
	amper=0;
	strcpy(command1,str);
		}

	if(fork()==0){//if child
        //if < (redirection)????
	    if(strchr(str,'<')!=NULL){// if < 
		command1=strtok(str,"<");// first cmd 
		command2=strtok(NULL,"<");//filename
		fd = open(command2,O_RDONLY);
		close(READ);
		dup(fd);
		close(fd);
		
  	    }else if(strchr(str,'>')!=NULL){//if >
		command1=strtok(str,">");// first cmd
                command2=strtok(NULL,">");//filename 
		fd=creat(command2,0644);
		close(WRITE);//close output 
		dup(fd);
		close(fd);
            }
	//if |(pipe)??????
	  if(strchr(str,'|')!=NULL){// if pipe (first|last)
	command1=strtok(str,"|");// first cmd 
	command2=strtok(NULL,"|");// last cmd
 		 
 	 pipe(fd2);//make pipe

	if(fork()==0){//grandchild
		close(1);//close stdo 
		dup(fd2[WRITE]);//dup stdo 1
		close(fd2[READ]);//close 0
		close(fd2[WRITE]);//close 1
		execlp(command1,command1,NULL);//exec first
		perror("pipe");
		
 	 }else{//if child 
		close(0);//close stdin
		dup(fd2[READ]);
		close(fd2[READ]);
		close(fd2[WRITE]);
		execlp(command2,command2,NULL);
		perror("pipe");
  	     }
	exit(0);
	}//endpipe


	execlp(command1,command1,NULL);//exec command0 
	exit(0);
	}//child end
	else{//if parent
	/*background process or not?*/
	if(amper==0){//fg
		wait(&status);
		}
	else{//bg

		}
	}

 }//while 
}
