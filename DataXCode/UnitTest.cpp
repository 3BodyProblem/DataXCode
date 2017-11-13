#include "UnitTest.h"
#ifdef	EXPORT_PYTHON_PYD_MODULE			///< ֻ���ڶ���pyd�����������²�֧�ֵ�Ԫ����(gtest)
#include "Encode.h"
#include "Decode.h"
#include <string>
#include <iostream>
#include <algorithm>


char					*g_pGlobalBuffer = NULL;								///< ȫ�ֻ����ַ
InterfaceEncode			*g_pEncoder = NULL;
InterfaceDecode			*g_pDecoder = NULL;


///< -------------------- DEMO���ֵĲ����ࣺ PROTOBUF���л�/�����л��ඨ�� ----------------------------------


static std::string	s_Param1 = "";											///< �����ǲ��Բ���( 1 ~ 6 )
static std::string	s_Param2 = "";
static std::string	s_Param3 = "";
static std::string	s_Param4 = "";
static std::string	s_Param5 = "";
static std::string	s_Param6 = "";


///< --------------------- DEMO���ֵĲ����ࣺ ��Ԫ�����ඨ�� --------------------------------


tagDemoMarketStatus TestDemoMarketInfo::m_lstTagStatus[MAX_MARKET_NUM] = { 0 };

void TestDemoMarketInfo::SetUpTestCase()
{
	for( int n = 0; n < MAX_MARKET_NUM; n++ )
	{
		m_lstTagStatus[n].Status = n % 2;
		m_lstTagStatus[n].Date = 20181212;
		m_lstTagStatus[n].Time = 150102 + n;
	}
}

void TestDemoMarketInfo::TearDownTestCase()
{
	::memset( m_lstTagStatus, 0, sizeof(m_lstTagStatus) );
}

void TestDemoMarketInfo::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoMarketInfo::TearDown()
{
}


tagDemoStaticType TestDemoStatic::m_lstTagName[MAX_STATIC_NUM] = { 0 };

void TestDemoStatic::SetUpTestCase()
{
	for( int n = 0; n < MAX_STATIC_NUM; n++ )
	{
		char		pszCode[7] = { 0 };

		m_lstTagName[n].Type = (n % 10) + 1;
		::sprintf( pszCode, "%u", 600001 + n );
		::memcpy( m_lstTagName[n].Code, pszCode, 6 );
		::strncpy( m_lstTagName[n].Name, "abcdefg", 8/*"��A��չ", sizeof("��A��չ")*/ );
	}
}

void TestDemoStatic::TearDownTestCase()
{
	::memset( m_lstTagName, 0, sizeof(m_lstTagName) );
}

void TestDemoStatic::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoStatic::TearDown()
{
}


tagDemoSnapType TestDemoSnap::m_lstTagSecurity[MAX_SNAP_NUM] = { 0 };

void TestDemoSnap::SetUpTestCase()
{
	for( int n = 0; n < MAX_SNAP_NUM; n++ )
	{
		char				pszCode[7] = { 0 };
		tagDemoSnapType		&snap = m_lstTagSecurity[n];
		unsigned int		nBasePrice = 102400 + n * 500;

		::sprintf( pszCode, "%u", 600001 + n );
		::memcpy( snap.Code, pszCode, 6 );
		snap.Now = nBasePrice;
		snap.High = nBasePrice + 500;
		snap.Low = nBasePrice - 500;
		snap.Open = nBasePrice - 10;
		snap.Close = nBasePrice + 20;
		snap.Voip = nBasePrice - 1000;
		snap.Amount = (nBasePrice * 128) + 0.123;
		snap.Volume = nBasePrice * 300;
		snap.Records = 1000;

		for( int i = 0; i < 10; i++ )
		{
			snap.Buy[i].Price = nBasePrice + i * 5;
			snap.Buy[i].Volume = nBasePrice * 1024 + i;
			snap.Sell[i].Price = nBasePrice - i * 5;
			snap.Sell[i].Volume = nBasePrice * 1024 - i;
		}
	}
}

void TestDemoSnap::TearDownTestCase()
{
	::memset( m_lstTagSecurity, 0, sizeof(m_lstTagSecurity) );
}

void TestDemoSnap::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoSnap::TearDown()
{
}


///< ------------------------- �����г���ʵ�������� ----------------------------------

void TestSHL2MarketData::SetUpTestCase()
{
}

void TestSHL2MarketData::TearDownTestCase()
{
}

void TestSHL2MarketData::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestSHL2MarketData::TearDown()
{
}



///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------


TEST_F( TestDemoMarketInfo, Bundle )
{
	unsigned short			nMsgID = 0;
	tagDemoMarketStatus		value = { 0 };

	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[0], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[1], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[2], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[2], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[1], sizeof(tagDemoMarketStatus) ) );
	int	nBufSize = g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[0], sizeof(tagDemoMarketStatus) );
	ASSERT_LT( 0, nBufSize );

	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, nBufSize ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[0], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[1], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[1], sizeof(value) ) );
	ASSERT_EQ( 0, g_pDecoder->DecodeMessage( 1, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[0], sizeof(value) ) );
}

