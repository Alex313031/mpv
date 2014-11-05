/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <libavutil/pixdesc.h>
#include <libavutil/avutil.h>

#include "video/img_format.h"
#include "fmt-conversion.h"
#include "config.h"

static const struct {
    int fmt;
    enum AVPixelFormat pix_fmt;
} conversion_map[] = {
    {IMGFMT_ARGB, AV_PIX_FMT_ARGB},
    {IMGFMT_BGRA, AV_PIX_FMT_BGRA},
    {IMGFMT_BGR24, AV_PIX_FMT_BGR24},
    {IMGFMT_RGB565_BE, AV_PIX_FMT_RGB565BE},
    {IMGFMT_RGB565_LE, AV_PIX_FMT_RGB565LE},
    {IMGFMT_RGB555_BE, AV_PIX_FMT_RGB555BE},
    {IMGFMT_RGB555_LE, AV_PIX_FMT_RGB555LE},
    {IMGFMT_RGB444_BE, AV_PIX_FMT_RGB444BE},
    {IMGFMT_RGB444_LE, AV_PIX_FMT_RGB444LE},
    {IMGFMT_RGB8,  AV_PIX_FMT_RGB8},
    {IMGFMT_RGB4,  AV_PIX_FMT_RGB4},
    {IMGFMT_MONO,  AV_PIX_FMT_MONOBLACK},
    {IMGFMT_MONO_W,  AV_PIX_FMT_MONOWHITE},
    {IMGFMT_RGB4_BYTE,  AV_PIX_FMT_RGB4_BYTE},
    {IMGFMT_BGR4_BYTE,  AV_PIX_FMT_BGR4_BYTE},
    {IMGFMT_RGB48_LE, AV_PIX_FMT_RGB48LE},
    {IMGFMT_RGB48_BE, AV_PIX_FMT_RGB48BE},
    {IMGFMT_ABGR, AV_PIX_FMT_ABGR},
    {IMGFMT_RGBA, AV_PIX_FMT_RGBA},
    {IMGFMT_RGB24, AV_PIX_FMT_RGB24},
    {IMGFMT_BGR565_BE, AV_PIX_FMT_BGR565BE},
    {IMGFMT_BGR565_LE, AV_PIX_FMT_BGR565LE},
    {IMGFMT_BGR555_BE, AV_PIX_FMT_BGR555BE},
    {IMGFMT_BGR555_LE, AV_PIX_FMT_BGR555LE},
    {IMGFMT_BGR444_BE, AV_PIX_FMT_BGR444BE},
    {IMGFMT_BGR444_LE, AV_PIX_FMT_BGR444LE},
    {IMGFMT_BGR8,  AV_PIX_FMT_BGR8},
    {IMGFMT_BGR4,  AV_PIX_FMT_BGR4},
    {IMGFMT_PAL8,  AV_PIX_FMT_PAL8},
    {IMGFMT_GBRP,  AV_PIX_FMT_GBRP},
    {IMGFMT_YUYV,  AV_PIX_FMT_YUYV422},
    {IMGFMT_UYVY,  AV_PIX_FMT_UYVY422},
    {IMGFMT_NV12,  AV_PIX_FMT_NV12},
    {IMGFMT_NV21,  AV_PIX_FMT_NV21},
    {IMGFMT_Y8,    AV_PIX_FMT_GRAY8},
    // FFmpeg prefers AV_PIX_FMT_GRAY8A, but Libav has only Y400A
    {IMGFMT_YA8,   AV_PIX_FMT_Y400A},
    {IMGFMT_Y16_LE, AV_PIX_FMT_GRAY16LE},
    {IMGFMT_Y16_BE, AV_PIX_FMT_GRAY16BE},
    {IMGFMT_410P,  AV_PIX_FMT_YUV410P},
    {IMGFMT_420P,  AV_PIX_FMT_YUV420P},
    {IMGFMT_411P,  AV_PIX_FMT_YUV411P},
    {IMGFMT_422P,  AV_PIX_FMT_YUV422P},
    {IMGFMT_444P,  AV_PIX_FMT_YUV444P},
    {IMGFMT_440P,  AV_PIX_FMT_YUV440P},

    {IMGFMT_420P16_LE,  AV_PIX_FMT_YUV420P16LE},
    {IMGFMT_420P16_BE,  AV_PIX_FMT_YUV420P16BE},
    {IMGFMT_420P9_LE,   AV_PIX_FMT_YUV420P9LE},
    {IMGFMT_420P9_BE,   AV_PIX_FMT_YUV420P9BE},
    {IMGFMT_420P10_LE,  AV_PIX_FMT_YUV420P10LE},
    {IMGFMT_420P10_BE,  AV_PIX_FMT_YUV420P10BE},
    {IMGFMT_422P10_LE,  AV_PIX_FMT_YUV422P10LE},
    {IMGFMT_422P10_BE,  AV_PIX_FMT_YUV422P10BE},
    {IMGFMT_444P9_BE ,  AV_PIX_FMT_YUV444P9BE},
    {IMGFMT_444P9_LE ,  AV_PIX_FMT_YUV444P9LE},
    {IMGFMT_444P10_BE,  AV_PIX_FMT_YUV444P10BE},
    {IMGFMT_444P10_LE,  AV_PIX_FMT_YUV444P10LE},
    {IMGFMT_422P16_LE,  AV_PIX_FMT_YUV422P16LE},
    {IMGFMT_422P16_BE,  AV_PIX_FMT_YUV422P16BE},
    {IMGFMT_422P9_LE,   AV_PIX_FMT_YUV422P9LE},
    {IMGFMT_422P9_BE,   AV_PIX_FMT_YUV422P9BE},
    {IMGFMT_444P16_LE,  AV_PIX_FMT_YUV444P16LE},
    {IMGFMT_444P16_BE,  AV_PIX_FMT_YUV444P16BE},

    // YUVJ are YUV formats that use the full Y range. Decoder color range
    // information is used instead. Deprecated in ffmpeg.
    {IMGFMT_420P,  AV_PIX_FMT_YUVJ420P},
    {IMGFMT_422P,  AV_PIX_FMT_YUVJ422P},
    {IMGFMT_444P,  AV_PIX_FMT_YUVJ444P},
    {IMGFMT_440P,  AV_PIX_FMT_YUVJ440P},

    {IMGFMT_420AP, AV_PIX_FMT_YUVA420P},
    {IMGFMT_422AP, AV_PIX_FMT_YUVA422P},
    {IMGFMT_444AP, AV_PIX_FMT_YUVA444P},

    {IMGFMT_XYZ12_LE,   AV_PIX_FMT_XYZ12LE},
    {IMGFMT_XYZ12_BE,   AV_PIX_FMT_XYZ12BE},

    // ffmpeg only
#if LIBAVUTIL_VERSION_MICRO >= 100
    {IMGFMT_420P12_LE,  AV_PIX_FMT_YUV420P12LE},
    {IMGFMT_420P12_BE,  AV_PIX_FMT_YUV420P12BE},
    {IMGFMT_420P14_LE,  AV_PIX_FMT_YUV420P14LE},
    {IMGFMT_420P14_BE,  AV_PIX_FMT_YUV420P14BE},
    {IMGFMT_422P12_LE,  AV_PIX_FMT_YUV422P12LE},
    {IMGFMT_422P12_BE,  AV_PIX_FMT_YUV422P12BE},
    {IMGFMT_422P14_LE,  AV_PIX_FMT_YUV422P14LE},
    {IMGFMT_422P14_BE,  AV_PIX_FMT_YUV422P14BE},
    {IMGFMT_444P12_BE,  AV_PIX_FMT_YUV444P12BE},
    {IMGFMT_444P12_LE,  AV_PIX_FMT_YUV444P12LE},
    {IMGFMT_444P14_BE,  AV_PIX_FMT_YUV444P14BE},
    {IMGFMT_444P14_LE,  AV_PIX_FMT_YUV444P14LE},

    {IMGFMT_RGBA64_BE,  AV_PIX_FMT_RGBA64BE},
    {IMGFMT_RGBA64_LE,  AV_PIX_FMT_RGBA64LE},
    {IMGFMT_BGRA64_BE,  AV_PIX_FMT_BGRA64BE},
    {IMGFMT_BGRA64_LE,  AV_PIX_FMT_BGRA64LE},

    {IMGFMT_BGR0,  AV_PIX_FMT_BGR0},
    {IMGFMT_0RGB,  AV_PIX_FMT_0RGB},
    {IMGFMT_RGB0,  AV_PIX_FMT_RGB0},
    {IMGFMT_0BGR,  AV_PIX_FMT_0BGR},
    {IMGFMT_BGR0,  AV_PIX_FMT_BGR0},
#else
    {IMGFMT_BGR0,  AV_PIX_FMT_BGRA},
    {IMGFMT_0RGB,  AV_PIX_FMT_ARGB},
    {IMGFMT_RGB0,  AV_PIX_FMT_RGBA},
    {IMGFMT_0BGR,  AV_PIX_FMT_ABGR},
    {IMGFMT_BGR0,  AV_PIX_FMT_BGRA},
#endif

    {IMGFMT_VDPAU, AV_PIX_FMT_VDPAU},
#if HAVE_VDA_HWACCEL
    {IMGFMT_VDA,   AV_PIX_FMT_VDA},
#endif
    {IMGFMT_VAAPI, AV_PIX_FMT_VAAPI_VLD},
    {IMGFMT_DXVA2, AV_PIX_FMT_DXVA2_VLD},

    {0, AV_PIX_FMT_NONE}
};

