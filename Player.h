#pragma once
#include <DxLib.h>
#include <memory>

using namespace std;

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

	// �`��
	void Draw();

	// �Q�b�^�[
	VECTOR GetPos();		// �ʒu���
	bool GetIsBoost();		// �u�[�X�g��Ԃ�
	float GetSlowRate();	// �X�����[�V�����̃��[�g

	// �Z�b�^�[
	void SetCamera(shared_ptr<Camera> pCamera);
private:
	shared_ptr<Model> pModel_;

	shared_ptr<Camera> pCamera_;

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