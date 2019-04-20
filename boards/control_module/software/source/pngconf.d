import core.stdc.config;

extern (C):

/* pngconf.h - machine configurable file for libpng
 *
 * libpng version 1.6.34, September 29, 2017
 *
 * Copyright (c) 1998-2002,2004,2006-2016 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * Any machine specific code is near the front of this file, so if you
 * are configuring libpng for a machine, you may want to read the section
 * starting here down to where it starts to typedef png_color, png_text,
 * and png_info.
 */

/* else includes may cause problems */

/* From libpng 1.6.0 libpng requires an ANSI X3.159-1989 ("ISOC90") compliant C
 * compiler for correct compilation.  The following header files are required by
 * the standard.  If your compiler doesn't provide these header files, or they
 * do not match the standard, you will need to provide/improve them.
 */

/* Library header files.  These header files are all defined by ISOC90; libpng
 * expects conformant implementations, however, an ISOC90 conformant system need
 * not provide these header files if the functionality cannot be implemented.
 * In this case it will be necessary to disable the relevant parts of libpng in
 * the build of pnglibconf.h.
 *
 * Prior to 1.6.0 string.h was included here; the API changes in 1.6.0 to not
 * include this unnecessary header file.
 */

/* Required for the definition of FILE: */

/* Required for the definition of jmp_buf and the declaration of longjmp: */

/* Required for struct tm: */

/* PNG_BUILDING_SYMBOL_TABLE */

/* Prior to 1.6.0 it was possible to turn off 'const' in declarations using
 * PNG_NO_CONST; this is no longer supported except for data declarations which
 * apparently still cause problems in 2011 on some compilers.
 */ /* backward compatibility only */

/* This controls optimization of the reading of 16-bit and 32-bit values
 * from PNG files.  It can be set on a per-app-file basis - it
 * just changes whether a macro is used when the function is called.
 * The library builder sets the default; if read functions are not
 * built into the library the macro implementation is forced on.
 */

/* COMPILER SPECIFIC OPTIONS.
 *
 * These options are provided so that a variety of difficult compilers
 * can be used.  Some are fixed at build time (e.g. PNG_API_RULE
 * below) but still have compiler specific implementations, others
 * may be changed on a per-file basis when compiling against libpng.
 */

/* The PNGARG macro was used in versions of libpng prior to 1.6.0 to protect
 * against legacy (pre ISOC90) compilers that did not understand function
 * prototypes.  It is not required for modern C compilers.
 */

extern (D) auto PNGARG(T)(auto ref T arglist)
{
    return arglist;
}

/* Function calling conventions.
 * =============================
 * Normally it is not necessary to specify to the compiler how to call
 * a function - it just does it - however on x86 systems derived from
 * Microsoft and Borland C compilers ('IBM PC', 'DOS', 'Windows' systems
 * and some others) there are multiple ways to call a function and the
 * default can be changed on the compiler command line.  For this reason
 * libpng specifies the calling convention of every exported function and
 * every function called via a user supplied function pointer.  This is
 * done in this file by defining the following macros:
 *
 * PNGAPI    Calling convention for exported functions.
 * PNGCBAPI  Calling convention for user provided (callback) functions.
 * PNGCAPI   Calling convention used by the ANSI-C library (required
 *           for longjmp callbacks and sometimes used internally to
 *           specify the calling convention for zlib).
 *
 * These macros should never be overridden.  If it is necessary to
 * change calling convention in a private build this can be done
 * by setting PNG_API_RULE (which defaults to 0) to one of the values
 * below to select the correct 'API' variants.
 *
 * PNG_API_RULE=0 Use PNGCAPI - the 'C' calling convention - throughout.
 *                This is correct in every known environment.
 * PNG_API_RULE=1 Use the operating system convention for PNGAPI and
 *                the 'C' calling convention (from PNGCAPI) for
 *                callbacks (PNGCBAPI).  This is no longer required
 *                in any known environment - if it has to be used
 *                please post an explanation of the problem to the
 *                libpng mailing list.
 *
 * These cases only differ if the operating system does not use the C
 * calling convention, at present this just means the above cases
 * (x86 DOS/Windows sytems) and, even then, this does not apply to
 * Cygwin running on those systems.
 *
 * Note that the value must be defined in pnglibconf.h so that what
 * the application uses to call the library matches the conventions
 * set when building the library.
 */

