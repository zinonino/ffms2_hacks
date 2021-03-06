//  Copyright (c) 2007-2011 Fredrik Mellbin
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "ffms.h"

extern "C" {
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#ifdef WITH_AVRESAMPLE
#include <libavresample/avresample.h>
#endif
}

// must be included after ffmpeg headers
#include "ffmscompat.h"


#ifdef LIBAVCODEC_VERSION_INT
#   if VERSION_CHECK(LIBAVCODEC_VERSION_INT, <, 54, 28, 0, 54, 59, 100)
#       ifdef FFMBC
void av_frame_free(AVFrame **frame) { av_freep(frame); }
#       endif
#   endif
#endif