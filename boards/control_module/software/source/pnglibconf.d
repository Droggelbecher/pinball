/* pnglibconf.h - library build configuration */

/* libpng version 1.6.34, September 29, 2017 */

/* Copyright (c) 1998-2017 Glenn Randers-Pehrson */

/* This code is released under the libpng license. */
/* For conditions of distribution and use, see the disclaimer */
/* and license in png.h */

/* pnglibconf.h */
/* Machine generated file: DO NOT EDIT */
/* Derived from: scripts/pnglibconf.dfa */

extern (C):

/* options */
/*#undef PNG_ARM_NEON_API_SUPPORTED*/
/*#undef PNG_ARM_NEON_CHECK_SUPPORTED*/
/*#undef PNG_BENIGN_WRITE_ERRORS_SUPPORTED*/
/*#undef PNG_ERROR_NUMBERS_SUPPORTED*/
/*#undef PNG_POWERPC_VSX_API_SUPPORTED*/
/*#undef PNG_POWERPC_VSX_CHECK_SUPPORTED*/
/* end of options */
/* settings */
enum PNG_API_RULE = 0;
enum PNG_DEFAULT_READ_MACROS = 1;
enum PNG_GAMMA_THRESHOLD_FIXED = 5000;
enum PNG_IDAT_READ_SIZE = PNG_ZBUF_SIZE;
enum PNG_INFLATE_BUF_SIZE = 1024;
enum PNG_MAX_GAMMA_8 = 11;
enum PNG_QUANTIZE_BLUE_BITS = 5;
enum PNG_QUANTIZE_GREEN_BITS = 5;
enum PNG_QUANTIZE_RED_BITS = 5;
enum PNG_TEXT_Z_DEFAULT_COMPRESSION = -1;
enum PNG_TEXT_Z_DEFAULT_STRATEGY = 0;
enum PNG_USER_CHUNK_CACHE_MAX = 1000;
enum PNG_USER_CHUNK_MALLOC_MAX = 8000000;
enum PNG_USER_HEIGHT_MAX = 1000000;
enum PNG_USER_WIDTH_MAX = 1000000;
enum PNG_ZBUF_SIZE = 8192;
enum PNG_ZLIB_VERNUM = 0x12b0;
enum PNG_Z_DEFAULT_COMPRESSION = -1;
enum PNG_Z_DEFAULT_NOFILTER_STRATEGY = 0;
enum PNG_Z_DEFAULT_STRATEGY = 1;
enum PNG_sCAL_PRECISION = 5;
enum PNG_sRGB_PROFILE_CHECKS = 2;
/* end of settings */
/* PNGLCONF_H */
