#include <new>
#include <math.h>
#include "Serializer.h"


FrameSerializer::FrameSerializer()
: m_buf( NULL ), m_buflen( 0 ), m_curpos( 0 )
, m_nDataBufSize( 0 )
{
}

int	FrameSerializer::GetDataSize()
{
	return m_curpos;
}

bool FrameSerializer::NewMessage()
{
	m_nDataBufSize = 0;
	if( NULL == m_buf )
	{
		return false;
	}

	return true;
}

void FrameSerializer::Attach( unsigned char *ptr, size_t size )
{
	m_buf = ptr;
	m_buflen = size;
	m_curpos = 0;
}

FrameSerializer::operator char*()
{
	return (char*)m_buf;
}

int FrameSerializer::operator << ( const SerialString& val )
{
	int nbit = 0;

	if( m_curpos + val.nStrLen >= m_buflen )
	{
		return 0;
	}

	if( val.nStrLen <= 0 )
	{
		nbit = 1;

		if (m_curpos>= m_buflen)
		{
			return -1;
		}

		m_buf[m_curpos++] = 0x80;
	}

	//最后一字节高位有stop bit
	if (nbit <=0 && val.nStrLen >0) //还未赋值
	{
		memcpy( &m_buf[m_curpos], val.pszStr, val.nStrLen );
		m_curpos += val.nStrLen;

		m_buf[m_curpos-1] |= 0x80;///<last()
		nbit = val.nStrLen;
	}
	
	return nbit;
}

int FrameSerializer::operator << ( const SerialBytes& val )
{
	int nbit = 0;

	if( m_curpos + val.nStrLen >= m_buflen )
	{
		return 0;
	}

	if( val.nStrLen <= 0 )
	{
		nbit = 1;

		if (m_curpos>= m_buflen)
		{
			return -1;
		}

		m_buf[m_curpos++] = 0x80;
	}

	//先长度(int32),再原样数据
	if( nbit<=0 && val.nStrLen>0 )
	{
		if( (nbit = inner_encodeUInt64( val.nStrLen )) > 0 )
		{
			if( m_curpos + val.nStrLen >= m_buflen )
			{
				nbit = 0;
			}
			else
			{
				memcpy( &m_buf[m_curpos], val.pszStr, val.nStrLen );
				m_curpos += val.nStrLen;
				nbit += val.nStrLen;
			}
		}
	}
	
	return nbit;
}

int	FrameSerializer::operator << ( const SerialNumber& val )
{
	if( true == val.IsSigned() )
	{
		int		nbits = 0;

		if( m_curpos + 12 >= m_buflen )
		{
			return 0;
		}

		if( val.IsPositive() )
		{
			nbits =  inner_gt0_encodeint64( &m_buf[m_curpos], (__int64)val );
		}
		else
		{
			nbits =  inner_lt0_encodeint64( &m_buf[m_curpos], (__int64)val );
		}

		m_curpos += nbits;

		return nbits;
	}
	else
	{
		return inner_encodeUInt64( (__int64)val );
	}
}

int FrameSerializer::inner_encodeUInt64( const unsigned __int64 val )
{
	int				nbit = 0;
	unsigned char	*pBuf = m_buf + m_curpos;

	if( m_curpos + 12 >= m_buflen )
	{
		return 0;
	}

	if( val <      0x0000000000000080 )
	{
		nbit =1;
		*pBuf = (unsigned char)((val & 0x7f) | 0x80);
	}
	else if( val < 0x0000000000004000 )
	{
		nbit =2;
		*pBuf = (unsigned char)((val >> 7) & 0x7F);
		*(pBuf+1) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0000000000200000 )
	{
		nbit =3;
		*pBuf = (unsigned char)((val >> 14) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+2) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0000000010000000 )
	{
		nbit =4;
		*pBuf = (unsigned char)((val >> 21) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+3) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0000000800000000 ) // 1000 0000 0000 0000 0000 0000 0000 0000 0000
	{
		nbit =5;
		*pBuf = (unsigned char)((val >> 28) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+4) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0000040000000000 ) // 100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
	{
		nbit =6;
		*pBuf = (unsigned char)((val >> 35) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 28) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+4) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+5) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0002000000000000 ) //
	{
		nbit =7;
		*pBuf = (unsigned char)((val >> 42) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 35) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 28) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+4) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+5) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+6) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x0100000000000000 ) // 1 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
	{
		nbit =8;
		*pBuf = (unsigned char)((val >> 49) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 42) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 35) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 28) & 0x7F);
		*(pBuf+4) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+5) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+6) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+7) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else if( val < 0x8000000000000000 ) //
	{
		nbit =9;
		*pBuf = (unsigned char)((val >> 56) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 49) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 42) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 35) & 0x7F);
		*(pBuf+4) =(unsigned char)((val >> 28) & 0x7F);
		*(pBuf+5) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+6) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+7) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+8) =(unsigned char)((val & 0x7F) | 0x80);
	}
	else
	{
		nbit =10;
		*pBuf = (unsigned char)((val >> 63) & 0x7F);
		*(pBuf+1) =(unsigned char)((val >> 56) & 0x7F);
		*(pBuf+2) =(unsigned char)((val >> 49) & 0x7F);
		*(pBuf+3) =(unsigned char)((val >> 42) & 0x7F);
		*(pBuf+4) =(unsigned char)((val >> 35) & 0x7F);
		*(pBuf+5) =(unsigned char)((val >> 28) & 0x7F);
		*(pBuf+6) =(unsigned char)((val >> 21) & 0x7F);
		*(pBuf+7) =(unsigned char)((val >> 14) & 0x7F);
		*(pBuf+8) =(unsigned char)((val >> 7) & 0x7F);
		*(pBuf+9) =(unsigned char)((val & 0x7F) | 0x80);
	}

	m_curpos += nbit;

	return nbit;
}

