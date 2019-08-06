#include "ppdcache.h"

dict_cache *cache = NULL;
dict_modtimes *modtimes = NULL;
list *queued = NULL;
bool connecting = false;
http_t *cups = NULL;

/* Inserting cache data - printer queue name and PPD file descriptor */
static void insert_cache(dict_cache **head, const char *str, FILE *fpname);

/* Inserting cache data - printer queue name and modification time */
static void insert_modtimes(dict_modtimes **head, const char *str, time_t value);

/* Inserting cache data - printer queue name and callback functions */
static void insert_list(list **head, const char *str, void(*func)());

/* Finding the printer queue name is in cache data or not */
static dict_modtimes *find_modtimes(dict_modtimes **head, const char *str);
static bool find_cache(dict_cache **head, const char *str);

/* Return the file descriptor of the corresponding printer queue if avaliable else return NULL */
static FILE *find_file(dict_cache **head, const char *str);

/* */
static void connected();

/* Connecting to asyncconn : Asyn_Connection*/
static void self_connect(void(*callback)(),
                         const char *host,
                         int port,
                         http_encryption_t encryption);

/* If the file is older, Cache the new version */
static void got_ppd3(const char *name, http_status_t status, time_t time, char *fname, void(*callback)(), bool check_uptodate);

/* */
static void schedule_callback(void(*callback)(), const char *name, FILE *ppd);

/* Function definations */

static void insert_cache(dict_cache **head, const char *str, FILE *fpname)
{
    dict_cache *c = (*head);
  
    if(*head == NULL)
    {
        (*head) = (dict_cache *)malloc(sizeof(dict_cache));
        (*head)->qname = str;
        (*head)->fp = fpname;
        (*head)->next = NULL;
    }
    else
    {
        while(c->next != NULL)
            c = c->next;
        c->next = (dict_cache *)malloc(sizeof(dict_cache));
        c->next->qname = str;
        c->next->fp = fpname;
        c->next->next = NULL;
    }
}

static void insert_modtimes(dict_modtimes **head, const char *str, time_t value)
{
    dict_modtimes *c = (*head);
  
    if(*head == NULL)
    {
        (*head) = (dict_modtimes *)malloc(sizeof(dict_modtimes));
        (*head)->qname = str;
        (*head)->time = value;
        (*head)->next = NULL;
    }
    else
    {
        while(c->next != NULL)
            c = c->next;
        c->next = (dict_modtimes *)malloc(sizeof(dict_modtimes));
        c->next->qname = str;
        c->next->time = value;
        c->next->next = NULL;
    }
}

static void insert_list(list **head, const char *str, void(*func)())
{
    list *c = (*head);
  
    if(*head == NULL)
    {
        (*head)=(list *)malloc(sizeof(list));
        (*head)->qname = str;
        (*head)->method = func;
        (*head)->next = NULL;
    }
    else
    {
        while(c->next != NULL)
            c = c->next;
        c->next = (list *)malloc(sizeof(list));
        c->next->qname = str;
        c->next->method = func;
        c->next->next = NULL;
    }
}

static bool find_cache(dict_cache **head, const char *str)
{
    bool found = false;
    dict_cache *c = (*head);
    while(c != NULL)
    {
        if(!strcmp(c->qname, str))
        {
            found = true;
            break;
        }
        c = c->next;
    }
    return found;
}

static dict_modtimes *find_modtimes(dict_modtimes **head, const char *str)
{
    bool found = false;
    dict_modtimes *c = (*head);
    while(c != NULL)
    {
        if(!strcmp(c->qname, str))
        {
            found = true;
            break;
        }
        c = c->next;
    }
    if(found)
        return c;
    else
        return NULL;
}

static FILE *find_file(dict_cache **head, const char *str)
{
    bool found = false;
    dict_cache *c = (*head);
    while(c != NULL)
    {
        if(!strcmp(c->qname, str))
        {
            found = true;
            break;
        }
        c = c->next;
    }
    if(found)
        return c->fp;
    else
        return NULL;
}

void fetch_ppd(const char *name, 
               void(*callback)(),
               bool check_uptodate,
               const char *host,
               int port,
               http_encryption_t encryption)
{
    http_status_t http_status;
    char filename[1024] = "\0";
    FILE *f;
    dict_modtimes *reference = find_modtimes(&modtimes, name);

    if(check_uptodate && reference != NULL)
    {
        http_status = cupsGetPPD3(CUPS_HTTP_DEFAULT, reference->qname, &(reference->time), filename, sizeof(filename));
        got_ppd3(name, http_status, modtimes->time, filename, callback, check_uptodate);
        return;
    }

    if(!find_cache(&cache, name))
    {
        //KeyError
        if(!cups)
        {
            insert_list(&queued, name, callback);
            if(!connecting)
                self_connect(NULL, host, port, encryption);
            return;
        }
        time_t default_time = 0;
        http_status = cupsGetPPD3(CUPS_HTTP_DEFAULT, name, &default_time, filename, sizeof(filename));
        got_ppd3(name, http_status, default_time, filename, callback, check_uptodate);
        return;
    }
    else
    {
        //RuntimeError
        f = find_file(&cache, name);
        if(f == NULL)
        {
            schedule_callback(callback, name, NULL);
            return;
        }
        fseek(f, 0, SEEK_SET);
        schedule_callback(callback, name, f);
    }

    /* fseek(f, 0, SEEK_SET); 
    
    f.seek (0)
        with NamedTemporaryFile () as tmpf:
            copyfileobj (f, tmpf)

            try:
                ppd = cups.PPD (tmpf.file)
                self._schedule_callback (callback, name, ppd, None)
            except Exception as e:
                self._schedule_callback (callback, name, None, e)
    */
}

static void connected()
{
    connecting = false;
    list *c = queued;
    while(c != NULL)
    {
        fetch_ppd(c->qname, c->method, true, NULL, 0, 0);
        c = c->next;
    }
    queued = NULL;
}

static void self_connect(void(*callback)(),
                         const char *host,
                         int port,
                         http_encryption_t encryption)
{
    connecting = true;
    cups = Async_Connection(connected, 
                            connected, 
                            NULL, 
                            host, 
                            port, 
                            encryption,  
                            true, 
                            true);
    connected();
}

static void got_ppd3(const char *name, 
                     http_status_t status, 
                     time_t time, 
                     char *fname, 
                     void(*callback)(),
                     bool check_uptodate)
{
    if(status == HTTP_STATUS_NOT_MODIFIED)
        remove(fname);
    else if(status == HTTP_STATUS_OK)
    {
        FILE *fn;
        fn = fopen(fname,"rb");
        if(fn == NULL)
        {
            schedule_callback(callback, name, NULL);
            return;
        }
        insert_cache(&cache, name, fn);
        remove(fname);
        insert_modtimes(&modtimes, name, time);

        fetch_ppd(name, callback, false, NULL, 0, 0);
    }
    else
        schedule_callback(callback, name, NULL);
}


static void schedule_callback(void(*callback)(), const char *name, FILE *ppd)
{
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_lock(&lock);
    callback(name, ppd);
    pthread_mutex_unlock(&lock);
}
