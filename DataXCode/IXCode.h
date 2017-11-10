#ifndef __QLX_INTERFACE_ENDECODE_H__
#define __QLX_INTERFACE_ENDECODE_H__


/**
 * @class	InterfaceEncode
 * @brief	Encode�ӿ�
 */
class InterfaceEncode
{
public:
	/**
	 * @brief		��ʼ��
	 * @param[in]	pszTemplateFile	��������ļ�
	 * @return		0			�ɹ�
	 */
	virtual int Initialize( const char* pszTemplateFile ) = 0;

	/**
	 * @brief		�ͷŽӿ�
	 */
	virtual void Release() = 0;

	/**
	 * @brief		׼����дһ���µ�frame�ṹ
	 * @param[in]	pBuffer		�����ַ
	 * @param[in]	nBufferLne	���泤��
	 * @return		= 0			�ɹ�
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[in]	nMsgID		Message ID
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		> 0			�ɹ����������ε����ۻ������л��ĳ���
	 */
	virtual int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen ) = 0;
};


/**
 * @class	InterfaceDecode
 * @brief	Decode�ӿ�
 */
class InterfaceDecode
{
public:
	/**
	 * @brief		��ʼ��
	 * @param[in]	pszTemplateFile	��������ļ�
	 * @return		0			�ɹ�
	 */
	virtual int Initialize( const char* pszTemplateFile ) = 0;

	/**
	 * @brief		�ͷŽӿ�
	 */
	virtual void Release() = 0;

	/**
	 * @brief		׼����дһ���µ�frame�ṹ
	 * @param[in]	pBuffer		�����ַ
	 * @param[in]	nBufferLne	������Ч���ݵĳ���
	 * @return		= 0			�ɹ�
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[out]	nMsgID		����Message ID
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		>= 0		�ɹ����������ε��ú�,δ���л����ֵĳ���
	 */
	virtual int DecodeMessage( unsigned short nMsgID, char *pData, unsigned int nLen ) = 0;
};


#endif





