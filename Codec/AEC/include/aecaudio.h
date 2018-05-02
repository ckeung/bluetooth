/********************************************************************************************
*
*   FileName    : aecaudio.h
*   Description :
*
*********************************************************************************************
*
*   TCC Version 1.0
*   Copyright (c) Telechips Inc.
*   All rights reserved
*
*   This source code contains confidential information of Telechips.
*   Any unauthorized use without a written permission of Telechips including not limited
*   to redistribution in source or binary form is strictly prohibited.
*   This source code is provided "AS IS" and nothing contained in this source code shall
*   constitute any express or implied warranty of any kind, including without limitation,
*   any warranty of merchantability, fitness for a particular purpose or non-infringement
*   of any patent, copyright or other third party intellectual property right.
*   No warranty is made, express or implied, regarding the information's accuracy,
*   completeness, or performance.
*   In no event shall Telechips be liable for any claim, damages or other liability arising
*   from, out of or in connection with this source code or the use in the source code.
*   This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
*   between Telechips and Company.
*
********************************************************************************************/

#ifndef _AEC_AUDIO_H_
#define _AEC_AUDIO_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <sched.h>

#include <alsa/asoundlib.h>
#include <sys/time.h>
#include <math.h>


/* define */
#ifndef NULL
#define NULL (_void_t *)0
#endif

#if( !defined( Min ) )
#define	Min( X, Y )		( ( (X) < (Y) ) ? (X) : (Y) )
#endif

#if( !defined( Max ) )
#define	Max( X, Y )		( ( (X) > (Y) ) ? (X) : (Y) )
#endif
#define Clamp( x, a, b )		Max( (a), Min( (b), (x) ) )

#define VOLUME_MIN					0
#define VOLUME_MAX					10
#define VOLUME_RATIO				10

#define	DUMMY_BUFFER_SIZE			64
#define AEC_AUDIO_NOERR				0

/* state */
typedef enum
{
    AEC_AUDIO_STATE_NONE,
    AEC_AUDIO_STATE_INIT,
    AEC_AUDIO_STATE_START,
    AEC_AUDIO_STATE_STOP
} aec_audio_state;

/* loc */
typedef enum
{
    AEC_AUDIO_LOC_NEAREND,
    AEC_AUDIO_LOC_FAREND
} aec_audio_loc;

typedef enum
{
	TSTAMP_MSEC,
	TSTAMP_USEC,
	TSTAMP_NSEC
} audio_tstamp;

typedef	enum SIMPLE_BUFFER_STATE
{
	BUFFER_EMPTY = 1,
	BUFFER_NORMAL,
	BUFFER_FULL
} S_STATE;

typedef struct _audio_attribute_t
{
	_uint32_t	samplerate;
	_uint32_t	periodlength;
	_uint32_t	periodcount;
	_uint32_t	framelength;
	_uint32_t	framecount;
} audio_attribute_t /*__attribute__ ((aligned(sizeof(_size_t))))*/;

typedef struct _audio_buffer_t
{
	pthread_mutex_t  mutex;
	_uint32_t 	mHead,mTail;
	_uint32_t	mMaxBufferSize;
	_char_t  *ptr;
	_char_t  *tmpptr;
	S_STATE mState;
} audio_buffer_t __attribute__ ((aligned(sizeof(_size_t))));

typedef struct _audio_stream_t
{
	// stream
	_const_t _char_t  *		device;

	snd_pcm_t *			handle;
	snd_pcm_stream_t	direction;
	snd_pcm_format_t	format;
	snd_pcm_access_t	access;
	_uint32_t		resample;
	_uint32_t		sample_rate;
	_uint32_t		channels;

	snd_pcm_uframes_t	buffer_size;
	_uint32_t		period_count;
	snd_pcm_uframes_t	period_size;
	_uint32_t		bytes_frame;

	// poll
	struct pollfd* 		poll_events;
	_int32_t					poll_count;
	_int32_t					piperd;
	_int32_t					pipewr;
	
    // thread
	pthread_t			thread;
	pthread_t *			thread_ptr;

	_void_t *				parent_handle;

	// stream buffer
	_char_t  *				buffer;    
	_char_t  *				tmpbuffer;
    _char_t  *              leftbuffer;
    _char_t  *              rightbuffer;

	_uchar_t 		start;
	_uchar_t 		done;

	// sync
	_uint64_t  framecount;	

	snd_htimestamp_t 	tstamp;
	snd_htimestamp_t 	trigger_tstamp;
	snd_htimestamp_t 	audio_tstamp;	
	snd_pcm_sframes_t 	avail;
	snd_pcm_sframes_t 	delay;
}audio_stream_t __attribute__ ((aligned(sizeof(_size_t))));

