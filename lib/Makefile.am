lib_LTLIBRARIES = libSCPService.la
libSCPService_la_SOURCES = CPNewPrinterDialog.c \
                           ppdcache.c \
                           asyncconn.c \
                           asyncpk.c \
                           asyncipp.c \
                           newprinter.c \
                           authinfocache.c \
                           nstring.c \
                           PPDialog.c \
                           MissingExecutables.c \
                           killtimer.c

libSCPService_la_CPPFLAGS  = $(GLIB_CFLAGS)
libSCPService_la_CPPFLAGS += $(GIO_CFLAGS)
libSCPService_la_CPPFLAGS += $(GIOUNIX_CFLAGS)
libSCPService_la_CPPFLAGS += $(GTK_CFLAGS)

libSCPService_la_LIBADD = -lcups -lxml2 -lz -lpthread -lm
libSCPService_la_LIBADD += $(GLIB_LIBS)
libSCPService_la_LIBADD += $(GIO_LIBS)
libSCPService_la_LIBADD += $(GIOUNIX_LIBS)
libSCPService_la_LIBADD += $(GTK_LIBS)
libSCPService_la_LIBADD += $(CUPS_LIBS)
libSCPService_la_LIBADD += $(CUPS_TEMP)

#libSCPService_la_LDFLAGS = -I/usr/include/libxml2 -L/usr/lib

##INSTALL HEADERS
scp_service_headersdir = $(includedir)/scp-services-headers
scp_service_headers_HEADERS = CPNewPrinterDialog.h \
                              ppdcache.h \
                              asyncconn.h \
                              asyncpk.h \
                              asyncipp.h \
                              newprinter.h \
                              authinfocache.h \
                              nstring.h \
                              PPDialog.h \
                              MissingExecutables.h \
                              killtimer.h

include_HEADERS = SCPService.h
