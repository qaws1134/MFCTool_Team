#include "stdafx.h"
#include "Single_Texture.h"


CSingle_Texture::CSingle_Texture()
	:m_tTexInfo({})
{
}


CSingle_Texture::~CSingle_Texture()
{
	Release_Texture(); 
}

HRESULT CSingle_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD dwCount /*= 0*/)
{
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tImageInfo)))
		goto ERR; 

	if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		m_tTexInfo.tImageInfo.Width, // �׸� ����
		m_tTexInfo.tImageInfo.Height, // �׸� ���� 
		m_tTexInfo.tImageInfo.MipLevels, // �ӷ��� 
		// �̹��� �ӷ����� �������� �ְ� ���� ���� �ִ�. 
		0, 
		m_tTexInfo.tImageInfo.Format, 
		D3DPOOL_MANAGED, // �츮�� «¥���� ���ڴ�. ����? 
		D3DX_DEFAULT, 
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr, 
		&m_tTexInfo.pTexture)))
		goto ERR; 

	return S_OK;
ERR:
	ERR_MSG(L"�׸����� ����ֱ� ����.");  
	return E_NOTIMPL;
}

const TEXINFO * CSingle_Texture::Get_TexInfo(const wstring & wstrStateKey /*= L""*/, const DWORD dwIndex /*= 0*/)
{
	return &m_tTexInfo;
}

void CSingle_Texture::Release_Texture()
{
	if (m_tTexInfo.pTexture)
		m_tTexInfo.pTexture->Release();
}