typedef struct _aec_audio_t
{
	audio_stream_t		playback;
	audio_stream_t		capture;
	audio_buffer_t		buffer;
	aec_audio_state		state;
	aec_audio_loc		loc;

	// sync
	pthread_mutex_t 	lock;
	_short_t 			dummybuffer[DUMMY_BUFFER_SIZE];
	unsigned int		pb_sync;
	unsigned int		cp_sync;
}aec_audio_t __attribute__ ((aligned(sizeof(_size_t))));

/* extern function */
_extern_t _int32_t _aec_nr_start(unsigned int uiSampleRate, unsigned int uiFrameLength);
_extern_t _int32_t _aec_nr_process(unsigned int frames, short* NESBuf);
_extern_t _int32_t _aec_nr_stop(void);
_extern_t _int32_t _aec_audio_start(_void_t **handle,
			                        _const_t _char_t *playback_device,
			                        _uint32_t playback_samplerate,
			                        _const_t _char_t *capture_device,
			                        _uint32_t capture_samplerate,
			                        _uint32_t aec_samplerate,
			                        aec_audio_loc loc);
_extern_t _int32_t _aec_audio_stop(_void_t **handle);
_extern_t _int32_t _aec_audio_status(_void_t **handle,			/* status */
							_uint64_t *playback_framecount,_uint64_t *playback_htstamp,
							_uint64_t *capture_framecount,_uint64_t *capture_htstamp);	
_extern_t _int32_t _aec_audio_status_realtime(_void_t **handle,	/* status */
							_uint64_t *playback_framecount,_uint64_t *playback_htstamp,
							_uint64_t *capture_framecount,_uint64_t *capture_htstamp);

_extern_t _bool_t _aec_audio_valid_spacesize(_void_t *handle,_uint32_t size);
_extern_t _bool_t _aec_audio_valid_datasize(_void_t *handle,_uint32_t size);
_extern_t _bool_t _aec_audio_writedata(_void_t *handle,_void_t *data,_uint32_t size);
_extern_t _bool_t _aec_audio_readdata(_void_t *handle,_void_t *data,_uint32_t size);

_extern_t _int32_t aec_audio_start(_const_t _char_t *nplayback_device,
									_uint32_t nplayback_samplerate,
			                        _const_t _char_t *ncapture_device,
			                        _uint32_t ncapture_samplerate,                        
									_const_t _char_t *fplayback_device,
			                        _uint32_t fplayback_samplerate,
			                        _const_t _char_t *fcapture_device,
			                        _uint32_t  fcapture_samplerate,
			                        _uint32_t aec_samplerate);																	
_extern_t _int32_t aec_audio_stop(void);
_extern_t _int32_t aec_audio_status(_uint64_t *nplayback_framecount,_uint64_t *nplayback_htstamp,
							_uint64_t *ncapture_framecount,_uint64_t *ncapture_htstamp,
							_uint64_t *fplayback_framecount,_uint64_t *fplayback_htstamp,
							_uint64_t *fcapture_framecount,_uint64_t *fcapture_htstamp);	
_extern_t _int32_t aec_audio_status_realtime(_uint64_t *nplayback_framecount,_uint64_t *nplayback_htstamp,
							_uint64_t *ncapture_framecount,_uint64_t *ncapture_htstamp,
							_uint64_t *fplayback_framecount,_uint64_t *fplayback_htstamp,
							_uint64_t *fcapture_framecount,_uint64_t *fcapture_htstamp);
							
/* utils */
_extern_t _int32_t aec_audio_get_state(_void_t *handle, aec_audio_state *state);

#if defined(__cplusplus)
};
#endif

#endif


