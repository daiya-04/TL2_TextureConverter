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

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	TextureConverter converter;
	//テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	//COMライブラリの終了
	CoUninitialize();

	system("pause");
	return 0;
}