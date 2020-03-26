#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <signal.h>
#include <unistd.h>
#include "packet.h"
#include<mysql/mysql.h>
#include<fcntl.h>
#include<sys/stat.h>

MYSQL *conn_ptr;
#define Radiotap_Header 18 //ieee802.11 Radiotap_Header
#define TEH_HEADER_SIZE 14
#define AVS_HEADER_SIZE 64
#define HEADER_SIZE 18
#define PRISM_HEADER_SIZE 144
#define DATA_80211_FRAME_SIZE 26
#define LLC_HEADER_SIZE 8
int Packet_Nu=30000;

int hello(struct Packet packet,MYSQL *conn_ptr);

struct ieee80211_radiotap_header
{
	u_int8_t it_version;
	u_int8_t it_pad;
	u_int16_t it_len;
	u_int32_t it_present;
}__attribute__((__packed__));
/*struct Packet
{
	int num;
	char src_ip[25];
	char dst_ip[25];
	char Get_Header[800];
	char User_Agent[800];
	char Host[800];
	char Full_Url[800];

};*/

struct sniff_ip
{
	u_char ip_vhl;
	u_char ip_tos;
	u_short ip_id;
	u_short ip_len;
	u_short ip_off;
	#define IP_RF 0x8000
	#define IP_DF 0x4000
	#define IP_MF 0x2000
	#define IP_OFFMASK 0x1fff
	u_char ip_ttl;
	u_char ip_p;
	u_short ip_sum;
	struct  in_addr ip_src;
	struct in_addr ip_dst;
	/*struct in_addr ip_src,ip_dst
	ip_dst;*/
};
#define IP_HL(ip) (((ip)->ip_vhl)&0x0f)
#define IP_V(ip) (((ip)->ip_vhl)>>4)
typedef u_int tcp_seq;
struct sniff_tcp
{
	u_short th_sport;
	u_short th_dport;
	tcp_seq th_seq;
	tcp_seq th_ack;
	u_char th_offx2;
	#define TH_OFF(th) (((th)->th_offx2&0xf0)>>4)
	u_char th_flags;
	#define TH_FIN 0x01
	#define TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define TH_CWR 0x80
	#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR | TH_PUSH)
	u_short th_win;
	u_short th_sum;
	u_short th_urp;  
};
char str[10000];
char *device;
int verbose=0;
pcap_t *handle;
int wired=0;

