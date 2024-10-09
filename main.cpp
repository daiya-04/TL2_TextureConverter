#include <iostream>
#include <cassert>
#include "TextureConverter.h"
#include <Windows.h>

enum Argument {
	kApplicationPath,
	kFilePath,

	NumArgument,
};

int main(int argc, char* argv[]) {
	assert(argc >= NumArgument);

	//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	TextureConverter converter;
	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COM���C�u�����̏I��
	CoUninitialize();

	system("pause");
	return 0;
}