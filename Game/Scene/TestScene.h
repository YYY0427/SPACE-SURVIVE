#pragma once
#include "SceneBase.h"
#include <memory>

// �v���g�^�C�v�錾
class RockManager;
class PlanetManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;
class Image3DManager;

// �e�X�g�V�[��
// �F�X�����悤�̃V�[��
class TestScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	TestScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~TestScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ʏ�̍X�V
	void NormalUpdate();

	// �Q�[���I�[�o�[���̍X�V
	void CollisionRockUpdate();

	// �v���C���[�������S���̍X�V
	void DeathFallPlayerUpdate();

	// �v���C���[�̗����������܂Ƃ߂�����
	void PlayerFallProcess();

	// �n�ʂ̐��̕`��
	void GroundLineDraw();

	/// <summary>
	/// �v���C���[�����̏�ɂ��邩����
	/// </summary>
	/// <returns>true : ���̏�ɂ���, false : ���̏�ɂ��Ȃ�</returns>
	bool JudgePlayerOnTheRoad();

	/// <summary>
	/// �v���C���[����L�΂������Ƃ��̐��ɓ����������̋���������̋����𒴂��Ă��邩
	/// </summary>
	/// <returns>true : ����̋����𒴂��Ă���, false : ����̋����𒴂��Ă��Ȃ�</returns>
	bool OverLimitPlayerHeight();

	/// <summary>
	/// �v���C���[���牺�ɐL�тĂ�����Ɠ��Ƃ̓����蔻��
	/// �������Ă��铹���������珈���I��
	/// ���̓|���S����2�g���ĕ`�悵�Ă��邽�߁Aresult��2��
	/// </summary>
	/// <param name="result">���Ɠ��������|���S��1�ڂ̓����蔻�茋��</param>
	/// <param name="result2">���ƃ|���S��2�ڂ̓����蔻�茋��</param>
	void CollisionRoadAndPlayer(HITRESULT_LINE& result, HITRESULT_LINE& result2);
private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (TestScene::* updateFunc_) ();

	// �|�C���^
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<Image3DManager> pImg3DManager_;
};

