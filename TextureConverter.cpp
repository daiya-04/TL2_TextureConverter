#include "TextureConverter.h"
#include "Windows.h"
#include <iostream>

using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[]) {
	
	//テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);
	//DDS形式に変換して書きだす
	SaveDDSTextureToFile(numOptions, options);

}

void TextureConverter::OutputUsage() {

	std::cout << "画像ファイルをWIC形式からDDS形式に変換します" << std::endl;
	std::cout << "\n";
	std::cout << "TextureConverter [ドライブ：][パス][ファイル名][ -ml レベル]" << std::endl;
	std::cout << "\n";
	std::cout << " [ドライブ：][パス][ファイル名][ -ml レベル] : 変換したいWIC形式の画像ファイルを指定します" << std::endl;
	std::cout << "\n";
	std::cout << "[ -ml レベル] : ミップマップのレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します" << std::endl;

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);
	//WICテクスチャの読み込み
	HRESULT hr = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));
	//フォルダパスとファイル名を分離
	SeparateFilePath(wFilePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {

	//空なら空を返す
	if (mString.empty()) {
		return std::wstring();
	}

	//ワイド文字列に変換した際の文字数の計算
	auto stringSize = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), NULL, 0);

	std::wstring wString(stringSize, 0);
	//ワイド文字列の変換
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), &wString[0], stringSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;
	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');

	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の後ろを拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//区切る文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	//区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('\\');
	//検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		//区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//区切り文字がないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {

	HRESULT hr;

	size_t mipLevel = 0;
	DXGI_FORMAT compressFormat = DXGI_FORMAT_BC7_UNORM_SRGB;

	//ミップマップのレベル指定を検索
	for (int index = 0; index < numOptions; index++) {
		if (std::string(options[index]) == "-ml") {
			//ミップレベル指定
			mipLevel = std::stoi(options[index + 1]);
			continue;
		}
		if (std::string(options[index]) == "-bc") {
			size_t compressNum = std::stoi(options[index + 1]);

			switch (compressNum) {
				case 1:
					compressFormat = DXGI_FORMAT_BC1_UNORM_SRGB;
					break;
				case 2:
					compressFormat = DXGI_FORMAT_BC2_UNORM_SRGB;
					break;
				case 3:
					compressFormat = DXGI_FORMAT_BC3_UNORM_SRGB;
					break;
				case 4:
					compressFormat = DXGI_FORMAT_BC4_UNORM;
					break;
				case 5:
					compressFormat = DXGI_FORMAT_BC5_UNORM;
					break;
				case 6:
					compressFormat = DXGI_FORMAT_BC6H_UF16;
					break;
				case 7:
					compressFormat = DXGI_FORMAT_BC7_UNORM_SRGB;
					break;
			}
			continue;
		}
	}

	ScratchImage mipChain;
	//ミップマップ生成
	hr = GenerateMipMaps(scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(), TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(hr)) {
		//イメージデータとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	//圧縮形式に変換
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, compressFormat, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}

	//読み込んだテクスチャをSRGBとして扱う
	metaData_.format = MakeSRGB(metaData_.format);

	//ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDSファイルの書き出し
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));

}
