/*
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2004 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you would like to negotiate alternate licensing terms, you may do
 * so by contacting: Underbit Technologies, Inc. <info@underbit.com>
 */

import core.stdc.config;

extern (C):

enum SIZEOF_INT = 4;
enum SIZEOF_LONG = 8;
enum SIZEOF_LONG_LONG = 8;

/* Id: version.h,v 1.26 2004/01/23 09:41:33 rob Exp */

enum MAD_VERSION_MAJOR = 0;
enum MAD_VERSION_MINOR = 15;
enum MAD_VERSION_PATCH = 1;
enum MAD_VERSION_EXTRA = " (beta)";

extern (D) string MAD_VERSION_STRINGIZE(T)(auto ref T str)
{
    import std.conv : to;

    return to!string(str);
}

alias MAD_VERSION_STRING = MAD_VERSION_STRINGIZE;

enum MAD_PUBLISHYEAR = "2000-2004";
enum MAD_AUTHOR = "Underbit Technologies, Inc.";
enum MAD_EMAIL = "info@underbit.com";

extern __gshared const(char)[] mad_version;
extern __gshared const(char)[] mad_copyright;
extern __gshared const(char)[] mad_author;
extern __gshared const(char)[] mad_build;

/* Id: fixed.h,v 1.38 2004/02/17 02:02:03 rob Exp */

alias mad_fixed_t = int;

alias mad_fixed64hi_t = int;
alias mad_fixed64lo_t = uint;

alias mad_fixed64_t = long;

alias mad_sample_t = int;

/*
 * Fixed-point format: 0xABBBBBBB
 * A == whole part      (sign + 3 bits)
 * B == fractional part (28 bits)
 *
 * Values are signed two's complement, so the effective range is:
 * 0x80000000 to 0x7fffffff
 *       -8.0 to +7.9999999962747097015380859375
 *
 * The smallest representable value is:
 * 0x00000001 == 0.0000000037252902984619140625 (i.e. about 3.725e-9)
 *
 * 28 bits of fractional accuracy represent about
 * 8.6 digits of decimal accuracy.
 *
 * Fixed-point numbers can be added or subtracted as normal
 * integers, but multiplication requires shifting the 64-bit result
 * from 56 fractional bits back to 28 (and rounding.)
 *
 * Changing the definition of MAD_F_FRACBITS is only partially
 * supported, and must be done with care.
 */

enum MAD_F_FRACBITS = 28;

/*
extern (D) auto MAD_F(T)(auto ref T x)
{
    import std.conv : to;

    return cast(mad_fixed_t) to!string(x) ~ "L";
}
*/

enum MAD_F_MIN = cast(mad_fixed_t) -0x80000000L;
enum MAD_F_MAX = cast(mad_fixed_t) +0x7fffffffL;

enum MAD_F_ONE = 0x10000000L;

extern (D) auto mad_f_tofixed(T)(auto ref T x)
{
    return cast(mad_fixed_t) x * cast(double) 1L << MAD_F_FRACBITS + 0.5;
}

extern (D) auto mad_f_todouble(T)(auto ref T x)
{
    return cast(double) x / cast(double) 1L << MAD_F_FRACBITS;
}

extern (D) auto mad_f_intpart(T)(auto ref T x)
{
    return x >> MAD_F_FRACBITS;
}

extern (D) auto mad_f_fracpart(T)(auto ref T x)
{
    return x & ((1L << MAD_F_FRACBITS) - 1);
}

/* (x should be positive) */

extern (D) auto mad_f_fromint(T)(auto ref T x)
{
    return x << MAD_F_FRACBITS;
}

extern (D) auto mad_f_add(T0, T1)(auto ref T0 x, auto ref T1 y)
{
    return x + y;
}

extern (D) auto mad_f_sub(T0, T1)(auto ref T0 x, auto ref T1 y)
{
    return x - y;
}

/*
 * This version should be the most accurate if 64-bit types are supported by
 * the compiler, although it may not be the most efficient.
 */

extern (D) auto mad_f_mul(T0, T1)(auto ref T0 x, auto ref T1 y)
{
    return cast(mad_fixed_t) (cast(mad_fixed64_t) x * y) >> MAD_F_SCALEBITS;
}

enum MAD_F_SCALEBITS = MAD_F_FRACBITS;

/* --- Intel --------------------------------------------------------------- */

/* no return value */

/* implicit return of eax */

