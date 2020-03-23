#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>  
#include <dirent.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <sys/types.h>
#include<fcntl.h>
#include<time.h>
typedef struct A
{
	char name[10];
	char mode[10];
	char uid[10];
	char gid[10];
	char time1[100];//creat time
	char time2[100];//inter time
	char time3[100];//modify time
	char mean[200];
	char number[10];
	char type[10];
	struct A *next;
}A;
void inlink(A *L,char str[],char name[],int type)
{
  A *p;
  int fd;
  struct stat buf;
  stat(str,&buf);
  p=(A*)malloc(sizeof(A));
  //strcpy(p->name,name);
  memset(p->name,0,10);
  memset(p->mode,0,10);
  memset(p->uid,0,10);
  memset(p->gid,0,10);
  memset(p->time1,0,100);
  memset(p->time2,0,100);
  memset(p->time3,0,100);
  memset(p->number,0,10);
  memset(p->type,0,10);
  memset(p->mean,0,200);
  sprintf(p->name,"%s ",name);
  DIR *pDir;
  struct dirent *ent;
 // pDir=opendir(str);
  int i=0;
 // while((ent=readdir(pDir))!=NULL)
 // {
  //        i=i+1;
 // }
 // i=i-2;
 // sprintf(p->number,"%d",i);
  //i=buf.st_mode;
  char o[10];
   //sprintf(p->mode,"%o ",buf.st_mode);
  sprintf(o,"%o ",buf.st_mode);
  for(int m=0;m<10;m++)
  {
   if(o[m]==' ')
    {
	    p->mode[0]=o[m-4];
	    p->mode[1]=o[m-3];
	    p->mode[2]=o[m-2];
	    p->mode[3]=o[m-1];
	    p->mode[4]=o[m];
	    break;
    }
  } 
 /* for(int m=0;m<7;m++)
  {
	  if(m<4)
	  p->mode[m]=p->mode[m+3];
	  else
	  p->mode[m]='\0';
  }*/
  sprintf(p->uid,"%d ",buf.st_uid);
  sprintf(p->gid,"%d ",buf.st_gid);
  sprintf(p->type,"%d\n",type);
  sprintf(p->time1,"%s ",ctime(&buf.st_ctime));
  int v=0;
  while(1)
  {
	if(p->time1[v]=='\n')
	{
		p->time1[v]='.';
		break;
	}
	v++;
  }
  sprintf(p->time2,"%s ",ctime(&buf.st_atime));
  v=0;  
  while(1)
  {
        if(p->time1[v]=='\n')
        {
                p->time1[v]='.';
                break;
        }
        v++;
  }
  sprintf(p->time3,"%s ",ctime(&buf.st_mtime));
  v=0;
    while(1)
  {
        if(p->time1[v]=='\n')
        {
                p->time1[v]='.';
                break;
        }
        v++;
  }
	i=0;
  if(type!=4)//dir file
  {
	 FILE *fp;
	 char tmp[200];
	 memset(tmp,0,200);
	 fp= fopen(str,"r");
	 fgets(tmp,200,fp);
	 sprintf(p->mean,"%s ",tmp); 
	 sprintf(p->number,"%d ",i);
  }
  else
  {
  	 pDir=opendir(str);
          while((ent=readdir(pDir))!=NULL)
          {
                  i=i+1;
          }
          i=i-2;
          sprintf(p->number,"%d ",i);
	  int h=0;
	  sprintf(p->mean,"%d ",h);
	   closedir(pDir);  
  }
  A *q;
  q=L;
  while(1)
  {
   if(q->next==NULL)
   {
        q->next=p;
        p->next=NULL;
	break;
   }
   q=q->next;
  }
}
void listDir(char *path,A *L)  
{  
        DIR              *pDir ,*qDir;  
        struct dirent    *ent  ;  
        int               i=0  ;  
        char              childpath[512];  
	A *p;
	p=(A*)malloc(sizeof(A));
        pDir=opendir(path);  
        memset(childpath,0,sizeof(childpath));   
        while((ent=readdir(pDir))!=NULL)  
        {  
                if(ent->d_type & DT_DIR)  
                {  

                        if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)  
                                continue;  

			//printf("%d",ent->d_type);
                      sprintf(childpath,"%s/%s",path,ent->d_name);
//		      printf("\n%d\n",ent->d_type);
		        inlink(L,childpath,ent->d_name,ent->d_type);  
 //                       printf("path:%s\n",ent->d_name);  
                       listDir(childpath,L);  
                }  
else
{
    sprintf(childpath,"%s/%s",path,ent->d_name);
inlink(L,childpath,ent->d_name,ent->d_type);
//printf("%s\n",ent->d_name);
}
	} 	
} 

void creattxt(A *L)
{
	FILE *fp;
	int i=0;
	char str[1000];
	memset(str,0,1000);
	A *p;
	p=L->next;
	int g=0;
	fp=fopen("cvf.txt","r+");
	while(p!=NULL)
	{
	 memset(str,0,1000);
	 strcat(str,p->name);
	 strcat(str,p->mode);
	 strcat(str,p->uid);
	 strcat(str,p->gid);
 	 strcat(str,p->time1);
	 strcat(str,p->time2);
 	 strcat(str,p->time3);
	 strcat(str,p->number);
	 strcat(str,p->mean);
 	 strcat(str,p->type);
	 //printf("%s\n",str);
	 fputs(str,fp);
	 p=p->next;
	}
	fclose(fp);
	//fp=open("cvf.txt",O_CREAT|O_RDWR,0777);
}
int main(int argc,char *argv[])  
{
	open("cvf.txt",O_CREAT|O_RDWR,0755);
	A *L,*p;
	L=(A*)malloc(sizeof(A));      
	L->next=NULL;
	int v=4;
	inlink(L,argv[1],argv[1],v);
       listDir(argv[1],L); 
	creattxt(L);
        return 0;  
}