int FrameSerializer::inner_gt0_encodeint64( unsigned char * buf, __int64 val64 )
{
	if( val64 < 0x0000000000000040 )
    {
		*buf = (unsigned char)((val64 & 0x7F) | 0x80);// .... .... .... ..7f
		return 1;
    }
    else if( val64 < 0x0000000000002000 )
    {
		*buf = (unsigned char)((val64 >> 7) & 0x7F);// .... .... .... 3F8.
		*(buf+1) = (unsigned char)((val64 & 0x7F) | 0x80);// .... .... .... ..7f
		return 2;
    }
    else if( val64 < 0x0000000000100000 )
    {
		*buf = (unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+1) =(unsigned char)((val64 >> 7)  & 0x7F); // .... .... .... 3F8.
		*(buf+2) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 3;
    }
    else if( val64 < 0x0000000008000000 )
    {
		*buf = (unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+1) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+2) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+3) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 4;
    }
    else if( val64 < 0x0000000400000000 )
    {
		*buf = (unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+1) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+2) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+3) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+4) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 5;
    }
    else if (val64 < 0x0000020000000000)
    {
		*buf = (unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+1) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+2) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+3) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+4) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+5) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 6;
    }
    else if( val64 < 0x0001000000000000 )
    {
		*buf = (unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+1) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+2) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+3) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+4) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+5) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+6) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 7;
    }
    else if( val64 < 0x0080000000000000 )
    {
		*buf = (unsigned char)((val64 >> 49)  & 0x7F); // ..FE .... .... ....
		*(buf+1) =(unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+2) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+3) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+4) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+5) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+6) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+7) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 8;
    }
    else if( val64 < 0x4000000000000000 )
    {
		*buf = (unsigned char)((val64 >> 56)  & 0x7F); // 7F.. .... .... ....
		*(buf+1) =(unsigned char)((val64 >> 49)  & 0x7F); // ..FE .... .... ....
		*(buf+2) =(unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+3) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+4) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+5) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+6) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+7) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+8) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 9;
    }
    else
    {
		*buf = (unsigned char)((val64 >> 63)  & 0x7F); // 8... .... .... ....  (this will always be zero)
		*(buf+1) =(unsigned char)((val64 >> 56)  & 0x7F); // 7F.. .... .... ....
		*(buf+2) =(unsigned char)((val64 >> 49)  & 0x7F); // ..FE .... .... ....
		*(buf+3) =(unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+4) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+5) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+6) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+7) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ..1F C...
		*(buf+8) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+9) =(unsigned char)((val64 & 0x7F) | 0x80); // .... .... .... ..7f
		return 10;
    }
}	