enum AVPixelFormat imgfmt2pixfmt(int fmt)
{
    if (fmt == IMGFMT_NONE)
        return AV_PIX_FMT_NONE;

    if (fmt >= IMGFMT_AVPIXFMT_START && fmt < IMGFMT_AVPIXFMT_END) {
        enum AVPixelFormat pixfmt = fmt - IMGFMT_AVPIXFMT_START;
        // Avoid duplicate format - each format must be unique.
        int mpfmt = pixfmt2imgfmt(pixfmt);
        if (mpfmt == fmt)
            return pixfmt;
        return AV_PIX_FMT_NONE;
    }

    for (int i = 0; conversion_map[i].fmt; i++) {
        if (conversion_map[i].fmt == fmt)
            return conversion_map[i].pix_fmt;
    }
    return AV_PIX_FMT_NONE;
}

int pixfmt2imgfmt(enum AVPixelFormat pix_fmt)
{
    if (pix_fmt == AV_PIX_FMT_NONE)
        return IMGFMT_NONE;

    for (int i = 0; conversion_map[i].pix_fmt != AV_PIX_FMT_NONE; i++) {
        if (conversion_map[i].pix_fmt == pix_fmt)
            return conversion_map[i].fmt;
    }

    int generic = IMGFMT_AVPIXFMT_START + pix_fmt;
    if (generic < IMGFMT_AVPIXFMT_END)
        return generic;

    return 0;
}
