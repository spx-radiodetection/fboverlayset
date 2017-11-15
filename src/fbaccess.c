/** @file
 *  @brief Functions for accessing the imx6 framebuffer overlay IOCTL.
 *  @author James Covey-Crump
 *  @copyright 2016 Radiodetection Limited.  Licensed under the MIT license.
 */

#include <glib.h>
#include "stdlib.h"
#include "stdio.h"
#include "config.h"
#include <linux/fb.h>
#include <linux/mxcfb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


/**
 * @brief  Open a framebuffer device
 * @param  fbdev_name name of device (e.g. /dev/fb0)
 * @return the file descriptor id for the opened fb device
 */
int open_fb (gchar *fbdev_name)
{
    int fbdev_fd = open (fbdev_name, O_RDWR, 0);
    if (fbdev_fd < 0) {
        g_print ("unable to open %s\n", fbdev_name);
    }
    return fbdev_fd;
}


/**
 * @brief  Open a framebuffer device
 * @param  fbdev_fd the file descriptor id for the opened fb device
 * @return the colour depth in bits
 */
int get_colour_depth (int fbdev_fd)
{
    struct fb_var_screeninfo variable_info;
    int err = ioctl (fbdev_fd, FBIOGET_VSCREENINFO, &variable_info);
    if (err) {
        perror ("FBIOGET_VSCREENINFO");
        return 0;
    }
    return (int)variable_info.bits_per_pixel;
}


/**
 * @brief  Set the global overlay (framebuffer merging)
 * @param  fbdev_fd the file descriptor id for the opened fb device
 * @param  enable true if to enable overlay
 * @param  alpha alpha level (0 transparent to 255 opaque)
 * @return errno
 */
int set_fb_global_overlay (int fbdev_fd, gboolean enable, gint alpha)
{
    int err;
    struct mxcfb_gbl_alpha a = {0};

    if (enable) {
        a.enable = 1;
        a.alpha = alpha;
    }

    err = ioctl (fbdev_fd, MXCFB_SET_GBL_ALPHA, &a);
    if (err) {
        perror ("MXCFB_SET_GBL_ALPHA");
    }

    return err;
}

/**
 * @brief  Set the local overlay (pixel level overlay)
 * @param  fbdev_fd the file descriptor id for the opened fb device
 * @param  enable true if to enable overlay
 * @return errno
 */
int set_fb_pixelbypixel_overlay (int fbdev_fd, gboolean enable)
{
    int err;

    struct mxcfb_loc_alpha a = {0};

    if (enable) {
        a.enable = 1;
        a.alpha_in_pixel = 1;
    }

    err = ioctl (fbdev_fd, MXCFB_SET_LOC_ALPHA, &a);
    if (err) {
        perror ("MXCFB_SET_GBL_ALPHA");
    }

    return err;
}


/**
 * @brief  Set the colour key (transparent colour)
 * @param  fbdev_fd the file descriptor id for the opened fb device
 * @param  enable true if to enable colour keying
 * @param  ck colour key value
 * @return errno
 */
int set_fb_set_colour_key (int fbdev_fd, gboolean enable, guint32 ck)
{
    int err;

    struct mxcfb_color_key c = {0};

    if (enable) {
        c.enable = 1;
        c.color_key = ck;
    }

    err = ioctl (fbdev_fd, MXCFB_SET_CLR_KEY, &c);
    if (err) {
        perror ("MXCFB_SET_CLR_KEY");
    }

    return err;
}


/**
 * @brief  Close a device
 * @param  fbdev_fd the file descriptor id for the opened fb device
 * @return errno
 */
void close_fb (int fbdev_fd)
{
    close (fbdev_fd);
}