int create_deamon(void)
{
	pid_t pid=0;
	pid= fork();
	if(pid<0)
	{
		perror("fork");
		exit(-1);
	}
	else if(pid > 0)
	{
		exit(-1);
	}
	else if(pid == 0)
	{
		pid = setsid();//子进程脱离控制台创建新的会话期
	
	if(pid < 0)
	{
		perror("setsid");
	}
	pid=fork();
	if(pid < 0)
	{
		exit(-1);
	}
	else if(pid > 0)
	{
		exit(-1);
	}
	chdir("/");
	umask(0);//取消文件的权限屏蔽
	int i = sysconf(_SC_OPEN_MAX);//得到文件描述符个数
    for(;i>0;i--)
    {
    	close(i);
    }
    //定位文件描述符0，1，2
    open("/dev/null",O_RDWR);
    open("/dev/null",O_RDWR);
    open("/dev/null",O_RDWR);
}
}
void ctrl_c()
{
	printf("Exiting\n");
	pcap_breakloop(handle);
	pcap_close(handle);
	exit(0);
}
int find(char *str_child)
{
	FILE *fp;

	fp=fopen("data.txt","r+");
	char *line=NULL;
	size_t len;
	memset(str,'\0',sizeof(str));
	//strcat(str,getline(fp));
	while(getline(&line,&len,fp)!=-1)
	{
		strcat(str,line);
	}
	if(strstr(str,str_child)!=NULL)
	{
		 return 1;
	}
	else
	{
		 return 0;
	}
		
	fclose(fp);
}
void usage()
{
	printf("./a.out wlx000f009943b4\n");
}
void print_hex_ascii_line(const u_char *payload,int len,int offset)
{
	int i;
	int gap;
	const u_char *ch;
	printf("%05d ", offset);
	ch=payload;
	for(i=0;i<len;i++)
	{
		printf("%02x ",*ch );
		ch++;
		if(i==7)
			printf(" ");
	}
	if(len<8)
		printf(" ");
	if(len<16)
	{
		gap=16-len;
		for(i=0;i<gap;i++)
		{
			printf(" ");
		}
	}
	printf(" ");
	ch=payload;
	for(i=0;i<len;i++)
	{
		if(isprint(*ch))
			printf("%c",*ch);
		else
			printf(".");
		ch++;

	}
	printf("\n");
	return;
}
void write_txt(const u_char * payload,int len)
{
	int len_rem=len;
	int count_len=0;
	const u_char *ch=payload;
	
	FILE *fp;
	fp=fopen("data.txt","w+");
	if(fp==NULL)
	{
		printf("error open data.txt\n");
		return;
	}
	
	while(1)
	{
		if(isprint(*ch))
		{


		putc(*ch,fp);
		
		}
		count_len++;
		ch=ch+1;
		if(count_len>1500)
			break;
	}
	
	fclose(fp);

}
void print_payload(const u_char * payload,int len)
{
	int len_rem=len;
	int line_width=16;

	int line_len;
	int count_len=0;
	int offset=0;
	const u_char *ch=payload;
	if(len<=0)
		return;

	if(len<=line_width)
	{
		print_hex_ascii_line(ch,len,offset);
		return;
	}
	for(;;)
	{
		line_len=line_width %len_rem;
		print_hex_ascii_line(ch,line_len,offset);

		len_rem=len_rem-line_len;

		ch=ch+line_len;

		count_len=count_len+line_len;
		if(count_len>1500)
		{
			return;
		}
		offset=offset+line_width;
		
		if(len_rem<=line_width)
		{
			print_hex_ascii_line(ch,len_rem,offset);
			break;
		}
	}
	return;
}


