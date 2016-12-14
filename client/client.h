#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include "config.h"
#include "linker.h"

namespace client
{

typedef std::map<int, CLinker> MAPLINKER;

class CClient
{
public:
	CClient();
	~CClient();

	int  init();				//客户端初始化
	void start_chat();			//主线程发送消息
	static void* recv_thread(void* arg); //子线程接受服务器消息 
	static void	sig_close(int);	//信号处理
	static MAPLINKER init_map();
	void  set_alias(const char szAlias[]);
	char* get_alias();
	int   get_sockfd();
	
private:
	int		m_sockfd;				//客户端套接字
	int  	m_iPort;				//客户端端口
	std::string m_strServerIP;		//服务端IP
	std::string	m_strIP;			//客户端IP
	char m_szAlias[ALIAS_SIZE]; 	//客户端昵称
	CClientConfig* m_pClientConfig; //客户端配置读取

public:
	static MAPLINKER m_mapLinker;
};

}//end namespace
#endif //__CLIENT_H__