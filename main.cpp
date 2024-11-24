#include <iostream>
#include <cassert>
#include "TextureConverter.h"
#include <Windows.h>

//�R�}���h���C������
enum Argument {
	kApplicationPath, //�A�v���P�[�V����(.exe)�̃p�X
	kFilePath,        //�n���ꂽ(�ϊ�����)�t�@�C���̃p�X

	NumArgument,
};

int main(int argc, char* argv[]) {
	//�R�}���h���C�������w��Ȃ�
	if (argc < NumArgument) {
		//�g�����̕\��
		TextureConverter::OutputUsage();
		return 0;
	}

	//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	TextureConverter converter;
	//�I�v�V�����̐�
	int numOptions = argc - NumArgument;
	//�I�v�V�����z��(�_�u���|�C���^)
	char** options = argv + NumArgument;

	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);

	//COM���C�u�����̏I��
	CoUninitialize();

	//system("pause");
	return 0;
}