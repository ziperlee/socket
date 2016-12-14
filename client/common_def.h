#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#define BUFF_SIZE 30
#define ALIAS_SIZE 30
#define MSG_LEN 100
#define SEND_MSG_LEN 150
#define ENTER_FUNC LOG_TRACE("\n==[ABM_SERV_INIT]==>enter func: %s ... ", __func__)
#define LEAVE_FUNC LOG_TRACE("\n==[ABM_SERV_INIT]==>leave func: %s ... ", __func__)

enum SERVER_ERROR
{
	CLIENT_CONFIG_ERROR	= 0, //配置初始化失败
	CLIENT_INIT_SOCKET_ERR = 1, //socket返回失败
	// CLIENT_BIND_SOCKET_ERR = 2, //bind返回失败
	// CLIENT_LISTEN_SOCKET_ERR = 3, //listen返回失败
	// CLIENT_ACCEPT_ERR = 4, 	//accept返回失败
	CLIENT_SEND_ERR =5, //发送失败
	CLIENT_CONNECT_SOCKET_ERR = 6,//连接服务器失败
	CLIENT_RECV_ERR = 7,//接受失败
};

#endif //__COMMON_DEF_H__