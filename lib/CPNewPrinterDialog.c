#include "CPNewPrinterDialog.h"

CPNewPrinterDialog_data ref_data;

static void change_ppd_got_ppd(const char *name, 
                               FILE *ppd);

static void change_ppd_with_dev(GHashTable *result, 
                                const char *name, 
                                FILE *ppd);

static void do_change_ppd(const char *device_uri, 
                          const char *name, 
                          FILE *ppd);

/* Public Functions */

void NewPrinterFromDevice(unsigned int xid, 
                          const char *device_uri,
                          const char *device_id)
{
    init("printer_with_uri", 
         device_uri, 
         NULL, 
         NULL, 
         device_id, 
         NULL, 
         0, 
         0);
}

void DownloadDriverForDeviceID(unsigned int xid, 
                               const char *device_id)
{
    init("download_driver", 
         NULL, 
         NULL, 
         NULL, 
         device_id, 
         NULL, 
         0, 
         0);
}

void ChangePPD(unsigned int xid, 
               const char *name,
               const char *device_id)
{
    ref_data.mxid = xid;
    ref_data.mname = name;
    ref_data.mdevice_id = device_id;
    fetch_ppd(name, 
              change_ppd_got_ppd, 
              true, 
              NULL, 
              0, 
              0);
}

/* Internal Functions */

static void change_ppd_got_ppd(const char *name, 
                               FILE *ppd)
{
    http_t *http = Async_Connection(NULL, 
                                    NULL, 
                                    NULL, 
                                    NULL, 
                                    0, 
                                    0, 
                                    true, 
                                    true);

    if(http)
        fprintf(stderr, "Connected to cups server\n");
    else
        fprintf(stderr, "Connection error\n");

    GHashTable *result = getURI(http);

    if(!result || !http)
      do_change_ppd(NULL, name, ppd);
    else
      change_ppd_with_dev(result, name, ppd);
}


static void change_ppd_with_dev(GHashTable *result, 
                                const char *name, 
                                FILE *ppd)
{
    if(g_hash_table_contains(result, name))
        do_change_ppd(g_hash_table_lookup(result, name), name, ppd);
    else
        do_change_ppd(NULL, name, ppd);
}

static void do_change_ppd(const char *device_uri, 
                          const char *name, 
                          FILE *ppd)
{  
    init("ppd",
         device_uri,
         name,
         ppd,
         ref_data.mdevice_id,
         NULL,
         0,
         ref_data.mxid);
}
