var http = require('http');
var fs=require("fs");
var url=require("url");
var querystring=require("querystring");
var session={};
var mysql  = require('mysql');
var dateBase=[ {
    num: 97,
    src_ip: '172.23.1.53',
    dst_ip: '61.151.183.16',
    Get_Header: 'GET / HTTP/1.1',
    User_Agent: 'User-Agent: MicroMessenger Client',
    Host: 'Host: weixin.qq.com',
    Full_Url: '' },
   {
    num: 98,
    src_ip: '172.23.1.53',
    dst_ip: '61.151.183.16',
    Get_Header: 'GET / HTTP/1.1',
    User_Agent: 'User-Agent: MicroMessenger Client',
    Host: 'Host: weixin.qq.com',
    Full_Url: '' }];
var whois=[{
    whois_ID: 97,
    workplace: '172.23.1.53',
    phone: '61.151.183.16' }];
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'root',
  database : 'chenyuzhe'
});
connection.connect();
connection.query('SELECT num,src_ip,dst_ip,Get_Header,User_Agent,Host,Full_Url FROM packet', function (error, results, fields) {
  if (error) throw error;
  dateBase=results;
});
connection.query('SELECT whois_ID,workplace,phone FROM whois', function (error, results, fields) {
    if (error) throw error;
    whois = results;
});
http.createServer(function (request, response) {
	var parseUrl=url.parse(request.url);
	var headers=request.headers;
	var cookie=headers.cookie;
	if(parseUrl.pathname.indexOf("/p")!=-1||parseUrl.pathname.indexOf("/css")!=-1||parseUrl.pathname.indexOf("/js")!=-1){
			response.write(fs.readFileSync("."+parseUrl.pathname));
			response.end();
			return;
	}		
	else if(parseUrl.pathname=="/date"){
			response.write(JSON.stringify(dateBase));
			response.end();
    }
    else if (parseUrl.pathname == "/whois") {
        response.write(JSON.stringify(whois));
        response.end();
    }
	else{

		if(cookie in session){
			response.setHeader('Content-Type', 'text/html');
			if(parseUrl.pathname.indexOf(".html")!=-1){
				console.log(parseUrl.pathname);
				response.write(fs.readFileSync("./html"+parseUrl.pathname));
			}
			else if(parseUrl.pathname=='/'||parseUrl.pathname=="/login"){
				response.write(fs.readFileSync("./html/homepage.html"));
			}
			response.end();
		}
		else if(parseUrl.pathname=="/"){
			response.write(fs.readFileSync("./html/enter.html"));
			response.end();
		}
		else if(parseUrl.pathname=="/login"){
			var post="";
			request.on("data",function(data){
				post+=data;
			});
			request.on("end",function(){
				var parseData=querystring.parse(post);
				if(parseData.user==='wlgc'&&parseData.password==='zqsj'){
					var newSession=new Date().getTime()+parseData.user;
					response.setHeader('Set-Cookie', newSession);
					session[newSession]=parseData.user;
					response.setHeader('Content-Type', 'text/html');
					response.write(fs.readFileSync("./html/homepage.html"));
				}
				else{
					response.setHeader('Content-Type', 'text/html');
					response.write(fs.readFileSync("./html/enter.html"));
				}
				response.end();
			});	
		}
	}
}).listen(80);
console.log("服务器启动！");