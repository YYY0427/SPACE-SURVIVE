#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Camera;

// �v���C���[�N���X
class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	virtual ~Player();

	// �ʏ�̍X�V
	void Update();

	/// <summary>
	/// �Q�[���I�[�o�[���̍X�V
	/// </summary>
	/// <returns>�G�t�F�N�g���Đ����I������</returns>
	bool GameOverUpdate();

	// �u�[�X�g�̏���
	void BoostProcess();

	// �X���[���[�V�����̏���
	void SlowProcess();

	// �G�l���M�[�̏���
	void EnergyProcess();

	// �`��
	void Draw();

	// �ʒu���̎擾
	VECTOR GetPos() const;			

	/// <summary>
	/// �u�[�X�g��Ԃ��̎擾
	/// </summary>
	/// <returns>true : �u�[�X�g��ԁAfalse : �ʏ���</returns>
	bool GetIsBoost() const;			

	// �X���[���[�V�����̃��[�g�̎擾
	float GetSlowRate() const;		

	// �v���C���[�̓����蔻��̔��a�̎擾
	float GetCollsionRadius() const;	

	// �J�����N���X�̃|�C���^�̐ݒ�
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);
private:
	// �|�C���^
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// �ʒu���
	VECTOR pos_;
	
	// �ړ��x�N�g��
	VECTOR moveVec_;

	// �v���C���[�̈ړ����x
	float moveSpeed_;

	// �G�l���M�[�Q�[�W
	float energyGauge_;

	// �X���[���[�V�����̃��[�g
	float slowRate_;

	// �ړ��x�N�g���𔽓]������
	bool isReverseMoveVec_;

	// �X���[���[�V������Ԃ��ǂ���
	bool isSlow_;

	// �u�[�X�g��Ԃ��ǂ���
	bool isBoost_;

	// ���X�e�B�b�N�����͂��ꂽ��
	bool isInput_;

	// �Q�[���I�[�o�[�G�t�F�N�g���Đ�������
	bool isPlayGameOverEffect_;
};