/* Symbol export
 * =============
 * When building a shared library it is almost always necessary to tell
 * the compiler which symbols to export.  The png.h macro 'PNG_EXPORT'
 * is used to mark the symbols.  On some systems these symbols can be
 * extracted at link time and need no special processing by the compiler,
 * on other systems the symbols are flagged by the compiler and just
 * the declaration requires a special tag applied (unfortunately) in a
 * compiler dependent way.  Some systems can do either.
 *
 * A small number of older systems also require a symbol from a DLL to
 * be flagged to the program that calls it.  This is a problem because
 * we do not know in the header file included by application code that
 * the symbol will come from a shared library, as opposed to a statically
 * linked one.  For this reason the application must tell us by setting
 * the magic flag PNG_USE_DLL to turn on the special processing before
 * it includes png.h.
 *
 * Four additional macros are used to make this happen:
 *
 * PNG_IMPEXP The magic (if any) to cause a symbol to be exported from
 *            the build or imported if PNG_USE_DLL is set - compiler
 *            and system specific.
 *
 * PNG_EXPORT_TYPE(type) A macro that pre or appends PNG_IMPEXP to
 *                       'type', compiler specific.
 *
 * PNG_DLL_EXPORT Set to the magic to use during a libpng build to
 *                make a symbol exported from the DLL.  Not used in the
 *                public header files; see pngpriv.h for how it is used
 *                in the libpng build.
 *
 * PNG_DLL_IMPORT Set to the magic to force the libpng symbols to come
 *                from a DLL - used to define PNG_IMPEXP when
 *                PNG_USE_DLL is set.
 */

/* System specific discovery.
 * ==========================
 * This code is used at build time to find PNG_IMPEXP, the API settings
 * and PNG_EXPORT_TYPE(), it may also set a macro to indicate the DLL
 * import processing is possible.  On Windows systems it also sets
 * compiler-specific macros to the values required to change the calling
 * conventions of the various functions.
 */

/* Windows system (DOS doesn't support DLLs).  Includes builds under Cygwin or
 * MinGW on any architecture currently supported by Windows.  Also includes
 * Watcom builds but these need special treatment because they are not
 * compatible with GCC or Visual C because of different calling conventions.
 */

/* If this line results in an error, either because __watcall is not
 * understood or because of a redefine just below you cannot use *this*
 * build of the library with the compiler you are using.  *This* build was
 * build using Watcom and applications must also be built using Watcom!
 */

/* If this line results in an error __stdcall is not understood and
 * PNG_API_RULE should not have been set to '1'.
 */

/* An older compiler, or one not detected (erroneously) above,
 * if necessary override on the command line to get the correct
 * variants for the compiler.
 */

/* compiler/api */

/* NOTE: PNGCBAPI always defaults to PNGCAPI. */

/* older Borland and MSC
 * compilers used '__export' and required this to be after
 * the type.
 */

/* newer compiler */

/* compiler */

/* !Windows */

/* !Windows/x86 && !OS/2 */
/* Use the defaults, or define PNG*API on the command line (but
 * this will have to be done for every compile!)
 */
/* other system, !OS/2 */
/* !Windows/x86 */

/* Now do all the defaulting . */

//enum PNGCBAPI = PNGCAPI;

//enum PNGAPI = PNGCAPI;

/* PNG_IMPEXP may be set on the compilation system command line or (if not set)
 * then in an internal header file when building the library, otherwise (when
 * using the library) it is set here.
 */

/* This forces use of a DLL, disallowing static linking */

/* In 1.5.2 the definition of PNG_FUNCTION has been changed to always treat
 * 'attributes' as a storage class - the attributes go at the start of the
 * function definition, and attributes are always appended regardless of the
 * compiler.  This considerably simplifies these macros but may cause problems
 * if any compilers both need function attributes and fail to handle them as
 * a storage class (this is unlikely.)
 */

/* The ordinal value is only relevant when preprocessing png.h for symbol
 * table entries, so we discard it here.  See the .dfn files in the
 * scripts directory.
 */

/* ANSI-C (C90) does not permit a macro to be invoked with an empty argument,
 * so make something non-empty to satisfy the requirement:
 */ /*empty list*/

extern (D) auto PNG_EXPORT(T0, T1, T2, T3)(auto ref T0 ordinal, auto ref T1 type, auto ref T2 name, auto ref T3 args)
{
    return PNG_EXPORTA(ordinal, type, name, args, PNG_EMPTY);
}

/* Use PNG_REMOVED to comment out a removed interface. */

/* Support for compiler specific function attributes.  These are used
 * so that where compiler support is available incorrect use of API
 * functions in png.h will generate compiler warnings.
 *
 * Added at libpng-1.2.41.
 */

/* Support for compiler specific function attributes.  These are used
 * so that where compiler support is available, incorrect use of API
 * functions in png.h will generate compiler warnings.  Added at libpng
 * version 1.2.41.  Disabling these removes the warnings but may also produce
 * less efficient code.
 */
