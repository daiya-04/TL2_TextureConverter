#pragma once
#include <DirectXTex.h>
#include <string>

class TextureConverter {
public:
	/// <summary>
	/// �e�N�X�`����WIC����DDS�ɕϊ�����
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void ConvertTextureWICToDDS(const std::string& filePath, int numOptions = 0, char* options[] = nullptr);
	/// <summary>
	/// �g�p���@��\������
	/// </summary>
	static void OutputUsage();

private:
	/// <summary>
	/// �}���`�o�C�g�����񂩂烏�C�h������ɕϊ�
	/// </summary>
	/// <param name="mString"></param>
	/// <returns></returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
	/// <summary>
	/// �e�N�X�`���t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadWICTextureFromFile(const std::string& filePath);
	/// <summary>
	/// �t�@�C���p�X�ƃt�@�C�����𕪗�����
	/// </summary>
	/// <param name="filePath"></param>
	void SeparateFilePath(const std::wstring& filePath);
	/// <summary>
	/// DDS�e�N�X�`���Ƃ��ď����o��
	/// </summary>
	void SaveDDSTextureToFile(int numOptions, char* options[]);
	
private:
	
	//�摜�̏��
	DirectX::TexMetadata metaData_;
	//�摜�C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage_;

	//�f�B���N�g���p�X
	std::wstring directoryPath_;
	//�t�@�C����
	std::wstring fileName_;
	//�g���q
	std::wstring fileExt_;

};

