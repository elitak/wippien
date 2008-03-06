#ifndef __BUFFER_CPP
#define __BUFFER_CPP

#ifdef HAVESSL
#include "openssl/bn.h"
#endif

#define GET_32BIT(cp) (((unsigned long)(unsigned char)(cp)[0] << 24) | \
		       ((unsigned long)(unsigned char)(cp)[1] << 16) | \
		       ((unsigned long)(unsigned char)(cp)[2] << 8) | \
		       ((unsigned long)(unsigned char)(cp)[3]))

#define PUT_32BIT(cp, value) do { \
  (cp)[0] = (value) >> 24; \
  (cp)[1] = (value) >> 16; \
  (cp)[2] = (value) >> 8; \
  (cp)[3] =  (value); } while (0)

#define GET_16BIT(cp) (((unsigned short)(unsigned char)(cp)[0] << 8) | \
		       ((unsigned short)(unsigned char)(cp)[1]))

#define PUT_16BIT(cp, value) do { \
  (cp)[0] = (unsigned char)((value) >> 8); \
  (cp)[1] = (unsigned char)(value); } while (0)



class _Buffer
{


public:
		char	*m_buf;		/* Buffer for data. */
		unsigned int	 m_alloc;		/* Number of bytes allocated for data. */
		unsigned int	 m_offset;	/* Offset of first byte containing data. */
		unsigned int	 m_end;		/* Offset of last byte containing data. */

public:
	_Buffer();
	~_Buffer();
	void	 Clear();

	unsigned int	 Len();
	char	*Ptr();

	void	 Append(const char *, unsigned int);
	void	 Append(const char *);
	void	 AppendSpace(char **, unsigned int);

#ifdef HAVESSL
	void Append(BSTR);
#endif
	void	 Get(char *, unsigned int);
	void	 Peek(char *, unsigned int);

	void	 Consume(unsigned int);
	void	 ConsumeEnd(unsigned int);


	unsigned short GetShort();
	unsigned short PeekShort();
	unsigned char PeekChar();
	void PutShort(unsigned short);
	unsigned int GetInt();
	unsigned int PeekInt();
	void PutInt(unsigned int);

	int GetChar();
	void PutChar(int);

	char *GetString(unsigned int *);

	void PutString(const void *, unsigned int);
	void PutCString(const char *);
#ifdef HAVESSL
	void PutCString(BSTR);
#endif
#ifdef _USRDLL
	void PutCString(BSTR);
#endif
	char *GetNextLine();

};

#ifdef HAVESSL
class Buffer : public _Buffer
{
public:
	void PutBignum(BIGNUM *);
	void PutBignum2(BIGNUM *);
	int GetBignum(BIGNUM *);
	int GetBignum2(BIGNUM *);
	int GetBignumSSH(BIGNUM *);


	void AddChildElem(char *Name, int Value)
	{
		char buff[16384];
		sprintf(buff, "<%s>%d</%s>\r\n", Name, Value, Name);
		Append(buff);
	}
	void AddChildElem(char *Name, char * Value)
	{
		char buff[16384];
		if (strlen(Value))
			sprintf(buff, "<%s>%s</%s>\r\n", Name, Value, Name);
		else	
			sprintf(buff, "<%s/>\r\n", Name);
		Append(buff);
	}
	void AddChildAttrib(char *Name, char * Value, char *AttrName, char *AttrValue)
	{
		char buff[16384];
		sprintf(buff, "<%s %s=\"%s\">%s</%s>\r\n", Name, AttrName, AttrValue, Value, Name);
		Append(buff);
	}
	void AddChildAttrib(char *Name, char * Value, char *AttrName1, char *AttrValue1, char *AttrName2, char *AttrValue2)
	{
		char buff[16384];
		sprintf(buff, "<%s %s=\"%s\" %s=\"%s\">%s</%s>\r\n", Name, AttrName1, AttrValue1, AttrName2, AttrValue2, Value, Name);
		Append(buff);
	}
	void AddChildAttrib(char *Name, char * Value, char *AttrName1, int AttrValue1, char *AttrName2, int AttrValue2)
	{
		char buff[16384];
		sprintf(buff, "<%s %s=\"%d\" %s=\"%d\">%s</%s>\r\n", Name, AttrName1, AttrValue1, AttrName2, AttrValue2, Value, Name);
		Append(buff);
	}
};
#endif 


#endif
