#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
int main()
{
	char mean[100];
	int fp;
	char name[10];
	char a[30];
	char b[30];
	char c[30];
	char d[30];
	char v;
	umask(0);
	srand((unsigned) time(NULL));
	memset(mean,'\0',100);
	memset(name,'\0',10);
	memset(a,'\0',30);
        memset(b,'\0',30);
        memset(c,'\0',30);
	memset(d,'\0',30);
	fp=mkdir("/home/newuser/source",0777);
	getcwd(a,30);
	chdir("source");
	getcwd(b,30);
	for(int i=0;i<5;i++)
	{
		memset(name,'\0',10);
		chdir(b);
		loop: for(int t=0;t<5;t++)
		{
		v='a'+rand()%26;
		name[t]=v;	
		}
		printf("%s\n",name);
		if(mkdirat(AT_FDCWD,name,0777)<0)
		{
			i=i-1;
			goto loop;
		}
		chdir(name);
		getcwd(c,30);
		for(int j=0;j<5;j++)
		{
		chdir(c);
		memset(name,'\0',10);
		look:for(int t=0;t<5;t++)
                {
                v='a'+rand()%26;
                name[t]=v;
                }	
		//mkdirat(AT_FDCWD,name,0777);
		if(mkdirat(AT_FDCWD,name,0777)<0)
                {
                        j=j-1;
                        goto look;
                }
		chdir(name);
		getcwd(d,30);
			for(int p=0;p<20;p++)
			{
				chdir(d);
			 	 for(int t=0;t<8;t++)
                		{
               		 	v='A'+rand()%26;
                		name[t]=v;
               			}	 
			fp=openat(AT_FDCWD,name,O_CREAT|O_RDWR,0777);
			for(int g=0;g<100;g++)
			{
				v='a'+rand()%26;
				mean[g]=v;
				//write(AT_FDCWD,v,1);
			}
			//write(AT_FDCWD,mean,sizeof(mean));
			if(write(fp,mean,sizeof(mean))<0)
			{
				printf("error\n");
			}
			}
	       	}
	}
}
