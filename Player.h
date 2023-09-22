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

	/// <summary>
	/// �Q�[���I�[�o�[���̃v���C���[�̍X�V
	/// </summary>
	/// <returns>�G�t�F�N�g���Đ����I������</returns>
	bool GameOverUpdate();

	// �`��
	void Draw();

	// �ʒu���̎擾
	VECTOR GetPos();			

	/// <summary>
	/// �u�[�X�g��Ԃ��̎擾
	/// </summary>
	/// <returns>true : �u�[�X�g��ԁAfalse : �ʏ���</returns>
	bool GetIsBoost();			

	// �X���[���[�V�����̃��[�g�̎擾
	float GetSlowRate();		

	// �v���C���[�̓����蔻��̔��a�̎擾
	float GetCollsionRadius();	

	// �J�����N���X�̃|�C���^�̐ݒ�
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);
private:
	// �|�C���^
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// �ʒu���
	VECTOR pos_;
	
	// �Q�[���I�[�o�[���̃G�t�F�N�g�҂��p�̃^�C�}�[
	int gameOverEffectWaitTimer_;

	// �v���C���[�̈ړ����x
	float moveSpeed_;

	// �G�l���M�[�Q�[�W
	float energyGauge_;

	// �X���[���[�V�����̃��[�g
	float slowRate_;

	// �X���[���[�V������Ԃ��ǂ���
	bool isSlow_;

	// �u�[�X�g��Ԃ��ǂ���
	bool isBoost_;

	// ���X�e�B�b�N�����͂��ꂽ��
	bool isInput_;

	// �G�t�F�N�g����񂾂��Đ�
	bool isEffectFirst_;
};