#include "TextureConverter.h"
#include "Windows.h"

using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {

	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {

	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	HRESULT hr = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));

	SeparateFilePath(wFilePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {

	if (mString.empty()) {
		return std::wstring();
	}

	auto stringSize = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), NULL, 0);
	if (stringSize == 0) {
		return std::wstring();
	}
	std::wstring wString(stringSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), &wString[0], stringSize);
	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind('.');

	if (pos1 != std::wstring::npos) {
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	pos1 = filePath.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDSTextureToFile() {

	HRESULT hr;

	ScratchImage mipChain;
	hr = GenerateMipMaps(scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}


	metaData_.format = MakeSRGB(metaData_.format);

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));

}
