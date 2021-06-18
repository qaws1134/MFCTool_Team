#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();
public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const DWORD dwCount = 0 )PURE; 
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"",
		const DWORD dwIndex = 0)PURE; 
	virtual void Release_Texture() PURE; 

	// ObjectKey, StateKey
	//�̰� ��Ƽ �ؽ����� ���� ���� 
	//map<wstring, CMulti_Texture(map<wstring, vector<TEXINFO*>>)> �����������K 
};

