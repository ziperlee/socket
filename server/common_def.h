#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#define BUFF_SIZE 30
#define MSG_LEN 100
#define ENTER_FUNC LOG_TRACE("\n==[ABM_SERV_INIT]==>enter func: %s ... ", __func__)
#define LEAVE_FUNC LOG_TRACE("\n==[ABM_SERV_INIT]==>leave func: %s ... ", __func__)

enum SERVER_ERROR
{
	SERVER_CONFIG_ERROR	= 0, //配置初始化失败
	SERVER_INIT_SOCKET_ERR = 1, //socket返回失败
	SERVER_BIND_SOCKET_ERR = 2, //bind返回失败
	SERVER_LISTEN_SOCKET_ERR = 3, //listen返回失败
	SERVER_ACCEPT_ERR = 4, //accept返回失败
	SERVER_SEND_SOCKET_ERR =5, //send返回失败
};

#endif //__COMMON_DEF_H__