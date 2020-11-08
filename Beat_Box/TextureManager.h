#pragma once
#include "Bitmap.h"
#include "singleton.h"
#include <windows.h>
#include <atlimage.h>
#include <map>

std::vector<GLubyte> GetCImageBits(const CImage& Cimg);

struct GLTextureData
{
	GLuint					textureIndex = 0;
	bool					bHasAlpha = false;
	std::vector<GLubyte>	pBytes;
	GLubyte* GetData() { return &(pBytes[0]); }
	GLTextureData() = default;

	GLTextureData(GLuint idx, bool bAlpha, std::vector<GLubyte>&& v)
		: textureIndex{ idx }
		, bHasAlpha{ bAlpha }
		, pBytes{ v } {}
	bool EnableTexture()const {

		GLboolean bDestAlphaState = false;
		if (bHasAlpha)
		{
			glGetBooleanv(GL_BLEND, &bDestAlphaState);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}

		glBindTexture(GL_TEXTURE_2D, textureIndex);

		return bDestAlphaState;
		//

	}
	void DisableTexture(bool EnableReturn)const {
		if (bHasAlpha && EnableReturn == false)
			glDisable(GL_BLEND);
	}

};

class CTextureManager : public Singleton<CTextureManager>
{
	std::map<ENUM_TEXTURE,GLTextureData>  m_ObjectID;

	//data
	void LoadTexture(ENUM_TEXTURE id,typePATH filename)
	{

		GLuint IDtmp[1];
		glGenTextures(1, IDtmp);
		CImage img;
		img.Load(filename.c_str() );

		glBindTexture(GL_TEXTURE_2D, IDtmp[0]);

		int iColorTable = img.GetBPP() / 8;

		GLTextureData data
		{
			IDtmp[0]
			, (iColorTable == 4)
			, GetCImageBits(img)
		};


		glTexImage2D(GL_TEXTURE_2D
			, 0
			, iColorTable
			, img.GetWidth(), img.GetHeight(), 0
			, (iColorTable == 3) ? GL_BGR_EXT : GL_BGRA_EXT
			, GL_UNSIGNED_BYTE
			, data.GetData()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);

		//m_ObjectID.insert(std::make_pair(id, data));
		m_ObjectID[id] = data;
	}

public:

	CTextureManager();
	virtual ~CTextureManager();

	bool EnableTextur(ENUM_TEXTURE id)const;
	void DisableTextur(ENUM_TEXTURE id,bool EnableTexturRetrun)const ;
	

};

