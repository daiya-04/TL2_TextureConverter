#include "TextureConverter.h"
#include "Windows.h"
#include <iostream>

using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[]) {
	
	//�e�N�X�`���t�@�C����ǂݍ���
	LoadWICTextureFromFile(filePath);
	//DDS�`���ɕϊ����ď�������
	SaveDDSTextureToFile(numOptions, options);

}

void TextureConverter::OutputUsage() {

	std::cout << "�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂�" << std::endl;
	std::cout << "\n";
	std::cout << "TextureConverter [�h���C�u�F][�p�X][�t�@�C����][ -ml ���x��]" << std::endl;
	std::cout << "\n";
	std::cout << " [�h���C�u�F][�p�X][�t�@�C����][ -ml ���x��] : �ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂�" << std::endl;
	std::cout << "\n";
	std::cout << "[ -ml ���x��] : �~�b�v�}�b�v�̃��x�����w�肵�܂��B0���w�肷���1x1�܂ł̃t���~�b�v�}�b�v�`�F�[���𐶐����܂�" << std::endl;

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	//�t�@�C���p�X�����C�h������ɕϊ�
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);
	//WIC�e�N�X�`���̓ǂݍ���
	HRESULT hr = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));
	//�t�H���_�p�X�ƃt�@�C�����𕪗�
	SeparateFilePath(wFilePath);

}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {

	//��Ȃ���Ԃ�
	if (mString.empty()) {
		return std::wstring();
	}

	//���C�h������ɕϊ������ۂ̕������̌v�Z
	auto stringSize = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), NULL, 0);

	std::wstring wString(stringSize, 0);
	//���C�h������̕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), &wString[0], stringSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {

	size_t pos1;
	std::wstring exceptExt;
	//��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');

	//�������q�b�g������
	if (pos1 != std::wstring::npos) {
		//��؂蕶���̌����g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		//��؂镶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	//��؂蕶��'\\'���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('\\');
	//�������q�b�g������
	if (pos1 != std::wstring::npos) {
		//��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//��؂蕶��'/'���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		//��؂蕶���̑O�܂ł��f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	//��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {

	HRESULT hr;

	size_t mipLevel = 0;
	DXGI_FORMAT compressFormat = DXGI_FORMAT_BC7_UNORM_SRGB;

	//�~�b�v�}�b�v�̃��x���w�������
	for (int index = 0; index < numOptions; index++) {
		if (std::string(options[index]) == "-ml") {
			//�~�b�v���x���w��
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
	//�~�b�v�}�b�v����
	hr = GenerateMipMaps(scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(), TEX_FILTER_DEFAULT, mipLevel, mipChain);
	if (SUCCEEDED(hr)) {
		//�C���[�W�f�[�^�ƃ��^�f�[�^���A�~�b�v�}�b�v�łŒu��������
		scratchImage_ = std::move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	//���k�`���ɕϊ�
	ScratchImage converted;
	hr = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, compressFormat, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT | TEX_COMPRESS_PARALLEL, 1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}

	//�ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metaData_.format = MakeSRGB(metaData_.format);

	//�t�@�C������ݒ肷��
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDS�t�@�C���̏����o��
	hr = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metaData_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));

}
