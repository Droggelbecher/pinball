/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

module mad_im;
static import core.stdc.config;

static import std.conv;

static import std.conv;
static import std.string;


private {
  version(linux) {
    version = Nix;
  } else version(darwin) {
    version = Nix;
  } else version(OSX) {
    version = Nix;
  } else version(FreeBSD) {
    version = Nix;
    version = freebsd;
  } else version(freebsd) {
    version = Nix;
  } else version(Unix) {
    version = Nix;
  } else version(Posix) {
    version = Nix;
  }

  version(Tango) {
    static import tango.stdc.string;
    static import tango.stdc.stringz;

    version (PhobosCompatibility) {
    } else {
      alias char[] string;
      alias wchar[] wstring;
      alias dchar[] dstring;
    }
  } else {
    version(D_Version2) {
      static import std.conv;
    }
    static import std.string;
    static import std.c.string;
  }

  version(D_Version2) {
    mixin("alias const(char)* CCPTR;");
  } else {
    alias char* CCPTR;
  }

  CCPTR swigToCString(string str) {
    version(Tango) {
      return tango.stdc.stringz.toStringz(str);
    } else {
      return std.string.toStringz(str);
    }
  }

  string swigToDString(CCPTR cstr) {
    version(Tango) {
      return tango.stdc.stringz.fromStringz(cstr);
    } else {
      version(D_Version2) {
        mixin("return std.conv.to!string(cstr);");
      } else {
        return std.c.string.toString(cstr);
      }
    }
  }
}

class SwigSwigSharedLibLoadException : Exception {
  this(in string[] libNames, in string[] reasons) {
    string msg = "Failed to load one or more shared libraries:";
    foreach(i, n; libNames) {
      msg ~= "\n\t" ~ n ~ " - ";
      if(i < reasons.length)
        msg ~= reasons[i];
      else
        msg ~= "Unknown";
    }
    super(msg);
  }
}

class SwigSymbolLoadException : Exception {
  this(string SwigSharedLibName, string symbolName) {
    super("Failed to load symbol " ~ symbolName ~ " from shared library " ~ SwigSharedLibName);
    _symbolName = symbolName;
  }

  string symbolName() {
    return _symbolName;
  }

private:
  string _symbolName;
}

private {
  version(Nix) {
    version(freebsd) {
      // the dl* functions are in libc on FreeBSD
    }
    else {
      //pragma(lib, "dl");
    }

    version(Tango) {
      import tango.sys.Common;
    } else version(linux) {
      import std.c.linux.linux;
    } else {
      extern(C) {
        const RTLD_NOW = 2;

        void *dlopen(CCPTR file, int mode);
        int dlclose(void* handle);
        void *dlsym(void* handle, CCPTR name);
        CCPTR dlerror();
      }
    }

    alias void* SwigSharedLibHandle;

    SwigSharedLibHandle swigLoadSharedLib(string libName) {
      return dlopen(swigToCString(libName), RTLD_NOW);
    }

    void swigUnloadSharedLib(SwigSharedLibHandle hlib) {
      dlclose(hlib);
    }

    void* swigGetSymbol(SwigSharedLibHandle hlib, string symbolName) {
      return dlsym(hlib, swigToCString(symbolName));
    }

    string swigGetErrorStr() {
      CCPTR err = dlerror();
      if (err is null) {
        return "Unknown Error";
      }
      return swigToDString(err);
    }
  } else version(Windows) {
    alias ushort WORD;
    alias uint DWORD;
    alias CCPTR LPCSTR;
    alias void* HMODULE;
    alias void* HLOCAL;
    alias int function() FARPROC;
    struct VA_LIST {}

    extern (Windows) {
      HMODULE LoadLibraryA(LPCSTR);
      FARPROC GetProcAddress(HMODULE, LPCSTR);
      void FreeLibrary(HMODULE);
      DWORD GetLastError();
      DWORD FormatMessageA(DWORD, in void*, DWORD, DWORD, LPCSTR, DWORD, VA_LIST*);
      HLOCAL LocalFree(HLOCAL);
    }

    DWORD MAKELANGID(WORD p, WORD s) {
      return (((cast(WORD)s) << 10) | cast(WORD)p);
    }

    enum {
      LANG_NEUTRAL                    = 0,
      SUBLANG_DEFAULT                 = 1,
      FORMAT_MESSAGE_ALLOCATE_BUFFER  = 256,
      FORMAT_MESSAGE_IGNORE_INSERTS   = 512,
      FORMAT_MESSAGE_FROM_SYSTEM      = 4096
    }

    alias HMODULE SwigSharedLibHandle;

    SwigSharedLibHandle swigLoadSharedLib(string libName) {
      return LoadLibraryA(swigToCString(libName));
    }

    void swigUnloadSharedLib(SwigSharedLibHandle hlib) {
      FreeLibrary(hlib);
    }

    void* swigGetSymbol(SwigSharedLibHandle hlib, string symbolName) {
      return GetProcAddress(hlib, swigToCString(symbolName));
    }

    string swigGetErrorStr() {
      DWORD errcode = GetLastError();

      LPCSTR msgBuf;
      DWORD i = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        null,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        cast(LPCSTR)&msgBuf,
        0,
        null);

      string text = swigToDString(msgBuf);
      LocalFree(cast(HLOCAL)msgBuf);

      if (i >= 2) {
        i -= 2;
      }
      return text[0 .. i];
    }
  } else {
    static assert(0, "Operating system not supported by the wrapper loading code.");
  }

  final class SwigSharedLib {
    void load(string[] names) {
      if (_hlib !is null) return;

      string[] failedLibs;
      string[] reasons;

      foreach(n; names) {
        _hlib = swigLoadSharedLib(n);
        if (_hlib is null) {
          failedLibs ~= n;
          reasons ~= swigGetErrorStr();
          continue;
        }
        _name = n;
        break;
      }

      if (_hlib is null) {
        throw new SwigSwigSharedLibLoadException(failedLibs, reasons);
      }
    }

    void* loadSymbol(string symbolName, bool doThrow = true) {
      void* sym = swigGetSymbol(_hlib, symbolName);
      if(doThrow && (sym is null)) {
        throw new SwigSymbolLoadException(_name, symbolName);
      }
      return sym;
    }

    void unload() {
      if(_hlib !is null) {
        swigUnloadSharedLib(_hlib);
        _hlib = null;
      }
    }

  private:
    string _name;
    SwigSharedLibHandle _hlib;
  }
}

