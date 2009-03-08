/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Mar 08 23:21:40 2009
 */
/* Compiler settings for D:\Wippien\Public\SDK\SDK.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IWippienSDK = {0xCAD56E0F,0x4F78,0x4D9B,{0xB4,0x25,0x92,0xB8,0xA5,0x36,0x1D,0x6D}};


const IID LIBID_WIPPIENSDKCOMLib = {0x4695EBF6,0x9789,0x4C5F,{0x81,0xE7,0x86,0xC7,0xFB,0x95,0xF2,0xCD}};


const IID DIID__IWippienSDKEvents = {0xBC5DC3B4,0x24B8,0x441A,{0xB2,0x4C,0x21,0x1C,0xF2,0x80,0xD3,0x1A}};


const CLSID CLSID_WippienSDK = {0x34AE1617,0xF394,0x4631,{0x8F,0x89,0xC2,0x91,0x69,0x1D,0x82,0x4F}};


#ifdef __cplusplus
}
#endif

