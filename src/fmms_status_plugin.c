#include <hildon/hildon.h>
#include <glib.h>
#include <dbus/dbus-glib.h>
#include <locale.h>

#include "fmms_status_plugin.h"

#define FMMS_STATUS_PLUGIN_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (obj,   \
                            TYPE_FMMS_STATUS_PLUGIN, FMMSStatusPluginPrivate))

struct _FMMSStatusPluginPrivate
{
    GtkWidget *label;
    gpointer data;
};

HD_DEFINE_PLUGIN_MODULE (FMMSStatusPlugin, fmms_status_plugin,   HD_TYPE_STATUS_MENU_ITEM);

static void
fmms_status_plugin_class_finalize (FMMSStatusPluginClass *class) {}

static void
fmms_status_plugin_class_init (FMMSStatusPluginClass *class)
{
    g_type_class_add_private (class, sizeof (FMMSStatusPluginPrivate));
}

static DBusHandlerResult signal_filter 
      (DBusConnection *connection, DBusMessage *message, void *user_data);


GtkIconTheme *icon_theme;
GtkWidget *button;
FMMSStatusPlugin *plugin;

static void
fmms_status_plugin_init (FMMSStatusPlugin *aplugin)
{
	GMainLoop *loop;
	DBusConnection *bus;
	DBusError error;
	plugin = aplugin;
	loop = g_main_loop_new (NULL, FALSE);

	dbus_error_init (&error);
	bus = hd_status_plugin_item_get_dbus_connection(HD_STATUS_PLUGIN_ITEM (plugin),
				DBUS_BUS_SESSION, &error);
	if (!bus) {
		g_warning ("Failed to connect to the D-BUS daemon: %s", error.message);
		dbus_error_free (&error);
		return;
	}
	dbus_connection_setup_with_g_main (bus, NULL);

	plugin->priv = FMMS_STATUS_PLUGIN_GET_PRIVATE (plugin);
	
	icon_theme = gtk_icon_theme_get_default ();

	/* listening to messages from all objects as no path is specified */
	static struct DBusObjectPathVTable dbus_vtable = {
	    NULL, signal_filter
	};
	
	dbus_bus_add_match (bus, "type='signal',interface='se.frals.fmms.statusmenu',path='/se/frals/fmms/statusmenu'", NULL);
	//dbus_connection_add_filter (bus, signal_filter, loop, NULL);
	if (dbus_connection_register_fallback(bus, "/se/frals/fmms/statusmenu", &dbus_vtable, NULL) == FALSE) {
		g_debug("Failed to register object on DBUS\n\n\n");
	}
	/*if(dbus_bus_request_name(bus, "se.frals.fmms.statusmenu", DBUS_NAME_FLAG_REPLACE_EXISTING, 
                           &error) != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		g_debug("Unable to acquire DBus service: '%s'. \n", error.message); 
	 }*/
	//g_main_loop_run (loop);
}

static void update_icon(int status) {
	if (status > 0) {
		
		GList *list = gtk_icon_theme_list_icons (icon_theme, NULL);
		GdkPixbuf *pixbuf = gtk_icon_theme_load_icon (icon_theme, "general_packetdata",
		    FMMS_STATUS_PLUGIN_ICON_SIZE, GTK_ICON_LOOKUP_NO_SVG, NULL);
		hd_status_plugin_item_set_status_area_icon (HD_STATUS_PLUGIN_ITEM (plugin), pixbuf);
		g_object_unref (pixbuf);
		gchar* str;
		if (status == 1) {
			str = dgettext("modest","mcen_li_outbox_sending");
		} else {
			str = g_strdup_printf(dgettext("hildon-application-manager","ai_nw_downloading"), "MMS");
		}
		button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT,
							1,
							"fMMS", str);
		
		GtkWidget *img = gtk_image_new_from_icon_name("fmms", GTK_ICON_SIZE_BUTTON);
		hildon_button_set_image (HILDON_BUTTON (button), img);
		hildon_button_set_image_position (HILDON_BUTTON (button), GTK_POS_LEFT);
		hildon_button_set_alignment(HILDON_BUTTON (button), 0.0, 0.5, 1, 1);	
		gtk_widget_show_all (button);

		plugin->priv->label = button;
		gtk_container_add (GTK_CONTAINER (plugin), plugin->priv->label);
		gtk_widget_show_all (plugin->priv->label);
		gtk_widget_show (GTK_WIDGET (plugin));
	} else {
		hd_status_plugin_item_set_status_area_icon (HD_STATUS_PLUGIN_ITEM (plugin), NULL);
		gtk_widget_destroy(GTK_WIDGET (plugin->priv->label));
	}
}

static DBusHandlerResult
signal_filter (DBusConnection *connection, DBusMessage *message, void *user_data)
{
	/* User data is the event loop we are running in */
	GMainLoop *loop = user_data;

	/*A signal on the se.frals.fmms.statusmenu interface */
	if (dbus_message_is_signal (message, "se.frals.fmms.statusmenu", "Send")) {
		update_icon(1);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else if (dbus_message_is_signal (message, "se.frals.fmms.statusmenu", "Download")) {
		update_icon(2);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else if (dbus_message_is_signal (message, "se.frals.fmms.statusmenu", "Disconnect")) {
		update_icon(0);
		return DBUS_HANDLER_RESULT_HANDLED;
	}
	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