/*
 * This Intel version is fast and accurate; the disposition of the least
 * significant bit depends on OPT_ACCURACY via mad_f_scale64().
 */

/*
 * This gives best accuracy but is not very fast.
 */

/* OPT_ACCURACY */

/*
 * Surprisingly, this is faster than SHRD followed by ADC.
 */

/*
 * Alternate Intel scaling that may or may not perform better.
 */

/* OPT_ACCURACY */

/* --- ARM ----------------------------------------------------------------- */

/*
 * This ARM V4 version is as accurate as FPM_64BIT but much faster. The
 * least significant bit is properly rounded at no CPU cycle cost!
 */

/*
 * This is faster than the default implementation via MAD_F_MLX() and
 * mad_f_scale64().
 */

/* In Thumb-2, the RSB-immediate instruction is only allowed with a zero
	operand.  If needed this code can also support Thumb-1
	(simply append "s" to the end of the second two instructions). */

/* ! __thumb__ */

/* __thumb__ */

/* --- MIPS ---------------------------------------------------------------- */

/*
 * This MIPS version is fast and accurate; the disposition of the least
 * significant bit depends on OPT_ACCURACY via mad_f_scale64().
 */

/*
 * This loses significant accuracy due to the 16-bit integer limit in the
 * multiply/accumulate instruction.
 */

/* --- SPARC --------------------------------------------------------------- */

/*
 * This SPARC V8 version is fast and accurate; the disposition of the least
 * significant bit depends on OPT_ACCURACY via mad_f_scale64().
 */

/* --- PowerPC ------------------------------------------------------------- */

/*
 * This PowerPC version is fast and accurate; the disposition of the least
 * significant bit depends on OPT_ACCURACY via mad_f_scale64().
 */

/*
 * This gives best accuracy but is not very fast.
 */

/*
 * This is slower than the truncating version below it.
 */

/* --- Default ------------------------------------------------------------- */

/*
 * This version is the most portable but it loses significant accuracy.
 * Furthermore, accuracy is biased against the second argument, so care
 * should be taken when ordering operands.
 *
 * The scale factors are constant as this is not used with SSO.
 *
 * Pre-rounding is required to stay within the limits of compliance.
 */

/* ------------------------------------------------------------------------- */

/* default implementations */

extern (D) auto mad_f_scale64(T0, T1)(auto ref T0 hi, auto ref T1 lo)
{
    return cast(mad_fixed_t) (hi << (32 - MAD_F_SCALEBITS)) | (lo >> MAD_F_SCALEBITS);
}

//enum MAD_F_SCALEBITS = MAD_F_FRACBITS;

/* C routines */

mad_fixed_t mad_f_abs (mad_fixed_t);
mad_fixed_t mad_f_div (mad_fixed_t, mad_fixed_t);

/* Id: bit.h,v 1.12 2004/01/23 09:41:32 rob Exp */

struct mad_bitptr
{
    const(ubyte)* byte_;
    ushort cache;
    ushort left;
}

void mad_bit_init (mad_bitptr*, const(ubyte)*); /* nothing */

uint mad_bit_length (const(mad_bitptr)*, const(mad_bitptr)*);

extern (D) auto mad_bit_bitsleft(T)(auto ref T bitptr)
{
    return bitptr.left;
}

const(ubyte)* mad_bit_nextbyte (const(mad_bitptr)*);

void mad_bit_skip (mad_bitptr*, uint);
c_ulong mad_bit_read (mad_bitptr*, uint);
void mad_bit_write (mad_bitptr*, uint, c_ulong);

ushort mad_bit_crc (mad_bitptr, uint, ushort);

/* Id: timer.h,v 1.16 2004/01/23 09:41:33 rob Exp */

struct mad_timer_t
{
    c_long seconds; /* whole seconds */
    c_ulong fraction; /* 1/MAD_TIMER_RESOLUTION seconds */
}

extern __gshared const mad_timer_t mad_timer_zero;

enum MAD_TIMER_RESOLUTION = 352800000UL;

enum mad_units
{
    MAD_UNITS_HOURS = -2,
    MAD_UNITS_MINUTES = -1,
    MAD_UNITS_SECONDS = 0,

    /* metric units */

    MAD_UNITS_DECISECONDS = 10,
    MAD_UNITS_CENTISECONDS = 100,
    MAD_UNITS_MILLISECONDS = 1000,

    /* audio sample units */

