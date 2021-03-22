// aom.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "bim.h"

#include "libMPSSE_spi.h"



BOOL AnsiToUnicode(LPSTR pszAnsiString, LPWSTR pszwUniBuff, DWORD dwUniBuffSize)
{
	int iRet = 0;
	iRet = MultiByteToWideChar(CP_ACP, 0, pszAnsiString, -1, pszwUniBuff, dwUniBuffSize);
	return ( 0 != iRet );
}


const int tempsize = 255;
TCHAR temp[tempsize];

const int BUF_SIZE = 100;
UCHAR inbuf[BUF_SIZE] = {0};
UCHAR outbuf[BUF_SIZE] = {0};
DWORD byteswritten;


uint32 channels = 0;
FT_HANDLE ftHandle;
ChannelConfig channelConf = { 0 };
FT_DEVICE_LIST_INFO_NODE devList = { 0 };
uint32 i = 0;





///////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char reverse_bits(unsigned char v)
{
	unsigned char r = v; // r will be reversed bits of v; first get LSB of v
	int s = sizeof(v) * 8 - 1; // extra shift needed at end

	for (v >>= 1; v; v >>= 1)
	{
		r <<= 1;
		r |= v & 1;
		s--;
	}
	r <<= s; // shift when v's highest bits are zero
	return r;
}


///////////////////////////////////////////////////////////////////////////////////////

BIM_API int BIM_init(void)
{
	FT_STATUS status;

	/*REGESTER CHANNEL CONFIG*/
	channelConf.ClockRate = 5000000; //1MHz
	channelConf.LatencyTimer = 2;
	channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3;
	channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/

	///*DAC CHANNEL CONFIG*/
	//channelConf.ClockRate = 2500000;			//2500000 - 500kHz; 10000 - 2kHz
	//channelConf.LatencyTimer = 2; 
	//channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS7;
	//channelConf.Pin = 0x00000000;				/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/

	///*COMMON CHANNEL CONFIG*/
	//channelConf.ClockRate = 2500000;			//2500000 - 500kHz; 10000 - 2kHz
	//channelConf.LatencyTimer = 2;
	//channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS7;
	//channelConf.Pin = 0x00000000;				/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/

	Init_libMPSSE();

	status = SPI_GetNumChannels(&channels);
	if (status != FT_OK) return BIM_GETCHANNELS_ERROR;
	if (channels < 1) return BIM_NOTFOUND;

	for (i = 0; i < channels; i++)
	{
		status = SPI_GetChannelInfo(i, &devList);
		if (status != FT_OK) return BIM_GETINFO_ERROR;
		if (memcmp(devList.SerialNumber, "BIM_0004", 8) == 0) break;
		if (i == channels - 1) return BIM_NOTFOUND;
	}

	status = SPI_OpenChannel(i, &ftHandle);
	if (status != FT_OK) return BIM_OPEN_ERROR;

	status = SPI_InitChannel(ftHandle, &channelConf);
	if (status != FT_OK) return BIM_INIT_ERROR;

	return BIM_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////

BIM_API int BIM_Set(char* sn, unsigned char TM1, unsigned char TM2, unsigned char TM3, unsigned char TM4, unsigned char BM1)
{
	FT_STATUS status;
	unsigned char data[5] = { 0 };
	uint32 transferred = 0;

	data[0] = reverse_bits(BM1); data[1] = reverse_bits(TM4); data[2] = reverse_bits(TM3); data[3] = reverse_bits(TM2); data[4] = reverse_bits(TM1);

	status = SPI_Write(ftHandle, data, sizeof(data), &transferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);
	if (status != FT_OK) return 10;

	status = SPI_ToggleCS(ftHandle, 0);
	if (status != FT_OK) return 11;
	status = SPI_ToggleCS(ftHandle, 1);
	if (status != FT_OK) return 12;

	//	status = SPI_CloseChannel(ftHandle);
	//	if (status != FT_OK) return BIM_CLOSE_ERROR;

	//Cleanup_libMPSSE();

	return BIM_OK;
}

//BIM_API int Set_DAC_voltage(char *sn, unsigned char *data_arr)
//{
//	FT_STATUS status;
//	uint32 transferred = 0;
//
//	status = SPI_ToggleCS(ftHandle, 0);
//	if (status != FT_OK) return BIM_WRITE_ERROR;
//
//	status = SPI_Write(ftHandle, data_arr, sizeof(data_arr), &transferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);
//	if (status != FT_OK) return BIM_WRITE_ERROR;
//
//	status = SPI_ToggleCS(ftHandle, 1);
//	if (status != FT_OK) return BIM_WRITE_ERROR;
//
//	status = SPI_CloseChannel(ftHandle);
//	if (status != FT_OK) return BIM_CLOSE_ERROR;
//
//	Cleanup_libMPSSE();
//
//	return BIM_OK;
//}

///////////////////////////////////////////////////////////////////////////////////////////

//BIM_API int BIM_Clean_channel(void)
//{
//	FT_STATUS status;
//	static FT_HANDLE ftHandle;
//
//	Cleanup_libMPSSE();
//	status = SPI_CloseChannel(ftHandle);
//	if (status != FT_OK) return BIM_CLOSE_ERROR;
//}



