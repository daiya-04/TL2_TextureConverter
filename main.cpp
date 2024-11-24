#include <iostream>
#include <cassert>
#include "TextureConverter.h"
#include <Windows.h>

//コマンドライン引数
enum Argument {
	kApplicationPath, //アプリケーション(.exe)のパス
	kFilePath,        //渡された(変換する)ファイルのパス

	NumArgument,
};

int main(int argc, char* argv[]) {
	//コマンドライン引数指定なし
	if (argc < NumArgument) {
		//使い方の表示
		TextureConverter::OutputUsage();
		return 0;
	}

	//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	TextureConverter converter;
	//オプションの数
	int numOptions = argc - NumArgument;
	//オプション配列(ダブルポインタ)
	char** options = argv + NumArgument;

	//テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);

	//COMライブラリの終了
	CoUninitialize();

	//system("pause");
	return 0;
}