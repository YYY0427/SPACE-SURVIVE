#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/DataReaderFromUnity.h"
#include "Util/Timer.h"

class Model;
class Camera;

// �v���C���[�N���X
class Player
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="data"></param>
	Player(UnityGameObject data);

	// �f�X�g���N�^
	virtual ~Player();

	// �ʏ�̍X�V
	void Update();

	/// <summary>
	/// ��Ƃ̏Փˎ��̍X�V
	/// </summary>
	/// <returns>�������I�������� true : �����I��, false : �����r��</returns>
	bool CollisionRockUpdate();

	// �u�[�X�g�̏���
	void BoostProcess();

	// �X���[���[�V�����̏���
	void SlowProcess();

	// �G�l���M�[�̏���
	void EnergyProcess();

	// �`��
	void Draw();

	/// <summary>
	/// �v���C���[�̗������� 
	/// </summary>
	/// <param name="fallSpeed">�����X�s�[�h</param>
	void Fall(float fallSpeed);

	// �v���C���[�̃��X�|�[������
	void Respawn(VECTOR restartPos);

	// �v���C���[�̃_���[�W����
	void OnDamage();

	/// </summary>
	/// �v���C���[�̍������������S����̍�����菬�����Ȃ�����
	/// </summary>
	/// <returns>true : �������Ȃ���, false : �������Ȃ��ĂȂ�</returns>
	bool IsDeathJudgHeight() const;

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

	/// <summary>
	/// �u�[�X�g��Ԃ��̎擾
	/// </summary>
	/// <returns>true : �u�[�X�g��ԁAfalse : �ʏ���</returns>
	bool GetIsBoost() const;			

	// �X���[���[�V�����̃��[�g�̎擾
	float GetSlowRate() const;		

	// �v���C���[�̓����蔻��̔��a�̎擾
	float GetCollsionRadius() const;	

	// �v���C���[���f���̃n���h���̎擾
	int GetModelHandle() const; 

	// �J�����N���X�̃|�C���^�̐ݒ�
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);
private:
	// �|�C���^
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// �ʒu���
	VECTOR pos_;
	
	// ��]���
	VECTOR rot_;

	// �ړ��x�N�g��
	VECTOR moveVec_;

	// ��
	int hp_;

	// ���G����
	int ultimateTimer_;

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

	float boostEffectScale_;
};