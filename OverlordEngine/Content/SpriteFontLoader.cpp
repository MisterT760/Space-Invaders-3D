#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"


SpriteFontLoader::SpriteFontLoader()
{

}


SpriteFontLoader::~SpriteFontLoader()
{
}

#pragma warning( disable : 4702 )
SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout
	
	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	char fileId1 = pBinReader->Read<char>();
	char fileId2 = pBinReader->Read<char>();
	char fileId3 = pBinReader->Read<char>();

	if (fileId1 != 'B' || fileId2 != 'M' || fileId3 != 'F')
	{

		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile! \nPath: \'%s\'", assetFile.c_str());

		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	int versionId = pBinReader->Read<char>();

	if (versionId != 3)
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported! \nFile Version: \'%i\'", versionId);

		return nullptr;
	}

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	int blockId = pBinReader->Read<char>();
	int blockSize = pBinReader->Read<int>();

	pSpriteFont->m_FontSize = pBinReader->Read<short>();
	pBinReader->MoveBufferPosition(12);
	pSpriteFont->m_FontName = pBinReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();

	pBinReader->MoveBufferPosition(4);
	pSpriteFont->m_TextureWidth = pBinReader->Read<unsigned short>();
	pSpriteFont->m_TextureHeight = pBinReader->Read<unsigned short>();

	int pageCount = pBinReader->Read<unsigned short>();
	if (pageCount > 1)
	{
		delete pBinReader;
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
	
		return nullptr;
	}

	pBinReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (store Local)
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();

	wstring pageName = pBinReader->ReadNullString();

	if (pageName == L"")
	{
		delete pBinReader;
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");

		return nullptr;
	}

	auto found = assetFile.rfind(L"/");
	auto path = assetFile;
	if (found != std::wstring::npos)
		path = assetFile.substr(0, found + 1).c_str();

	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(path + pageName);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Retrieve Every Character, For every Character:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
	//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
	//> Set IsValid to true [FontMetric::IsValid]
	//> Set Character (CharacterId) [FontMetric::Character]
	//> Retrieve Xposition (store Local)
	//> Retrieve Yposition (store Local)
	//> Retrieve & Set Width [FontMetric::Width]
	//> Retrieve & Set Height [FontMetric::Height]
	//> Retrieve & Set OffsetX [FontMetric::OffsetX]
	//> Retrieve & Set OffsetY [FontMetric::OffsetY]
	//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
	//> Retrieve & Set Page [FontMetric::Page]
	//> Retrieve Channel (BITFIELD!!!) 
	//	> See documentation for BitField meaning [FontMetrix::Channel]
	//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();

	auto CharCount = blockSize / 20;
	for (int i = 0; i < CharCount; ++i)
	{
		wchar_t charId = wchar_t(pBinReader->Read<unsigned int>());
		if (!pSpriteFont->IsCharValid(charId))
		{
			Logger::LogWarning(L"SpriteFontLoader::LoadContent > CharacterId: Invalid CharacterId!");
			pBinReader->MoveBufferPosition(16);
			continue;
		}

		auto &fontMetric = pSpriteFont->GetMetric(charId);
		fontMetric.IsValid = true;
		fontMetric.Character = charId;

		auto xPos = pBinReader->Read<unsigned short>();
		auto yPos = pBinReader->Read<unsigned short>();

		fontMetric.Width = pBinReader->Read<USHORT>();
		fontMetric.Height = pBinReader->Read<USHORT>();
		fontMetric.OffsetX = pBinReader->Read<short>();
		fontMetric.OffsetY = pBinReader->Read<short>();
		fontMetric.AdvanceX = pBinReader->Read<short>();
		fontMetric.Page = pBinReader->Read<UCHAR>();
		auto channel = pBinReader->Read<char>();

		switch (channel)
		{
		case 1:
			fontMetric.Channel = 2;
			break;
		case 2:
			fontMetric.Channel = 1;
			break;
		case 4:
			fontMetric.Channel = 0;
			break;
		case 8:
			fontMetric.Channel = 3;
			break;
		default:
			break;
		}

		fontMetric.TexCoord.x = float(xPos) / pSpriteFont->m_TextureWidth;
		fontMetric.TexCoord.y = float(yPos) / pSpriteFont->m_TextureHeight;
	}

	//DONE :)

	delete pBinReader;
	return pSpriteFont;
	
	#pragma warning(default:4702)  
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
