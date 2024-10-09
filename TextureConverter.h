#pragma once
#include <DirectXTex.h>
#include <string>

class TextureConverter {
public:

	void ConvertTextureWICToDDS(const std::string& filePath);

private:

	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
	
	void LoadWICTextureFromFile(const std::string& filePath);
	
	void SeparateFilePath(const std::wstring& filePath);

	void SaveDDSTextureToFile();
	
private:
	
	DirectX::TexMetadata metaData_;
	DirectX::ScratchImage scratchImage_;

	std::wstring directoryPath_;
	std::wstring fileName_;
	std::wstring fileExt_;

};

