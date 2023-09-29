#include "StringManager.h"
#include <cassert>
#include <fstream>
#include <sstream>

namespace
{
	// �ǂݍ��ރt�H���g�t�@�C����
	const LPCSTR font_data_path[] =
	{
		"Data/Font/Makinas-4-Flat.otf",
		"Data/Font/Makinas-4-Square.otf",
		"Data/Font/azuki.ttf"
	};
}

// �R���X�g���N�^
StringManager::StringManager()
{

}

// �f�X�g���N�^
StringManager::~StringManager()
{
}

// �B��̃C���X�^���X��Ԃ�
StringManager& StringManager::GetInstance()
{
	// �B��̎���
	static StringManager instance;

	// �B��̎��ԎQ�Ƃ�Ԃ�
	return instance;
}

// �t�@�C�����當����̃f�[�^��ǂݎ���ăf�[�^�e�[�u���Ɋi�[
void StringManager::LoadAndStoreStringFileData()
{
	// �t�H���g�̃��[�h
	for (auto& fontPath : font_data_path)
	{
		if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
		{
			// ���[�h�Ɏ��s������~�߂�
			assert(0);
		}
	}

	// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
	std::ifstream ifs("Data/Csv/String.csv");
	assert(ifs);

	// csv�f�[�^��1�s���ǂݎ��
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1�s�ڂ͓ǂݍ��܂Ȃ�
		// 1�s�ڂɂ͍��ڂ������Ă��邽��
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csv�f�[�^�P�s��','�ŕ����̕�����ɕϊ�
		std::vector<std::string> strvec = SplitString(line, ',');

		// csv�f�[�^����擾�����f�[�^����t�H���g�n���h���̍쐬���Ċi�[
		StringData data;
		std::string font = strvec[static_cast<int>(StringDataType::FONT)];		
		int size = std::stoi(strvec[static_cast<int>(StringDataType::SIZE)]);
		int thick = std::stoi(strvec[static_cast<int>(StringDataType::THICK)]);
		data.fontHandle = CreateFontToHandle(font.c_str(), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

		// ������̊i�[
		data.string = strvec[static_cast<int>(StringDataType::STRING)];	

		// �f�[�^�̊i�[
		stringDataTable_[strvec[static_cast<int>(StringDataType::ID)]] = data;
	}
}

// �I������
void StringManager::End()
{
	// �t�H���g�f�[�^�̍폜
	for (auto& font : stringDataTable_)
	{
		DeleteFontToHandle(font.second.fontHandle);
	}
	// �t�H���g�̃A�����[�h
	for (auto& fontPath : font_data_path)
	{
		if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) == 0)
		{
			// �A�����[�h�Ɏ��s������~�߂�
			assert(0);
		}
	}
}

// ���������؂�(�����̕�����ɕϊ�)
std::vector<std::string> StringManager::SplitString(std::string& input, char delimiter)
{
	// string��getline�Ŏg����f�[�^�ɕϊ�
	std::istringstream stream(input);	

	// ��������������1�����i�[
	std::string field;					

	// ������̕�����̔z��
	std::vector<std::string> result;	

	while (getline(stream, field, delimiter))
	{
		// ��������������1�����i�[����
		result.push_back(field);
	}
	return result;
}

// �t�@�C���ɓǂݍ��񂾕�����̕\��
void StringManager::DrawString(std::string id, int x, int y, unsigned int color)
{
	// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
	assert(stringDataTable_.find(id) != stringDataTable_.end());	

	DrawStringToHandle(x, y, stringDataTable_[id].string.c_str(), color, stringDataTable_[id].fontHandle);
}

// �t�@�C���ɓǂݍ��񂾕�������w�肵�����W�𒆐S�Ƃ����ʒu�ɕ\��
void StringManager::DrawStringCenter(std::string id, int x, int y, unsigned int color)
{
	// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
	assert(stringDataTable_.find(id) != stringDataTable_.end());	

	// �t�H���g�T�C�Y�A�A������̒������當����̉����̎擾
	std::string string = stringDataTable_[id].string;
	int fontHandle = stringDataTable_[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);

	// �擾����������̉������當������������ʒu�ɕ\��
	DrawStringToHandle(x - width / 2, y, string.c_str(), color, fontHandle);
}

// �t�H���g���g�����������w�肵�����W�𒆐S�Ƃ��ĕ\��
void StringManager::DrawFormatStringCenter(std::string id, int num, int x, int y, unsigned int color)
{
	// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
	assert(stringDataTable_.find(id) != stringDataTable_.end());

	// �t�H���g�T�C�Y�A�A������̒������當����̉����̎擾
	std::string string = std::to_string(num);
	int fontHandle = stringDataTable_[id].fontHandle;
	int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);

	// �擾����������̉������當������������ʒu�ɕ\��
	DrawFormatStringToHandle(x - width / 2, y, color, fontHandle, "%d", num);
}