static this() {
  string[] possibleFileNames;
  version (Posix) {
    version (OSX) {
      possibleFileNames ~= ["libmad_wrap.dylib", "libmad_wrap.bundle"];
    }
    possibleFileNames ~= ["libmad_wrap.so"];
  } else version (Windows) {
    possibleFileNames ~= ["mad_wrap.dll", "libmad_wrap.so"];
  } else {
    static assert(false, "Operating system not supported by the wrapper loading code.");
  }

  auto library = new SwigSharedLib;
  library.load(possibleFileNames);

  string bindCode(string functionPointer, string symbol) {
    return functionPointer ~ " = cast(typeof(" ~ functionPointer ~
      "))library.loadSymbol(`" ~ symbol ~ "`);";
  }

  //#if !defined(SWIG_D_NO_EXCEPTION_HELPER)
  mixin(bindCode("swigRegisterExceptionCallbacksmad", "SWIGRegisterExceptionCallbacks_mad"));
  //#endif // SWIG_D_NO_EXCEPTION_HELPER
  //#if !defined(SWIG_D_NO_STRING_HELPER)
  mixin(bindCode("swigRegisterStringCallbackmad", "SWIGRegisterStringCallback_mad"));
  //#endif // SWIG_D_NO_STRING_HELPER
  
  mixin(bindCode("SIZEOF_INT_get", "D_SIZEOF_INT_get"));
  mixin(bindCode("SIZEOF_LONG_get", "D_SIZEOF_LONG_get"));
  mixin(bindCode("SIZEOF_LONG_LONG_get", "D_SIZEOF_LONG_LONG_get"));
  mixin(bindCode("MAD_VERSION_MAJOR_get", "D_MAD_VERSION_MAJOR_get"));
  mixin(bindCode("MAD_VERSION_MINOR_get", "D_MAD_VERSION_MINOR_get"));
  mixin(bindCode("MAD_VERSION_PATCH_get", "D_MAD_VERSION_PATCH_get"));
  mixin(bindCode("MAD_VERSION_EXTRA_get", "D_MAD_VERSION_EXTRA_get"));
  mixin(bindCode("MAD_VERSION_get", "D_MAD_VERSION_get"));
  mixin(bindCode("MAD_PUBLISHYEAR_get", "D_MAD_PUBLISHYEAR_get"));
  mixin(bindCode("MAD_AUTHOR_get", "D_MAD_AUTHOR_get"));
  mixin(bindCode("MAD_EMAIL_get", "D_MAD_EMAIL_get"));
  mixin(bindCode("mad_version_get", "D_mad_version_get"));
  mixin(bindCode("mad_copyright_get", "D_mad_copyright_get"));
  mixin(bindCode("mad_author_get", "D_mad_author_get"));
  mixin(bindCode("mad_build_get", "D_mad_build_get"));
  mixin(bindCode("MAD_F_FRACBITS_get", "D_MAD_F_FRACBITS_get"));
  mixin(bindCode("MAD_F_SCALEBITS_get", "D_MAD_F_SCALEBITS_get"));
  mixin(bindCode("mad_f_abs", "D_mad_f_abs"));
  mixin(bindCode("mad_f_div", "D_mad_f_div"));
  mixin(bindCode("mad_bitptr__byte_set", "D_mad_bitptr__byte_set"));
  mixin(bindCode("mad_bitptr__byte_get", "D_mad_bitptr__byte_get"));
  mixin(bindCode("mad_bitptr_cache_set", "D_mad_bitptr_cache_set"));
  mixin(bindCode("mad_bitptr_cache_get", "D_mad_bitptr_cache_get"));
  mixin(bindCode("mad_bitptr_left_set", "D_mad_bitptr_left_set"));
  mixin(bindCode("mad_bitptr_left_get", "D_mad_bitptr_left_get"));
  mixin(bindCode("new_mad_bitptr", "D_new_mad_bitptr"));
  mixin(bindCode("delete_mad_bitptr", "D_delete_mad_bitptr"));
  mixin(bindCode("mad_bit_init", "D_mad_bit_init"));
  mixin(bindCode("mad_bit_length", "D_mad_bit_length"));
  mixin(bindCode("mad_bit_nextbyte", "D_mad_bit_nextbyte"));
  mixin(bindCode("mad_bit_skip", "D_mad_bit_skip"));
  mixin(bindCode("mad_bit_read", "D_mad_bit_read"));
  mixin(bindCode("mad_bit_crc", "D_mad_bit_crc"));
  mixin(bindCode("mad_timer_t_seconds_set", "D_mad_timer_t_seconds_set"));
  mixin(bindCode("mad_timer_t_seconds_get", "D_mad_timer_t_seconds_get"));
  mixin(bindCode("mad_timer_t_fraction_set", "D_mad_timer_t_fraction_set"));
  mixin(bindCode("mad_timer_t_fraction_get", "D_mad_timer_t_fraction_get"));
  mixin(bindCode("new_mad_timer_t", "D_new_mad_timer_t"));
  mixin(bindCode("delete_mad_timer_t", "D_delete_mad_timer_t"));
  mixin(bindCode("mad_timer_zero_get", "D_mad_timer_zero_get"));
  mixin(bindCode("MAD_TIMER_RESOLUTION_get", "D_MAD_TIMER_RESOLUTION_get"));
  mixin(bindCode("mad_timer_compare", "D_mad_timer_compare"));
  mixin(bindCode("mad_timer_negate", "D_mad_timer_negate"));
  mixin(bindCode("mad_timer_abs", "D_mad_timer_abs"));
  mixin(bindCode("mad_timer_set", "D_mad_timer_set"));
  mixin(bindCode("mad_timer_add", "D_mad_timer_add"));
  mixin(bindCode("mad_timer_multiply", "D_mad_timer_multiply"));
  mixin(bindCode("mad_timer_count", "D_mad_timer_count"));
  mixin(bindCode("mad_timer_fraction", "D_mad_timer_fraction"));
  mixin(bindCode("mad_timer_string", "D_mad_timer_string"));
  mixin(bindCode("MAD_BUFFER_GUARD_get", "D_MAD_BUFFER_GUARD_get"));
  mixin(bindCode("MAD_BUFFER_MDLEN_get", "D_MAD_BUFFER_MDLEN_get"));
  mixin(bindCode("mad_stream_buffer_set", "D_mad_stream_buffer_set"));
  mixin(bindCode("mad_stream_buffer_get", "D_mad_stream_buffer_get"));
  mixin(bindCode("mad_stream_bufend_set", "D_mad_stream_bufend_set"));
  mixin(bindCode("mad_stream_bufend_get", "D_mad_stream_bufend_get"));
  mixin(bindCode("mad_stream_skiplen_set", "D_mad_stream_skiplen_set"));
  mixin(bindCode("mad_stream_skiplen_get", "D_mad_stream_skiplen_get"));
  mixin(bindCode("mad_stream_sync_set", "D_mad_stream_sync_set"));
  mixin(bindCode("mad_stream_sync_get", "D_mad_stream_sync_get"));
  mixin(bindCode("mad_stream_freerate_set", "D_mad_stream_freerate_set"));
  mixin(bindCode("mad_stream_freerate_get", "D_mad_stream_freerate_get"));
  mixin(bindCode("mad_stream_this_frame_set", "D_mad_stream_this_frame_set"));
  mixin(bindCode("mad_stream_this_frame_get", "D_mad_stream_this_frame_get"));
  mixin(bindCode("mad_stream_next_frame_set", "D_mad_stream_next_frame_set"));
  mixin(bindCode("mad_stream_next_frame_get", "D_mad_stream_next_frame_get"));
  mixin(bindCode("mad_stream_ptr_set", "D_mad_stream_ptr_set"));
  mixin(bindCode("mad_stream_ptr_get", "D_mad_stream_ptr_get"));
  mixin(bindCode("mad_stream_anc_ptr_set", "D_mad_stream_anc_ptr_set"));
  mixin(bindCode("mad_stream_anc_ptr_get", "D_mad_stream_anc_ptr_get"));
  mixin(bindCode("mad_stream_anc_bitlen_set", "D_mad_stream_anc_bitlen_set"));
  mixin(bindCode("mad_stream_anc_bitlen_get", "D_mad_stream_anc_bitlen_get"));
  mixin(bindCode("mad_stream_main_data_set", "D_mad_stream_main_data_set"));
  mixin(bindCode("mad_stream_main_data_get", "D_mad_stream_main_data_get"));
  mixin(bindCode("mad_stream_md_len_set", "D_mad_stream_md_len_set"));
  mixin(bindCode("mad_stream_md_len_get", "D_mad_stream_md_len_get"));
  mixin(bindCode("mad_stream_options_set", "D_mad_stream_options_set"));
  mixin(bindCode("mad_stream_options_get", "D_mad_stream_options_get"));
  mixin(bindCode("mad_stream_error_set", "D_mad_stream_error_set"));
  mixin(bindCode("mad_stream_error_get", "D_mad_stream_error_get"));
  mixin(bindCode("new_mad_stream", "D_new_mad_stream"));
  mixin(bindCode("delete_mad_stream", "D_delete_mad_stream"));
  mixin(bindCode("mad_stream_init", "D_mad_stream_init"));
  mixin(bindCode("mad_stream_finish", "D_mad_stream_finish"));
  mixin(bindCode("mad_stream_buffer", "D_mad_stream_buffer"));
  mixin(bindCode("mad_stream_skip", "D_mad_stream_skip"));
  mixin(bindCode("mad_stream_sync", "D_mad_stream_sync"));
  mixin(bindCode("mad_stream_errorstr", "D_mad_stream_errorstr"));
  mixin(bindCode("mad_header_layer_set", "D_mad_header_layer_set"));
  mixin(bindCode("mad_header_layer_get", "D_mad_header_layer_get"));
  mixin(bindCode("mad_header_mode_set", "D_mad_header_mode_set"));
  mixin(bindCode("mad_header_mode_get", "D_mad_header_mode_get"));
  mixin(bindCode("mad_header_mode_extension_set", "D_mad_header_mode_extension_set"));
  mixin(bindCode("mad_header_mode_extension_get", "D_mad_header_mode_extension_get"));
  mixin(bindCode("mad_header_emphasis_set", "D_mad_header_emphasis_set"));
  mixin(bindCode("mad_header_emphasis_get", "D_mad_header_emphasis_get"));
  mixin(bindCode("mad_header_bitrate_set", "D_mad_header_bitrate_set"));
  mixin(bindCode("mad_header_bitrate_get", "D_mad_header_bitrate_get"));
  mixin(bindCode("mad_header_samplerate_set", "D_mad_header_samplerate_set"));
  mixin(bindCode("mad_header_samplerate_get", "D_mad_header_samplerate_get"));
  mixin(bindCode("mad_header_crc_check_set", "D_mad_header_crc_check_set"));
  mixin(bindCode("mad_header_crc_check_get", "D_mad_header_crc_check_get"));
  mixin(bindCode("mad_header_crc_target_set", "D_mad_header_crc_target_set"));
  mixin(bindCode("mad_header_crc_target_get", "D_mad_header_crc_target_get"));
  mixin(bindCode("mad_header_flags_set", "D_mad_header_flags_set"));
  mixin(bindCode("mad_header_flags_get", "D_mad_header_flags_get"));
  mixin(bindCode("mad_header_private_bits_set", "D_mad_header_private_bits_set"));
  mixin(bindCode("mad_header_private_bits_get", "D_mad_header_private_bits_get"));
  mixin(bindCode("mad_header_duration_set", "D_mad_header_duration_set"));
  mixin(bindCode("mad_header_duration_get", "D_mad_header_duration_get"));
  mixin(bindCode("new_mad_header", "D_new_mad_header"));
  mixin(bindCode("delete_mad_header", "D_delete_mad_header"));
  mixin(bindCode("mad_frame_header_set", "D_mad_frame_header_set"));
  mixin(bindCode("mad_frame_header_get", "D_mad_frame_header_get"));
  mixin(bindCode("mad_frame_options_set", "D_mad_frame_options_set"));
  mixin(bindCode("mad_frame_options_get", "D_mad_frame_options_get"));
  mixin(bindCode("mad_frame_sbsample_set", "D_mad_frame_sbsample_set"));
  mixin(bindCode("mad_frame_sbsample_get", "D_mad_frame_sbsample_get"));
  mixin(bindCode("mad_frame_overlap_set", "D_mad_frame_overlap_set"));
  mixin(bindCode("mad_frame_overlap_get", "D_mad_frame_overlap_get"));
  mixin(bindCode("new_mad_frame", "D_new_mad_frame"));
  mixin(bindCode("delete_mad_frame", "D_delete_mad_frame"));
  mixin(bindCode("mad_header_init", "D_mad_header_init"));
  mixin(bindCode("mad_header_decode", "D_mad_header_decode"));
  mixin(bindCode("mad_frame_init", "D_mad_frame_init"));
  mixin(bindCode("mad_frame_finish", "D_mad_frame_finish"));
  mixin(bindCode("mad_frame_decode", "D_mad_frame_decode"));
  mixin(bindCode("mad_frame_mute", "D_mad_frame_mute"));
  mixin(bindCode("mad_pcm_samplerate_set", "D_mad_pcm_samplerate_set"));
  mixin(bindCode("mad_pcm_samplerate_get", "D_mad_pcm_samplerate_get"));
  mixin(bindCode("mad_pcm_channels_set", "D_mad_pcm_channels_set"));
  mixin(bindCode("mad_pcm_channels_get", "D_mad_pcm_channels_get"));
  mixin(bindCode("mad_pcm_length_set", "D_mad_pcm_length_set"));
  mixin(bindCode("mad_pcm_length_get", "D_mad_pcm_length_get"));
  mixin(bindCode("mad_pcm_samples_set", "D_mad_pcm_samples_set"));
  mixin(bindCode("mad_pcm_samples_get", "D_mad_pcm_samples_get"));
  mixin(bindCode("new_mad_pcm", "D_new_mad_pcm"));
  mixin(bindCode("delete_mad_pcm", "D_delete_mad_pcm"));
  mixin(bindCode("mad_synth_filter_set", "D_mad_synth_filter_set"));
  mixin(bindCode("mad_synth_filter_get", "D_mad_synth_filter_get"));
  mixin(bindCode("mad_synth_phase_set", "D_mad_synth_phase_set"));
  mixin(bindCode("mad_synth_phase_get", "D_mad_synth_phase_get"));
  mixin(bindCode("mad_synth_pcm_set", "D_mad_synth_pcm_set"));
  mixin(bindCode("mad_synth_pcm_get", "D_mad_synth_pcm_get"));
  mixin(bindCode("new_mad_synth", "D_new_mad_synth"));
  mixin(bindCode("delete_mad_synth", "D_delete_mad_synth"));
  mixin(bindCode("mad_synth_init", "D_mad_synth_init"));
  mixin(bindCode("mad_synth_mute", "D_mad_synth_mute"));
  mixin(bindCode("mad_synth_frame", "D_mad_synth_frame"));
  mixin(bindCode("mad_decoder_mode_set", "D_mad_decoder_mode_set"));
  mixin(bindCode("mad_decoder_mode_get", "D_mad_decoder_mode_get"));
  mixin(bindCode("mad_decoder_options_set", "D_mad_decoder_options_set"));
  mixin(bindCode("mad_decoder_options_get", "D_mad_decoder_options_get"));
  mixin(bindCode("mad_decoder_async_get", "D_mad_decoder_async_get"));
  mixin(bindCode("mad_decoder_sync_get", "D_mad_decoder_sync_get"));
  mixin(bindCode("mad_decoder_cb_data_set", "D_mad_decoder_cb_data_set"));
  mixin(bindCode("mad_decoder_cb_data_get", "D_mad_decoder_cb_data_get"));
  mixin(bindCode("mad_decoder_input_func_set", "D_mad_decoder_input_func_set"));
  mixin(bindCode("mad_decoder_input_func_get", "D_mad_decoder_input_func_get"));
  mixin(bindCode("mad_decoder_header_func_set", "D_mad_decoder_header_func_set"));
  mixin(bindCode("mad_decoder_header_func_get", "D_mad_decoder_header_func_get"));
  mixin(bindCode("mad_decoder_filter_func_set", "D_mad_decoder_filter_func_set"));
  mixin(bindCode("mad_decoder_filter_func_get", "D_mad_decoder_filter_func_get"));
  mixin(bindCode("mad_decoder_output_func_set", "D_mad_decoder_output_func_set"));
  mixin(bindCode("mad_decoder_output_func_get", "D_mad_decoder_output_func_get"));
  mixin(bindCode("mad_decoder_error_func_set", "D_mad_decoder_error_func_set"));
  mixin(bindCode("mad_decoder_error_func_get", "D_mad_decoder_error_func_get"));
  mixin(bindCode("mad_decoder_message_func_set", "D_mad_decoder_message_func_set"));
  mixin(bindCode("mad_decoder_message_func_get", "D_mad_decoder_message_func_get"));
  mixin(bindCode("new_mad_decoder", "D_new_mad_decoder"));
  mixin(bindCode("delete_mad_decoder", "D_delete_mad_decoder"));
  mixin(bindCode("mad_decoder_sync_stream_set", "D_mad_decoder_sync_stream_set"));
  mixin(bindCode("mad_decoder_sync_stream_get", "D_mad_decoder_sync_stream_get"));
  mixin(bindCode("mad_decoder_sync_frame_set", "D_mad_decoder_sync_frame_set"));
  mixin(bindCode("mad_decoder_sync_frame_get", "D_mad_decoder_sync_frame_get"));
  mixin(bindCode("mad_decoder_sync_synth_set", "D_mad_decoder_sync_synth_set"));
  mixin(bindCode("mad_decoder_sync_synth_get", "D_mad_decoder_sync_synth_get"));
  mixin(bindCode("new_mad_decoder_sync", "D_new_mad_decoder_sync"));
  mixin(bindCode("delete_mad_decoder_sync", "D_delete_mad_decoder_sync"));
  mixin(bindCode("mad_decoder_async_pid_set", "D_mad_decoder_async_pid_set"));
  mixin(bindCode("mad_decoder_async_pid_get", "D_mad_decoder_async_pid_get"));
  mixin(bindCode("mad_decoder_async__in_set", "D_mad_decoder_async__in_set"));
  mixin(bindCode("mad_decoder_async__in_get", "D_mad_decoder_async__in_get"));
  mixin(bindCode("mad_decoder_async__out_set", "D_mad_decoder_async__out_set"));
  mixin(bindCode("mad_decoder_async__out_get", "D_mad_decoder_async__out_get"));
  mixin(bindCode("new_mad_decoder_async", "D_new_mad_decoder_async"));
  mixin(bindCode("delete_mad_decoder_async", "D_delete_mad_decoder_async"));
  mixin(bindCode("mad_decoder_init", "D_mad_decoder_init"));
  mixin(bindCode("mad_decoder_finish", "D_mad_decoder_finish"));
  mixin(bindCode("mad_decoder_run", "D_mad_decoder_run"));
  mixin(bindCode("mad_decoder_message", "D_mad_decoder_message"));
}

