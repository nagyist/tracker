/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2006, Mr Jamie McCracken (jamiemcc@gnome.org)
 * Copyright (C) 2008, Nokia

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef __TRACKER_INDEXER_H__
#define __TRACKER_INDEXER_H__

#include <glib-object.h>
#include <dbus/dbus-glib.h>

#define TRACKER_DAEMON_SERVICE	     "org.freedesktop.Tracker"
#define TRACKER_INDEXER_SERVICE      "org.freedesktop.Tracker.Indexer"
#define TRACKER_INDEXER_PATH	     "/org/freedesktop/Tracker/Indexer"
#define TRACKER_INDEXER_INTERFACE    "org.freedesktop.Tracker.Indexer"

/* Transaction every 'x' items */
#define TRACKER_INDEXER_TRANSACTION_MAX	4000

G_BEGIN_DECLS

#define TRACKER_TYPE_INDEXER	     (tracker_indexer_get_type())
#define TRACKER_INDEXER(o)	     (G_TYPE_CHECK_INSTANCE_CAST ((o), TRACKER_TYPE_INDEXER, TrackerIndexer))
#define TRACKER_INDEXER_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c),    TRACKER_TYPE_INDEXER, TrackerIndexerClass))
#define TRACKER_IS_INDEXER(o)	     (G_TYPE_CHECK_INSTANCE_TYPE ((o), TRACKER_TYPE_INDEXER))
#define TRACKER_IS_INDEXER_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c),    TRACKER_TYPE_INDEXER))
#define TRACKER_INDEXER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o),  TRACKER_TYPE_INDEXER, TrackerIndexerClass))

typedef struct TrackerIndexer	     TrackerIndexer;
typedef struct TrackerIndexerClass   TrackerIndexerClass;
typedef struct TrackerIndexerPrivate TrackerIndexerPrivate;

struct TrackerIndexer {
	GObject parent_instance;

	/* Private struct */
	TrackerIndexerPrivate *private;
};

struct TrackerIndexerClass {
	GObjectClass parent_class;

	void (*status)		(TrackerIndexer *indexer,
				 gdouble	 seconds_elapsed,
				 const gchar	*current_module_name,
				 guint           items_processed,
				 guint		 items_indexed,
				 guint		 items_remaining);
	void (*started)		(TrackerIndexer *indexer);
	void (*paused)		(TrackerIndexer *indexer);
	void (*continued)	(TrackerIndexer *indexer);
	void (*finished)	(TrackerIndexer *indexer,
				 gdouble	 seconds_elapsed,
				 guint           items_processed,
				 guint		 items_indexed);
	void (*module_started)	(TrackerIndexer *indexer,
				 const gchar	*module_name);
	void (*module_finished) (TrackerIndexer *indexer,
				 const gchar	*module_name);
	void (*indexing_error)  (TrackerIndexer *indexer,
				 const gchar    *reason,
				 gboolean        requires_reindex);
	void (*event_happened)	(TrackerIndexer *indexer,
				 GPtrArray      *events);
};

GType		tracker_indexer_get_type	    (void) G_GNUC_CONST;

TrackerIndexer *tracker_indexer_new                 (void);
gboolean        tracker_indexer_get_running         (TrackerIndexer         *indexer);
void            tracker_indexer_set_running         (TrackerIndexer         *indexer,
						     gboolean                running);
gboolean        tracker_indexer_get_stoppable       (TrackerIndexer         *indexer);
void            tracker_indexer_stop                (TrackerIndexer         *indexer);
void            tracker_indexer_process_all         (TrackerIndexer         *indexer);
void            tracker_indexer_process_modules     (TrackerIndexer         *indexer,
						     gchar                 **modules);
void		tracker_indexer_transaction_commit  (TrackerIndexer         *indexer);
void		tracker_indexer_transaction_open    (TrackerIndexer         *indexer);
void		tracker_indexer_make_event_happen   (TrackerIndexer         *indexer,
						     GPtrArray              *events);

/* DBus methods */
void            tracker_indexer_pause               (TrackerIndexer         *indexer,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_pause_for_duration  (TrackerIndexer         *indexer,
						     guint                   seconds,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_continue            (TrackerIndexer         *indexer,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void		tracker_indexer_turtle_add	   (TrackerIndexer *indexer,
						    const gchar             *file,
						    DBusGMethodInvocation *context,
						    GError **error);
void            tracker_indexer_files_check         (TrackerIndexer         *indexer,
						     const gchar            *module,
						     GStrv                   files,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_files_update        (TrackerIndexer         *indexer,
						     const gchar            *module,
						     GStrv                   files,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_files_delete        (TrackerIndexer         *indexer,
						     const gchar            *module,
						     GStrv                   files,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_file_move           (TrackerIndexer         *indexer,
						     const gchar            *module_name,
						     gchar                  *from,
						     gchar                  *to,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_volume_disable_all  (TrackerIndexer         *indexer,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_volume_update_state (TrackerIndexer         *indexer,
						     const gchar            *volume_uuid,
						     const gchar            *path,
						     gboolean                enabled,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_insert_statement    (TrackerIndexer         *indexer,
						     const gchar            *subject,
						     const gchar            *predicate,
						     const gchar            *object,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_delete_statement    (TrackerIndexer         *indexer,
						     const gchar            *subject,
						     const gchar            *predicate,
						     const gchar            *object,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void		tracker_indexer_sparql_update	    (TrackerIndexer         *indexer,
						     const gchar	    *update,
						     DBusGMethodInvocation  *context,
						     GError		   **error);
void            tracker_indexer_restore_backup      (TrackerIndexer         *indexer,
						     const gchar            *backup_file,
						     DBusGMethodInvocation  *context,
						     GError                **error);
void            tracker_indexer_shutdown            (TrackerIndexer         *indexer,
						     DBusGMethodInvocation  *context,
						     GError                **error);

GStrv           tracker_indexer_get_notifiable_classes  (void);

G_END_DECLS

#endif /* __TRACKER_INDEXER_H__ */
