/*
 * net_socket.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include "net_api.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_namespace.h"
#include "net_impl.h"
#include "net_timer.h"
#include "net_handler.h"
#include "../common/common_cyclebuffer.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

NETEVENT_NAMESPACE_BEGIN

class CSocket: public IIOHandler
{
public:
	CSocket();
	virtual ~CSocket();

	virtual void Reset()
	{
		Clear();

		m_nSocketAttr = enmSocketAttr_Unknown;
		m_nBelongPort = 0;
	}

	//����socket�����Ҫ�������ݳ�Ա����������˺���
	virtual void Clear()
	{
		m_nSocketFD = enmInvalidSocketFD;
		m_nSocketType = enmSocketType_Communicate;
		m_nSocketStatus = enmSocketStatus_Closed;
		memset(m_szClientAddress, 0, sizeof(m_szClientAddress));
		m_nPeerAddress = 0;
		m_nPeerPort = 0;
		m_nCreateTime = 0;
		m_nTotalRecvBytes = 0;
		m_nLastRecvTime = 0;
		m_nTotalSendBytes = 0;
		m_nLastSendTime = 0;
		m_stRecvBuffer.Reset();
		m_stSendBuffer.Reset();
//		m_nConnectTimerIndex = enmInvalidTimerIndex;
		m_pSocketTimer = NULL;
		m_pNetHandler = NULL;
		m_nIOEvents = 0;
	}
	//�����׽���������
	void SetSocketFD(SocketFD nSocketFD);
	//��ȡ�׽���������
	SocketFD GetSocketFD() const;
	//�����׽���״̬
	void SetSocketStatus(SocketStatus nSocketStatus);
	//��ȡ�׽���״̬
	SocketStatus GetStatus() const;

	void SetSocketType(SocketType nSocketType);

	SocketType GetSocketType();

	void SetSocketAttr(SocketAttr nSocketAttr);

	SocketAttr GetSocketAttr();

	char *GetPeerAddressStr();

	void SetPeerAddress(uint32_t nAddress);

	uint32_t GetPeerAddress();

	void SetPeerPort(uint16_t nPort);

	uint16_t GetPeerPort();

	uint32_t GetListenAddress();

	uint16_t GetListenPort();

	void SetBelongPort(uint16_t nPort);

	uint16_t GetBelongPort();

	void SetCreateTime(time_t nCreateTime);

	void SetNetHandler(CNetHandler *pNetHandler);

	virtual int32_t ReadEvent();

	virtual int32_t WriteEvent();

	virtual int32_t ErrorEvent();

	//���׽���
	virtual int32_t OpenSocketAsServer(const char* szLocalIP, uint16_t nPort);
	//���׽���
	virtual int32_t OpenSocketAsClient(const char* szLocalIP = NULL);
	//�ر��׽���
	virtual void CloseSocket(int32_t nCloseCode = 0);
	//�����˽�������
	virtual int32_t Connect(const char* szRemoteIP, uint16_t nPort);
	//��������
	virtual int32_t Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes);
	//��������
	virtual int32_t Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes);

	uint32_t GetIOEvents();

	void SetIOEvents(uint32_t nEvents);

protected:
	//���¼��ص�
	virtual int32_t OnRead(int32_t nErrorCode) = 0;
	//д�¼��ص�
	virtual int32_t OnWrite(int32_t nErrorCode) = 0;
	//�쳣�¼��ص�
	virtual int32_t OnError(int32_t nErrorCode) = 0;
	//�������ӻص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);
	//���ӳɹ��ص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnConnected();
	//���ӳ�ʱ�ص�
	virtual int32_t OnConnectTimeout();
	//�Ͽ����ӻص�(���ش˺�����������������Щ��Դ���յĹ���)
	virtual int32_t OnDisconnect(int32_t nCloseCode);
	//���ͻ�����ʣ�������
	virtual int32_t SendRestData();
	//���Ծ����Ŭ����ȡ���������
	int32_t nRead(uint8_t *pBuffer, const int32_t nLength,bool &isConnClose);
	//���Ծ����Ŭ��д����д������
	int32_t nWrite(const uint8_t *pBuffer, const int32_t nLength);
	//�ı�epoll�¼�
	void ChangeWriteEvent();

private:
	//��ʱ���¼�
	int32_t OnTimerEvent(CFrameSocketTimer *pTimer);
	//���ӳ�ʱ
	int32_t ConnectTimeout();
	//���ӳɹ�
	int32_t Connected();
	//���ӹر�
	int32_t Disconnected(int32_t nCloseCode);

protected:
	SocketFD		m_nSocketFD;
	SocketType		m_nSocketType;
	SocketStatus	m_nSocketStatus;
	SocketAttr		m_nSocketAttr;
	char			m_szClientAddress[enmMaxIPAddressLength];
	uint32_t		m_nPeerAddress;				//�Զ�ip
	uint16_t		m_nPeerPort;				//�Զ�port
	uint32_t		m_nListenAddress;			//������ַ
	uint16_t		m_nListenPort;				//�����˿�
	uint16_t		m_nBelongPort;				//�����˿�(���׽���ֻ�ڴ˶˿�������)
	time_t			m_nCreateTime;				//���Ӵ���ʱ��
	time_t			m_nLastRecvTime;			//���һ�δӶԶ˽������ݵ�ʱ��
	time_t			m_nLastSendTime;			//���һ�η����Զ����ݵ�ʱ��
	int32_t			m_nTotalRecvBytes;			//�յ������ݴ�С
	int32_t			m_nTotalSendBytes;			//���͵����ݴ�С
	CycleBuffer<enmRecvBufferSize>	m_stRecvBuffer;	//���ջ�����
	CycleBuffer<enmSendBufferSize>	m_stSendBuffer;	//���ͻ��棬���׽����쳣����ϵͳ���ͻ�����������£����ݽ��ᱻ�Ž��˻���

//	TimerIndex		m_nConnectTimerIndex;		//���ӳ�ʱ��ʱ������
	CFrameSocketTimer *m_pSocketTimer;
	CNetHandler		*m_pNetHandler;
	uint32_t		m_nIOEvents;
};

NETEVENT_NAMESPACE_END


#endif /* NET_SOCKET_H_ */
