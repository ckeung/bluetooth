/****************************************************************************
 *   FileName    : hfring.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.

*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    15/05/08     0.1            created                     Hwang, Jin Ho.
*******************************************************************************/

#ifndef _HF_RING_H_
#define _HF_RING_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "hfringtype.h"

typedef enum
{
    HF_RING_NONE,
    HF_RING_INIT,
    HF_RING_START,
    HF_RING_STOP
} hf_ring_state;

typedef struct
{
    _const_t _short_t	*data;
    _ulong_t 			len;
} hf_ring_array;


_extern_t _int_t hf_ring_start(_const_t _char_t *playback_device,
                        _uint_t playback_samplerate,_const_t _short_t *ringdata,_ulong_t ringlen);
                        
_extern_t _int_t hf_ring_stop(_void_t);

_extern_t _int_t hf_ring_get_state(_void_t *handle, hf_ring_state *state);

#if defined(__cplusplus)
};
#endif

#endif

