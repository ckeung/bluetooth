#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "TcBtMusic.h"
#include "TcBtEvt.h"
#include "BtApp_Av.h"
static bool bStart = FALSE;
#define PLAY_START  68
TcBtMusic::TcBtMusic(QObject *parent) :
    QObject(parent)
{
      m_BtmusicSignal = new TcBtMusicSignal;
      memset(AvrcpPasValidList, 0, sizeof(AvrcpPasValidList));
      memset(AvrcpPasCurrValue, 0, sizeof(AvrcpPasCurrValue));
      connect(m_BtmusicSignal, SIGNAL(AV_Event(int , void *)), this, SLOT(OnAVHandler(int , void *)), Qt::QueuedConnection);
}
TcBtMusic ::~TcBtMusic()
{

}
TcBtMusic *TcBtMusic::getInstance()
{
    static TcBtMusic tcmusic;
    return &tcmusic;
}
void  TcBtMusic::requeseBtMusicAudioSource()
{
   emit btRequestBtMusicAudioSource();
}
void  TcBtMusic::releaseBtMusicAudioSource()
{
   emit btReleaseBtMusicAudioSource();
}
bool TcBtMusic::openBtMusic()
{
    BtApp_AV_CT_MetadataInit();
   // BtApp_AV_SNK_EnterBTAMode();
    BtApp_AV_SNK_AudioStart();
   //emit btRequestBtMusicAudioSource();
}
bool TcBtMusic::closeBtMusic()
{
    BtApp_AV_SNK_AudioEnd();
   // emit btReleaseBtMusicAudioSource();
}

void TcBtMusic::cmdMusicStart(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_PLAY,  0);
}
void TcBtMusic::cmdMusicStop(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_PAUSE, 0);
}
void TcBtMusic::cmdMusicNext(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_FORWARD, false);
}
void TcBtMusic::cmdMusicPre(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_BACKWARD, false);
}
void TcBtMusic::cmdMusicFF(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_FFORWARD, false);
}
void TcBtMusic::cmdMusicREW(void)
{
    BtApp_AV_CT_KeySend(BTAPP_AV_CT_KEY_ID_REWIND, false);
}
void TcBtMusic::cmdSetRepeatMode(int mode)
{
    unsigned char setting_list[BTAPP_AV_CT_PAS_ATT_END], count;

#if 0	/* Exception Case : Samsung Galaxy Series */
    memcpy(setting_list, pstBTAudioAppVar->UiBT_AVInfo.AvrcpPasCurrValue, sizeof(setting_list));
#else
    memset(setting_list, 0, sizeof(setting_list));
#endif

    for(count = AvrcpPasCurrValue[BTAPP_AV_CT_PAS_REPEAT]; count < BTAPP_AV_CT_PAS_REPEAT_GROUP; count++)
    {
        if(AvrcpPasValidList[BTAPP_AV_CT_PAS_REPEAT] & (1 << count))
        {
            setting_list[BTAPP_AV_CT_PAS_REPEAT] = (count + 1);
            break;
        }
    }
    if(count == BTAPP_AV_CT_PAS_REPEAT_GROUP)
    {
        setting_list[BTAPP_AV_CT_PAS_REPEAT] = BTAPP_AV_CT_PAS_REPEAT_OFF;
    }
    switch(mode)
    {
        case 0:
            setting_list[BTAPP_AV_CT_PAS_REPEAT] = BTAPP_AV_CT_PAS_REPEAT_OFF;
        break;
        case 1:
    setting_list[BTAPP_AV_CT_PAS_REPEAT] = BTAPP_AV_CT_PAS_REPEAT_SINGLE;
        break;
        case 2:
            setting_list[BTAPP_AV_CT_PAS_REPEAT] = BTAPP_AV_CT_PAS_REPEAT_ALL;
        break;
    default:
        break;
    }
    fprintf(stderr,"come to here %s    %d     mode %d\n",__FUNCTION__,__LINE__,setting_list[BTAPP_AV_CT_PAS_REPEAT] );
   // setting_list[BTAPP_AV_CT_PAS_REPEAT] = BTAPP_AV_CT_PAS_REPEAT_SINGLE;
#if defined(BT_AUDIO_SINK_AVRCP_PAS)
    BtApp_AV_CT_PasSetSeetings(setting_list);
#endif
}

