#ifndef __FMMS_STATUS_PLUGIN_H__
#define __FMMS_STATUS_PLUGIN_H__

#include <libhildondesktop/libhildondesktop.h>

G_BEGIN_DECLS

#define TYPE_FMMS_STATUS_PLUGIN            (fmms_status_plugin_get_type ())

#define FMMS_STATUS_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                    TYPE_FMMS_STATUS_PLUGIN, FMMSStatusPlugin))
 
#define FMMS_STATUS_PLUGIN_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST ((class), \
                                TYPE_FMMS_STATUS_PLUGIN, FMMSStatusPluginClass))

#define IS_FMMS_STATUS_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                                    TYPE_FMMS_STATUS_PLUGIN))

#define IS_FMMS_STATUS_PLUGIN_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), \
                                                    TYPE_FMMS_STATUS_PLUGIN))

#define FMMS_STATUS_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                            TYPE_FMMS_STATUS_PLUGIN, FMMSStatusPluginClass))

#define FMMS_STATUS_PLUGIN_ICON_SIZE 22

typedef struct _FMMSStatusPlugin        FMMSStatusPlugin;
typedef struct _FMMSStatusPluginClass   FMMSStatusPluginClass;
typedef struct _FMMSStatusPluginPrivate FMMSStatusPluginPrivate; 

struct _FMMSStatusPlugin
{
    HDStatusMenuItem parent;

    FMMSStatusPluginPrivate *priv;
};

struct _FMMSStatusPluginClass
{
    HDStatusMenuItemClass parent;
};

GType fmms_status_plugin_get_type (void);

G_END_DECLS

#endif

