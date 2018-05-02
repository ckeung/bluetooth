/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2010 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */

#ifndef __TCC_LATM_DMX_H__
#define __TCC_LATM_DMX_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define LATM_DMX_VERSION "2.00.03"

// Initialization mode
#define	NORMAL_MODE	0
#define	CONFIG_MODE	1

typedef enum
{	
	// LATM/LOAS (Low Overhead Audio Stream): LATM with sync information
	TF_AAC_LOAS			= 0,	// default value

	// LATM (Low-overhead MPEG-4 Audio Transport Multiplex), without LOAS Sync-Layer, No random access is possible
	TF_AAC_LATM_MCP1	= 1,	// LATM wiht muxConfigPresent = 1
	TF_AAC_LATM_MCP0	= 2,	// LATM wiht muxConfigPresent = 0

	// ADTS (Audio Data Transport Stream)
	TF_AAC_ADTS			= 3,	

	// ADIF (Audio Data Interchange Format)
	TF_AAC_ADIF			= 4,	// not supported

	TF_UNKNOWN			= 0x7FFFFFFF	// Unknown format
}TransportFormat;
 
void* latm_parser_init(unsigned char *pucPacketDataPtr, 
					   unsigned int iDataLength, 
					   int *piSamplingFrequency, 
					   int *piChannelNumber, 
					   cdk_callback_func_t *pCallback,
					   TransportFormat eFormat
);

int latm_parser_get_frame( void *pLatmDmxHandle,
						unsigned char *pucPacketDataPtr, int iStreamLength,
						unsigned char **pucAACRawDataPtr, int *piAACRawDataLength,
						unsigned int uiInitFlag );

int latm_parser_close( void *pLatmDmxHandle );

int latm_parser_get_header_type( void *pLatmDmxHandle );
int latm_parser_flush_buffer(void *pLatmDmxHandle);

//int latm_parser_get_aac_config_info( void *pLatmDmxHandle, void **pConfig, int *pConfigLength );
//int latm_parser_check_parameter_change(void *pLatmDmxHandle);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
