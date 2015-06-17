
#ifndef NET_CAPI_H_
#define NET_CAPI_H_

#include "net_typedef.h"
#include "net_handler.h"
#include "net_connector.h"
#include "net_acceptor.h"
#include "net_typedef.h"
#include "../common/common_export.h"

using namespace NETEVENT;

extern "C"
{

EXPORT CNetHandler* init_context(callback_net_parser func_net_parser, callback_net_accepted func_net_accepted,
				callback_net_connected func_net_connected, callback_net_connect_timeout func_net_connect_timeout,
				callback_net_read func_net_read, callback_net_writen func_net_writen,
				callback_net_closed func_net_closed, callback_net_error func_net_error,
				const char *log_dir, const char *log_name);

EXPORT void uninit_context(CNetHandler *pNetHandler);

EXPORT void net_run(CNetHandler* pNetHandler);

EXPORT CAcceptor *create_acceptor(CNetHandler* pNetHandler);

EXPORT void destory_acceptor(CAcceptor *pAcceptor);

EXPORT CConnector *create_connector(CNetHandler* pNetHandler);

EXPORT void destory_connector(CConnector *pConnector);

EXPORT int32_t net_bind(CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

EXPORT int32_t net_accepted(IIOSession *pIoSession, callback_net_accepted func_net_accepted);

EXPORT int32_t net_connect(CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

EXPORT int32_t net_connect_completed(IIOSession *pIoSession, callback_net_connected func_net_connected);

EXPORT int32_t net_read_completed(IIOSession *pIoSession, callback_net_read func_net_read);

EXPORT int32_t net_write(CNetHandler *pNetHandler, SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

EXPORT int32_t net_write_completed(IIOSession *pIoSession, callback_net_writen func_net_writen, uint8_t *pData, int32_t nBytes);

EXPORT int32_t net_close(CNetHandler *pNetHandler, SessionID nSessionID);

EXPORT int32_t net_close_completed(IIOSession *pIoSession, callback_net_closed func_net_closed);

EXPORT int64_t get_last_recv_time(CNetHandler *pNetHandler, SessionID nSessionID);

EXPORT int64_t get_last_send_time(CNetHandler *pNetHandler, SessionID nSessionID);

}

#endif
