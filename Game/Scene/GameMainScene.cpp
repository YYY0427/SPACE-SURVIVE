#include "GameMainScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LazerManager.h"
#include "../Background.h"
#include "../Shield.h"
#include "../common.h"

namespace
{
}

// �R���X�g���N�^
GameMainScene::GameMainScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&GameMainScene::NormalUpdate)
{
	// �I�u�W�F�N�g�̔z�u�f�[�^�̓ǂݍ���
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData("Data/ObjectData.dat");

	// �C���X�^���X����
	pBackground_ = std::make_shared<Background>();
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>();
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_);
	pPlanetManager_ = std::make_shared<PlanetManager>();
	pCamera_ = std::make_shared<Camera>(pPlayer_);

	/*Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		windEffectH_, 
		EffectID::wind, 
		{ pPlayer_->GetPos().x, pPlayer_->GetPos().y, pPlayer_->GetPos().z + 500.0f},
		200.0f,
		0.7f,
		{ 0.0f, -DX_PI_F / 2, 0.0f});
	*/
}

// �f�X�g���N�^
GameMainScene::~GameMainScene()
{
	// �S�ẴG�t�F�N�g�̍폜
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// �����o�֐��|�C���^�̍X�V
void GameMainScene::Update()
{
	timer_.Update(1);
	
	(this->*updateFunc_)();
}

// �`��
void GameMainScene::Draw()
{
	// �e�N���X�̕`��
	pBackground_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();
	pLazerManager_->Draw();

	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("GameMainScene");

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}

// �ʏ�̍X�V
void GameMainScene::NormalUpdate()
{
	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (item_)
		{
			// �|�[�Y�V�[���ɑJ��
		case SceneItem::PAUSE:
			manager_.PushScene(new PauseScene(manager_));
			break;

		case SceneItem::TITLE:
#ifdef _DEBUG
			manager_.ChangeScene(new DebugScene(manager_));
#else
			manager_.ChangeScene(new TitleScene(manager_));
#endif
			return;
		}
		// PushScene����̂ŃV�[�����c�邽�߃t�F�[�h�C���̐ݒ�
		StartFadeIn();
		return;
	}

	// �X�V
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update(timer_.GetTime());
	pLazerManager_->Update();
	pPlanetManager_->Update();
	pCamera_->Update();

	// ���[�U�[�ƃV�[���h�̓����蔻��
	for (auto& lazer : pLazerManager_->GetLazeres())
	{
		// �V�[���h���o���Ă��Ȃ������画����s��Ȃ�
		if (!pPlayer_->GetShield()->GetIsShield()) continue;

		// ���ˉ\�ȃ��[�U�[�ȊO�̏ꍇ�͔�����s��Ȃ�
		if (lazer.type != LazerType::NORMAL) continue;
		
		// ���ˌ�̃��[�U�[�̏ꍇ�͓����蔻����s��Ȃ�
		if (lazer.pLazer->GetIsRefrect()) continue;

		VECTOR leftTop = pPlayer_->GetShield()->GetVertex()[0].pos;
		VECTOR rightTop = pPlayer_->GetShield()->GetVertex()[1].pos;
		VECTOR leftBottom = pPlayer_->GetShield()->GetVertex()[2].pos;
		VECTOR rightBottom = pPlayer_->GetShield()->GetVertex()[3].pos;

		// �V�[���h�͓�̃|���S������ł��Ă�̂œ�̃|���S���Ƃ��`�F�b�N
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
		MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

		// 1�ł��|���S���Ɠ������Ă�����
		if (result.HitNum > 0 || result2.HitNum > 0)
		{
			// �V�[���h�̖@�����
			VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;

			// ���[�U�[�𔽎�
			lazer.pLazer->Refrect(pPlayer_->GetShield()->GetPos(), shieldNorm);
		}

		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
		MV1CollResultPolyDimTerminate(result2);
	}

	// ���[�U�[�ƓG�̓����蔻��
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// ���ˉ\�ȃ��[�U�[�ȊO�̏ꍇ�͔�����s��Ȃ�
		if (laser.type != LazerType::NORMAL) continue;

		// ���ˌ�̃��[�U�[�Ƃ��������蔻����s��Ȃ�
		if (!laser.pLazer->GetIsRefrect()) continue;

		// �S�Ă̓G���`�F�b�N
		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			// ���[�U�[�̃��f���ƓG�����̂ɂ݂��Ăē����蔻��
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLazer->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			// �|���S����ɂł��������Ă�����
			if (result.HitNum > 0)
			{
				enemy->OnDamage(10.0f, result.Dim->HitPosition);

				// �����������̂�h�����߈�񓖂������烌�[�U�[���폜
				laser.pLazer->Delete();
			}

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// ���[�U�[�ƃv���C���[�̓����蔻��
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// ���G���Ԓ��Ȃ瓖����Ȃ�
		if (pPlayer_->IsUltimate()) continue;

		// ���ˌ�̃��[�U�[�̏ꍇ�v���C���[�Ɠ����蔻����s��Ȃ�
		if (laser.pLazer->GetIsRefrect()) continue;

		// ���[�U�[�ƃv���C���[�̓����蔻��
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLazer->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1�ł��|���S���Ɠ������Ă�����
		if (result.HitNum > 0)
		{
			// Update���Q�[���I�[�o�[����Update�ɕύX
			updateFunc_ = &GameMainScene::CollisionRockUpdate;

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
			return;
		}

		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
	}

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
		item_ = SceneItem::PAUSE;
	}

	// �Q�[���I�[�o�[
	if (!pPlayer_->IsLive())
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(255, 10);
		item_ = SceneItem::TITLE;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

void GameMainScene::CollisionRockUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		if (pPlayer_->IsLive())
		{
			StartFadeIn();

			// Update��ʏ펞��Update�ɕύX
			updateFunc_ = &GameMainScene::NormalUpdate;
			return;
		}
	}

	if (!IsFadeing())
	{
		bool isEnd = pPlayer_->OnDamageUpdate();

		if (isEnd)
		{
			// �t�F�[�h�A�E�g�̊J�n
			StartFadeOut(255);
		}
	}

	// �X�V
	pCamera_->Update();
	pLazerManager_->Update();
	pEnemyManager_->Update(timer_.GetTime());

	// �t�F�[�h�̍X�V
	UpdateFade();
}