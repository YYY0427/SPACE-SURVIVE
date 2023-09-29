#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include "../common.h"

class StringManager
{
public:
	// �f�X�g���N�^
	~StringManager();

	/// <summary>
	/// �B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static StringManager& GetInstance();

	// �t�@�C�����當����̃f�[�^��ǂݎ���ăf�[�^�e�[�u���Ɋi�[
	void LoadAndStoreStringFileData();

	// �I������
	void End();

	/// <summary>
	/// ���������؂�(�����̕�����ɕϊ�)
	/// </summary>
	/// <param name="input">��؂肽��������</param>
	/// <param name="delimiter">���������؂镶��</param>
	/// <returns>��؂���������</returns>
	std::vector<std::string> SplitString(std::string& input, char delimiter);

	/// <summary>
	/// �t�@�C���ɓǂݍ��񂾕�����̕\��
	/// </summary>
	/// <param name="id">�\���������������ID</param>
	/// <param name="x">Y���̍��W</param>
	/// <param name="y">Y���̍��W</param>
	/// <param name="color">�����̕`��F</param>
	void DrawString(std::string id, int x, int y, unsigned int color);

	/// <summary>
	/// �t�@�C���ɓǂݍ��񂾕�������w�肵�����W�𒆐S�Ƃ����ʒu�ɕ\��
	/// </summary>
	/// <param name="id">�\���������������ID</param>
	/// <param name="x">�^�񒆂Ɏw�肵����X���̍��W</param>
	/// <param name="y">Y���̍��W</param>
	/// <param name="color">�����̕`��F</param>
	void DrawStringCenter(std::string id, int x, int y, unsigned int color);

	/// <summary>
	/// �t�H���g�ɓǂݍ��񂾃f�[�^����쐬�����t�H���g�f�[�^���g����
	/// �w�肵�����W�𒆐S�Ƃ����ʒu�ɐ����̕\��
	/// </summary>
	/// <param name="id">�g�������t�H���g��ID</param>
	/// <param name="num">�\������������</param>
	/// <param name="x">�^�񒆂Ɏw�肵����X���̍��W</param>
	/// <param name="y">Y���̍��W</param>
	/// <param name="color">�����̕`��F</param>
	void DrawFormatStringCenter(std::string id, int num, int x, int y, unsigned int color);

private:
	// �R���X�g���N�^
	// �V���O���g���p�^�[���Ȃ̂�private
	StringManager();

	// �R�s�[�Ƒ���̋֎~
	StringManager(const StringManager&) = delete;			// �R�s�[�R���X�g���N�^�֎~
	void operator = (const StringManager&) = delete;		// ����֎~
private:
	// ������f�[�^�̎��
	enum class StringDataType
	{
		ID,
		STRING,
		FONT,
		SIZE,
		THICK
	};
private:
	// ������̃f�[�^
	struct StringData
	{
		int fontHandle;
		std::string string;
	};
private:
	// CSV�t�@�C���Ŏw�肵���������ID�Ƃ���������̃f�[�^�e�[�u��
	std::unordered_map<std::string, StringData> stringDataTable_;
};