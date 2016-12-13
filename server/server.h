#ifndef __SERVER_H__
#define __SERVER_H__

#include <map>
#include <string>
#include "linker.h"
#include "config.h"

namespace server
{

typedef std::map<int, CLinker> MAPLINKER;

class CServer
{
private:
	CServer();
	CServer(const CServer &); 								//禁止拷贝构造
	CServer & operator = (const CServer &); 				//禁止拷贝构造

public:
	~CServer();
	static CServer & get_instance();						//单例模式
	int 	init();			    							//初始化
	void	start_server();	    							//服务端服务
	static  void	sig_close(int);	    							//信号
	void	broadcast(char* szMsg, const int iSockfd);	    //向所有客户端广播
	static void*   server_thread(void* arg); 				//线程处理函数
private:
	MAPLINKER m_maplinker;
	int 	m_sockfd;										//服务器套接字
	int 	m_iPort;										//服务器端口
	int 	m_iMixListenNum;								//服务器最大监听数
	std::string	m_strIP;									//服务器IP
	CServerConfig* m_pServerConfig; 						//配置获取类
	bool 	m_bIfLocalTest;									//服务器是否为本地测试，0网络测试，1本地测试

};

#define g_cServer CServer::get_instance() 

} // end namepace server
#endif //__SERVER_H__