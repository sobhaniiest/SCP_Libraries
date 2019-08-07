#ifndef PPDIALOG_H
#define PPDIALOG_H

#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h> 
#include <stdbool.h>
#include <cups/cups.h>
#include "newprinter.h"
#include "asyncconn.h"
#include "killtimer.h"
#include "PPDialog.h"

void PrintTestPage(unsigned int xid, const char *name);

#endif