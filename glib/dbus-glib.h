/* -*- mode: C; c-file-style: "gnu" -*- */
/* dbus-glib.h GLib integration
 *
 * Copyright (C) 2002, 2003  CodeFactory AB
 * Copyright (C) 2003 Red Hat, Inc.
 *
 * Licensed under the Academic Free License version 1.2
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef DBUS_GLIB_H
#define DBUS_GLIB_H

#include <dbus/dbus.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define DBUS_INSIDE_DBUS_GLIB_H 1

GQuark dbus_g_error_quark (void);
#define DBUS_GERROR dbus_g_error_quark ()

typedef enum
{
  /* FIXME map all the DBUS_ERROR to DBUS_GERROR, should
   * probably be automated in some way, perhaps
   * via lame perl script
   */
  DBUS_GERROR_FAILED
} DBusGError;

void dbus_set_g_error (GError   **gerror,
                       DBusError *derror);

void dbus_g_thread_init                (void);
void dbus_connection_setup_with_g_main (DBusConnection *connection,
					GMainContext   *context);
void dbus_server_setup_with_g_main     (DBusServer     *server,
					GMainContext   *context);

typedef struct DBusGObjectInfo DBusGObjectInfo;
typedef struct DBusGMethodInfo DBusGMethodInfo;

/**
 * Object typically generated by dbus-glib-tool that
 * stores a mapping from introspection data to a
 * function pointer for a C method to be invoked.
 */
struct DBusGMethodInfo
{
  GCallback                 function;    /**< C method to invoke */
  DBusHandleMessageFunction marshaller;  /**< Marshaller to go DBusMessage to C method */
  int                       data_offset; /**< Offset into the introspection data */
};

/**
 * Introspection data for a GObject, normally autogenerated by
 * a tool such as dbus-glib-tool.
 */
struct DBusGObjectInfo
{
  const DBusGMethodInfo *infos; /**< Array of method pointers */
  const unsigned char *data;    /**< Introspection data */
  void *dbus_internal_padding1; /**< Reserved for expansion */
  void *dbus_internal_padding2; /**< Reserved for expansion */
};

void dbus_g_object_class_install_info  (GObjectClass          *object_class,
                                        const DBusGObjectInfo *info);
void dbus_connection_register_g_object (DBusConnection        *connection,
                                        const char            *at_path,
                                        GObject               *object);


typedef struct DBusGProxy       DBusGProxy;

DBusGProxy*      dbus_gproxy_new_for_service       (DBusConnection   *connection,
                                                    const char       *service_name,
                                                    const char       *path_name,
                                                    const char       *interface_name);
DBusGProxy*      dbus_gproxy_new_for_service_owner (DBusConnection   *connection,
                                                    const char       *service_name,
                                                    const char       *path_name,
                                                    const char       *interface_name,
                                                    GError          **error);
DBusGProxy*      dbus_gproxy_new_for_peer          (DBusConnection   *connection,
                                                    const char       *path_name,
                                                    const char       *interface_name);

void             dbus_gproxy_ref                   (DBusGProxy          *proxy);
void             dbus_gproxy_unref                 (DBusGProxy          *proxy);
gboolean         dbus_gproxy_connect_signal        (DBusGProxy          *proxy,
                                                    const char          *signal_name,
                                                    GCallback            callback,
                                                    void                *data,
                                                    GFreeFunc            free_data_func,
                                                    GError             **error);
DBusPendingCall* dbus_gproxy_begin_call            (DBusGProxy          *proxy,
                                                    const char          *method,
                                                    int                  first_arg_type,
                                                    ...);
gboolean         dbus_gproxy_end_call              (DBusGProxy          *proxy,
                                                    DBusPendingCall     *pending,
                                                    GError             **error,
                                                    int                  first_arg_type,
                                                    ...);
void             dbus_gproxy_oneway_call           (DBusGProxy          *proxy,
                                                    const char          *method,
                                                    int                  first_arg_type,
                                                    ...);
void             dbus_gproxy_send                  (DBusGProxy          *proxy,
                                                    DBusMessage         *message,
                                                    dbus_uint32_t       *client_serial);

#undef DBUS_INSIDE_DBUS_GLIB_H

G_END_DECLS

#endif /* DBUS_GLIB_H */



