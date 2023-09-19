#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Camera;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	virtual ~Player();

	// �X�V
	void Update();
	bool GameOverUpdate();

	// �`��
	void Draw();

	// �Q�b�^�[
	VECTOR GetPos();		// �ʒu���
	bool GetIsBoost();		// �u�[�X�g��Ԃ�
	float GetSlowRate();	// �X�����[�V�����̃��[�g
	float GetCollsionRadius();

	// �Z�b�^�[
	void SetCamera(std::shared_ptr<Camera> pCamera);
private:
	// �|�C���^
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// �ʒu���
	VECTOR pos_;

	// �v���C���[�̈ړ����x
	float moveSpeed_;

	// �G�l���M�[�Q�[�W
	float energyGauge_;

	// �X���[���[�V�����̃��[�g
	float slowRate_;

	// ���X�e�B�b�N�����͂��ꂽ��
	bool isInput_;
};