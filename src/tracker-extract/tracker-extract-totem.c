/* Tracker - audio/video metadata extraction that will call Totem
 * Copyright (C) 2006, Edward Duffy (eduffy@gmail.com)
 *
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include <glib.h>


static gchar *tags[][2] = {
	{ "TOTEM_INFO_VIDEO_HEIGHT",		"Video.Height"		},
	{ "TOTEM_INFO_VIDEO_WIDTH",		"Video.Width"		},
	{ "TOTEM_INFO_FPS",			"Video.FrameRate"	},
	{ "TOTEM_INFO_VIDEO_CODEC",		"Video.Codec"		},
	{ "TOTEM_INFO_VIDEO_BITRATE",		"Video.Bitrate"		},
	{ "TOTEM_INFO_TITLE",			"Video.Title"		},
	{ "TOTEM_INFO_AUTHOR",			"Video.Author"		},
	// { "TOTEM_INFO_YEAR", ...
	// { "TOTEM_INFO_ALBUM", ...
	// { "TOTEM_INFO_DURATION", ...
	// { "TOTEM_INFO_TRACK_NUMBER", ...
	{ "TOTEM_INFO_AUDIO_BITRATE",		"Audio.Bitrate"		},
	{ "TOTEM_INFO_AUDIO_SAMPLE_RATE",	"Audio.Samplerate"	},
	{ "TOTEM_INFO_AUDIO_CODEC",		"Audio.Codec"		},
	{ "TOTEM_INFO_AUDIO_CHANNELS",		"Audio.Channels"	},
	{ NULL,					NULL			}
};


void
tracker_extract_totem (gchar *filename, GHashTable *metadata)
{
	gchar *argv[3];
	gchar *totem;

	argv[0] = g_strdup ("totem-video-indexer");
	argv[1] = g_strdup (filename);
	argv[2] = NULL;

	if(g_spawn_sync (NULL,
			 argv,
			 NULL,
			 G_SPAWN_SEARCH_PATH | G_SPAWN_STDERR_TO_DEV_NULL,
			 NULL,
			 NULL,
			 &totem,
			 NULL,
			 NULL,
			 NULL)) {

		gchar **lines, **line;

		lines = g_strsplit (totem, "\n", -1);

		for (line = lines; *line; ++line) {
			gint i;

			for (i = 0; tags[i][0]; i++) {
				if (g_str_has_prefix (*line, tags[i][0])) {
					g_hash_table_insert (metadata,
							     g_strdup (tags[i][1]),
							     g_strdup ((*line) + strlen (tags[i][0]) + 1));
					break;
				}
			}
		}
	}
}