//#if !defined(SWIG_D_NO_EXCEPTION_HELPER)
extern(C) void function(
  SwigExceptionCallback exceptionCallback,
  SwigExceptionCallback illegalArgumentCallback,
  SwigExceptionCallback illegalElementCallback,
  SwigExceptionCallback ioCallback,
  SwigExceptionCallback noSuchElementCallback) swigRegisterExceptionCallbacksmad;
//#endif // SWIG_D_NO_EXCEPTION_HELPER

//#if !defined(SWIG_D_NO_STRING_HELPER)
extern(C) void function(SwigStringCallback callback) swigRegisterStringCallbackmad;
//#endif // SWIG_D_NO_STRING_HELPER


mixin template SwigOperatorDefinitions() {
  public override bool opEquals(Object o) {
    if (auto rhs = cast(typeof(this))o) {
      if (swigCPtr == rhs.swigCPtr) return true;
      static if (is(typeof(swigOpEquals(rhs)))) {
        return swigOpEquals(rhs);
      } else {
        return false; 
      }
    }
    return super.opEquals(o);
  }


}


private class SwigExceptionHelper {
  static this() {
	// The D1/Tango version maps C++ exceptions to multiple exception types.
    swigRegisterExceptionCallbacksmad(
      &setException,
      &setException,
      &setException,
      &setException,
      &setException
    );
  }

