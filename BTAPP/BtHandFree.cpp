//***************************header file************************//
#include <QDebug>
#include "BtHandFree.h"
#include "BtApp_HF.h"
#include "TcBtHFEvt.h"
#include "BtSys.h"
#include "TC_BT/inc/BtPlatform.h"
#include "BtApp_HfEvt.h"
#include "../inc/BtBase.h"
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
#include "aecif.h"
#include <alsa/asoundlib.h>
#endif
#endif
//****************************static param *****************//
static long previousModeVolume = -1;
//**************************static fun ********************//

//*****************************inter face *******************//
BtHandFree::BtHandFree()
{
}
BtHandFree::~BtHandFree()
{
}
static BtHandFree  _bthf;
BtHandFree *GetglobalBtHandFree()
{
    return &_bthf;
}

void BtHandFree::BtApp_HF_CmdOpenAudio(void)
{
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)

    //ALSA
    long outvol;
    int err;
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_channel_id_t channel;
    int i;
    char *card = "hw:0";
    if ((err = snd_mixer_open(&handle, 0)) < 0)
    {
        printf( "Mixer %s open error: %s", card);
        return;
    }
    if ((err = snd_mixer_attach(handle, card)) < 0)
    {
        printf("Mixer attach %s error: %s", card);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_selem_register(handle,NULL,NULL)) < 0 )
    {
        printf("ALSA: snd_mixer_selem_register failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_load(handle)) < 0 )
    {
        printf("ALSA: snd_mixer_load failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }
    elem = snd_mixer_first_elem(handle);
    if(!elem)
        return;

    for(i=0; i<SND_MIXER_SCHN_LAST; ++i)
    {
         channel = (snd_mixer_selem_channel_id_t)i;
         if(snd_mixer_selem_has_playback_channel(elem, channel))
         {
                err = snd_mixer_selem_get_playback_volume(elem,channel,&outvol);
                if( err ) continue;
                else
                {
                    previousModeVolume = outvol;
                }
                outvol=-1;
        }
    }
#endif
#endif
}

void BtHandFree::BtApp_HF_CmdCloseAudio(void)
{
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)

    int err;
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_channel_id_t channel;
    int i;
    long min=0, max=0;
    char *card = "hw:0";

    if (previousModeVolume < 0)
        return;

    if ((err = snd_mixer_open(&handle, 0)) < 0)
    {
        printf( "Mixer %s open error: %s", card);
        return;
    }
    if ((err = snd_mixer_attach(handle, card)) < 0)
    {
        printf("Mixer attach %s error: %s", card);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_selem_register(handle,NULL,NULL)) < 0 )
    {
        printf("ALSA: snd_mixer_selem_register failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_load(handle)) < 0 )
    {
        printf("ALSA: snd_mixer_load failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }

    elem = snd_mixer_first_elem(handle);
    if(!elem)
        return;

    if( (err=snd_mixer_selem_set_playback_volume_all(elem, previousModeVolume)) < 0 )
    {
        printf("ALSA: snd_mixer_selem_set_playback_volume_all failed: %d\n",err);
    }
    else
    {
        previousModeVolume = -1;
    }
#endif
#else
    gpTcBtHFInst->ModeOff(nextSrc, 0);
#endif

}
void BtHandFree::BtApp_HF_CmdSetVol(int value)
{
#ifdef __arm__
#if defined(BT_AEC_INCLUDE)
    long volume;
    long ALSA_volume;
    int err;
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;
    char *card = "hw:0";
    long min=0, max=0;

    volume = value;

    if ((err = snd_mixer_open(&handle, 0)) < 0)
    {
        printf( "Mixer %s open error: %s", card);
        return;
    }
    if ((err = snd_mixer_attach(handle, card)) < 0)
    {
        printf("Mixer attach %s error: %s", card);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_selem_register(handle,NULL,NULL)) < 0 )
    {
        printf("ALSA: snd_mixer_selem_register failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }
    if( (err=snd_mixer_load(handle)) < 0 )
    {
        printf("ALSA: snd_mixer_load failed: %d\n",err);
        snd_mixer_close(handle);
        return;
    }

    elem = snd_mixer_first_elem(handle);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);


    ALSA_volume = AECIf_GetSpeakerVolumeGain(volume);
    ALSA_volume= 50;
    printf("ALSA: snd_mixer_selem_set_playback_volume_all ALSA_volume: %d\n",ALSA_volume);
    if( (err=snd_mixer_selem_set_playback_volume_all(elem, ALSA_volume)) < 0 )
    {
        printf("ALSA: snd_mixer_selem_set_playback_volume_all failed: %d\n",err);
    }
#endif
#endif
}

void BtHandFree::BtApp_HF_CmdAccOn(void)
{
    BTAPP_Lock();
      //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
      BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdAccOff(void)
{
    BTAPP_Lock();
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdPwrOn(void)
{
    BTAPP_Lock();
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdPwrOff(unsigned char devno)
{
    BTAPP_Lock();
    int		iIdx;
    //unsigned char value;
    unsigned char end=FALSE;

    /* timer off */
    for (iIdx = TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_BEGIN,devno) ; iIdx <=TC_BT_TIMER_SET_HFP(TIMER_BT_HFx_END,devno) ; iIdx ++)
    {
        TCCAPI_Timer_CancelTime(iIdx);
    }
    /* audio off
        - BtApp_SYS_ClearStatus(BTAPP_STATUS_MASK_HF_CONNECT)
        - BtApp_SYS_ClearStatus(BTAPP_STATUS_MASK_HS_CONNECT)
    */
    {
        BtApp_HF_SetCurrDevNum(devno);

        /* handsfree */
        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_CALL))!=0)
        {
            end=TRUE;
            BtApp_HF_ReleaseAllHeldCalls();
            BtApp_HF_TerminateCall();
            BtApp_HF_SetCallEnd();
            //DMP_MessageEvtUpdate(DISK_DEVICE_BT_HF, BTAPP_EVT_MASK_SET_DEVNO(BT_HF_EVT_POWEROFF,devno));
        }

        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_RING))!=0)
        {
            end=TRUE;
            BtApp_HF_RejectCall();
            BtApp_HF_SetIRingEnd();
            //DMP_MessageEvtUpdate(DISK_DEVICE_BT_HF, BTAPP_EVT_MASK_SET_DEVNO(BT_HF_EVT_POWEROFF,devno));
        }

        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING))!=0)
        {
            end=TRUE;
            BtApp_HF_DrawCall();
            (void)G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_OUTGOING_BY_HF)!=0);
            BtApp_HF_SetOutgoingEnd();
      }

        if(BtApp_HF_VoiceRecognitionLearning()!=0)
        {
            end=TRUE;
            BtApp_HF_TerminateVoiceRecognition();
            BtApp_HF_Close_Call();
       }

        if(BtApp_HF_VoiceRecognition()!=0)
        {
            end=TRUE;
            BtApp_HF_TerminateVoiceRecognition();
            BtApp_HF_Close_Call();
     }

        if((G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HFx_AUDIO))!=0) && (end==FALSE))
        {
            end=TRUE;
            BtApp_HF_SetCallEnd();
       }

        /* headset */
        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HSx_RING))!=0)
        {
            end=TRUE;
            /* reserved */
            #ifdef BT_HFP_RING_FOR_NO_IN_BAND
            BtApp_HF_Close_Ring();
            #endif
      }

        if(G_BtSys->BtApp_SYS_ClearStatus(BTAPP_STATUS_SET_HFP(BTAPP_STATUS_HSx_AUDIO))!=0)
        {
            end=TRUE;
            /* reserved */
            //UI_BT_Close_HSAudio();
      }
    }
    if(end==FALSE)
    {
#if 0	/* Telechips Linux HF remark - porting */
        UI_DRV_StopPlay();
        UI_DRV_CloseTrack();
#endif
    }
    else
    {
        BtApp_HF_EvtPowerOff(devno);
    }
    /* Set forse stop when mode off, and release when mode on */

    BTAPP_Unlock();

}
/*activate*/
void BtHandFree::BtApp_HF_CmdUnitActivate(void)
{
    BTAPP_Lock();

    BtApp_HF_ActivateHandsfree();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdUnitDeactivate(void)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdUnitDeactivate",__FILE__,__LINE__);

    BtApp_HF_DeactivateHandsfree();

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
/*connect*/
void BtHandFree::BtApp_HF_CmdUnitConnect(TcBtBdAddr *pBdAddr)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdUnitConnect",__FILE__,__LINE__);

    BtApp_HF_ConnectHandsfree(pBdAddr);

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdUnitDisconnect(unsigned char devno)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdUnitDisconnect",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_DisconnectHandsfree();

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
/*menu*/
void BtHandFree::BtApp_HF_CmdOptionsAutoAnswer(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsAutoAnswer",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

/* Telechips Linux HFP remark - porting */
#if 1
    BtApp_HF_SetAutoAnswerIndex(value);
#endif

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsMicGain(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/
    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsMicGain",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

/* Telechips Linux HFP remark - porting */
#if 1
    BtApp_HF_SetDefaultMicGainIndex(value);
#endif

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsSMSAlarm(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/
    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsSMSAlarm",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

/* Telechips Linux HFP remark - porting */
#if 1
    BtApp_HF_SetSmsAlarmIndex(value);
#endif

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsRing(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsRing",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);
/* Telechips Linux HFP remark - porting */
#if 1
    BtApp_HF_SetRingPriorityIndex(value);
#endif

    /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsBuiltIn(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
   /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

   printf("[%s : %05d] Info : BtApp_HF_CmdOptionsBuiltIn",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);
/* Telechips Linux HFP remark - porting */
#if 1
   BtApp_HF_SetRingSelectIndex(value);
#endif

   /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsNREC(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
   /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

   printf("[%s : %05d] Info : BtApp_HF_CmdOptionsNREC",__FILE__,__LINE__);


   BtApp_HF_SetCurrDevNum(devno);

   if (value != 0)
   {
       BtApp_HF_EchoOn();
   }
   else
   {
       BtApp_HF_EchoOff();
   }

   /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsClcc(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsClcc",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    if (value != 0)
    {
        BtApp_HF_GetClcc();
    }

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsBia(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
   /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/
   printf("[%s : %05d] Info : BtApp_HF_CmdOptionsBia",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   if (value != 0)
   {
       BtApp_HF_SetBia();
   }

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOptionsBiev(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();

    printf("[%s : %05d] Info : BtApp_HF_CmdOptionsBiev",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    if (value != 0)
    {
        BtApp_HF_SetBiev();
    }

    BTAPP_Unlock();
}
/* status - DIAL */
void BtHandFree::BtApp_HF_CmdDialRedial(unsigned char devno)
{
    BTAPP_Lock();
   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   printf("[%s : %05d] Info : BtApp_HF_CmdDialRedial",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_SetDial();
   BtApp_HF_Redial();

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdDialPlaceByNumber(unsigned char devno, unsigned char size, unsigned char* data)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdDialPlaceByNumber",__FILE__,__LINE__);

    //pvalue=(unsigned char *)pData;

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetDial();

    gBtAppHfCallNumPos[devno] = size;	/* NUM / MEMORY Dial '>' */
    memcpy(gBtAppHfCallNumber[devno], data , size);

    BtApp_HF_InputNumberDial();

    BTAPP_Unlock();
}
/* status - VOICE */
 void BtHandFree::BtApp_HF_CmdVoiceRecognition(unsigned char devno)
 {
     BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognition",__FILE__,__LINE__);


    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetVoiceRecognition();

    BtApp_HF_SetVoiceRecognitionOnOff();

    BTAPP_Unlock();
 }
void BtHandFree::BtApp_HF_CmdVoiceRecognitionStart(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionStart",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetCallStart(BLUETOOTH_HF_VOICE_DIAL_ID);

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdVoiceRecognitionEnd(unsigned char devno)
{
    BTAPP_Lock();

   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   //unsigned char *pvalue;

   printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionEnd",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_Close_Call();

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdVoiceRecognitionLearn(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionLearn",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetVoiceRecognition();

    BtApp_HF_SetVoiceRecognitionLearning();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdVoiceRecognitionLearnStart(unsigned char devno)
{
    BTAPP_Lock();
   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   //unsigned char *pvalue;

   printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionLearnStart",__FILE__,__LINE__);


   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_SetCallStart(BLUETOOTH_HF_VOICE_DIAL_ID);

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdVoiceRecognitionLearnEnd(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionLearnEnd",__FILE__,__LINE__);


    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_Close_Call();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdVoiceRecognitionTerminate(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    //unsigned char *pvalue;

    printf("[%s : %05d] Info : BtApp_HF_CmdVoiceRecognitionTerminate",__FILE__,__LINE__);


    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_TerminateVoiceRecognition();

    BTAPP_Unlock();
}
/* status - OUTGOING */
void BtHandFree::BtApp_HF_CmdOutgoingStart(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdOutgoingStart",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetCallStart(BLUETOOTH_HF_OUTGOING_ID);

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOutgoingEnd(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdOutgoingEnd",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetOutgoingEnd();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdOutgoingDraw(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdOutgoingDraw",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_DrawCall();

    BTAPP_Unlock();
}
/* status - INCOMING */
void BtHandFree::BtApp_HF_CmdIncomingStart(unsigned char devno)
{
    BTAPP_Lock();

   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   printf("[%s : %05d] Info : BtApp_HF_CmdIncomingStart",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_SetRingStart(BLUETOOTH_HF_RING_ID);

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingInbandStart(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdIncomingInbandStart",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetCallStart(BLUETOOTH_HF_RING_ID);

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingAudioStart(unsigned char devno)
{
    BTAPP_Lock();

       //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

       printf("[%s : %05d] Info : BtApp_HF_CmdIncomingAudioStart",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

       BtApp_HF_SetAudioStart(BLUETOOTH_HF_RING_ID);

       BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingEnd(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdIncomingEnd",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetIRingEnd();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingAnswer(unsigned char devno)
{
    BTAPP_Lock();
   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   printf("[%s : %05d] Info : BtApp_HF_CmdIncomingAnswer",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_AnswerCall();

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingReject(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdIncomingReject",__FILE__,__LINE__);


    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_RejectCall();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdIncomingResponseAndHold(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdIncomingResponseAndHold",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    gBtAppHfResponseAndHoldControlIdx[devno] = value;

    BtApp_HF_ExcuteResponseAndHoldControl();

    BTAPP_Unlock();
}
/* status - CALL */
void BtHandFree::BtApp_HF_CmdCallStart(unsigned char devno)
{
    BTAPP_Lock();

        //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
        printf("[%s : %05d] Info : BtApp_HF_CmdCallStart",__FILE__,__LINE__);


        BtApp_HF_SetCurrDevNum(devno);

        BtApp_HF_SetCall();
        BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallEnd(unsigned char devno)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
    printf("[%s : %05d] Info : BtApp_HF_CmdCallEnd",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetCallEnd();
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallTerminate(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdCallTerminate",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_TerminateCall();
    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallAudioOnOff(unsigned char devno)
{
    BTAPP_Lock();
     //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

     printf("[%s : %05d] Info : BtApp_HF_CmdCallAudioOnOff",__FILE__,__LINE__);

     BtApp_HF_SetCurrDevNum(devno);

     BtApp_HF_AudioOnOff();
     BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallAudioAccept(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
        //pPLAYBACK_INFO pPlayback = Playback_API_GetPlaybackInfo(loc);

        printf("[%s : %05d] Info : BtApp_HF_CmdCallAudioAccept",__FILE__,__LINE__);

        BtApp_HF_SetCurrDevNum(devno);

        BtApp_HF_AudioAccept(value);
        BTAPP_Unlock();
}
 void BtHandFree::BtApp_HF_CmdCallMicMute(unsigned char devno)
 {
     BTAPP_Lock();
     //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

     printf("[%s : %05d] Info : BtApp_HF_CmdCallMicMute",__FILE__,__LINE__);

     BtApp_HF_SetCurrDevNum(devno);

 #ifdef BT_HFP_MIC_MUTE
 #if 1
     BtApp_HF_SetMicMuteStatus();
 #else
     BtApp_HF_MicMuteToggle();
 #endif
 #endif
     BTAPP_Unlock();
 }
void BtHandFree::BtApp_HF_CmdCallDTMF(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();

    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdCallDTMF",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_SetDtmf(value);

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdReleaseAllHeldCalls(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdReleaseAllHeldCalls",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_ReleaseAllHeldCalls();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallHoldAndAccept(unsigned char devno)
{
    BTAPP_Lock();
   //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

   printf("[%s : %05d] Info : BtApp_HF_CmdCallHoldAndAccept",__FILE__,__LINE__);

   BtApp_HF_SetCurrDevNum(devno);

   BtApp_HF_HoldAndAcceptCall();

   BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallReleaseAndAccept(unsigned char devno)
{
    BTAPP_Lock();
    //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

    printf("[%s : %05d] Info : BtApp_HF_CmdCallReleaseAndAccept",__FILE__,__LINE__);

    BtApp_HF_SetCurrDevNum(devno);

    BtApp_HF_ReleaseAndAcceptCall();

    BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallExcuteEnhancedCall(unsigned char devno, unsigned char value)
{
    BTAPP_Lock();
        //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

        printf("[%s : %05d] Info : BtApp_HF_CmdCallReleaseAndAccept",__FILE__,__LINE__);

        BtApp_HF_SetCurrDevNum(devno);

        gBtAppHfEnhancedCallControlIdx[devno] = value;

        BtApp_HF_ExcuteEnhancedCallControl();

        BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCallLookbackTest(unsigned char devno)
{
    BTAPP_Lock();
    #ifdef BT_HFP_LOOPBACK_TEST
        //pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

        printf("[%s : %05d] Info : BtApp_HF_CmdCallLookbackTest",__FILE__,__LINE__);

        BtApp_HF_SetCurrDevNum(devno);

        BtApp_HF_ToggleLoopbackTest();
    #endif
        BTAPP_Unlock();
}
#ifdef BT_HFP_UPDATE_SUPPORTED_CODEC
void BtHandFree::BtApp_HF_CmdCommonUpdateSupportedCodec(unsigned char ucMask, unsigned char boEnable)
{
    BTAPP_Lock();
        /*pPLAYBACK_INFO pPlayback = Playback_API_GetPlaybackInfo(loc);*/

        printf("[%s : %05d] Info : BtApp_HF_CmdCommonUpdateSupportedCodec",__FILE__,__LINE__);

        BtApp_HF_UpdateSupportedCodec((unsigned int)ucMask,boEnable);

        BTAPP_Unlock();
}
#endif
void BtHandFree::BtApp_HF_CmdCommonMicUp(unsigned char devno)
{
    BTAPP_Lock();
   //	pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
       printf("[%s : %05d] Info : BtApp_HF_CmdCommonMicUp",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

   #if 0	/* Telechips Linux HF remark - porting */
       if(gBtAppHfMicGainIndex[devno]<AEC_MIC_VOL_MAX)
   #else
       if(gBtAppHfMicGainIndex[devno]<15)
   #endif
       {
           gBtAppHfMicGainIndex[devno]=gBtAppHfMicGainIndex[devno]+1;
       }

       BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[devno]);
       BtApp_HF_MicUp();

       BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCommonMicDown(unsigned char devno)
{
    BTAPP_Lock();
   //	pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

       printf("[%s : %05d] Info : BtApp_HF_CmdCommonMicDown",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

       if(gBtAppHfMicGainIndex[devno]>0)
       {
           gBtAppHfMicGainIndex[devno]=gBtAppHfMicGainIndex[devno]-1;
       }

       BtApp_HF_SetMicGainIndex(gBtAppHfMicGainIndex[devno]);
       BtApp_HF_MicDown();

       BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCommonSpeakerUp(unsigned char devno)
{
    BTAPP_Lock();
    //	pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

        printf("[%s : %05d] Info : BtApp_HF_CmdCommonSpeakerUp",__FILE__,__LINE__);

        BtApp_HF_SetCurrDevNum(devno);

    #if 0	/* Telechips Linux HF remark - porting */
        if(gBtAppHfSpkVolIndex[devno]<AEC_SPK_VOL_MAX)
    #else
        if(gBtAppHfSpkVolIndex[devno]<15)
    #endif
        {
            gBtAppHfSpkVolIndex[devno]=gBtAppHfSpkVolIndex[devno]+1;
        }

        BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[devno]);
        BtApp_HF_SpeakerUp();

        BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdCommonSpeakerDown(unsigned char devno)
{
    BTAPP_Lock();
   //	pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);

       printf("[%s : %05d] Info : BtApp_HF_CmdCommonSpeakerDown",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

       if(gBtAppHfSpkVolIndex[devno]>0)
       {
           gBtAppHfSpkVolIndex[devno]=gBtAppHfSpkVolIndex[devno]-1;
       }

       BtApp_HF_SetSpkVolumeIndex(gBtAppHfSpkVolIndex[devno]);
       BtApp_HF_SpeakerDown();

       BTAPP_Unlock();
}
void BtHandFree::BtApp_HF_CmdGetInformation(unsigned char devno)
{
    BTAPP_Lock();
       /*pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);*/

       printf("[%s : %05d] Info : BtApp_HF_CmdGetInformation",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

   //	BtApp_SYS_Info_Device();

   #if 0
       DMP_MessageEvtUpdate(DISK_DEVICE_BT_HF, BTAPP_EVT_MASK_SET_DEVNO(BT_HF_EVT_GET_INFORMATION_INF,devno));
   #else
       BtApp_HF_EvtGetInformationInf(devno);
   #endif
       //UI_BT_SetCurrLCDUpdateFlag(BT_LCDUPDATE_HFx_GET_INFORMATION_INF);
       /*DMP_MessageEvtUpdate(BT_SYS_mCurrentDisk, BT_SYS_EVT_START_PLAY);*/
       BTAPP_Unlock();

}
void BtHandFree::BtApp_HF_CmdUtilSimulation(unsigned char devno)
{
    BTAPP_Lock();
   //	pPLAYBACK_INFO pPlayback = MP_GetPlayback(loc);
       //unsigned char value;

       printf("[%s : %05d] Info : BtApp_HF_CmdCommonSimulation",__FILE__,__LINE__);

       BtApp_HF_SetCurrDevNum(devno);

   //	AEC_Simulation();/* block to avoid error */
       BTAPP_Unlock();
}