    MAD_UNITS_8000_HZ = 8000,
    MAD_UNITS_11025_HZ = 11025,
    MAD_UNITS_12000_HZ = 12000,

    MAD_UNITS_16000_HZ = 16000,
    MAD_UNITS_22050_HZ = 22050,
    MAD_UNITS_24000_HZ = 24000,

    MAD_UNITS_32000_HZ = 32000,
    MAD_UNITS_44100_HZ = 44100,
    MAD_UNITS_48000_HZ = 48000,

    /* video frame/field units */

    MAD_UNITS_24_FPS = 24,
    MAD_UNITS_25_FPS = 25,
    MAD_UNITS_30_FPS = 30,
    MAD_UNITS_48_FPS = 48,
    MAD_UNITS_50_FPS = 50,
    MAD_UNITS_60_FPS = 60,

    /* CD audio frames */

    MAD_UNITS_75_FPS = 75,

    /* video drop-frame units */

    MAD_UNITS_23_976_FPS = -24,
    MAD_UNITS_24_975_FPS = -25,
    MAD_UNITS_29_97_FPS = -30,
    MAD_UNITS_47_952_FPS = -48,
    MAD_UNITS_49_95_FPS = -50,
    MAD_UNITS_59_94_FPS = -60
}

int mad_timer_compare (mad_timer_t, mad_timer_t);

extern (D) auto mad_timer_sign(T)(auto ref T timer)
{
    return mad_timer_compare(timer, mad_timer_zero);
}

void mad_timer_negate (mad_timer_t*);
mad_timer_t mad_timer_abs (mad_timer_t);

void mad_timer_set (mad_timer_t*, c_ulong, c_ulong, c_ulong);
void mad_timer_add (mad_timer_t*, mad_timer_t);
void mad_timer_multiply (mad_timer_t*, c_long);

c_long mad_timer_count (mad_timer_t, mad_units);
c_ulong mad_timer_fraction (mad_timer_t, c_ulong);
void mad_timer_string (
    mad_timer_t,
    char*,
    const(char)*,
    mad_units,
    mad_units,
    c_ulong);

/* Id: stream.h,v 1.20 2004/02/05 09:02:39 rob Exp */

enum MAD_BUFFER_GUARD = 8;
enum MAD_BUFFER_MDLEN = 511 + 2048 + MAD_BUFFER_GUARD;

enum mad_error
{
    MAD_ERROR_NONE = 0x0000, /* no error */

    MAD_ERROR_BUFLEN = 0x0001, /* input buffer too small (or EOF) */
    MAD_ERROR_BUFPTR = 0x0002, /* invalid (null) buffer pointer */

    MAD_ERROR_NOMEM = 0x0031, /* not enough memory */

    MAD_ERROR_LOSTSYNC = 0x0101, /* lost synchronization */
    MAD_ERROR_BADLAYER = 0x0102, /* reserved header layer value */
    MAD_ERROR_BADBITRATE = 0x0103, /* forbidden bitrate value */
    MAD_ERROR_BADSAMPLERATE = 0x0104, /* reserved sample frequency value */
    MAD_ERROR_BADEMPHASIS = 0x0105, /* reserved emphasis value */

    MAD_ERROR_BADCRC = 0x0201, /* CRC check failed */
    MAD_ERROR_BADBITALLOC = 0x0211, /* forbidden bit allocation value */
    MAD_ERROR_BADSCALEFACTOR = 0x0221, /* bad scalefactor index */
    MAD_ERROR_BADMODE = 0x0222, /* bad bitrate/mode combination */
    MAD_ERROR_BADFRAMELEN = 0x0231, /* bad frame length */
    MAD_ERROR_BADBIGVALUES = 0x0232, /* bad big_values count */
    MAD_ERROR_BADBLOCKTYPE = 0x0233, /* reserved block_type */
    MAD_ERROR_BADSCFSI = 0x0234, /* bad scalefactor selection info */
    MAD_ERROR_BADDATAPTR = 0x0235, /* bad main_data_begin pointer */
    MAD_ERROR_BADPART3LEN = 0x0236, /* bad audio data length */
    MAD_ERROR_BADHUFFTABLE = 0x0237, /* bad Huffman table select */
    MAD_ERROR_BADHUFFDATA = 0x0238, /* Huffman data overrun */
    MAD_ERROR_BADSTEREO = 0x0239 /* incompatible block_type for JS */
}

extern (D) auto MAD_RECOVERABLE(T)(auto ref T error)
{
    return error & 0xff00;
}