TEST_F( TestDemoStatic, Single )
{
	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
}

TEST_F( TestDemoStatic, Loop )
{
	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	int	nBufSize = g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) );
	ASSERT_LT( 0, nBufSize );

	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, nBufSize ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_EQ( 0, g_pDecoder->DecodeMessage( 10, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
}

TEST_F( TestDemoSnap, Single )
{
	unsigned short		nMsgID = 0;
	tagDemoSnapType		snap = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 13, (char*)&m_lstTagSecurity[0], sizeof(snap) ) );
	g_pDecoder->DecodeMessage( 13, (char*)&snap, sizeof(snap) );
	ASSERT_EQ( 0, ::memcmp( &snap, &m_lstTagSecurity[0], sizeof(snap) ) );
}

TEST_F( TestDemoSnap, Loop )
{
	unsigned __int64	nSizeCnt = 0;

	for( int i = 0; i < 1024*8; i++ )
	{
		SetUp();

		for( int n = 0; n < MAX_SNAP_NUM; n++ )
		{
			ASSERT_LT( 0, g_pEncoder->EncodeMessage( 13, (char*)&m_lstTagSecurity[n], sizeof(tagDemoSnapType) ) );
			nSizeCnt += sizeof(tagDemoSnapType);
		}

		for( int n = 0; n < MAX_SNAP_NUM; n++ )
		{
			unsigned short		nMsgID = 0;
			tagDemoSnapType		snap = { 0 };
			g_pDecoder->DecodeMessage( 13, (char*)&snap, sizeof(snap) );
			ASSERT_EQ( 0, ::memcmp( &snap, &m_lstTagSecurity[n], sizeof(snap) ) );
		}
	}

	::printf( "ѹ��ǰ�ۼ�: %I64d \n", nSizeCnt );
}


///< ------------------------- ��Ԫ���Գ�ʼ���ඨ�� ----------------------------------------------


static std::string	s_XmlTemplatePath = "";									///< ���ݽ���ģ���ļ�·��


void QLXEnDeCodeTestEnv::SetUp()
{
	g_pGlobalBuffer = new char[GLOBAL_BUFFER_SIZE];							///< ����һ��encode��decode���õĴ󻺴�,�������δ�Ŷ��Messages
	ASSERT_NE( 0, (int)g_pGlobalBuffer );									///< ��⻺���Ƿ����ɹ�
	g_pEncoder = new Encode();
	ASSERT_NE( 0, (int)g_pEncoder );
	g_pDecoder = new Decode();
	ASSERT_NE( 0, (int)g_pDecoder );
	ASSERT_EQ( 0, g_pEncoder->Initialize( s_XmlTemplatePath.c_str() ) );
	ASSERT_EQ( 0, g_pDecoder->Initialize( s_XmlTemplatePath.c_str() ) );
}

void QLXEnDeCodeTestEnv::TearDown()
{
	delete [] g_pGlobalBuffer;
	g_pGlobalBuffer = NULL;

	delete g_pEncoder;
	delete g_pDecoder;
	g_pEncoder = NULL;
	g_pDecoder = NULL;
}


///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------
///< ------------------ ��Ԫ���Ե����������� -------------------------------


/**
 * @brief		��ʼ��gtest��������
				&&
				�����ò���Case����(filter)
 */
void PrepareGTestEnv( char* pszFilter )
{
	static	bool	s_bInit = false;

	if( false == s_bInit )	{
		int			nArgc = 1;
		char*		pszArgv[32] = { "DataXCode.dll", };

		s_bInit = true;
		testing::AddGlobalTestEnvironment( new QLXEnDeCodeTestEnv() );
		testing::InitGoogleTest( &nArgc, pszArgv );
	}

	::testing::GTEST_FLAG( filter ) = pszFilter;
}

int	RunAllDemoTest_Encode( char* pszXmlEnDeTemplate, std::string sParam1, std::string sParam2, std::string sParam3, std::string sParam4, std::string sParam5, std::string sParam6 )
{
	std::string		sUsageDesc = "\
	�����Ե�Ԫ�����ڲ������õ��������͡�\n\
	\ta) ʹ�����õĲ������ݲ��� [RunAllDemoTest_Encode( \"./DemoTemplate.xml\", \"*Demo*\" )] \n\
	\tb) ʹ���ⲿ�����Ĳ������ݲ��� [RunAllDemoTest_Encode( \"./DemoTemplate.xml\", \"*SHL2*\", \"./template.2.13.xml\", \"./fastdata\" )] \n";

	::printf( "%s\n", sUsageDesc.c_str() );

	s_Param1 = sParam1;		///< �г���Ʊ�ʶ�ַ���
	s_Param2 = sParam2;
	s_Param3 = sParam3;
	s_Param4 = sParam4;
	s_Param5 = sParam5;
	s_Param6 = sParam6;
	s_XmlTemplatePath = pszXmlEnDeTemplate;

	PrepareGTestEnv( (char*)s_Param1.c_str() );

	return RUN_ALL_TESTS();
}


#endif






