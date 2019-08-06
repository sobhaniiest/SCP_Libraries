#ifndef PPDIALOG_H
#define PPDIALOG_H

#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h> 
#include <stdbool.h>
#include <cups/cups.h>
#include "newprinter.h"
#include "asyncconn.h"
#include "PPDialog.h"

void PrintTestPage(guint xid, const gchar *name);

#endif