struct mad_stream
{
    const(ubyte)* buffer; /* input bitstream buffer */
    const(ubyte)* bufend; /* end of buffer */
    c_ulong skiplen; /* bytes to skip before next frame */

    int sync; /* stream sync found */
    c_ulong freerate; /* free bitrate (fixed) */

    const(ubyte)* this_frame; /* start of current frame */
    const(ubyte)* next_frame; /* start of next frame */
    mad_bitptr ptr; /* current processing bit pointer */

    mad_bitptr anc_ptr; /* ancillary bits pointer */
    uint anc_bitlen; /* number of ancillary bits */

    ubyte[2567]* main_data;
    /* Layer III main_data() */
    uint md_len; /* bytes in main_data */

    int options; /* decoding options (see below) */
    mad_error error; /* error code (see above) */
}

enum
{
    MAD_OPTION_IGNORECRC = 0x0001, /* ignore CRC errors */
    MAD_OPTION_HALFSAMPLERATE = 0x0002 /* generate PCM at 1/2 sample rate */
    /* not yet implemented */
    /* decode left channel only */
    /* decode right channel only */
    /* combine channels */
}

void mad_stream_init (mad_stream*);
void mad_stream_finish (mad_stream*);

void mad_stream_buffer (mad_stream*, const(ubyte)*, c_ulong);
void mad_stream_skip (mad_stream*, c_ulong);

int mad_stream_sync (mad_stream*);

const(char)* mad_stream_errorstr (const(mad_stream)*);

/* Id: frame.h,v 1.20 2004/01/23 09:41:32 rob Exp */

enum mad_layer
{
    MAD_LAYER_I = 1, /* Layer I */
    MAD_LAYER_II = 2, /* Layer II */
    MAD_LAYER_III = 3 /* Layer III */
}

enum mad_mode
{
    MAD_MODE_SINGLE_CHANNEL = 0, /* single channel */
    MAD_MODE_DUAL_CHANNEL = 1, /* dual channel */
    MAD_MODE_JOINT_STEREO = 2, /* joint (MS/intensity) stereo */
    MAD_MODE_STEREO = 3 /* normal LR stereo */
}

enum mad_emphasis
{
    MAD_EMPHASIS_NONE = 0, /* no emphasis */
    MAD_EMPHASIS_50_15_US = 1, /* 50/15 microseconds emphasis */
    MAD_EMPHASIS_CCITT_J_17 = 3, /* CCITT J.17 emphasis */
    MAD_EMPHASIS_RESERVED = 2 /* unknown emphasis */
}

struct mad_header
{
    mad_layer layer; /* audio layer (1, 2, or 3) */
    mad_mode mode; /* channel mode (see above) */
    int mode_extension; /* additional mode info */
    mad_emphasis emphasis; /* de-emphasis to use (see above) */

    c_ulong bitrate; /* stream bitrate (bps) */
    uint samplerate; /* sampling frequency (Hz) */

    ushort crc_check; /* frame CRC accumulator */
    ushort crc_target; /* final target CRC checksum */

    int flags; /* flags (see below) */
    int private_bits; /* private bits (see below) */

    mad_timer_t duration; /* audio playing time of frame */
}

struct mad_frame
{
    mad_header header; /* MPEG audio header */

    int options; /* decoding options (from stream) */

    mad_fixed_t[32][36][2] sbsample; /* synthesis subband filter samples */
    mad_fixed_t[18][32][2]* overlap; /* Layer III block overlap data */
}

extern (D) int MAD_NCHANNELS(T)(auto ref T header)
{
    return header.mode ? 2 : 1;
}

extern (D) int MAD_NSBSAMPLES(T)(auto ref T header)
{
    return header.layer == mad_layer.MAD_LAYER_I ? 12 : ((header.layer == mad_layer.MAD_LAYER_III && (header.flags & .MAD_FLAG_LSF_EXT)) ? 18 : 36);
}

enum
{
    MAD_FLAG_NPRIVATE_III = 0x0007, /* number of Layer III private bits */
    MAD_FLAG_INCOMPLETE = 0x0008, /* header but not data is decoded */

    MAD_FLAG_PROTECTION = 0x0010, /* frame has CRC protection */
    MAD_FLAG_COPYRIGHT = 0x0020, /* frame is copyright */
    MAD_FLAG_ORIGINAL = 0x0040, /* frame is original (else copy) */
    MAD_FLAG_PADDING = 0x0080, /* frame has additional slot */