int FrameSerializer::inner_lt0_encodeint64( unsigned char * buf, __int64 val64 )
{
    __int64 absv = -val64;	// using absolute value avoids tricky word length issues

	//    if(absv == value) // Apparently this is not a valid check on all compilers
    if( (val64 << 1) == 0 )
    {
		// encode the most negative possible number
		*buf = (0x7F);    // 8... .... .... ....
		*(buf+1) =(0x00);    // 7F.. .... .... ....
		*(buf+2) =(0x00);    // . FE .... .... ....
		*(buf+3) =(0x00);    // ...1 FC.. .... ....
		*(buf+4) =(0x00);    // .... .3F8 .... ....
		*(buf+5) =(0x00);    // .... ...7 F... ....
		*(buf+6) =(0x00);    // .... .... .FE. ....
		*(buf+7) =(0x00);    // .... .... ...1 FC..
		*(buf+8) =(0x00);    // .... .... .... 3F8.
		*(buf+9) =(0x80);    // .... .... .... ..7f
		return 10;
    }
    else if( absv <= 0x0000000000000040 )
    {
		*buf = (unsigned char)(val64 & 0xFF); // .... .... .... ..7f
		return 1;
    }
    else if( absv <= 0x0000000000002000 )
    {
		*buf = (unsigned char)((val64 >> 7) & 0x7F); // .... .... .... 3F8.
		*(buf+1) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 2;
    }
    else if( absv <= 0x0000000000100000 )
    {
		*buf = (unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+1) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+2) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 3;
    }
    else if( absv <= 0x0000000008000000 )
    {
		*buf = (unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+1) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+2) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+3) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 4;
    }
    else if( absv <= 0x0000000400000000 )
    {
		*buf = (unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+1) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+2) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+3) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+4) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 5;
    }
    else if( absv <= 0x0000020000000000 )
    {
		*buf = (unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+1) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+2) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+3) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+4) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+5) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 6;
    }
    else if( absv <= 0x0001000000000000 )
    {
		*buf = (unsigned char)((val64 >> 42)  & 0x7F);// ...1 FC.. .... ....
		*(buf+1) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+2) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+3) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+4) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+5) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+6) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 7;
    }
    else if( absv <= 0x0080000000000000 )
    {
		*buf = (unsigned char)((val64 >> 49)  & 0x7F); // ..FE .... .... ....
		*(buf+1) =(unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+2) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+3) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+4) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+5) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+6) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+7) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 8;
    }
    else
    {
		*buf = (unsigned char)((val64 >> 56)  & 0x7F); // 7F.. .... .... ....
		*(buf+1) =(unsigned char)((val64 >> 49)  & 0x7F); // ..FE .... .... ....
		*(buf+2) =(unsigned char)((val64 >> 42)  & 0x7F); // ...1 FC.. .... ....
		*(buf+3) =(unsigned char)((val64 >> 35)  & 0x7F); // .... .3F8 .... ....
		*(buf+4) =(unsigned char)((val64 >> 28)  & 0x7F); // .... ...7 F... ....
		*(buf+5) =(unsigned char)((val64 >> 21)  & 0x7F); // .... .... .FE. ....
		*(buf+6) =(unsigned char)((val64 >> 14)  & 0x7F); // .... .... ...1 FC..
		*(buf+7) =(unsigned char)((val64 >> 7)   & 0x7F); // .... .... .... 3F8.
		*(buf+8) =(unsigned char)((val64 & 0x7F)  | 0x80);  // .... .... .... ..7f
		return 9;
    }
}



//////////////////////////////////////////


FrameUnserializer::FrameUnserializer()
: m_buf( NULL ), m_buflen( 0 ), m_curpos( 0 )
{
}

int	FrameUnserializer::GetDataSize()
{
	return m_buflen - m_curpos;
}

void FrameUnserializer::Attach( unsigned char *ptr, size_t size )
{
	m_buf = ptr;
	m_buflen = size;
	m_curpos = 0;
}

int FrameUnserializer::operator >> ( SerialNumber& val )
{
	if( true == val.IsSigned() )
	{
		return inner_decodeInt64( val );
	}
	else
	{
		return inner_decodeUInt64( val );
	}
}

int FrameUnserializer::operator >> ( SerialBytes& val )
{
	unsigned __int64		nVectorLen = 0;
	int						size = inner_decodeUInt64( nVectorLen );

	if( size > 0 )
	{
		if( nVectorLen > 0 )
		{
			memcpy( val.pszStr, m_buf + m_curpos, nVectorLen );
			val.nStrLen = nVectorLen;
			m_curpos += nVectorLen;
		}

		/*if( size < val.nStrLen ) // 保护边界
		{
			*(val.pszStr + size) = '\0';
		}

		m_curpos += size;*/
	}

	return size;
}

int FrameUnserializer::operator >> ( SerialString& val )
{
	if( m_curpos >= m_buflen )
	{
		return 0;
	}

	int size = DecodeBytes( m_buf + m_curpos, val.pszStr, val.nStrLen );

	if( size > 0 )
	{
		if( size < val.nStrLen ) // 保护边界
		{
			*(val.pszStr + size) = '\0';
		}

		m_curpos += size;

		if( val.pszStr[0] == '\0' )
		{
			val.nStrLen = 0;
		}
		else
		{
			val.nStrLen = size;
		}
	}

	return size;
}

int	FrameUnserializer::DecodeBytes( unsigned char* buf, char* b,int size )
{
	for( int i=0;i<size;i++ )
	{
		if( (buf[i]&0x80) != 0 ) 
		{
			b[i] = buf[i] & 0x7f;
			return i+1;
		}
		else
		{
			b[i] = buf[i];
		}
	}

	return -1;
}

int FrameUnserializer::inner_decodeUInt64( unsigned __int64 &val )
{
	if( m_curpos >= m_buflen )
	{
		return 0;
	}

	char	tmp[12];
	int		size = DecodeBytes( m_buf + m_curpos, tmp, 10 );

	if( size > 0 )
	{
		for( int i = 0 ; i < size ; i++ )
		{
			val = ( val << 7 ) | tmp[i];
		}

		m_curpos += size;
	}

	return size;
}

int FrameUnserializer::inner_decodeInt64( __int64 &val )
{
	if( m_curpos >= m_buflen )
	{
		return 0;
	}

	char	tmp[12];
	int		size = DecodeBytes( m_buf + m_curpos, tmp, 10 );

	if( size > 0 )
	{
		if( tmp[0] & 0x40 )
			val = -1;
		else	
			val = 0;

		for( int i = 0 ; i < size ; i++ )
		{
			val = ( val << 7 ) | tmp[i];
		}

		m_curpos += size;
	}

	return size;
}









