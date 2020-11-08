#include "Bitmap.h"
#include "TextureManager.h"
#include "minmax.h"
std::vector<GLubyte> GetCImageBits(const CImage & Cimg)
{
	// 리턴값
	std::vector<GLubyte> retval;

	// 비트맵(DDB) 정보 얻기
	BITMAP bmp;
	GetObject(Cimg, sizeof(BITMAP), &bmp);

	// 비트맵(DIB) 정보 설정
	BITMAPINFOHEADER bmih{ 0 };

	// bitmap header는 고정 길이
	bmih.biSize = sizeof(BITMAPINFOHEADER);

	bmih.biWidth = bmp.bmWidth;    // 가로
	bmih.biHeight = bmp.bmHeight;  // 세로

	// 대상 장치에 대한 면의 수. 무조건 1로 지정.
	bmih.biPlanes = 1;
	// 픽셀 당 비트 수(Bit per Pixel).
	// R8G8B8 = 24가 기본이지만 아이콘은 A8이 붙어서 32를 사용한다.
	bmih.biBitCount = Cimg.GetBPP();
	// RGB 포맷. 32 BPP일 경우 BI_BITFIELDS가 유효
	bmih.biCompression = (BI_RGB);

	// 비트맵(DIB) 데이터 추출
	// 데이터의 크기를 알아낸다.
	HDC hDC = ::GetDC(NULL);
	{
		// 이미지의 크기를 확인하기 위해 lpvBits를 NULL로 주고
		// bitmap header의 biSizeImage에 값을 입력받는다.
		::GetDIBits(hDC, Cimg, 0, bmp.bmHeight, NULL, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
		// 획득한 비트맵 사이즈를 받고
		// 데이터 저장 공간을 확보한다.
		retval.resize(bmih.biSizeImage);
		// 배열에 픽셀 정보가 저장된다.
		::GetDIBits(hDC, Cimg, 0, bmp.bmHeight, &(retval[0]), (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
	}
	::ReleaseDC(NULL, hDC);

	return retval;
}
CTextureManager::CTextureManager()
{
	LoadTexture(ENUM_TEXTURE::MAIN, "BeatBox_main.bmp");
	LoadTexture(ENUM_TEXTURE::TEXT, "gametext.bmp");
	LoadTexture(ENUM_TEXTURE::SONG1, "HandClap.bmp");
	LoadTexture(ENUM_TEXTURE::SONG2, "POPSTAR.bmp");
	LoadTexture(ENUM_TEXTURE::SONG3, "플랑도르s.bmp");
	LoadTexture(ENUM_TEXTURE::BACKGROUND, "BACKGROUND.bmp");
	LoadTexture(ENUM_TEXTURE::SIDE, "SIDE.bmp");
	LoadTexture(ENUM_TEXTURE::UNDER, "UNDER.bmp");
	LoadTexture(ENUM_TEXTURE::HP1, "HP1.bmp");
	LoadTexture(ENUM_TEXTURE::HP2, "HP2.bmp");
	LoadTexture(ENUM_TEXTURE::HP3, "HP3.bmp");
	LoadTexture(ENUM_TEXTURE::HP4, "HP4.bmp");
	LoadTexture(ENUM_TEXTURE::HP5, "HP5.bmp");
	LoadTexture(ENUM_TEXTURE::HP_TOOL, "HP_Tool.png");
	LoadTexture(ENUM_TEXTURE::COMBO, "Combo.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_0, "0.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_1, "1.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_2, "2.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_3, "3.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_4, "4.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_5, "5.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_6, "6.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_7, "7.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_8, "8.png");
	LoadTexture(ENUM_TEXTURE::NUMPER_9, "9.png");
	LoadTexture(ENUM_TEXTURE::SCORE, "score.png");
	LoadTexture(ENUM_TEXTURE::GAMEOVER, "GameOver.png");
	LoadTexture(ENUM_TEXTURE::GAMEClear, "GameClear.png");
}

CTextureManager::~CTextureManager()
{

}

bool CTextureManager::EnableTextur(ENUM_TEXTURE id) const
{
	auto& tmpID = m_ObjectID.find(id)->second;
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	return	tmpID.EnableTexture();

}

void CTextureManager::DisableTextur(ENUM_TEXTURE id, bool EnableTexturRetrun) const
{
	auto& tmpID = m_ObjectID.find(id)->second;
	tmpID.DisableTexture(EnableTexturRetrun);

	glDisable(GL_TEXTURE_2D);
}
