#include "net_typedef.h"
#include "net_handler.h"
#include "net_connector.h"
#include "net_acceptor.h"
#include "../common/common_export.h"

using namespace NETEVENT;

EXPORT CallBackSet g_CallBackSet;
static CNetHandler *g_NetHandler;

extern "C"
{

EXPORT void init_context(callback_net_accepted func_net_accepted, callback_net_connected func_net_connected,
						  callback_net_read func_net_read, callback_net_writen func_net_writen,
						  callback_net_closed func_net_closed);

EXPORT void net_run();

EXPORT CAcceptor *create_acceptor();

EXPORT void destory_acceptor(CAcceptor *pAcceptor);

EXPORT CConnector *create_connector();

EXPORT void destory_connector(CConnector *pConnector);

EXPORT int32_t net_bind(CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

EXPORT int32_t net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

EXPORT int32_t net_connect(CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

EXPORT int32_t net_connect_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

EXPORT int32_t net_read_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

EXPORT int32_t net_write(SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

EXPORT int32_t net_write_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

EXPORT int32_t net_close(SessionID nSessionID);

EXPORT int32_t net_close_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

}
