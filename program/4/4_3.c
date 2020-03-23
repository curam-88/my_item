
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>

typedef void (* signal_handler)(int);
void signal_handler_fun(int signl_num)
{
char *c;
char name[10];
char name1[10];
char v;
int number;
c="#include<stdio.h>%c#include<stdlib.h>%c#include<unistd.h>%c#include<time.h>%c#include <string.h>%c#include <dirent.h>%c#include <sys/stat.h>%c#include <unistd.h>%c#include <sys/types.h>%c#include<sys/wait.h>%c#include<fcntl.h>%c#include<signal.h>%ctypedef void (* signal_handler)(int);void signal_handler_fun(int signl_num)%c{char *c;char name[10];char name1[10];char v;int number;c=%c%s%c;loop:number=rand()%7;number=number+1;memset(name,0,sizeof(name));memset(name1,0,sizeof(name));for(int i=0;i<number+1;i++){if(i<number){v='a'+rand()%26;name[i]=v;}else{strcpy(name1,name);name[i]='.';name[i+1]='c';	}}if(freopen(name,%cw%c,stdout)){    printf(c,10,10,10,10,10,10,10,10,10,10,10,10,10,34,c,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34);fclose(stdout);freopen(%c/dev/tty%c,%cw%c,stdout);}else{goto loop;}char a[30]=%cgcc -o %c;char b[20]=%c./%c;char g[30];int d;	FILE *fd;strcat(a,name1);strcat(a,%c %c);strcat(a,name);strcat(b,name1);if((fd=fopen(%c3.txt%c,%cr%c))==NULL)	{printf(%cerror in open 3.txt%c);}else{fgets(g,30,fd);d=atoi(g);fclose(fd);}if(d!=0){char g1[5];	fd=fopen(%c3.txt%c,%cw%c);d=d-1;memset(g1,'0',5);printf(%c%%d %c,d);sprintf(g1,%c%%d%c,d);	fputs(g1,fd);fclose(fd);if(fork()==0){system(a);execl(b,name1,NULL);}else{wait(NULL);}}else{exit(0);}exit(0);}int main(){srand((unsigned)time(NULL));signal_handler p_signal=signal_handler_fun;	sigset_t set;sigemptyset(&set);sigprocmask(SIG_SETMASK, &set, NULL);signal(SIGINT,p_signal);pause();return 0;}";
//c="#include<stdio.h>%c#include<stdlib.h>%c#include<unistd.h>%c#include<time.h>%c#include <string.h>%c#include <dirent.h>%c#include <sys/stat.h>%c#include <unistd.h>%c#include <sys/types.h>%c#include<sys/wait.h>%c#include<fcntl.h>%cint main()%c{srand((unsigned)time(NULL));char *c;char name[10];char name1[10];char v;int number;c=%c%s%c;number=rand()%7;number=number+1;loop:for(int i=0;i<number+1;i++){if(i<number){v='a'+rand()%26;name[i]=v;}else{strcpy(name1,name);name[i]='.';name[i+1]='c';	}}if(freopen(name,%cw%c,stdout)){    printf(c,10,10,10,10,10,10,10,10,10,10,10,10,34,c,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34);fclose(stdout);freopen(%c/dev/tty%c,%cw%c,stdout);}else{goto loop;}char a[30]=%cgcc -o %c;char b[20]=%c./%c;char g[30];int d;	FILE *fd;strcat(a,name1);strcat(a,%c %c);strcat(a,name);strcat(b,name1);if((fd=fopen(%c3.txt%c,%cr%c))==NULL)	{printf(%cerror in open 3.txt%c);}else{fgets(g,30,fd);d=atoi(g);fclose(fd);}if(d!=0){char g1[5];fd=fopen(%c3.txt%c,%cw%c);d=d-1;memset(g1,'0',5);printf(%c%%d %c,d);sprintf(g1,%c%%d%c,d);fputs(g1,fd);fclose(fd);if(d=fork()==0){system(a);printf(%cpid=%%d%c,d);execl(b,name1,NULL);}else{wait(NULL);}}else{return 0;}return 0;}";
loop:number=rand()%7;
	number=number+1;
	memset(name,0,sizeof(name));
	memset(name1,0,sizeof(name));
for(int i=0;i<number+1;i++)
{
	if(i<number){
		v='a'+rand()%26;
	name[i]=v;
}
else
{
	strcpy(name1,name);
	name[i]='.';
	name[i+1]='c';
}
}
if(freopen(name,"w",stdout)){    
		printf(c,10,10,10,10,10,10,10,10,10,10,10,10,10,34,c,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34);
		//printf(c,10,10,10,10,10,10,10,10,10,10,10,10,34,c,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34);
			fclose(stdout);
			freopen("/dev/tty","w",stdout);
		}
		else
		{
		goto loop;
		}
		char a[30]="gcc -o ";
		char b[20]="./";
		char g[30];
		int d;
			FILE *fd;strcat(a,name1);
			strcat(a," ");
			strcat(a,name);strcat(b,name1);
			if((fd=fopen("3.txt","r"))==NULL)	
		{
				printf("error in open 3.txt");
		}
		else{fgets(g,30,fd);
			d=atoi(g);
			fclose(fd);
		}
		if(d!=0)
			{
				char g1[5];	
				fd=fopen("3.txt","w");
				d=d-1;memset(g1,'0',5);
				printf("%d ",d);
				sprintf(g1,"%d",d);
					fputs(g1,fd);
					fclose(fd);
					if(fork()==0)
						{
							//printf("\npid=%d\n",getpid());
							system(a);
							execl(b,name1,NULL);
						}
					else{wait(NULL);
					}
				}
				else
					{
						exit(0);
					}
					exit(0);
}
int main()
{
	srand((unsigned)time(NULL));
	sigset_t set;
	sigemptyset(&set);

	sigprocmask(SIG_SETMASK, &set, NULL);
	//signal_handler p_signal=signal_handler_fun;
	signal(SIGINT,signal_handler_fun);
	//for(; ;);
	pause();
		return 0;
}
