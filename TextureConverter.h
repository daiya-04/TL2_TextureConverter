#pragma once
#include <DirectXTex.h>
#include <string>

class TextureConverter {
public:
	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath, int numOptions = 0, char* options[] = nullptr);
	/// <summary>
	/// 使用方法を表示する
	/// </summary>
	static void OutputUsage();

private:
	/// <summary>
	/// マルチバイト文字列からワイド文字列に変換
	/// </summary>
	/// <param name="mString"></param>
	/// <returns></returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);
	/// <summary>
	/// ファイルパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath"></param>
	void SeparateFilePath(const std::wstring& filePath);
	/// <summary>
	/// DDSテクスチャとして書き出し
	/// </summary>
	void SaveDDSTextureToFile(int numOptions, char* options[]);
	
private:
	
	//画像の情報
	DirectX::TexMetadata metaData_;
	//画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_;

	//ディレクトリパス
	std::wstring directoryPath_;
	//ファイル名
	std::wstring fileName_;
	//拡張子
	std::wstring fileExt_;

};

