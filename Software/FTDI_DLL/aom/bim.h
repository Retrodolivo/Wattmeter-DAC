#ifndef __BIM_H__
#define __BIM_H__

#ifdef BIM_EXPORTS
   #ifdef BIM_LIB_STATIC
      #define BIM_API
   #else
      #define BIM_API    __declspec(dllexport)
   #endif
#else
   #ifdef BIM_LIB_STATIC
      #define BIM_API 
   #else
      #define BIM_API    __declspec(dllimport)
   #endif
#endif

enum {
	BIM_OK,
	BIM_NOTFOUND,
	BIM_GETCHANNELS_ERROR,
	BIM_GETINFO_ERROR,
	BIM_OPEN_ERROR,
	BIM_INIT_ERROR,
	BIM_WRITE_ERROR,
	BIM_CLOSE_ERROR
};



#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

	BIM_API int BIM_Set(char* sn, unsigned char TM1, unsigned char TM2, unsigned char TM3, unsigned char TM4, unsigned char BM1);
 //sn: "BIM_0004"
	BIM_API int BIM_init(void);
//    BIM_API int Set_DAC_voltage(char *sn, unsigned char *data_arr);

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif //__BIM_H__