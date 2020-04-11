#include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <time.h>
#include <signal.h>

//统计的数值
volatile int timerexpored = 0;//闹钟信号判断，时间到了设为1
int speed = 0;//速度
int failed = 0;//失败次数
int bytes = 0;//字节数

//需求的全局变量
int http10=1;
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
//#define PROGRAM_VERSION "1.5"

//默认设置
int method=METHOD_GET;
int clients=1;//客户端数量
int force=0;//等待响应
int force_reload=0;//失败重新请求
int proxyport=80;
char *proxyhost=NULL;//代理的服务器
int benchtime=30;//模拟时间
int mypipe[2];//建立管道
char host[MAXHOSTNAMELEN];//网络地址
#define REQUEST_SIZE 2048
char request[REQUEST_SIZE];//请求
static const struct option long_options[]=
{
	{"force",no_argument,&force,1},
	{"reload",no_argument,&force_reload,1},
	{"time",required_argument,NULL,'t'},
	{"help",no_argument,NULL,'?'},
	{"http09",no_argument,NULL,'9'},
	{"http10",no_argument,NULL,'1'},
	{"http11",no_argument,NULL,'2'},
	{"get",no_argument,&method,METHOD_GET},
	{"head",no_argument,&method,METHOD_HEAD},
	{"options",no_argument,&method,METHOD_OPTIONS},
	{"trace",no_argument,&method,METHOD_TRACE},
	{"version",no_argument,NULL,'V'},
	{"proxy",required_argument,NULL,'p'},
	{"clients",required_argument,NULL,'c'},
	{NULL,0,NULL,0}
};
//根据闹钟信号统计一定时间内客户端创建socket发送的数量
static void benchcore(const char* host,const int port,const char *request);
//实现fork多个客户端
static int bench(void);
//根据域名确定http的request
static void bulid_request(const char *url);
static void alarm_handler(int signal)
{
	timerexpored = 1;
}
static void usage(void)
{
	fprintf(stderr,
	"webbench [option]... URL\n"
	"  -f|--force               Don't wait for reply from server.\n"
	"  -r|--reload              Send reload request - Pragma: no-cache.\n"
	"  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
	"  -p|--proxy <server:port> Use proxy server for request.\n"
	"  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
	"  -9|--http09              Use HTTP/0.9 style requests.\n"
	"  -1|--http10              Use HTTP/1.0 protocol.\n"
	"  -2|--http11              Use HTTP/1.1 protocol.\n"
	"  --get                    Use GET request method.\n"
	"  --head                   Use HEAD request method.\n"
	"  --options                Use OPTIONS request method.\n"
	"  --trace                  Use TRACE request method.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
	);
};
int main(int argc, char *argv[])
{
	int opt=0;
	int options_index=0;
	char *tmp=NULL;
	//不带参数
	if(argc==1)
	{
		usage();
		return 2;
	}
	while((opt=getopt_long(argc,argv,"912Vfrt:p:c:?h",long_options,&options_index))!=EOF)
	{
		switch(opt)
		{
		case  0 : break;//退出
		case 'f': force=1;break;
		case 'r': force_reload=1;break;
		case '9': http10=0;break;
		case '1': http10=1;break;
		case '2': http10=2;break;
		case 'V': 
		//printf(PROGRAM_VERSION"\n");
		exit(0);
		case 't': benchtime=atoi(optarg);break;//设置发送的时间长度
		case 'p': //服务器端口
				tmp = strchr(optarg,':');
				proxyhost=optarg;
				if(tmp==NULL)
				{
					break;
				}
				if(tmp==optarg)
				{
					fprintf(stderr,"Error in option --proxy %s: Missing hostname.\n",optarg);
		     		return 2;
				}
				if(tmp==optarg+strlen(optarg)-1)
				{
					fprintf(stderr,"Error in option --proxy %s Port number is missing.\n",optarg);
		     		return 2;
				}
				*tmp='\0';
				proxyport=atoi(tmp+1);break;
		case ':':
		case 'h':
		case '?':usage();return 2;break;
		case 'c': clients=atoi(optarg);break;//客户端的数目
		}
	}
	//optind被getopt_long 设置为命令行参数中未被识别的字符串
	if(optind==argc){
		fprintf(stderr,"webbench: Missing URL!\n");
		      usage();
		      return 2;
	}
	if(clients==0) clients = 1;
	if(benchtime==0) benchtime = 60;
	bulid_request(argv[optind]);
	printf("\nBenchmarking: ");
	switch(method)
	{
		case METHOD_GET:
		Default:
			printf("GET");break;
		case METHOD_OPTIONS:
			printf("OPTIONS");break;
		case METHOD_TRACE:
			printf("TRACE");break;
	}
	printf(" %s",argv[optind]);
	switch(http10)
	{
		case 0: printf(" (using HTTP/0.9)");break;
		case 2: printf(" (using HTTP/1.1)");break;
	}
	printf("\n");
	if(clients==1) printf("1 client");
	else
		printf("%d clients",clients);

	printf(", runing %d sec", benchtime);
	if(force) printf(", early socket close");
	if(proxyhost!=NULL) printf(",via proxy server %s:%d",proxyhost,proxyport);
	if(force_reload) printf(",forcing reload");
	printf(".\n");
	return bench();
}
void bulid_request(const char *url){
	char tmp[10];
	int i;
	bzero(host,MAXHOSTNAMELEN);
	bzero(request,REQUEST_SIZE);
	if(force_reload&&proxyhost!=NULL && http10<1) http10=1;
	if(method==METHOD_HEAD&&http10<1)http10=1;
	if(method==METHOD_OPTIONS && http10<2)http10=2;
	if(method==METHOD_TRACE && http10<2)http10=2;
	switch(method){
		Default:
		case METHOD_GET: strcpy(request,"GET");break;
		case METHOD_HEAD: strcpy(request,"HEAD");break;
		case METHOD_OPTIONS: strcpy(request,"OPTIONS");break;
		case METHOD_TRACE: strcpy(request,"TRACE");
	}
	strcat(request," ");
	if(NULL==strstr(url,"://"))//判断://后面是否有字符
	{
		fprintf(stderr, "\n%s: is not a valid URL.\n",url);
		exit(2);
	}
	if(strlen(url)>1500)//长度大于1500
	{
		fprintf(stderr,"URL is too long.\n");
		exit(2);
	}
	if(proxyhost==NULL)
		if(0!=strncasecmp("http://",url,7))//比较字符串前n个字符相同返回0
		{
			fprintf(stderr,"\nOnly HTTP protocol is directly supported, set --proxy for others.\n");
             exit(2);
		}
	i=strstr(url,"://")-url+3;
	printf("i的位置%d\n",i);
	if(strchr(url+i,'/')==NULL)
	{
       fprintf(stderr,"\nInvalid URL syntax - hostname don't ends with '/'.\n");
       exit(2);
	}
	if(proxyhost==NULL)
	{
		//获取端口--获取:的位置且:要在host最后的/后面
		if(index(url+i,':')!=NULL&&index(url+i,':')<index(url+i,'/'))
		{
			strncpy(host,url+i,strchr(url+i,':')-url-i);
			bzero(tmp,10);
			strncpy(tmp,index(url+i,':')+1,strchr(url+i,'/')-index(url+i,':')-1);
			printf("tmp is %s\n",tmp);
		}
		else
		{
			strncpy(host,url+i,strcspn(url+i,"/"));
		}
		strcat(request+strlen(request),url+i+strcspn(url+i,"/"));
	}
	else
	{
		strcat(request,url);
	}
	if(http10==1)
		strcat(request," HTTP/1.0");
	else if (http10==2)
		strcat(request,"HTTP/1.1");
	strcat(request,"\r\n");
	if(http10>0)
		strcat(request,"User-Agent:webbench \r\n");
	if(proxyhost==NULL&&http10>0)
	{
		strcat(request,"Host: ");
		strcat(request,host);
		strcat(request,"\r\n");
	}
	if(force_reload && proxyhost!=NULL)
	{
		strcat(request,"Pragma: no-cache\r\n");
	}
	if(http10>1)
		strcat(request,"Connection: close\r\n");
	//最后要添加空行
	if(http10>0)strcat(request,"\r\n");
	printf("Req=%s\n",request);
}
static int bench(void)
{
	int i,j,k;
	pid_t pid=0;
	FILE *f;
	//检查给的端口和host是否能够创建套接字
	if(i<0)
	{
		fprintf(stderr,"\nConnect to server failed.Aborting benchmark.\n");
			return 1;
	}
	close(i);
	if(pipe(mypipe))//建立管道
	{
		perror("pipe failed.");
		return 3;
	}
	for(int i=0;i<clients;i++)
	{
		pid=fork();
		if(pid <= (pid_t)0)
		{
			sleep(1);//确保所以的子进程创建完毕
			break;
		}
	}
	if(pid < (pid_t) 0 )
	{
	  fprintf(stderr,"problems forking worker no. %d\n",i);
	  perror("fork failed.");
	  return 3;
	}
	if(pid==(pid_t)0)
	{
		if(proxyhost==NULL)//创建套接字统计数目
			benchcore(host,proxyport,request);
		else
			benchcore(proxyhost,proxyport,request);

		//写到pipe
		f=fdopen(mypipe[1],"w");
		if(f==NULL)
		{
			perror("open pipe for writing failed.");
			return 3;
		}
		fprintf(f,"%d %d %d\n",speed,failed,bytes);
		fclose(f);
		return 0;
	}
	else//父进程
	{
		f=fdopen(mypipe[0],"r");
		if(f==NULL)
		{
			perror("open pipe for reading failed.");
			return 3;
		}
		setvbuf(f,NULL,_IONBF,0);
		speed=0;
		failed=0;
		bytes=0;
		while(1)
		{
			pid=fscanf(f,"%d %d %d",&i,&j,&k);
			if(pid<2)
			{
				fprintf(stderr,"Some of our childrens died.\n");
                       break;
			}
			speed+=i;
			failed+=j;
			bytes+=k;
			if(--clients==0) break;
		}
		fclose(f);
		printf("\nSpeed=%d pages/min, %d bytes/sec.\nRequests: %d susceed, %d failed.\n",
		  (int)((speed+failed)/(benchtime/60.0f)),
		  (int)(bytes/(float)benchtime),
		  speed,
		  failed);
  	}
  	return i;
	}
	void benchcore(const char *host,const int prot,const char *req)
	{
		int rlen;
		char buf[1500];
		int s,i;
		struct sigaction sa;
		sa.sa_handler=alarm_handler;
		sa.sa_flags=0;
		if(sigaction(SIGALRM,&sa,NULL))
			exit(3);
		alarm(benchtime);
		rlen=strlen(req);
		nexttry:while(1)
		{
			if(timerexpored){
				if(failed>0)
				{
					failed--;
				}
				return;
			}
		s=Socket(host,prot);
		if(s<0){
			failed++;
			continue;
		}
		if(rlen!=write(s,req,rlen)){failed++;close(s);continue;}
		if(force==0)
		{
			while(1)
			{
				if(timerexpored) break;
				i=read(s,buf,1500);
				if(i<0)
				{
					failed++;
					close(s);
					goto nexttry;
				}
				else
				{
					if(i==0)break;
					else
						bytes+=i;
				}
			}
		}
		if(close(s)){failed++;continue;}
		speed++;
	}
}



