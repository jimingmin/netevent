#include "net_capi.h"
#include "../common/common_api.h"
#include "net_logger.h"

CNetHandler* init_context(callback_net_parser func_net_parser, callback_net_accepted func_net_accepted,
					callback_net_connected func_net_connected, callback_net_connect_timeout func_net_connect_timeout,
					callback_net_read func_net_read, callback_net_writen func_net_writen,
					callback_net_closed func_net_closed, callback_net_error func_net_error,
					const char *log_dir, const char *log_name)
{
	CNetHandler *pNetHandler = new CNetHandler();
	pNetHandler->CreateReactor(enmReactorType_Select);

	NetFuncEntry *pFuncEntry = new NetFuncEntry();
	pFuncEntry->func_net_parser = func_net_parser;
	pFuncEntry->func_net_accepted = func_net_accepted;
	pFuncEntry->func_net_connected = func_net_connected;
	pFuncEntry->func_net_connect_timeout = func_net_connect_timeout;
	pFuncEntry->func_net_read = func_net_read;
	pFuncEntry->func_net_writen = func_net_writen;
	pFuncEntry->func_net_closed = func_net_closed;
	pFuncEntry->func_net_error = func_net_error;
	pNetHandler->SetNetHandlerCallBack(pFuncEntry);

	if((log_dir != NULL) && (log_name != NULL))
	{
		LoggerConfig *pLoggerConfig = new LoggerConfig();

		pLoggerConfig->log_dir = (char *)malloc(strlen(log_dir) + 1);
		strcpy(pLoggerConfig->log_dir, log_dir);
		pLoggerConfig->log_dir[strlen(log_dir)] = '\0';

		pLoggerConfig->log_name = (char *)malloc(strlen(log_name) + 1);
		strcpy(pLoggerConfig->log_name, log_name);
		pLoggerConfig->log_name[strlen(log_name)] = '\0';

		g_pLoggerConfig = pLoggerConfig;

		//pNetHandler->SetLoggerConfig(pLoggerConfig);
	}
	else
	{
		g_pLoggerConfig = NULL;
	}

	return pNetHandler;
}

void uninit_context(CNetHandler *pNetHandler)
{
	pNetHandler->DestoryReactor();

	NetFuncEntry *pFuncEntry = pNetHandler->GetNetHandlerCallBack();
	delete pFuncEntry;
	delete pNetHandler;

	if(g_pLoggerConfig != NULL)
	{
		free(g_pLoggerConfig->log_dir);
		free(g_pLoggerConfig->log_name);
		free(g_pLoggerConfig);

		g_pLoggerConfig = NULL;
	}
}

void net_run(CNetHandler* pNetHandler)
{
	//while(true)
	{
		int32_t nIdle = pNetHandler->Run();
		if(nIdle == 0)
		{
			Delay(10 * US_PER_MS);
		}
	}
}

CAcceptor *create_acceptor(CNetHandler* pNetHandler)
{
	return new CAcceptor(pNetHandler, new CNetPacketParserFactory(pNetHandler->GetNetHandlerCallBack()->func_net_parser),
		new CNetMessageHandler());
}

void destory_acceptor(CAcceptor *pAcceptor)
{
	delete pAcceptor->GetPacketParserFactory();
	delete pAcceptor->GetIOHandler();
	delete pAcceptor;
}

CConnector *create_connector(CNetHandler* pNetHandler)
{
	return new CConnector(pNetHandler, new CNetPacketParserFactory(pNetHandler->GetNetHandlerCallBack()->func_net_parser),
		new CNetMessageHandler());
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

int32_t net_accepted(IIOSession *pIoSession, callback_net_accepted func_net_accepted)
{
	return func_net_accepted(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
}

int32_t net_connect(CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort)
{
	return pConnector->Connect(pPeerAddress, nPeerPort);
}

int32_t net_connect_completed(IIOSession *pIoSession, callback_net_connected func_net_connected)
{
	return func_net_connected(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
}

int32_t net_read_completed(IIOSession *pIoSession, callback_net_read func_net_read, uint8_t *pData, int32_t nBytes)
{
	return func_net_read(pIoSession->GetSessionID(), pData, nBytes);
}

int32_t net_write(CNetHandler *pNetHandler, SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes)
{
	CConnMgt &conn_mgt = pNetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		WRITE_DEBUG_LOG(g_pLoggerConfig, "it's not found session connection!{sessionid=%u}\n", nSessionID);
		return -1;
	}

	return conn->Write(arrBuf, nBytes);
}

int32_t net_write_completed(IIOSession *pIoSession, callback_net_writen func_net_writen, uint8_t *pData, int32_t nBytes)
{
	return func_net_writen(pIoSession->GetSessionID(), pData, nBytes);
}

int32_t net_close(CNetHandler *pNetHandler, SessionID nSessionID)
{
	CConnMgt &conn_mgt = pNetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		return -1;
	}

	conn->Close();
	return 0;
}

int32_t net_close_completed(IIOSession *pIoSession, callback_net_closed func_net_closed)
{
	return func_net_closed(pIoSession->GetSessionID(), pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
}

int64_t get_last_recv_time(CNetHandler *pNetHandler, SessionID nSessionID)
{
	CConnMgt &conn_mgt = pNetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		return 0;
	}

	return conn->GetLastRecvTime();
}

int64_t get_last_send_time(CNetHandler *pNetHandler, SessionID nSessionID)
{
	CConnMgt &conn_mgt = pNetHandler->GetConnMgt();
	CConnection *conn = conn_mgt.GetConnection(nSessionID);
	if(conn == NULL)
	{
		return 0;
	}

	return conn->GetLastSendTime();
}

