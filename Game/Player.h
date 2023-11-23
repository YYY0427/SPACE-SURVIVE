#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Timer.h"
#include <deque>

class Model;
class Shield;

// �v���C���[�N���X
class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// �X�V
	void Update(float cameraYaw);

	// �`��
	void Draw();
	void DrawUI();

	/// <summary>
	/// �Փˎ��̍X�V
	/// </summary>
	/// <returns>�������I�������� true : �����I��, false : �����r��</returns>
	bool OnDamageUpdate();

	// �G�l���M�[�̏���
	void EnergyProcess();

	// �v���C���[�̃��X�|�[������
	void Respawn(VECTOR restartPos);

	// �v���C���[�̃_���[�W����
	void OnDamage();

	/// <summary>
	/// �v���C���[�����G���Ԓ���
	/// </summary>
	/// <returns>true : ���G���Ԓ�, false : ���G���Ԓ�����Ȃ�</returns>
	bool IsUltimate() const;

	/// <summary>
	/// �v���C���[�������Ă��邩
	/// </summary>
	/// <returns>true : �����Ă���, false : ����ł���</returns>
	bool IsLive() const;

	// �ʒu���̎擾
	VECTOR GetPos() const;			

	// �v���C���[�̓����蔻��̔��a�̎擾
	float GetCollsionRadius() const;	

	// �v���C���[���f���̃n���h���̎擾
	int GetModelHandle() const; 

	// �V�[���h�̃C���X�^���X�̎擾
	std::shared_ptr<Shield> GetShield() const;

	// ���߂�ꂽ�t���[���̐������ʒu����ۑ�����e�[�u���̎擾
	std::deque<VECTOR> GetPosLogTable() const;

private:
	// �|�C���^
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Shield> pShield_;

	// ���߂�ꂽ�t���[���O�܂ňʒu����ۑ����Ă���e�[�u��
	std::deque<VECTOR> posLogTable_;

	// �G�t�F�N�g�n���h��
	int boostEffectHandle_;
	int playerDeadEffectHandle_;

	// �ʒu���
	VECTOR pos_;
	
	// ��]���
	VECTOR rot_;

	// �ړ��x�N�g��
	VECTOR moveVec_;

	// �u�[�X�g�G�t�F�N�g�̊g�嗦
	VECTOR boostEffectScale_;

	// �u�[�X�g�G�t�F�N�g�̍Đ����x
	float boostEffectSpeed_;

	// ��
	int hp_;

	// ���G����
	int ultimateTimer_;

	// �v���C���[�̈ړ����x
	float moveSpeed_;

	// �G�l���M�[�Q�[�W
	float energyGauge_;

	// �ړ��x�N�g���𔽓]������
	bool isReverseMoveVec_;

	// ���X�e�B�b�N�����͂��ꂽ��
	bool isInput_;

	// �Q�[���I�[�o�[�G�t�F�N�g���Đ�������
	bool isPlayGameOverEffect_;
};