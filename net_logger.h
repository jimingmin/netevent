#include "../logger/logger.h"
using namespace LOGGER;

extern LoggerConfig *g_pLoggerConfig;

#define WRITE_DEBUG_LOG(LOGGERCONFIG_PTR, FORMAT, ...)	\
	if(LOGGERCONFIG_PTR != NULL) CLogger::Debug(LOGGERCONFIG_PTR->log_name, __FILE__, __LINE__, FORMAT, ##__VA_ARGS__)
