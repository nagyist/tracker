/*
 * Copyright (C) 2011, Carlos Garnacho <carlos@lanedo.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef __TRACKER_PRIORITY_QUEUE_H__
#define __TRACKER_PRIORITY_QUEUE_H__

#include <glib.h>

G_BEGIN_DECLS

/* Some defines for priorities */
#define TRACKER_QUEUE_PRIORITY_HIGH    -100
#define TRACKER_QUEUE_PRIORITY_DEFAULT 0
#define TRACKER_QUEUE_PRIORITY_LOW     100

typedef struct _TrackerPriorityQueue TrackerPriorityQueue;

TrackerPriorityQueue *tracker_priority_queue_new   (void);

TrackerPriorityQueue *tracker_priority_queue_ref   (TrackerPriorityQueue *queue);
void                  tracker_priority_queue_unref (TrackerPriorityQueue *queue);

gboolean tracker_priority_queue_is_empty           (TrackerPriorityQueue *queue);

void     tracker_priority_queue_add     (TrackerPriorityQueue *queue,
                                         gpointer              data,
                                         gint                  priority);

void     tracker_priority_queue_foreach (TrackerPriorityQueue *queue,
                                         GFunc                 func,
                                         gpointer              user_data);

gboolean tracker_priority_queue_foreach_remove (TrackerPriorityQueue *queue,
                                                GEqualFunc            compare_func,
                                                gpointer              compare_user_data,
                                                GDestroyNotify        destroy_notify);

gpointer tracker_priority_queue_find           (TrackerPriorityQueue *queue,
                                                gint                 *priority_out,
                                                GEqualFunc            compare_func,
                                                gpointer              data);

gpointer tracker_priority_queue_peek    (TrackerPriorityQueue *queue,
                                         gint                 *priority_out);
gpointer tracker_priority_queue_pop     (TrackerPriorityQueue *queue,
                                         gint                 *priority_out);


G_END_DECLS

#endif /* __TRACKER_PRIORITY_QUEUE_H__ */