  static void setException(const char* message) {
    auto exception = new object.Exception(std.conv.to!string(message));
    SwigPendingException.set(exception);
  }
}

package struct SwigPendingException {
public:
  static this() {
    m_sPendingException = null;
  }

  static bool isPending() {
    return m_sPendingException !is null;
  }

  static void set(object.Exception e) {
    if (m_sPendingException !is null) {
      e.next = m_sPendingException;
      throw new object.Exception("FATAL: An earlier pending exception from C/C++ code " ~
        "was missed and thus not thrown (" ~ m_sPendingException.classinfo.name ~
        ": " ~ m_sPendingException.msg ~ ")!", e);
    }

    m_sPendingException = e;
  }

  static object.Exception retrieve() {
    auto e = m_sPendingException;
    m_sPendingException = null;
    return e;
  }

private:
  // The reference to the pending exception (if any) is stored thread-local.
  static object.Exception m_sPendingException;
}
alias void function(const char* message) SwigExceptionCallback;


private class SwigStringHelper {
  static this() {
    swigRegisterStringCallbackmad(&createString);
  }

  static const(char)* createString(const(char*) cString) {
    // We are effectively dup'ing the string here.
    // TODO: Is this also correct for D2/Phobos?
    return std.string.toStringz(std.conv.to!string(cString));
  }
}
alias const(char)* function(const(char*) cString) SwigStringCallback;


