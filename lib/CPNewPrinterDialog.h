#ifndef CPNEWPRINTERDIALOG_H
#define CPNEWPRINTERDIALOG_H

#include <stdio.h> /*fprintf*/
#include <glib.h> 
#include <gio/gio.h>
#include <string.h> /*strcmp*/
#include <cups/cups.h> /*http_t*/
#include <stdbool.h> /*bool*/
#include "asyncconn.h" /*Async_Connection*/
#include "ppdcache.h" /*fetch_ppd*/
#include "asyncipp.h" /*getURI*/
#include "newprinter.h" /*init*/
#include "killtimer.h" 
#include "CPNewPrinterDialog.h"

typedef struct _CPNewPrinterDialog_data
{
    guint mxid;
    const gchar *mname;
    const gchar *mdevice_id;
}CPNewPrinterDialog_data;

extern CPNewPrinterDialog_data ref_data;

/* Public Functions */

void NewPrinterFromDevice(unsigned int xid, 
                          const char *device_uri,
                          const char *device_id);

void DownloadDriverForDeviceID(unsigned int xid, 
                               const char *device_id);

void ChangePPD(unsigned int xid, 
               const char *name,
               const char *device_id);

#endif

