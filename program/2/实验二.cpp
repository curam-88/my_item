#include<map>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;
struct delta{
	char a[5];
	char b[5];
	char c[5];
	char d[5];
	char e[5];
	struct delta *next;
};
void chushihua(char Q[],char sigma[],char q0[],char accept[],char reject[],delta *L)
{
	
	FILE* fp;
	char c[10];
	char ch;
	int b=1;
	int item;
	printf("后继->1\n奇偶->2(a为奇数,b为偶数)\n字符反转->3\n");
	scanf("%d",&item);
	if(item==1)
	{
	if((fp=fopen("1.txt","r"))==NULL)
	{
		printf("error!");
		return;
	}
	}
	else	if(item==2)
	{
	if((fp=fopen("2.txt","r"))==NULL)
	{
		printf("error!");
		return;
	}
	}
	else	if(item==3)
	{
	if((fp=fopen("3.txt","r"))==NULL)
	{
		printf("error!");
		return;
	}
	}
	else
	{
		printf("error!\n");
		return;
	}
	int line=1;
	while(1)
	{ 	
		while(1)
		{
			ch=fgetc(fp);
			if(ch==':')
			break;
		}
		
		for(int i=0;i<100;i++)
		{
		ch=fgetc(fp);
		Q[i]=ch;
		if(ch=='\n')
		break;
		}
		
			while(1)
		{
			ch=fgetc(fp);
			if(ch==':')
			break;
		}
		for(int i=0;i<100;i++)
		{
		ch=fgetc(fp);
		sigma[i]=ch;
		if(ch=='\n')
		break;
		}
			while(1)
		{
			ch=fgetc(fp);
			if(ch==':')
			break;
		}
		for(int i=0;i<100;i++)
		{
		ch=fgetc(fp);
		if(ch=='\n')
		break;
		q0[i]=ch;
		}
		for(int i=0;i<100;i++)
		{
		ch=fgetc(fp);
		accept[i]=ch;
		if(ch=='\n')
		break;
		}
		for(int i=0;i<100;i++)
		{
		ch=fgetc(fp);
		reject[i]=ch;
		if(ch=='\n')
		break;
		}
		delta *p,*q;
		q=L;
		p=(delta*)malloc(sizeof(delta));
		while(1)
		{
			ch=fgetc(fp);
			if(ch=='(')
			{
			memset(p->a, 0, 5);
			memset(p->b, 0, 5);
			memset(p->c, 0, 5);
			memset(p->d, 0, 5);
			memset(p->e, 0, 5);
			for(int i=0;i<5;i++)
			{
				ch=fgetc(fp);
				if(ch==',')
				break;
				p->a[i]=ch;	
			}
				for(int i=0;i<5;i++)
			{
				ch=fgetc(fp);
				if(ch==')')
				break;
				p->b[i]=ch;	
			} 
			 	while(1)
			 	{
			 		ch=fgetc(fp);
			 		if(ch=='(')
			 		break;
				 }
				for(int i=0;i<5;i++)
			{
				ch=fgetc(fp);
				if(ch==',')
				break;
				p->c[i]=ch;	
			} 
				for(int i=0;i<5;i++)
			{
				ch=fgetc(fp);
				if(ch==',')
				break;
				p->d[i]=ch;	
			} 
					for(int i=0;i<5;i++)
			{
				ch=fgetc(fp);
				if(ch==')')
				break;
				p->e[i]=ch;	
			}
			p->next=q->next;
			q->next=p;
			q=p; 
			p->next=NULL;
			}
			if(ch=='\n')
			{
			p=(delta*)malloc(sizeof(delta)); 
			}
			if(ch=='-')
			break;	
		}
	break;
}
return;	
}
int action(delta *L,char paper[],char q0[])
{
	char tai[10];
	strcpy(tai,q0);
	delta *q;
	q=L->next;
	int wz=10;
	int t=0;
	while(1)
	{
		if(!strcmp(tai,q->a)&&(paper[wz]==q->b[0]))
		{
		printf("(%s,",tai);
		printf("%c)",paper[wz]);
		printf("->(%s,%s,%s)\n",q->c,q->d,q->e);
			t=0;
			strcpy(tai,q->c);
			if(q->d[0]!='-')
			{
				paper[wz]=q->d[0];
			}
			if(q->e[0]=='R')
			{
				wz=wz+1;
				q=q->next; 
			}
			else if(q->e[0]=='L')
			{
				wz=wz-1;
				q=q->next; 
			}
			else if(q->e[0]=='-')
			{
				//if()
				return 1;
			}
		}
		else
		{	
		if(q->next==NULL)
		{
			q=L->next;
			t=t+1;
			if(t>2)
			{
			printf("dd");
			break;
			}
		}
		else
		{
			q=q->next; 
		}
		}
		
	}
}
//获取当前状态
//获取左右
//获取打印字符
//获取状态数值 
int main()
{
	while(1)
	{
	
	char Q[100];
	char sigma[100];
	char q0[10];
	char accept[100];
	char reject[100];
			memset(Q, 0, 100);
			memset(sigma, 0, 100);
			memset(q0, 0, 10);
			memset(accept, 0, 100);
			memset(reject, 0, 100);
	delta *L;
	L=(delta *)malloc(sizeof(delta));
	L->next=NULL;
	chushihua(Q,sigma,q0,accept,reject,L);
		char paper[100];
	char c;
	memset(paper,'B',100);
	getchar();
	printf("请输入纸带:\n");
	int i=10;
	while(1)
	{
	scanf("%c",&c);
	if(c=='\n')
		{
			break;
		}
	paper[i]=c;
	i++;
	}
	delta *p;
	p=(delta *)malloc(sizeof(delta)); 
	p=L->next;
	action(L,paper,q0);
	for(int i=0;i<100;i++)
	{
		if(paper[i]!='B')
		printf("%c",paper[i]);
	}
	printf("\n");
	}
}
