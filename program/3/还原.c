#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
typedef struct A
{
	char name[10];
	char mode[10];
	char uid[10];
	char gid[10];
	char time1[100];
	char time2[100];
	char time3[100];
	char number[10];
	char mean[100];
	char  type[10];
      	struct A *next;
}A;
void inlink(A *L,char str[])
{
	//printf("1");
	char c;
	c=str[1];
	A *q;
	int d=0;
	q=(A *)malloc(sizeof(A));
	memset(q->name,0,10);
	memset(q->mode,0,10);
	memset(q->uid,0,10);
  	memset(q->gid,0,10);
  	memset(q->time1,0,100);
  	memset(q->time2,0,100);
  	memset(q->time3,0,100);
  	memset(q->number,0,10);
  	memset(q->type,0,10);
  	memset(q->mean,0,100);
	 for(int g=0;g<20;g++)
	 {
	  c=str[g];
 	  if(c==' ')
	  {
	   d=g+1;
	   break;
	  }
	  q->name[g]=c;
	 }
	 for(int g=0;g<20;g++)
         {
          c=str[d];
	  d++;
          if(c==' ')
	  {
           break;
	  }
          q->mode[g]=c;
         }
	 for(int g=0;g<20;g++)
         {
          c=str[d];
	  d++;
          if(c==' ')
	  {
	   break;
	  }
          q->uid[g]=c;
         }
	 for(int g=0;g<20;g++)
         {
          c=str[d];
	  d++;
          if(c==' ')
           break;
          q->gid[g]=c;
         }
	 for(int g=0;g<100;g++)
         {
          c=str[d];
	  d++;
          if(c=='.')
           break;
          q->time1[g]=c;
         }
	 for(int g=0;g<100;g++)
         {
          c=str[d];
	  d++;
          if(c=='.')
           break;
          q->time2[g]=c;
         }
	 for(int g=0;g<100;g++)
         {
          c=str[d];
	  d++;
          if(c=='.')
           break;
          q->time3[g]=c;
         }
	 d=d+1;
	 for(int g=0;g<20;g++)
         {
          c=str[d];
	  d++;
          if(c==' ')
           break;
          q->number[g]=c;
         }
	 for(int g=0;g<100;g++)
	 {
	  c=str[d];
	  d++;
	  if(c==' ')
	    break;
	  q->mean[g]=c;
	 }
	 for(int g=0;g<20;g++)
         {
          c=str[d];
	  d++;
          if(c=='\n')
           break;
	  if(c!=' ')
	  {
          q->type[g]=c;
	  }
         }
//	printf("%s %s",q->name,q->time1);
	A *p;
	p=L;
	while(1)
	{
	if(p->next==NULL)
	{
	p->next=q;
	q->next=NULL;
	break;
	}
	p=p->next;
	}
}
int main()
{
	   int u,uid,gid;
	char str[1000];
	char a[100],b[100];
	memset(str,'\0',1000);
	A *L,*p;
	L=(A *)malloc(sizeof(A));
	L->next=NULL;
	p=(A *)malloc(sizeof(A));
	FILE *fd;
	char c;
	fd=fopen("cvf.txt","r");
	//printf("12");
	while(1)
	{
	fgets(str,1000,fd);
	//printf("%c",str[0]);
	if(str[1]=='\0')
	 break;
	inlink(L,str);
	memset(str,'\0',1000);
	}	
	p=L->next;
	while(1)
	{ 
	if(p==NULL)
	{
		//printf("NULL");
		break;
	}
	//printf("123");
	//printf("%s\n",p->mean);
	p=p->next;
	}
	int fp;
	p=L->next;
	u=atoi(p->mode);
	  if(u==444)
                 mkdir(p->name,0444);
                 else if(u==600)
                          mkdir(p->name,0600);
                 else if(u==644)
                          mkdir(p->name,0644);
                 else if(u==666)
                          mkdir(p->name,0666);
                 else if(u==700)
                          mkdir(p->name,0700);
                 else if(u==744)
                          mkdir(p->name,0744);
                 else if(u==755)
                          mkdir(p->name,0755);
                 else if(u==777)
                          mkdir(p->name,0777);
	uid=atoi(p->uid);
        gid=atoi(p->gid);
	fp=mkdir(p->name,0777);
	chown(p->name,uid,gid);
	chdir(p->name);
	int n1,g,n2,n3;
	g=atoi(p->type);
	n1=atoi(p->number);
	if(n1>0)
	{
	getcwd(a,100);
	for(int i=0;i<n1;i++)
	{
		uid=atoi(p->uid);
		gid=atoi(p->gid);
		chdir(a);
		p=p->next;
		u=atoi(p->mode);
		//printf("%d\n",u);
		if(u==444)
		 mkdir(p->name,0444);
		 else if(u==600)
		 	  mkdir(p->name,0600);
		 else if(u==644)
			  mkdir(p->name,0644);
		 else if(u==666)
			  mkdir(p->name,0666);
		 else if(u==700)
			  mkdir(p->name,0700);
 	 	 else if(u==744)
			  mkdir(p->name,0744);
		 else if(u==755)
			  mkdir(p->name,0755);
		 else if(u==777)
			  mkdir(p->name,0777);
		chown(p->name,uid,gid);
		n2=atoi(p->number);
		if(n2>0)
		{
		chdir(p->name);
		getcwd(b,100);
		for(int i1=0;i1<n2;i1++)
		{
			chdir(b);
			p=p->next;
			u=atoi(p->mode);
			uid=atoi(p->uid);
		        gid=atoi(p->gid);
                printf("%d\n",u);
                if(u==444)
                 mkdir(p->name,0444);
                 else if(u==600)
                          mkdir(p->name,0600);
                 else if(u==644)
                          mkdir(p->name,0644);
                 else if(u==666)
                          mkdir(p->name,0666);
                 else if(u==700)
                          mkdir(p->name,0700);
                 else if(u==744)
                          mkdir(p->name,0744);
                 else if(u==755)
                          mkdir(p->name,0755);
                 else if(u==777)
                          mkdir(p->name,0777);
			//mkdir(p->name,0777);
			chown(p->name,uid,gid);
			n3=atoi(p->number);
			chdir(p->name);
			if(n3>0)
			{
			for(int i2=0;i2<n3;i2++)
			{
			p=p->next;
			       u=atoi(p->mode);
        		       // printf("%d\n",u);
                if(u==444)
                 fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0444);
                 else if(u==600)
                        fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0600);
                 else if(u==644)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0644);
                 else if(u==666)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0666);
                 else if(u==700)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0700);
                 else if(u==744)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0744);
                 else if(u==755)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0755);
                 else if(u==777)
                          fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0777);
			//fp=openat(AT_FDCWD,p->name,O_CREAT|O_RDWR,0777);
		        write(fp,p->mean,100);
			}
			}
		}
		}
	}
	}
	return 0;	
}
