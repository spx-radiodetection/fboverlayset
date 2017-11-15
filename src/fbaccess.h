/** @file
 *  @brief Functions for accessing the imx6 framebuffer overlay IOCTL.
 *  @author James Covey-Crump
 *  @copyright 2016 Radiodetection Limited.  Licensed under the MIT license.
 */

#ifndef _FB_ACCESS_H
#define _FB_ACCESS_H

#include <glib.h>

int open_fb (gchar *fbdev_name);
int get_colour_depth (int fbdev_fd);
int set_fb_global_overlay (int fbdev_fd, gboolean enable, gint alpha);
int set_fb_pixelbypixel_overlay (int fbdev_fd, gboolean enable);
int set_fb_set_colour_key (int fbdev_fd, gboolean enable, guint32 ck);
void close_fb (int fbdev_fd);

#endif