void process_packet(u_char *args,const struct pcap_pkthdr *header,const u_char * packet)
{

	static int count =13198;
	struct Packet Packet_Num;
	const struct ieee80211_radiotap_header *it_header;
	const struct sniff_ip *ip_packet;
	const struct sniff_tcp *tcp_packet;
	const char *payload;
	int size_ip,size_tcp,size_payload;
	
	if(wired)
	{
		printf("it's a mistake,sorry,bro\n");
	}
	else
	{
		it_header=(struct ieee80211_radiotap_header *)(packet);
		ip_packet=(struct sniff_ip *)(packet+it_header->it_len+DATA_80211_FRAME_SIZE+LLC_HEADER_SIZE);
		size_ip=IP_HL(ip_packet)*4;
		//	printf("1\n");
		//	printf("From:%s\n",inet_ntoa(ip_packet->ip_src));
		//	printf("TO:%s\n",inet_ntoa(ip_packet->ip_dst));
		switch(ip_packet->ip_p)
		{
			case IPPROTO_TCP:
				//printf("Protocol:TCP\n");
				break;
			case IPPROTO_UDP:
				//printf("Protocol:UDP\n");
				//Packet_Num=Packet_Num+1;;
				return;	
			case IPPROTO_ICMP:
				//Packet_Num=Packet_Num+1;
				//printf("Protocol:ICMP\n");
				return;
			case IPPROTO_IP:
				//Packet_Num=Packet_Num+1;
				//printf("Protocol:IP\n");
				return;
			default:
				//Packet_Num=Packet_Num+1;
				//printf("Protocol Unknew\n");
				return;
		}
		if(size_ip<20)
		{
			printf("error ip len %u",size_ip);
			return;
		}

		/*printf("From:%s\n",inet_ntoa(ip_packet->ip_src));
		printf("TO:%s\n",inet_ntoa(ip_packet->ip_dst));
		printf("\npacket num=%d\n",count);
		count++;*/
		tcp_packet=(struct sniff_tcp *)(packet+size_ip+it_header->it_len+DATA_80211_FRAME_SIZE+LLC_HEADER_SIZE);
		size_tcp=TH_OFF(tcp_packet)*4;

		if(size_tcp<20)
		{
			printf("error tcp len %u",size_tcp);
			return;
		}
		if(ntohs(tcp_packet->th_dport)==80)
		{
			strcpy(Packet_Num.src_ip,inet_ntoa(ip_packet->ip_src));
			strcpy(Packet_Num.dst_ip,inet_ntoa(ip_packet->ip_dst));
			Packet_Num.num=count;


		payload=(char *)(packet+size_tcp+size_ip+it_header->it_len+DATA_80211_FRAME_SIZE+LLC_HEADER_SIZE);
		//printf("%hd\n",ip_packet->ip_len);
		//printf("%hd\n",ntohs(ip_packet->ip_len));
		size_payload=ntohs(ip_packet->ip_len)-(size_ip+size_tcp);
		
		
		//size_payload=size_payload-20000;
		if(size_payload>1)
		{
			
			//printf("12\n");
			const u_char *data_c=payload;
			if(*data_c=='G'&&*(data_c+1)=='E'&&*(data_c+2)=='T')
			{
				int q;
			}
			else
			{
				return;
			}
			printf(" Src port: %d\n", ntohs(tcp_packet->th_sport));
	        printf(" Dst port: %d\n", ntohs(tcp_packet->th_dport));

	        printf(" Payload (%d bytes):\n", size_payload);
	        
			printf("From:%s\n",inet_ntoa(ip_packet->ip_src));
			printf("TO:%s\n",inet_ntoa(ip_packet->ip_dst));
			printf("\npacket num=%d\n",count);
			print_payload(payload,size_payload);
			int Get_flag=0,flag=0,Get_i=0;
			int Get_num=0;
			
			char Temp[800];
			int data_num=0;
			while(1)
			{
				flag=Get_flag;
				
				
				if(*data_c=='\r'&&*(data_c+1)=='\n'){
					Temp[Get_i]='\0';
					Get_flag++;
				}
				else
				{
					Temp[Get_i]=*data_c;
					data_c=data_c+1;
					data_num++;
					Get_i++;
				}
				if(flag<Get_flag)
				{
					if(Temp[0]=='G'&&Temp[1]=='E'&&Temp[2]=='T')
					{
						//*Temp=*Temp+4;
						strcpy(Packet_Num.Get_Header,Temp);
						//Packet_Num.Get_Header[]
						memset(Temp,0,800);
						Get_i=0;

					}
					else if(Temp[0]=='H'&&Temp[1]=='o'&&Temp[2]=='s')
					{
						//*Temp=*Temp+5;
						strcpy(Packet_Num.Host,Temp);
						memset(Temp,0,800);
						Get_i=0;
						//break;
					}
					else if(Temp[0]=='U'&&Temp[1]=='s'&&Temp[2]=='e')
					{
						//*Temp=*Temp+11;
						strcpy(Packet_Num.User_Agent,Temp);
						memset(Temp,0,800);
						Get_i=0;
					}
					else
					{
						memset(Temp,0,800);
						Get_i=0;
					}

					data_c=data_c+2;
					if(*data_c=='\r')
						data_c=data_c+2;
				}
				if(Get_flag>10||data_num>500)
					break;
				

			}

			char Ful_Temp[800];
			char Ful_Te1[800];
			int Ful_i=4;
			//*Ful_Temp=*Packet_Num.Get_Header+4;
			while(Packet_Num.Get_Header[Ful_i])
			{
				
				if(Packet_Num.Get_Header[Ful_i]==' ')
					break;
				else
				{

					Ful_Temp[Ful_i-4]=Packet_Num.Get_Header[Ful_i];
					Ful_i++;
				}

			}
			Ful_Temp[Ful_i-4]='\0';
			printf("%s\n",Ful_Temp);
			for(Ful_i=6;Ful_i<strlen(Packet_Num.Host)+6;Ful_i++)
			{
			//	printf("222\n");
				Ful_Te1[Ful_i-6]=Packet_Num.Host[Ful_i];				
			}
			Ful_Te1[Ful_i-6]='\0';
			printf("%s\n",Ful_Te1);
			strcat(Ful_Te1,Ful_Temp);
			strcpy(Packet_Num.Full_Url,Ful_Te1);
			hello(Packet_Num,conn_ptr);
			printf("Num:%d\n",Packet_Num.num);
			printf("dst_ip:%s\n",Packet_Num.dst_ip);
			printf("src_ip:%s\n",Packet_Num.src_ip);
			printf("Get_Header:%s\n",Packet_Num.Get_Header);
			printf("User_Agent:%s\n",Packet_Num.User_Agent);
			printf("Host:%s\n",Packet_Num.Host);
			printf("Full_Url:%s\n",Ful_Te1);
			count++;		
			
	}
		return;

	}

}
}
int mysql()
{
	conn_ptr = mysql_init(NULL);//初始化
	 if(!conn_ptr)
    {
        printf("mysql_init failed!\n");
        return -1;
    }
    conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","root","chenyuzhe",0,NULL,0);//连接MySQL testdb数据库
     if(conn_ptr)
     {
     	printf("connect succeed!");
     }
}
int creat_table(*conn_ptr);//建表
int main(int argc, char const *argv[])
{
	//create_deamon();
	mysql();//连接数据库
	creat_table(*conn_ptr);
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr header;
	const u_char *packet;
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	char *filter ="tcp and ip";
	struct bpf_program fp;
	int r;
	pcap_if_t *alldevsp;
	if(argc==2)
	{
		device=argv[1];
	}
	else if(argv>2)
	{
		fprintf(stderr, "error:unreco\n");
		usage();
		exit(EXIT_FAILURE);
	}
	else
	{
		device=pcap_lookupdev(errbuf);
		if(device==NULL)
		{
			fprintf(stderr, "%s",errbuf);
			exit(1);
		}
	}
	errbuf[0]=0;
	signal(SIGINT,ctrl_c);

	handle=pcap_open_live(device,BUFSIZ,1,0,errbuf);
	if(handle==NULL)
	{
		fprintf(stderr, "%s",errbuf);
		exit(1);
	}
	if(strlen(errbuf)>0)
	{
		fprintf(stderr, "Warning:%s",errbuf);
		errbuf[0]=0;
	}
		printf("Using device:%s\n",device);
	
	if(pcap_datalink(handle)==DLT_EN10MB)
	{

		wired=1;
		printf("mistake\n");
		exit(1);
	}
	else
	{
		if(pcap_datalink(handle)==DLT_IEEE802_11_RADIO)
		{
			wired=0;
		}
		else
		{
			fprintf(stderr, "interface error\n");
			exit(1);
			
		}
	}
	if(pcap_compile(handle,&fp,filter,0,PCAP_NETMASK_UNKNOWN)==-1)
	{
		fprintf(stderr, "%s",pcap_geterr(handle));
		
		exit(1);
	}

	if(pcap_setfilter(handle,&fp)==-1)
	{
		fprintf(stderr, "%s",pcap_geterr(handle));
		
		exit(1);
	}
	pcap_freecode(&fp);

	if((r==pcap_loop(handle,Packet_Nu,process_packet,NULL))<0)
	{
		
		
		if(r==-1)
		{

			fprintf(stderr, "%s",pcap_geterr(handle));
			
			exit(1);
		}
	}

	pcap_close(handle);
	return 0;
}
