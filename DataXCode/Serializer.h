#ifndef __QLX_SERIALIZER_H__
#define __QLX_SERIALIZER_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"


#define		MAX_DATABLOCK_SIZE			2048				///< ��������ݽṹ����


/**
 * @class		FrameSerializer
 * @brief		֡���л�
 */
class FrameSerializer
{
public:
	FrameSerializer();

	/**
	 * @brief		�����Ѿ����л��ĳ���
	 */
	int	GetDataSize();

	/**
	 * @brief			ȡ�û����ַ
	 */
	operator char*();

	/**
	 * @brief			attachһ�����д��Ļ���
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			��ֵMessageID,�����buffer�ĳ��ȼ���
	 */
	bool NewMessage();

	/**
	 * @brief			���л�һ��bytes����
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialBytes& val );

	/**
	 * @breif			���л�һ���ַ���
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialString& val );

	/**
	 * @brief			���л�һ�����ⳤ�ȵ���ֵ
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialNumber& val );

private:
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< ��һ��encode����ʹ�õ�message�Ĳ�ֵ����
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf����Ч���ݳ���
	unsigned char*			m_buf;								///< ����
	size_t					m_buflen;							///< �����ܳ���
	size_t					m_curpos;							///< ��д��λ��
};


/**
 * @class		FrameUnserializer
 * @brief		֡�����л�
 */
class FrameUnserializer
{
public:
	FrameUnserializer();

	/**
	 * @brief		����δ���л����ֵĳ���
	 */
	int	GetDataSize();

	/**
	 * @brief			attachһ����Զ�ȡ�Ļ���
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			����һ��bytes��
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialBytes& val );

	/**
	 * @brief			����һ���ַ���
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialString& val );

	/**
	 * @brief			����һ�����ⳤ�ȵ���ֵ
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialNumber& val );

private:
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< ��һ��encode����ʹ�õ�message�Ĳ�ֵ����
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf����Ч���ݳ���
	unsigned char*			m_buf;								///< ����
	size_t					m_buflen;							///< �����ܳ���
	size_t					m_curpos;							///< ������λ��
};




#endif





