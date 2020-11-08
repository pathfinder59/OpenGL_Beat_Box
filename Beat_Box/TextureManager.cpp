#include "Bitmap.h"
#include "TextureManager.h"
#include "minmax.h"
std::vector<GLubyte> GetCImageBits(const CImage & Cimg)
{
	// ���ϰ�
	std::vector<GLubyte> retval;

	// ��Ʈ��(DDB) ���� ���
	BITMAP bmp;
	GetObject(Cimg, sizeof(BITMAP), &bmp);

	// ��Ʈ��(DIB) ���� ����
	BITMAPINFOHEADER bmih{ 0 };

	// bitmap header�� ���� ����
	bmih.biSize = sizeof(BITMAPINFOHEADER);

	bmih.biWidth = bmp.bmWidth;    // ����
	bmih.biHeight = bmp.bmHeight;  // ����

	// ��� ��ġ�� ���� ���� ��. ������ 1�� ����.
	bmih.biPlanes = 1;
	// �ȼ� �� ��Ʈ ��(Bit per Pixel).
	// R8G8B8 = 24�� �⺻������ �������� A8�� �پ 32�� ����Ѵ�.
	bmih.biBitCount = Cimg.GetBPP();
	// RGB ����. 32 BPP�� ��� BI_BITFIELDS�� ��ȿ
	bmih.biCompression = (BI_RGB);

	// ��Ʈ��(DIB) ������ ����
	// �������� ũ�⸦ �˾Ƴ���.
	HDC hDC = ::GetDC(NULL);
	{
		// �̹����� ũ�⸦ Ȯ���ϱ� ���� lpvBits�� NULL�� �ְ�
		// bitmap header�� biSizeImage�� ���� �Է¹޴´�.
		::GetDIBits(hDC, Cimg, 0, bmp.bmHeight, NULL, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
		// ȹ���� ��Ʈ�� ����� �ް�
		// ������ ���� ������ Ȯ���Ѵ�.
		retval.resize(bmih.biSizeImage);
		// �迭�� �ȼ� ������ ����ȴ�.
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
	LoadTexture(ENUM_TEXTURE::SONG3, "�ö�����s.bmp");
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