void TcBtMusic::cmdSetShuffleMode(int mode)
{
    unsigned char setting_list[BTAPP_AV_CT_PAS_ATT_END], count;

#if 0	/* Exception Case : Samsung Galaxy Series */
    memcpy(setting_list, pstBTAudioAppVar->UiBT_AVInfo.AvrcpPasCurrValue, sizeof(setting_list));
#else
    memset(setting_list, 0, sizeof(setting_list));
#endif

    for(count = AvrcpPasCurrValue[BTAPP_AV_CT_PAS_SHUFFLE]; count < BTAPP_AV_CT_PAS_SHUFFLE_GROUP; count++)
    {
        if(AvrcpPasValidList[BTAPP_AV_CT_PAS_SHUFFLE] & (1 << count))
        {
            setting_list[BTAPP_AV_CT_PAS_SHUFFLE] = (count + 1);
            break;
        }
    }
    if(count == BTAPP_AV_CT_PAS_SHUFFLE_GROUP)
    {
        setting_list[BTAPP_AV_CT_PAS_SHUFFLE] = BTAPP_AV_CT_PAS_SHUFFLE_OFF;
    }
    switch(mode)
    {
        case 0:
            setting_list[BTAPP_AV_CT_PAS_SHUFFLE] = BTAPP_AV_CT_PAS_SHUFFLE_OFF;
        break;
        case 1:
            setting_list[BTAPP_AV_CT_PAS_SHUFFLE] = BTAPP_AV_CT_PAS_SHUFFLE_ALL;
        break;
        case 2:
            setting_list[BTAPP_AV_CT_PAS_SHUFFLE] = BTAPP_AV_CT_PAS_SHUFFLE_GROUP;
        break;
    default:
        break;
    }
#if defined(BT_AUDIO_SINK_AVRCP_PAS)
    BtApp_AV_CT_PasSetSeetings(setting_list);
#endif
}
void TcBtMusic::OnAVHandler(int type, void *pTcBtEvt)
{
    if ((type != 34) &&(type != 35) &&(type != 36) )
            printf("===========OnAVHandler===type = %d=============\n",type);
    switch (type)
    {
    /*------ A2DP Sink De/Activation ---------------------------------------------*/
        case BT_AV_EVT_SNK_ACTIVATE:
        {
        }break;
        case BT_AV_EVT_SNK_DEACTIVATE:
        {

        }break;

    /*------ A2DP Sink Dis/Connection --------------------------------------------*/
        case BT_AV_EVT_SNK_CONNECT:
        {
            emit btavSinkConnected();
        }break;
        case BT_AV_EVT_SNK_DISCONNECT:
        {
            emit btavSinkDisConnected();
        }break;
     case BT_AV_EVT_SNK_START:
     {
        bStart = true;
        BtA2dpEvt *pTcBtA2dpEvt = (BtA2dpEvt *)pTcBtEvt;
       // fprintf(stderr,"   =====BT_AV_EVT_SNK_START  reslut%d\n     ",pTcBtA2dpEvt->avResultFlag);
         emit   btgetPlayStatus(PLAY_START);
     }
     break;
     case BT_AV_EVT_SNK_CLOSE:
     {
        bStart = false;
        BtA2dpEvt *pTcBtA2dpEvt = (BtA2dpEvt *)pTcBtEvt;
       // fprintf(stderr,"   =====BT_AV_EVT_SNK_CLOSE  reslut%d\n     ",pTcBtA2dpEvt->avResultFlag);
        emit btgetPlayStatus(PLAY_START + 1);
     }
     break;
    /* this event is used to inform whether the stream data is bypassing or not */
        case BT_AV_EVT_SNK_BYPASS:
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)pTcBtEvt;
           // printf("[%s : %05d] Info : BT_AV_EVT_SNK_BYPASS : %d\n", __FILENAME__, __LINE__,*pTcBtAVEvt);

        }break;
    /*------ AVRCP Controller De/Activation --------------------------------------*/
    /* This is tied to A2DP Sink */

    /*------ AVRCP Controller Dis/Connection -------------------------------------*/
        case BT_AV_EVT_CT_CONNECT:
        {


        }break;
        case BT_AV_EVT_CT_DISCONNECT:
        {


        }break;

    /*------ AVRCP Cotroller Key -------------------------------------------------*/
        case BT_AV_EVT_CT_KEY_RESULT :
        {
                TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
                   unsigned int key_result, key_operation, key_state;

                   memcpy(&key_operation, &pTcBtAVEvt[0], 4);
                   memcpy(&key_state,     &pTcBtAVEvt[4], 4);
                   memcpy(&key_result,    &pTcBtAVEvt[8], 4);
                   //printf("[%s : %06d] Info : BT_AV_EVT_CT_KEY_RESULT : 0x%x 0x%x %d (0:fail, 1:success)\n", __FILENAME__, __LINE__, key_operation, key_state, key_result);
                     emit   btgetPlayStatus(key_operation);

        }  break;

    /*------ AVRCP Cotroller Player Application Setting --------------------------*/
        case BT_AV_EVT_CT_PAS_SUPPORT :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned int count;

            for(count = 0; count < BTAPP_AV_CT_PAS_ATT_END; count++)		/* Array[0] should not be used */
            {
            //    printf("[%s : %06d] Info : BT_AV_EVT_CT_PAS_SUPPORT : 0x%02x\n", __FUNCTION__, __LINE__, pTcBtAVEvt[count]);
                AvrcpPasValidList[count] = pTcBtAVEvt[count];
            }
        #if defined(BT_AUDIO_SINK_AVRCP_PAS)
            /* Example : Get Current Player Application Setting Values */
            for(count = 0; count < BTAPP_AV_CT_PAS_ATT_END; count++)
            {
                if(AvrcpPasValidList[count] > 0)
                {
                    BtApp_AV_CT_PasGetSeetings();
                    break;
                }
            }
        #endif
        }break;
        case BT_AV_EVT_CT_PAS_GET_SETTINGS :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned int count;

            for(count = 0; count < BTAPP_AV_CT_PAS_ATT_END; count++)		/* Array[0] should not be used */
            {
           //     fprintf(stderr,"[%s : %06d] Info : BT_AV_EVT_CT_PAS_GET_SETTINGS : 0x%02x\n", __FUNCTION__, __LINE__, pTcBtAVEvt[count]);
                AvrcpPasCurrValue[count] = pTcBtAVEvt[count];
            }

        #if defined(BT_AUDIO_SINK_AVRCP_PAS)
            /* Example : Display Current Player Application Setting Values */
            emit   btchangeRepeatMode(AvrcpPasCurrValue[BTAPP_AV_CT_PAS_REPEAT]);
            emit  btchangeShuffleMode(AvrcpPasCurrValue[BTAPP_AV_CT_PAS_SHUFFLE]);
        #endif
        }break;
        case BT_AV_EVT_CT_PAS_SET_SETTINGS :
            //printf("[%s : %06d] Info : BT_AV_EVT_CT_PAS_SET_SETTINGS\n", __FILENAME__, __LINE__);

            break;
        case BT_AV_EVT_CT_PAS_GET_ATT_TEXT :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;

        }break;
        case BT_AV_EVT_CT_PAS_GET_VAL_TEXT :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
        }break;
        case BT_AV_EVT_CT_PAS_INFORM_CHAR_SET :
            //printf("[%s : %06d] Info : BT_AV_EVT_CT_PAS_INFORM_CHAR_SET\n", __FILENAME__, __LINE__);
            break;
        case BT_AV_EVT_CT_PAS_INFORM_BATTERY :
            //printf("[%s : %06d] Info : BT_AV_EVT_CT_PAS_INFORM_BATTERY\n", __FILENAME__, __LINE__);
            break;

        case BT_AV_EVT_CT_INFO_VER :
        {
            TcBtUint16 *pTcBtAVEvt = (TcBtUint16 *)pTcBtEvt;

        }break;
        case BT_AV_EVT_CT_NOTIFICATION :
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)pTcBtEvt;

        }break;
        case BT_AV_EVT_CT_SET_VOLUME :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;

        }break;

    /*------ AVRCP Cotroller Metadata --------------------------------------------*/
        case BT_AV_EVT_CT_META_SUPPORT :
        {



        } break;
        case BT_AV_EVT_CT_ELEM_TITLE :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned char size = (strlen((const char *)pTcBtAVEvt)+1);
            unsigned char AVNameOfTitle[64];	/* Title of the media */

            memcpy(AVNameOfTitle, pTcBtAVEvt, size);
            //const QChar *data =(const QChar *)(AVNameOfTitle);
            QString title((char *)AVNameOfTitle);
            emit btgetMetaDataTitle(title);

        }break;
        case BT_AV_EVT_CT_ELEM_ARTIST :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned char size = (strlen((const char *)pTcBtAVEvt)+1);
            unsigned char AVNameOfArtist[64];	/* Title of the media */

            memcpy(AVNameOfArtist, pTcBtAVEvt, size);
           // const QChar *data =(const QChar *)(AVNameOfArtist);
            QString artist((char *)AVNameOfArtist);
            emit btgetMetaDataArtist(artist);
        }break;
        case BT_AV_EVT_CT_ELEM_ALBUM :
        {

            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned char size = (strlen((const char *)pTcBtAVEvt)+1);
            unsigned char AVNameOfAlbum[64];	/* Title of the media */

            memcpy(AVNameOfAlbum, pTcBtAVEvt, size);
            const QChar *data =(const QChar *)(AVNameOfAlbum);
            QString album((char *)AVNameOfAlbum);
            emit btgetMetaDataAlbum(album);
        }break;
        case BT_AV_EVT_CT_ELEM_GENRE :
        {
            TcBtUint8 *pTcBtAVEvt = (TcBtUint8 *)pTcBtEvt;
            unsigned char size = (strlen((const char *)pTcBtAVEvt)+1);
            unsigned char AVNameOfGenre[64];	/* Title of the media */

            memcpy(AVNameOfGenre, pTcBtAVEvt, size);
           // const QChar *data =(const QChar *)(AVNameOfGenre);
            QString genre((char *)AVNameOfGenre);
            emit btgetMetaDataGenre(genre);
        } break;
        case BT_AV_EVT_CT_ELEM_TRACK :
        {
            int track = *(TcBtUint8 *)pTcBtEvt;
            emit btgetMetaDataTrack(track);

        }break;
        case BT_AV_EVT_CT_ELEM_TOTAL :
        {


        }break;
        case BT_AV_EVT_CT_ELEM_TIME :
        {
        int time =* (int *)pTcBtEvt;
        emit btgetTotalTime(time/1000);

        }  break;
        case BT_AV_EVT_CT_PLAY_LENGTH :
        {
            int len = *(int *)pTcBtEvt;
            emit btgetTotalLength(len);

        }break;
        case BT_AV_EVT_CT_PLAY_POSITION :
        {

            int postion =* (int *)pTcBtEvt;
              // fprintf(stderr,"   =====BT_AV_EVT_CT_PLAY_POSITION  postion%d\n     ",postion/1000);
            emit btgetCurrentPostion(postion/1000);
        } break;
        case BT_AV_EVT_CT_PLAY_STATUS :
        {
             int status = *(int *)pTcBtEvt;
           //  fprintf(stderr,"   =====BT_AV_EVT_CT_PLAY_STATUS  status%d\n     ",status);
             if (status == 1)
             {
                    emit btgetPlayStatus(PLAY_START);
             }else if (status == 2)
             {
                   emit btgetPlayStatus(PLAY_START + 1);
             }
             else
             {

             }
        }   break;

    /*------ AVRCP Controller Browsing --------------------------------------------*/
        case BT_AV_EVT_CT_BROWSE_SUPPORT :
        {

        } break;
        case BT_AV_EVT_CT_PLAYER_CHANGED:
            fprintf(stderr,"[%s : %06d] Info : BT_AV_EVT_CT_PLAYER_CHANGED\n", __FUNCTION__, __LINE__);

            break;
        case BT_AV_EVT_CT_SET_PLAYER_RESULT:
        {

            break;
        }
        case BT_AV_EVT_CT_CHANGE_PATH_RESULT:
        {

            break;
        }
        case BT_AV_EVT_CT_FOLDER_ITEMS:
        {


        }  break;
        case BT_AV_EVT_CT_PLAY_ITEM_RESULT:
        {
            TcBtUint32 *pTcBtAVEvt = (TcBtUint32 *)pTcBtEvt;
            printf("[%s : %06d] Info : BT_AV_EVT_CT_PLAY_ITEM_RESULT : %d (0:fail, 1:success)\n", __FUNCTION__, __LINE__, *pTcBtAVEvt);
            break;
        }
        case BT_AV_EVT_CT_SEARCH :
            printf("[%s : %06d] Info : BT_AV_EVT_CT_SEARCH\n", __FUNCTION__, __LINE__);
            break;
        case BT_AV_EVT_CT_ADD_TO_NPL :
            printf("[%s : %06d] Info : BT_AV_EVT_CT_ADD_TO_NPL\n", __FUNCTION__, __LINE__);
            break;
        case BT_AV_EVT_CT_MEDIA_DB_CHANGED:


            break;

        default:
        /*	printf("[%s : %06d] error : id : %d : %d\n", __FILENAME__, __LINE__, pstEvent->id, TccUISys_GetData4Bytes(data)); */
            break;

    }
}
