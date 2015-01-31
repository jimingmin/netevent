#include "net_capi.h"
#include "../common/common_api.h"

void init_context(callback_net_accepted func_net_accepted, callback_net_connected func_net_connected,
						   callback_net_read func_net_read, callback_net_writen func_net_writen,
						   callback_net_closed func_net_closed)
{
	//g_CallBackContext.net_handler = new CNetHandler();
	g_NetHandler = new CNetHandler();
	g_NetHandler->CreateReactor();
	g_CallBackSet.func_net_accepted = func_net_accepted;
	g_CallBackSet.func_net_connected = func_net_connected;
	g_CallBackSet.func_net_read = func_net_read;
	g_CallBackSet.func_net_writen = func_net_writen;
	g_CallBackSet.func_net_closed = func_net_closed;

	//return g_NetHandler;
}

void net_run()
{
	while(true)
	{
		int32_t nIdle = g_NetHandler->Run();
		if(nIdle == 0)
		{
			Delay(10 * US_PER_MS);
		}
	}
}

CAcceptor *create_acceptor()
{
	return new CAcceptor(g_NetHandler, new CNetPacketParserFactory(), new CNetMessageHandler());
}

void destory_acceptor(CAcceptor *pAcceptor)
{
	delete pAcceptor->GetPacketParserFactory();
	delete pAcceptor->GetIOHandler();
	delete pAcceptor;
}

CConnector *create_connector()
{
	return new CConnector(g_NetHandler, new CNetPacketParserFactory(), new CNetMessageHandler());
}

void destory_connector(CConnector *pConnector)
{
	delete pConnector->GetPacketParserFactory();
	delete pConnector->GetIOHandler();
	delete pConnector;
}

int32_t net_bind(CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort)
{
	return pAcceptor->Bind(pLocalAddress, nLocalPort);
}

int32_t net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return g_CallBackSet.func_net_accepted(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_connect(CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort)
{
	return pConnector->Connect(pPeerAddress, nPeerPort);
}

int32_t net_connect_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return g_CallBackSet.func_net_connected(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_read_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return g_CallBackSet.func_net_read(nSessionID, pData, nBytes);
}

int32_t net_write(SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes)
{
	CConnMgt &conn_mgt = g_NetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		return -1;
	}

	return conn->Write(arrBuf, nBytes);
}

int32_t net_write_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return g_CallBackSet.func_net_writen(nSessionID, pData, nBytes);
}

int32_t net_close(SessionID nSessionID)
{
	CConnMgt &conn_mgt = g_NetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		return -1;
	}

	conn->Close();
	return 0;
}

int32_t net_close_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return g_CallBackSet.func_net_closed(nSessionID, pPeerAddress, nPeerPort);
}
