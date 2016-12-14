#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "public/app_intf/app_config/app_config.h"
#include <string>
#include "common_def.h"

namespace server 
{

class CServerParams
{
public:
	CServerParams():
		m_iServerPort(8888),
		m_bIfLocalTest(1)
	{
		memset(m_szServerIP, 0, sizeof(m_szServerIP));
	};
	//~CServerParams(){};

public:
	char m_szServerIP[BUFF_SIZE];	//服务端IP
	int  m_iServerPort;				//服务端端口
	bool m_bIfLocalTest;			//服务器是否为本地测试，0网络测试，1本地测试
	int  m_iMixListenNum;			//服务器最大监听数

	APP_READ_CONFIG_BEGIN
        APP_READ_VALUE(m_szServerIP, "server_ip", "10.10.10.183", NAMESPACE_OD_APP_CFG emValueOptional);
        APP_READ_VALUE(m_iServerPort, "server_port", 6666, NAMESPACE_OD_APP_CFG emValueOptional);
        APP_READ_VALUE(m_bIfLocalTest, "if_local_test", 1, NAMESPACE_OD_APP_CFG emValueOptional);
        APP_READ_VALUE(m_iMixListenNum, "mix_listen_num", 100, NAMESPACE_OD_APP_CFG emValueOptional);
    APP_READ_CONFIG_END
};

class CServerConfig : public NAMESPACE_OD_APP_CFG CAppConfigBase
{
public:
	CServerConfig(){};
	virtual ~CServerConfig(){};

public:
	CServerParams m_cServerParams;

	APP_READ_CONFIG_BEGIN
        APP_READ_OBJ(m_cServerParams, "server_params", NAMESPACE_OD_APP_CFG emValueOptional)
    APP_READ_CONFIG_END
};

}//end namespace

namespace client
{

class CClientParams
{
public:
	CClientParams():
		m_iClientPort(8888)
	{
		memset(m_szClientIP, 0, sizeof(m_szClientIP));
		memset(m_szServerIP, 0, sizeof(m_szServerIP));
	};
	//~CClientParams(){};

public:
	char m_szClientIP[BUFF_SIZE];
	int  m_iClientPort;
	char m_szServerIP[BUFF_SIZE];

	APP_READ_CONFIG_BEGIN
        APP_READ_VALUE(m_szClientIP, "client_ip", "10.10.10.183", NAMESPACE_OD_APP_CFG emValueOptional);
        APP_READ_VALUE(m_iClientPort, "client_port", 8888, NAMESPACE_OD_APP_CFG emValueOptional);
        APP_READ_VALUE(m_szServerIP, "server_ip", "127.0.0.1", NAMESPACE_OD_APP_CFG emValueOptional);
    APP_READ_CONFIG_END	
	
};

class CClientConfig : public NAMESPACE_OD_APP_CFG CAppConfigBase
{
public:
	CClientConfig(){};
	virtual ~CClientConfig(){};

public:
	CClientParams m_cClientParams;


	APP_READ_CONFIG_BEGIN
        APP_READ_OBJ(m_cClientParams, "client_params", NAMESPACE_OD_APP_CFG emValueOptional)
    APP_READ_CONFIG_END
	
};

} //end namespace client
#endif // __CONFIG_H__