    MAD_FLAG_I_STEREO = 0x0100, /* uses intensity joint stereo */
    MAD_FLAG_MS_STEREO = 0x0200, /* uses middle/side joint stereo */
    MAD_FLAG_FREEFORMAT = 0x0400, /* uses free format bitrate */

    MAD_FLAG_LSF_EXT = 0x1000, /* lower sampling freq. extension */
    MAD_FLAG_MC_EXT = 0x2000, /* multichannel audio extension */
    MAD_FLAG_MPEG_2_5_EXT = 0x4000 /* MPEG 2.5 (unofficial) extension */
}

enum
{
    MAD_PRIVATE_HEADER = 0x0100, /* header private bit */
    MAD_PRIVATE_III = 0x001f /* Layer III private bits (up to 5) */
}

void mad_header_init (mad_header*); /* nothing */

int mad_header_decode (mad_header*, mad_stream*);

void mad_frame_init (mad_frame*);
void mad_frame_finish (mad_frame*);

int mad_frame_decode (mad_frame*, mad_stream*);

void mad_frame_mute (mad_frame*);

/* Id: synth.h,v 1.15 2004/01/23 09:41:33 rob Exp */

struct mad_pcm
{
    uint samplerate; /* sampling frequency (Hz) */
    ushort channels; /* number of channels */
    ushort length; /* number of samples per channel */
    mad_fixed_t[1152][2] samples; /* PCM output samples [ch][sample] */
}

struct mad_synth
{
    mad_fixed_t[8][16][2][2][2] filter; /* polyphase filterbank outputs */
    /* [ch][eo][peo][s][v] */

    uint phase; /* current processing phase */

    mad_pcm pcm; /* PCM output */
}

/* single channel PCM selector */
enum
{
    MAD_PCM_CHANNEL_SINGLE = 0
}

/* dual channel PCM selector */
enum
{
    MAD_PCM_CHANNEL_DUAL_1 = 0,
    MAD_PCM_CHANNEL_DUAL_2 = 1
}

/* stereo PCM selector */
enum
{
    MAD_PCM_CHANNEL_STEREO_LEFT = 0,
    MAD_PCM_CHANNEL_STEREO_RIGHT = 1
}

void mad_synth_init (mad_synth*); /* nothing */

void mad_synth_mute (mad_synth*);

void mad_synth_frame (mad_synth*, const(mad_frame)*);

/* Id: decoder.h,v 1.17 2004/01/23 09:41:32 rob Exp */

enum mad_decoder_mode
{
    MAD_DECODER_MODE_SYNC = 0,
    MAD_DECODER_MODE_ASYNC = 1
}

enum mad_flow
{
    MAD_FLOW_CONTINUE = 0x0000, /* continue normally */
    MAD_FLOW_STOP = 0x0010, /* stop decoding normally */
    MAD_FLOW_BREAK = 0x0011, /* stop decoding and signal an error */
    MAD_FLOW_IGNORE = 0x0020 /* ignore the current frame */
}

struct mad_decoder
{
    mad_decoder_mode mode;

    int options;

    struct _Anonymous_0
    {
        c_long pid;
        int in_;
        int out_;
    }

    _Anonymous_0 async;

    struct _Anonymous_1
    {
        mad_stream stream;
        mad_frame frame;
        mad_synth synth;
    }

    _Anonymous_1* sync;

    void* cb_data;

    mad_flow function (void*, mad_stream*) input_func;
    mad_flow function (void*, const(mad_header)*) header_func;
    mad_flow function (void*, const(mad_stream)*, mad_frame*) filter_func;
    mad_flow function (void*, const(mad_header)*, mad_pcm*) output_func;
    mad_flow function (void*, mad_stream*, mad_frame*) error_func;
    mad_flow function (void*, void*, uint*) message_func;
}

void mad_decoder_init (
    mad_decoder*,
    void*,
    mad_flow function (void*, mad_stream*),
    mad_flow function (void*, const(mad_header)*),
    mad_flow function (void*, const(mad_stream)*, mad_frame*),
    mad_flow function (void*, const(mad_header)*, mad_pcm*),
    mad_flow function (void*, mad_stream*, mad_frame*),
    mad_flow function (void*, void*, uint*));
int mad_decoder_finish (mad_decoder*);

int mad_decoder_run (mad_decoder*, mad_decoder_mode);
int mad_decoder_message (mad_decoder*, void*, uint*);

