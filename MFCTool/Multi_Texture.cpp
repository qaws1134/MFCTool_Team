#include "stdafx.h"
#include "Multi_Texture.h"


CMulti_Texture::CMulti_Texture()
{
}


CMulti_Texture::~CMulti_Texture()
{
	Release_Texture(); 
}
//..\Texture\Stage\Terrain\Tile\Tile%d.png
HRESULT CMulti_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD dwCount /*= 0*/)
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_mapMultiTex.find(wstrStateKey);
	if (m_mapMultiTex.end() == iter)
	{
		TEXINFO* pTexInfo = nullptr; 
		for (size_t i = 0 ; i < dwCount ; ++i)
		{
			pTexInfo = new TEXINFO; 
			TCHAR szFilePath[MAX_PATH] = L"";
			swprintf_s(szFilePath, wstrFilePath.c_str(), i);
			if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImageInfo)))
			{
				Safe_Delete(pTexInfo);
				goto ERR;
			}
				
			if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
				szFilePath,
				pTexInfo->tImageInfo.Width, // �׸� ����
				pTexInfo->tImageInfo.Height, // �׸� ���� 
				pTexInfo->tImageInfo.MipLevels, // �ӷ��� 
												 // �̹��� �ӷ����� �������� �ְ� ���� ���� �ִ�. 
				0,
				pTexInfo->tImageInfo.Format,
				D3DPOOL_MANAGED, // �츮�� «¥���� ���ڴ�. ����? 
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				&pTexInfo->pTexture)))
			{
				Safe_Delete(pTexInfo);
				goto ERR;
			}

			m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo);
		}
	}
	return S_OK;
ERR:
	return E_NOTIMPL;
}

const TEXINFO * CMulti_Texture::Get_TexInfo(const wstring & wstrStateKey /*= L""*/, const DWORD dwIndex /*= 0*/)
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_mapMultiTex.find(wstrStateKey);
	if(m_mapMultiTex.end() == iter )
		return nullptr;

	if (iter->second.size() <= dwIndex)
		return nullptr; 

	return iter->second[dwIndex];
	//return m_mapMultiTex[wstrStateKey][dwIndex]; 
}

void CMulti_Texture::Release_Texture()
{
	for (pair<const wstring, vector<TEXINFO*>>& rPair : m_mapMultiTex)
	{
		for (TEXINFO*& pTexInfo : rPair.second)
			Safe_Delete(pTexInfo); 

		rPair.second.clear();
		//rPair.second.swap(vector<TEXINFO*>()); 
		rPair.second.shrink_to_fit(); //size�� �°� �޸𸮰����� ������ִ� �Լ�. 
	}
	m_mapMultiTex.clear(); 
}
