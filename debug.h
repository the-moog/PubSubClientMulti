#ifndef __PSCT_DEBUG_H__
#define __PSCT_DEBUG_H__


#ifndef USE_DBG_LEVEL
#define USE_DBG_LEVEL

//Higher = more debug
#ifndef PUB_SUB_CLIENT_TOOLS_DBGLVL
#define PUB_SUB_CLIENT_TOOLS_DBGLVL 0
#endif

#if (PUB_SUB_CLIENT_TOOLS_DBGLVL > 0)
 #define Serial_print(...) Serial.print(__VA_ARGS__)
#else
 #define Serial_print(...)
#endif

#if (PUB_SUB_CLIENT_TOOLS_DBGLVL > 0)
 #define Serial_println(...) Serial.println(__VA_ARGS__)
#else
 #define Serial_println(...)
#endif

#if (PUB_SUB_CLIENT_TOOLS_DBGLVL > 0)
 #define Serial_printf(...) Serial.printf(__VA_ARGS__)
#else
 #define Serial_printf(...)
#endif

#define DBGf(lvl,...) if(lvl < PUB_SUB_CLIENT_TOOLS_DBGLVL)Serial_printf(__VA_ARGS__)
#define DBGln(lvl,...) if(lvl < PUB_SUB_CLIENT_TOOLS_DBGLVL)Serial_println(__VA_ARGS__)

#endif

#endif

