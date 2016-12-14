#include "client.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //struct sockaddr_in
#include <arpa/inet.h> //inet_addr
#include "app_intf/app_config_test/app_config_test_lib.h" //配置读取
#include <signal.h>//信号
#include "aiconfig.h"


namespace client
{

// std::map<int, CLinker> CClient::m_mapLinker = new std::map<int, CLinker>;

CClient::CClient():
	m_sockfd(-1)
{
	memset(m_szAlias, 0, ALIAS_SIZE);
	m_pClientConfig = NAMESPACE_OD_APP_CFG AppReadConfig<CClientConfig>();
	if (NULL == m_pClientConfig)
    {
        LOG_ERROR(CLIENT_CONFIG_ERROR, "==[CLIENT]======= Failed to read config!========");
        return ;
    }
	m_strIP = m_pClientConfig->m_cClientParams.m_szClientIP	;	
	m_iPort = m_pClientConfig->m_cClientParams.m_iClientPort;
	m_strServerIP = m_pClientConfig->m_cClientParams.m_szServerIP;
}

CClient::~CClient()
{
	if (m_sockfd >= 0)
	{
		close(m_sockfd);
	}
}

//调用静态成员函数来初始化静态成员map
MAPLINKER CClient::init_map()
{
	MAPLINKER mapLinker;
	return mapLinker;
}
MAPLINKER CClient::m_mapLinker(CClient::init_map());
// CClient::m_mapLinker.clear();

int CClient::init()
{
	ENTER_FUNC;

	//创建套接字
	if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		LOG_ERROR(CLIENT_INIT_SOCKET_ERR, "init socket error!!!");
		perror("init sockfd failed!!!");
		return -1;
	}
	LOG_TRACE("==[SERVER]== init socket success:%d\n", m_sockfd);
	CLinker cLinker;
	cLinker.set_sockfd(m_sockfd);
	client::CClient::m_mapLinker.insert(std::make_pair(m_sockfd, cLinker));

	//设置套接字选项避免地址使用错误  
    int opt = 1;  
    if((setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }  

	//设置地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_iPort);
	addr.sin_addr.s_addr = inet_addr(m_strServerIP.c_str());

	// if (bind(m_sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	// {
	// 	LOG_ERROR(CLIENT_BIND_SOCKET_ERR, "bind error!!!");
	// 	perror("bind failed!!!");
	// 	return -1;		
	// }
	// LOG_TRACE("==[SERVER]== bind socket success:%d\n", m_sockfd);
	//连接服务器
	if (connect(m_sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		LOG_ERROR(CLIENT_CONNECT_SOCKET_ERR, "connect socket error!!!");
		perror("connect sockfd failed!!!");
		return -1;
	}
	LOG_TRACE("==[SERVER]== connect socket success:%d\n", m_sockfd);

	LEAVE_FUNC;
}	

void CClient::start_chat()
{
	ENTER_FUNC;

	pthread_t pid;
	pthread_create(&pid, 0, recv_thread, &m_sockfd);
	while(1)
	{
		char szMsg[MSG_LEN] = {};
		char szSendMsg[SEND_MSG_LEN] = {};
		fgets(szMsg, MSG_LEN, stdin);
		sprintf(szSendMsg, "%s: %s", get_alias(), szMsg);
		if (send(m_sockfd, szSendMsg, strlen(szSendMsg), 0) == -1)
		{
			LOG_ERROR(CLIENT_SEND_ERR, "send error!!!");
			perror("send failed!!!");
			return;			
		}
	}

	LEAVE_FUNC;
}		

void* CClient::recv_thread(void* arg)
{
	int iSockfd = *(int*)arg;
	char szMsg[SEND_MSG_LEN] = {};
	while(1)
	{
		memset(szMsg, 0, SEND_MSG_LEN);
		if (recv(iSockfd, szMsg, SEND_MSG_LEN, 0) < 0)
		{
			LOG_ERROR(CLIENT_RECV_ERR, "recv error!!!");
			perror("recv failed!!!");
			return (void*)0;	
		}
		// LOG_TRACE("%s", szMsg);
		printf("%s\n", szMsg);
	}	
} 

void CClient::sig_close(int)
{
	std::map<int, CLinker>::const_iterator iter = m_mapLinker.begin();
	for (; iter != m_mapLinker.end(); ++iter)
	{
		if (iter->first >= 0 )
		{
			close(iter->first);
		}
	}
	m_mapLinker.erase(iter->first);
	LOG_TRACE("the client has to be closed!!!");
	exit(0);
}

void CClient::set_alias(const char szAlias[])
{
	strcpy(m_szAlias, szAlias);
}

char* CClient::get_alias()
{
	return m_szAlias;
}

int   CClient::get_sockfd()
{
	return m_sockfd;
}

}//end namespace

int main(int argc, char const *argv[])
{
	std::string strConfig = "client_init_config.xml";
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

    // sal::Startup();
	client::CClient cClient;
    LOG_TRACE("please input your alias:");
    char buf[ALIAS_SIZE] = {};
    fgets(buf, ALIAS_SIZE, stdin);
    buf[strlen(buf) - 1] = ' ';
    cClient.set_alias(buf);

    //修改信号 ctrl+c
	client::CClient::m_mapLinker.clear();
	signal(SIGINT, client::CClient::sig_close);	

	cClient.init();
	send (cClient.get_sockfd(), cClient.get_alias(), ALIAS_SIZE, 0);
	cClient.start_chat();

    // sal::Shutdown();

	return 0;
}