/* Clang defines both __clang__ and __GNUC__. Check __clang__ first. */

/* Doesn't work so we use deprecated instead*/

/* __GNUC__.__GNUC_MINOR__ > 3.0 */
/* __GNUC__ >= 3 */

/* not supported */

/* PNG_PEDANTIC_WARNINGS */

/* Use of this function is deprecated */

/* The result of this function must be checked */

/* This function does not return */

/* The result of the function is new memory */

/* This is a private libpng function */

/* The C99 "restrict" feature */

/* A floating point API. */

/* No floating point APIs */

/* A fixed point API. */

/* No fixed point APIs */

/* Some typedefs to get us started.  These should be safe on most of the common
 * platforms.
 *
 * png_uint_32 and png_int_32 may, currently, be larger than required to hold a
 * 32-bit value however this is not normally advisable.
 *
 * png_uint_16 and png_int_16 should always be two bytes in size - this is
 * verified at library build time.
 *
 * png_byte must always be one byte in size.
 *
 * The checks below use constants from limits.h, as defined by the ISOC90
 * standard.
 */
alias png_byte = ubyte;

alias png_int_16 = short;

alias png_uint_16 = ushort;

alias png_int_32 = int;

alias png_uint_32 = uint;

/* Prior to 1.6.0 it was possible to disable the use of size_t, 1.6.0, however,
 * requires an ISOC90 compiler and relies on consistent behavior of sizeof.
 */
alias png_size_t = c_ulong;
alias png_ptrdiff_t = c_long;

/* libpng needs to know the maximum value of 'size_t' and this controls the
 * definition of png_alloc_size_t, below.  This maximum value of size_t limits
 * but does not control the maximum allocations the library makes - there is
 * direct application control of this through png_set_user_limits().
 */

/* Compiler specific tests for systems where size_t is known to be less than
 * 32 bits (some of these systems may no longer work because of the lack of
 * 'far' support; see above.)
 */

/* png_alloc_size_t is guaranteed to be no smaller than png_size_t, and no
 * smaller than png_uint_32.  Casts from png_size_t or png_uint_32 to
 * png_alloc_size_t are not necessary; in fact, it is recommended not to use
 * them at all so that the compiler can complain when something turns out to be
 * problematic.
 *
 * Casts in the other direction (from png_alloc_size_t to png_size_t or
 * png_uint_32) should be explicitly applied; however, we do not expect to
 * encounter practical situations that require such conversions.
 *
 * PNG_SMALL_SIZE_T must be defined if the maximum value of size_t is less than
 * 4294967295 - i.e. less than the maximum value of png_uint_32.
 */

alias png_alloc_size_t = c_ulong;

/* Prior to 1.6.0 libpng offered limited support for Microsoft C compiler
 * implementations of Intel CPU specific support of user-mode segmented address
 * spaces, where 16-bit pointers address more than 65536 bytes of memory using
 * separate 'segment' registers.  The implementation requires two different
 * types of pointer (only one of which includes the segment value.)
 *
 * If required this support is available in version 1.2 of libpng and may be
 * available in versions through 1.5, although the correctness of the code has
 * not been verified recently.
 */

/* Typedef for floating-point numbers that are converted to fixed-point with a
 * multiple of 100,000, e.g., gamma
 */
alias png_fixed_point = int;

/* Add typedefs for pointers */
alias png_voidp = void*;
alias png_const_voidp = const(void)*;
alias png_bytep = ubyte*;
alias png_const_bytep = const(ubyte)*;
alias png_uint_32p = uint*;
alias png_const_uint_32p = const(uint)*;
alias png_int_32p = int*;
alias png_const_int_32p = const(int)*;
alias png_uint_16p = ushort*;
alias png_const_uint_16p = const(ushort)*;
alias png_int_16p = short*;
alias png_const_int_16p = const(short)*;
alias png_charp = char*;
alias png_const_charp = const(char)*;
alias png_fixed_point_p = int*;
alias png_const_fixed_point_p = const(int)*;
alias png_size_tp = c_ulong*;
alias png_const_size_tp = const(c_ulong)*;

/* Pointers to pointers; i.e. arrays */
alias png_bytepp = ubyte**;
alias png_uint_32pp = uint**;
alias png_int_32pp = int**;
alias png_uint_16pp = ushort**;
alias png_int_16pp = short**;
alias png_const_charpp = const(char*)*;
alias png_charpp = char**;
alias png_fixed_point_pp = int**;

/* Pointers to pointers to pointers; i.e., pointer to array */
alias png_charppp = char***;

/* PNG_BUILDING_SYMBOL_TABLE */

/* PNGCONF_H */
