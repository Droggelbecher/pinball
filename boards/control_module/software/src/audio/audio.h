#ifndef audio_h_INCLUDED
#define audio_h_INCLUDED

#if WITH_OPENAL_AUDIO
	#include "audio/openal_audio.h"
#else
	#include "audio/mock_audio.h"
#endif

#endif // audio_h_INCLUDED

