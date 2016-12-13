#ifndef __LINKER_H__
#define __LINKER_H__

#include <string>
#include <pthread.h> 

class CLinker
{
public:
	CLinker():
		m_sockfd(-1),
		m_strIP(""),
		m_strAlias(""),
		m_pid(-1)
		{};
	~CLinker(){};

	void set_sockfd(const int sockfd) {m_sockfd = sockfd;};
	void set_ip(const std::string strIP){m_strIP = strIP;};
	void set_alias(const std::string strAlias){m_strAlias = strAlias;};
	void set_pid(const pthread_t pid){m_pid = pid;};

	int get_sockfd() 		 const {return m_sockfd;};
	std::string get_ip() 	 const {return m_strIP;};
	std::string get_alias()  const {return m_strAlias;};
	pthread_t get_pid() 	 	   {return m_pid;};
private:
	int		m_sockfd;	//客户端套接字
	std::string	m_strIP;	//客户端IP
	std::string	m_strAlias; //用户昵称
	pthread_t m_pid;	//线程ID
	
};

#endif //__LINKER_H__