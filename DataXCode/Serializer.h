#ifndef __QLX_SERIALIZER_H__
#define __QLX_SERIALIZER_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"


#define		MAX_DATABLOCK_SIZE			2048				///< 最长行情数据结构长度


/**
 * @class		FrameSerializer
 * @brief		帧序列化
 */
class FrameSerializer
{
public:
	FrameSerializer();

	/**
	 * @brief		返回已经序列化的长度
	 */
	int	GetDataSize();

	/**
	 * @brief			取得缓存地址
	 */
	operator char*();

	/**
	 * @brief			attach一块可以写入的缓存
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			序列化一个bytes数组
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialBytes& val );

	/**
	 * @breif			序列化一个字符串
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialString& val );

	/**
	 * @brief			序列化一个任意长度的数值
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialNumber& val );

private:
	inline int	inner_encodeUInt64( const unsigned __int64 val );
	inline int	inner_gt0_encodeint64( unsigned char * buf, __int64 val64 );	
	inline int	inner_lt0_encodeint64( unsigned char * buf, __int64 val64 );

private:
	unsigned char*			m_buf;								///< 缓存
	size_t					m_buflen;							///< 缓存总长度
	size_t					m_curpos;							///< 待写入位置
};


/**
 * @class		FrameUnserializer
 * @brief		帧反序列化
 */
class FrameUnserializer
{
public:
	FrameUnserializer();

	/**
	 * @brief		返回未序列化部分的长度
	 */
	int	GetDataSize();

	/**
	 * @brief			attach一块可以读取的缓存
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			读出一个bytes串
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialBytes& val );

	/**
	 * @brief			读出一个字符串
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialString& val );

	/**
	 * @brief			读出一个任意长度的数值
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialNumber& val );

private:
	/**
	 * @brief		按照Stop bit 规则获取字符序列
	 * @return		size, <0 表示出错
					>0,		读出的反序列化缓存中的bit数据
	 */
	inline int	DecodeBytes( unsigned char* buf, char* b, int size );
	inline int	inner_decodeInt64( __int64 &val );
	inline int	inner_decodeUInt64( unsigned __int64 &val );

private:
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< 对一次encode，所使用的message的差值缓存
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf的有效数据长度
	unsigned char*			m_buf;								///< 缓存
	size_t					m_buflen;							///< 缓存总长度
	size_t					m_curpos;							///< 待读出位置
};




#endif






