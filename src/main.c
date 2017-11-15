/** @file
 *  @brief fboverlayset main function
 *  @author James Covey-Crump
 *  @copyright 2016 Radiodetection Limited.  Licensed under the MIT license.
 */

#include <glib.h>
#include "stdlib.h"
#include "stdio.h"
#include "config.h"
#include "fbaccess.h"

static gboolean verbose = FALSE;

/**
 * Main Function
 * @param  argc Number of arguments
 * @param  argv Array of argument strings
 * @return      EXIT_SUCCESS (0) if successful
 */
int main (int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    gboolean version = FALSE;
    gboolean dry_run = FALSE;

    gchar *fbdev_name = NULL;
    gint gbl_alpha = -1;
    gboolean pixel_level = FALSE;
    gint64 colour_key_param = -1;
    guint32 colour_key;

    GOptionEntry entries[] = {
        { "version", 0,   0, G_OPTION_ARG_NONE, &version, "output version", NULL },
        { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "be verbose", NULL },
        { "dry-run", 0, 0, G_OPTION_ARG_NONE, &dry_run, "don't execute", NULL },
        { "fb", 'f', 0, G_OPTION_ARG_FILENAME, &fbdev_name, "framebuffer (mandatory)", "FRAMEBUFFER" },
        { "gbl-alpha", 'g', 0, G_OPTION_ARG_INT, &gbl_alpha, "global alpha (0 - 255)", "ALPHA" },
        { "colour-key", 'c', 0, G_OPTION_ARG_INT64, &colour_key_param, "colour key (can use hex)", "COLOURKEY" },
        { "loc-alpha", 'l', 0, G_OPTION_ARG_NONE, &pixel_level, "pixel level alpha" },
        { NULL }
    };

    context = g_option_context_new (" - set the overlay characteristics of a framebuffer");
    g_option_context_add_main_entries (context, entries, NULL);
    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("option parsing failed: %s\n", error->message);
        g_error_free (error);
        exit (EXIT_FAILURE);
    }

    /* down convert colour_key_param (G_OPTION_ARG_INT64 allows hex expression) */
    colour_key = (guint32) colour_key_param;

    if (version) {
        g_print (" %s\n", PACKAGE_STRING);
        exit (EXIT_SUCCESS);
    }

    if (fbdev_name == NULL) {
        g_print ("please provide framebuffer (-fb option)\n");
        exit (EXIT_FAILURE);
    }

    /* Validation */
    if (verbose) {
        g_print ("Framebuffer: %s\n", fbdev_name);

        if (pixel_level) {
            g_print ("local (pixel level) alpha applied to this and underlying framebuffer\n");
            if (gbl_alpha >= 0) {
                g_print ("!WARNING! global alpha level parameter ignored\n");
            }
        } else if (gbl_alpha >= 0) {
            if (gbl_alpha > 255) {
                g_print ("!ERROR! global alpha level must be in the range of 0-255\n");
                exit (EXIT_FAILURE);
            }
            g_print ("global alpha %d (%d%% transparent) applied to this and underlying framebuffer\n",
                     gbl_alpha, (255 - gbl_alpha) * 100 / 255);
        } else {
            g_print ("disable alpha overlay\n");
        }

        if (colour_key_param >= 0) {
            g_print ("colour key 0x%08x to make transparent\n", colour_key);
        }

    }

    /* Execute */
    if (!dry_run) {
        int fbdev_fd = open_fb (fbdev_name);
        if (fbdev_fd > 0) {
            int depth = get_colour_depth (fbdev_fd);

            if (pixel_level) {
                /* pixel level overlay */
                set_fb_pixelbypixel_overlay (fbdev_fd, TRUE);
            } else if (gbl_alpha >= 0) {
                /* global merge overlay */
                set_fb_global_overlay (fbdev_fd, TRUE, gbl_alpha);
            } else {
                /* no overlay */
                set_fb_global_overlay (fbdev_fd, TRUE, 0);
            }

            /* colour keying */
            set_fb_set_colour_key (fbdev_fd, colour_key_param >= 0, colour_key);
            if (colour_key_param >= 0 && depth != 32 && verbose) {
                g_print ("!WARNING! Colour depth is not 32bit (ARGB)\n");
            }

            close_fb (fbdev_fd);
        }
    }

    return (EXIT_SUCCESS);
}
