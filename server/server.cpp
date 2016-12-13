#include "server.h"
#include "config.h"
#include "app_intf/app_config_test/app_config_test_lib.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "common_def.h"
#include "aiconfig.h"//init_config()
#include "stdafx.h"	//sal

namespace server 
{

CServer::CServer()
{
	m_maplinker.clear();
	m_pServerConfig = NAMESPACE_OD_APP_CFG AppReadConfig<CServerConfig>();
	if (NULL == m_pServerConfig)
    {
        LOG_ERROR(SERVER_CONFIG_ERROR, "==[SERVER]======= Failed to read config!========");
        return ;
    }
    m_iPort = m_pServerConfig->m_cServerParams.m_iServerPort;
    m_strIP = m_pServerConfig->m_cServerParams.m_szServerIP;
    m_bIfLocalTest = m_pServerConfig->m_cServerParams.m_bIfLocalTest;
    m_iMixListenNum = m_pServerConfig->m_cServerParams.m_iMixListenNum;
}
		
CServer::~CServer()
{
	close(m_sockfd);
}

CServer & CServer::get_instance()
{	
	static CServer cServer;
	return cServer;
}
int CServer::init()
{
	ENTER_FUNC;

	//创建socket
	if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		LOG_ERROR(SERVER_INIT_SOCKET_ERR, "==[SERVER]== init socket error");
		return -1;
	}
	LOG_TRACE("==[SERVER]== init socket success:%d\n", m_sockfd);

	//设置套接字选项避免地址使用错误  
    int opt = 1;  
    if((setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }  
	//绑定地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_iPort);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(m_sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror ("绑定失败");
		LOG_ERROR(SERVER_BIND_SOCKET_ERR, "==[SERVER]== bind socket error");
		return -1;
	}
	LOG_TRACE("==[SERVER]== bind socket success");

	if(listen(m_sockfd, m_iMixListenNum) == -1)
	{
		LOG_ERROR(SERVER_LISTEN_SOCKET_ERR, "==[SERVER]== listen socket error");
		return -1;
	}
	LOG_TRACE("==[SERVER]== listen socket success");

    return 0;
	LEAVE_FUNC;
}
void CServer::start_server()
{
	ENTER_FUNC;

	while(1)
	{
		sockaddr_in clientAddr;
		//&操作数必须为左值，&(socklen_t(sizeof(clientAddr))) 为非法
		socklen_t clientAddrLen = sizeof(clientAddr);
		CLinker cLinker;
		int iSockTmp = accept(m_sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (iSockTmp == -1)
		{
			LOG_ERROR(SERVER_ACCEPT_ERR, "==[SERVER]== accept error");
			continue;
		}
		cLinker.set_sockfd(iSockTmp);
		m_maplinker.insert(std::make_pair(iSockTmp, cLinker));
		pthread_t iPidTmp = cLinker.get_pid();
		pthread_create(&iPidTmp, 0, server_thread, &iSockTmp);
	}

	LEAVE_FUNC;
}

void CServer::broadcast(char* szMsg, const int iSockfd)
{
	MAPLINKER::const_iterator iter = m_maplinker.begin();
	for(; iter != m_maplinker.end(); ++iter)
	{
		if (iter->first != iSockfd)
		{
			int iMsgLen = -1;//信息超长
			LOG_TRACE("======%s", szMsg);
			if(send(iter->first, szMsg, strlen(szMsg), 0) == -1)
			{
				LOG_ERROR(SERVER_SEND_SOCKET_ERR, "==[SERVER]== send message error");
				continue;			
			}
			LOG_TRACE("broadcast message to %s", iter->second.get_alias().c_str());
		}
	}
}
//
void* CServer::server_thread(void* arg)
{
	int iSockfd = *(int*)arg;
	char szAlias[MSG_LEN] = {};
	char szMsg[MSG_LEN] = {};	
	MAPLINKER::iterator mapLinkerIter = g_cServer.m_maplinker.find(iSockfd);
	if (recv(iSockfd, szAlias, MSG_LEN, 0) > 0)	//获取用户昵称
	{
		if (mapLinkerIter != g_cServer.m_maplinker.end())
		{
			mapLinkerIter->second.set_alias(std::string(szAlias));
		}
		else
		{
			LOG_TRACE("this must be error, the iSockfd: %d not connect to server", iSockfd);
			return (void*)0;
		}
	}
	else
	{
		LOG_TRACE("fail to get alias, user default 'ziper'");
		// szMsg = "ziper";
		strcpy(szAlias, "ziper");
		mapLinkerIter->second.set_alias(std::string(szAlias));
	}
	sprintf(szMsg, "welcome %s join\n", szAlias);
	g_cServer.broadcast(szMsg, iSockfd);
	while(1)
	{
		memset(szMsg, 0, MSG_LEN);
		if (recv(iSockfd, szMsg, MSG_LEN, 0) == 0)	//客户端退出时返回0
		{
			g_cServer.m_maplinker.erase(iSockfd);
			sprintf(szAlias, "%s has left\n", szAlias);
			g_cServer.broadcast(szMsg, iSockfd);
			close(iSockfd);
			break;
		}
		g_cServer.broadcast(szMsg, iSockfd);
	}
}

void CServer::sig_close(int)
{
	LOG_TRACE("the server has to be closed!!!");
	close(g_cServer.m_sockfd);
	exit(0);
}

}//end namespace

int main(int argc, char const *argv[])
{
	std::string strConfig = "server_init_config.xml";
    //初始化日志
    LOGGER_INITIALIZE_XML_FILE("./uttestlog.xml");
    LOG_TRACE("==[SERVER]== log init success ! ");

    // 读取配置文件
    int iRet = init_config(strConfig.c_str());
    if (0 == iRet)
    {
        LOG_ERROR(0, "==[SERVER]== init config fail !!!");
        return iRet;
    }
    app_cfg_test::CAppCfgTest appTest;
    appTest.Init();

    sal::Startup();

	signal(SIGINT, server::g_cServer.sig_close);
	if (0 != server::g_cServer.init())
	{
	   LOG_ERROR(0, "==[SERVER]== server init fail !!!");
	   return 0;
	}
	server::g_cServer.start_server();

    sal::Shutdown();

	return 0;
}