template SwigExternC(T) if (is(typeof(*(T.init)) P == function)) {
  static if (is(typeof(*(T.init)) R == return)) {
    static if (is(typeof(*(T.init)) P == function)) {
      alias extern(C) R function(P) SwigExternC;
    }
  }
}

SwigExternC!(int function()) SIZEOF_INT_get;
SwigExternC!(int function()) SIZEOF_LONG_get;
SwigExternC!(int function()) SIZEOF_LONG_LONG_get;
SwigExternC!(int function()) MAD_VERSION_MAJOR_get;
SwigExternC!(int function()) MAD_VERSION_MINOR_get;
SwigExternC!(int function()) MAD_VERSION_PATCH_get;
SwigExternC!(const(char)* function()) MAD_VERSION_EXTRA_get;
SwigExternC!(const(char)* function()) MAD_VERSION_get;
SwigExternC!(const(char)* function()) MAD_PUBLISHYEAR_get;
SwigExternC!(const(char)* function()) MAD_AUTHOR_get;
SwigExternC!(const(char)* function()) MAD_EMAIL_get;
SwigExternC!(const(char)* function()) mad_version_get;
SwigExternC!(const(char)* function()) mad_copyright_get;
SwigExternC!(const(char)* function()) mad_author_get;
SwigExternC!(const(char)* function()) mad_build_get;
SwigExternC!(int function()) MAD_F_FRACBITS_get;
SwigExternC!(int function()) MAD_F_SCALEBITS_get;
SwigExternC!(int function(int jarg1)) mad_f_abs;
SwigExternC!(int function(int jarg1, int jarg2)) mad_f_div;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_bitptr__byte_set;
SwigExternC!(void* function(void* jarg1)) mad_bitptr__byte_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_bitptr_cache_set;
SwigExternC!(ushort function(void* jarg1)) mad_bitptr_cache_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_bitptr_left_set;
SwigExternC!(ushort function(void* jarg1)) mad_bitptr_left_get;
SwigExternC!(void* function()) new_mad_bitptr;
SwigExternC!(void function(void* jarg1)) delete_mad_bitptr;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_bit_init;
SwigExternC!(uint function(void* jarg1, void* jarg2)) mad_bit_length;
SwigExternC!(void* function(void* jarg1)) mad_bit_nextbyte;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_bit_skip;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1, uint jarg2)) mad_bit_read;
SwigExternC!(ushort function(void* jarg1, uint jarg2, ushort jarg3)) mad_bit_crc;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_long jarg2)) mad_timer_t_seconds_set;
SwigExternC!(core.stdc.config.c_long function(void* jarg1)) mad_timer_t_seconds_get;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_timer_t_fraction_set;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1)) mad_timer_t_fraction_get;
SwigExternC!(void* function()) new_mad_timer_t;
SwigExternC!(void function(void* jarg1)) delete_mad_timer_t;
SwigExternC!(void* function()) mad_timer_zero_get;
SwigExternC!(core.stdc.config.c_ulong function()) MAD_TIMER_RESOLUTION_get;
SwigExternC!(int function(void* jarg1, void* jarg2)) mad_timer_compare;
SwigExternC!(void function(void* jarg1)) mad_timer_negate;
SwigExternC!(void* function(void* jarg1)) mad_timer_abs;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2, core.stdc.config.c_ulong jarg3, core.stdc.config.c_ulong jarg4)) mad_timer_set;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_timer_add;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_long jarg2)) mad_timer_multiply;
SwigExternC!(core.stdc.config.c_long function(void* jarg1, int jarg2)) mad_timer_count;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_timer_fraction;
SwigExternC!(void function(void* jarg1, const(char)* jarg2, const(char)* jarg3, int jarg4, int jarg5, core.stdc.config.c_ulong jarg6)) mad_timer_string;
SwigExternC!(int function()) MAD_BUFFER_GUARD_get;
SwigExternC!(int function()) MAD_BUFFER_MDLEN_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_buffer_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_buffer_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_bufend_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_bufend_get;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_stream_skiplen_set;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1)) mad_stream_skiplen_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_stream_sync_set;
SwigExternC!(int function(void* jarg1)) mad_stream_sync_get;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_stream_freerate_set;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1)) mad_stream_freerate_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_this_frame_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_this_frame_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_next_frame_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_next_frame_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_ptr_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_ptr_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_anc_ptr_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_anc_ptr_get;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_stream_anc_bitlen_set;
SwigExternC!(uint function(void* jarg1)) mad_stream_anc_bitlen_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_stream_main_data_set;
SwigExternC!(void* function(void* jarg1)) mad_stream_main_data_get;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_stream_md_len_set;
SwigExternC!(uint function(void* jarg1)) mad_stream_md_len_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_stream_options_set;
SwigExternC!(int function(void* jarg1)) mad_stream_options_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_stream_error_set;
SwigExternC!(int function(void* jarg1)) mad_stream_error_get;
SwigExternC!(void* function()) new_mad_stream;
SwigExternC!(void function(void* jarg1)) delete_mad_stream;
SwigExternC!(void function(void* jarg1)) mad_stream_init;
SwigExternC!(void function(void* jarg1)) mad_stream_finish;
SwigExternC!(void function(void* jarg1, void* jarg2, core.stdc.config.c_ulong jarg3)) mad_stream_buffer;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_stream_skip;
SwigExternC!(int function(void* jarg1)) mad_stream_sync;
SwigExternC!(const(char)* function(void* jarg1)) mad_stream_errorstr;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_layer_set;
SwigExternC!(int function(void* jarg1)) mad_header_layer_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_mode_set;
SwigExternC!(int function(void* jarg1)) mad_header_mode_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_mode_extension_set;
SwigExternC!(int function(void* jarg1)) mad_header_mode_extension_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_emphasis_set;
SwigExternC!(int function(void* jarg1)) mad_header_emphasis_get;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_ulong jarg2)) mad_header_bitrate_set;
SwigExternC!(core.stdc.config.c_ulong function(void* jarg1)) mad_header_bitrate_get;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_header_samplerate_set;
SwigExternC!(uint function(void* jarg1)) mad_header_samplerate_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_header_crc_check_set;
SwigExternC!(ushort function(void* jarg1)) mad_header_crc_check_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_header_crc_target_set;
SwigExternC!(ushort function(void* jarg1)) mad_header_crc_target_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_flags_set;
SwigExternC!(int function(void* jarg1)) mad_header_flags_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_header_private_bits_set;
SwigExternC!(int function(void* jarg1)) mad_header_private_bits_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_header_duration_set;
SwigExternC!(void* function(void* jarg1)) mad_header_duration_get;
SwigExternC!(void* function()) new_mad_header;
SwigExternC!(void function(void* jarg1)) delete_mad_header;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_frame_header_set;
SwigExternC!(void* function(void* jarg1)) mad_frame_header_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_frame_options_set;
SwigExternC!(int function(void* jarg1)) mad_frame_options_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_frame_sbsample_set;
SwigExternC!(void* function(void* jarg1)) mad_frame_sbsample_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_frame_overlap_set;
SwigExternC!(void* function(void* jarg1)) mad_frame_overlap_get;
SwigExternC!(void* function()) new_mad_frame;
SwigExternC!(void function(void* jarg1)) delete_mad_frame;
SwigExternC!(void function(void* jarg1)) mad_header_init;
SwigExternC!(int function(void* jarg1, void* jarg2)) mad_header_decode;
SwigExternC!(void function(void* jarg1)) mad_frame_init;
SwigExternC!(void function(void* jarg1)) mad_frame_finish;
SwigExternC!(int function(void* jarg1, void* jarg2)) mad_frame_decode;
SwigExternC!(void function(void* jarg1)) mad_frame_mute;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_pcm_samplerate_set;
SwigExternC!(uint function(void* jarg1)) mad_pcm_samplerate_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_pcm_channels_set;
SwigExternC!(ushort function(void* jarg1)) mad_pcm_channels_get;
SwigExternC!(void function(void* jarg1, ushort jarg2)) mad_pcm_length_set;
SwigExternC!(ushort function(void* jarg1)) mad_pcm_length_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_pcm_samples_set;
SwigExternC!(void* function(void* jarg1)) mad_pcm_samples_get;
SwigExternC!(void* function()) new_mad_pcm;
SwigExternC!(void function(void* jarg1)) delete_mad_pcm;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_synth_filter_set;
SwigExternC!(void* function(void* jarg1)) mad_synth_filter_get;
SwigExternC!(void function(void* jarg1, uint jarg2)) mad_synth_phase_set;
SwigExternC!(uint function(void* jarg1)) mad_synth_phase_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_synth_pcm_set;
SwigExternC!(void* function(void* jarg1)) mad_synth_pcm_get;
SwigExternC!(void* function()) new_mad_synth;
SwigExternC!(void function(void* jarg1)) delete_mad_synth;
SwigExternC!(void function(void* jarg1)) mad_synth_init;
SwigExternC!(void function(void* jarg1)) mad_synth_mute;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_synth_frame;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_decoder_mode_set;
SwigExternC!(int function(void* jarg1)) mad_decoder_mode_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_decoder_options_set;
SwigExternC!(int function(void* jarg1)) mad_decoder_options_get;
SwigExternC!(void* function(void* jarg1)) mad_decoder_async_get;
SwigExternC!(void* function(void* jarg1)) mad_decoder_sync_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_cb_data_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_cb_data_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_input_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_input_func_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_header_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_header_func_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_filter_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_filter_func_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_output_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_output_func_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_error_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_error_func_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_message_func_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_message_func_get;
SwigExternC!(void* function()) new_mad_decoder;
SwigExternC!(void function(void* jarg1)) delete_mad_decoder;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_sync_stream_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_sync_stream_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_sync_frame_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_sync_frame_get;
SwigExternC!(void function(void* jarg1, void* jarg2)) mad_decoder_sync_synth_set;
SwigExternC!(void* function(void* jarg1)) mad_decoder_sync_synth_get;
SwigExternC!(void* function()) new_mad_decoder_sync;
SwigExternC!(void function(void* jarg1)) delete_mad_decoder_sync;
SwigExternC!(void function(void* jarg1, core.stdc.config.c_long jarg2)) mad_decoder_async_pid_set;
SwigExternC!(core.stdc.config.c_long function(void* jarg1)) mad_decoder_async_pid_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_decoder_async__in_set;
SwigExternC!(int function(void* jarg1)) mad_decoder_async__in_get;
SwigExternC!(void function(void* jarg1, int jarg2)) mad_decoder_async__out_set;
SwigExternC!(int function(void* jarg1)) mad_decoder_async__out_get;
SwigExternC!(void* function()) new_mad_decoder_async;
SwigExternC!(void function(void* jarg1)) delete_mad_decoder_async;
SwigExternC!(void function(void* jarg1, void* jarg2, void* jarg3, void* jarg4, void* jarg5, void* jarg6, void* jarg7, void* jarg8)) mad_decoder_init;
SwigExternC!(int function(void* jarg1)) mad_decoder_finish;
SwigExternC!(int function(void* jarg1, int jarg2)) mad_decoder_run;
SwigExternC!(int function(void* jarg1, void* jarg2, void* jarg3)) mad_decoder_message;