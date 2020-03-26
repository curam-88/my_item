#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<mysql/mysql.h>
#include<string.h>

//MYSQL *conn_ptr;
//unsigned int timeout = 7;   //超时时间7秒
struct Packet
{
	int num;
	char src_ip[25];
	char dst_ip[25];
	char Get_Header[800];
	char User_Agent[800];
	char Host[800];
	char Full_Url[800];
}Packet;
int creat_table(*conn_ptr)
{
    ret = mysql_query(conn_ptr,"create table packet(num int(11) primary key,src_ip varchar(100),dst_ip varchar(100),Get_Header varchar(800),User_Agent varchar(800),Host varchar(200),Full_Url varchar(800),whois_ID int(11));"); //执行SQL语句
        if(!ret)
        {
            printf("create packet succeed");
        }
        else
        {
            printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
        }
        ret = mysql_query(conn_ptr,"create table whois(whois_ID int(11) primary key,workplace varchar(100),phone varchar(100));"); 
         if(!ret)
        {
            printf("create packet succeed");
        }
        else
        {
            printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
        }
}
int hello(struct Packet packet,MYSQL *conn_ptr)
{
    char insert1[100];
    char insert2[5];
    char insert3[5];
    char insert[2000];
    //初始化
    memset(insert,0,2000);
    memset(insert1,0,100);
    memset(insert2,0,5);
    memset(insert3,0,5);
    memset(insert,0,500);
    memset(insert1,0,100);
    memset(insert2,0,5);
    memset(insert3,0,5);

    strcpy(insert1,"INSERT INTO packet(num,src_ip,dst_ip,Get_Header,User_Agent,Host,Full_Url,whois_ID) VALUES(");
    strcpy(insert2,"','");
    strcpy(insert3,"')");
    /*strcpy(packet.src_ip,"127.0.0.1");
    strcpy(packet.dst_ip,"127.0.0.1");
    strcpy(packet.Get_Header,"1");
    strcpy(packet.User_Agent,"1");
    strcpy(packet.Full_Url,"1");
    strcpy(packet.Host,"1");
    */
	int ret = 0;
//	conn_ptr = mysql_init(NULL);//初始化
//	 if(!conn_ptr)
  //  {
    //    printf("mysql_init failed!\n");
      //  return -1;
    //}
    /*ret = mysql_options(conn_ptr,MYSQL_OPT_CONNECT_TIMEOUT,(const char*)&timeout);//设置超时选项
    if(ret)
    {
        printf("Options Set ERRO!\n");
    }*/
    //conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","root","chenyuzhe",0,NULL,0);//连接MySQL testdb数据库
     //if(conn_ptr)
   // {
        printf("aaa");
        printf("Connection Succeed!\n");
       strcat(insert,insert1);
       char number[10];
       //将结构体字符串插入sql语句
      sprintf(number,"%d",packet.num);  
        strcat(insert,number);
        strcat(insert,",'");
        //将数组字符串链接
        strcat(insert,packet.src_ip);
        strcat(insert,insert2);
        strcat(insert,packet.dst_ip);
        strcat(insert,insert2);
        strcat(insert,packet.Get_Header);
        strcat(insert,insert2);
        strcat(insert,packet.User_Agent);
        strcat(insert,insert2);
        strcat(insert,packet.Host);
        strcat(insert,insert2);
        //printf("HOST:\n%s\n",packet.Host);
        //printf("GET_Header\n%s\n",packet.Get_Header);
        //printf("Full_Url:\n%s\n",packet.Full_Url);
        strcat(insert,packet.Full_Url);
        strcat(insert,insert2);
        strcat(insert,packet.Full_Url);
        strcat(insert,insert3);
        printf("\n%s\n",insert);
        ret = mysql_query(conn_ptr,insert); //执行SQL语句
        if(!ret)
        {
            printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));//返回上次UPDATE更改行数
        }
        else
        {
            printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
        }

        /*mysql_close(conn_ptr);
        printf("Connection closed!\n");
    }
    else    //错误处理
    {
        printf("Connection Failed!\n");
        if(mysql_errno(conn_ptr))
        {
            printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
        }
        return -2;
    }*/
}
    


