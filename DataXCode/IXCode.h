#ifndef __QLX_INTERFACE_ENDECODE_H__
#define __QLX_INTERFACE_ENDECODE_H__


/**
 * @class	InterfaceEncode
 * @brief	Encode接口
 */
class InterfaceEncode
{
public:
	/**
	 * @brief		初始化
	 * @param[in]	pszTemplateFile	编码策略文件
	 * @return		0			成功
	 */
	virtual int Initialize( const char* pszTemplateFile ) = 0;

	/**
	 * @brief		释放接口
	 */
	virtual void Release() = 0;

	/**
	 * @brief		准备填写一个新的frame结构
	 * @param[in]	pBuffer		缓存地址
	 * @param[in]	nBufferLne	缓存长度
	 * @return		= 0			成功
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[in]	nMsgID		Message ID
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		> 0			成功，返回历次调用累积的序列化的长度
	 */
	virtual int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen ) = 0;
};


/**
 * @class	InterfaceDecode
 * @brief	Decode接口
 */
class InterfaceDecode
{
public:
	/**
	 * @brief		初始化
	 * @param[in]	pszTemplateFile	编码策略文件
	 * @return		0			成功
	 */
	virtual int Initialize( const char* pszTemplateFile ) = 0;

	/**
	 * @brief		释放接口
	 */
	virtual void Release() = 0;

	/**
	 * @brief		准备填写一个新的frame结构
	 * @param[in]	pBuffer		缓存地址
	 * @param[in]	nBufferLne	缓存有效数据的长度
	 * @return		= 0			成功
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[out]	nMsgID		返回Message ID
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		>= 0		成功，返回历次调用后,未序列化部分的长度
	 */
	virtual int DecodeMessage( unsigned short nMsgID, char *pData, unsigned int nLen ) = 0;
};


#endif





