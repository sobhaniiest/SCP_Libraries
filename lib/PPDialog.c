#include "PPDialog.h"

static int activate_PPDialog(const char *name);
static int activate_error(const char *name);

static int activate_PPDialog(const char *name)
{
    GtkApplication *app;
    int status;
    char buffer[1024];
    snprintf(buffer, 1024, "\tPrinter Properties - '%s' on localhost\t\n\n\tGoto 'system-config-printer' and Click on '%s' and select 'Print Test Page'\t\n",name, name);
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), (gchar *)buffer);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);
    return status;
}

static int activate_error(const char *name)
{
    GtkApplication *app;
    int status;
    char buffer[1024];
    snprintf(buffer, 1024, "\tPrinter Properties - '%s' Printer not found on localhost\t\n",name);
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), (gchar *)buffer);
    status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);
    return status;
}

void PrintTestPage(unsigned int xid, const char *name)
{
    http_t *http = Async_Connection(NULL, 
                                    NULL, 
                                    NULL, 
                                    NULL, 
                                    0, 
                                    0, 
                                    true, 
                                    true);

    GHashTable *result = getURI(http);
    alive();
    if(g_hash_table_contains(result, name))
    {
        fprintf(stderr, "Printing test page\n");
        activate_PPDialog(g_locale_to_utf8 (name, -1, 0, 0, 0));
    }
    else
    {
        fprintf(stderr, "Printer not found\n");
        activate_error(g_locale_to_utf8 (name, -1, 0, 0, 0));
    }
    add_hold ();
}
