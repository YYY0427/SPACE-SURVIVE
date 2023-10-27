#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Road.h"
#include "../RoadManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LazerManager.h"
#include "../common.h"

namespace
{
	// ������ǂ̂��炢�̍����܂Ńv���C���[���㏸�ł��邩
	constexpr float rise_possible_height = 2000.0f;

	// �ʏ�̃v���C���[�̗������x
	constexpr float normal_player_fall_speed = 20.0f;

	// �v���C���[�̍���������̍������������Ȃ������̗������x
	constexpr float special_player_fall_speed = 40.0f;
}

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&TestScene::NormalUpdate),
	nextRoad_(1)
{
	// �I�u�W�F�N�g�̔z�u�f�[�^�̓ǂݍ���
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();

	// �ǂݍ��񂾔z�u�f�[�^����I�u�W�F�N�g�̃C���X�^���X�̐���
	pRoadManager_ = std::make_shared<RoadManager>(pDataReader_->GetDataType("Stage"));
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>(pDataReader_->GetDataType("Player2").front());
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_, pDataReader_->GetDataType("BossEnemy").front(), pDataReader_->GetDataType("NormalEnemy"));
	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetDataType("Rock"), pDataReader_->GetDataType("Meteor"));
	pPlanetManager_ = std::make_shared<PlanetManager>(pDataReader_->GetDataType("Sun"), pDataReader_->GetDataType("Earth"));
	pCamera_ = std::make_shared<Camera>(pDataReader_->GetDataType("Camera").front());
	pSkyDome_ = std::make_shared<SkyDome>();
}

//  �f�X�g���N�^
TestScene::~TestScene()
{
	// �S�ẴG�t�F�N�g�̍폜
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// �����o�֐��|�C���^�̍X�V
void TestScene::Update()
{
//	UpdateLoadAsync();

	(this->*updateFunc_)();
}

// �`��
void TestScene::Draw()
{
	// �e�N���X�̕`��
	pSkyDome_->Draw();
	pRoadManager_->Draw();
	pRockManager_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pLazerManager_->Draw();
	pPlayer_->Draw();

	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("TestScene");

#ifdef _DEBUG
	// �n�ʂ̊p�̈ʒu�\��
	for (auto& road : pRoadManager_->GetRoads())
	{
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		DrawSphere3D(leftTop, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(rightTop, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(leftBottom, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(rightBottom, 32, 8, 0xff0000, 0xff0000, 0xff0000);
	}
#endif

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}

// �ʏ�̍X�V
void TestScene::NormalUpdate()
{
	// �e�N���X�̍X�V
	pSkyDome_->Update(pPlayer_->GetPos());
	pRoadManager_->Update(pPlayer_->GetPos());
	pCamera_->Update(pPlayer_->GetPos());
	pLazerManager_->Update();
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update();
	pRockManager_->Update();
	pPlanetManager_->Update();

	// ���̖����X�N���[������
	RoadInfiniteScroll();

	// �v���C���[�̗�������
	PlayerFallProcess();

	// ��ƂԂ�������Q�[���I�[�o�[
	for (auto& rocks : pRockManager_->GetRocks())
	{
		// ���G���Ԓ��Ȃ瓖����Ȃ�
		if (pPlayer_->IsUltimate()) continue;

		// ��ƃv���C���[�̓����蔻��`�F�b�N
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(rocks->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1�ł��|���S���Ɠ������Ă�����
		if (result.HitNum > 0)
		{
			// Update���Q�[���I�[�o�[����Update�ɕύX
			updateFunc_ = &TestScene::CollisionRockUpdate;
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
	}

	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		// PushScene����̂ŃV�[�����c�邽�߃t�F�[�h�C���̐ݒ�
		StartFadeIn();

		// �|�[�Y�V�[���ɑJ��
		manager_.PushScene(new PauseScene(manager_));
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// ��ƏՓˎ��̍X�V
void TestScene::CollisionRockUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// �J�����̍X�V
	pCamera_->Update(pPlayer_->GetPos());

	if (pPlayer_->CollisionRockUpdate() && !IsFadeing())
	{
		// �v���C���[�������Ă��邩
		if (pPlayer_->IsLive())
		{
			// Update��ʏ펞��Update�ɕύX
			updateFunc_ = &TestScene::NormalUpdate;
			return;
		}
		else
		{
			// �Q�[���I�[�o�[
			// �t�F�[�h�A�E�g�̊J�n
			StartFadeOut(255);
		}
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �v���C���[�������S���̍X�V
void TestScene::DeathFallPlayerUpdate()
{
	// �S�ẴG�t�F�N�g�̍폜
	Effekseer3DEffectManager::GetInstance().StopAllEffect();

	// �t�F�[�h�A�E�g���I�������烊�X�|�[��
	if (IsStartFadeOutAfterFadingOut())
	{
		// �_���[�W����
		pPlayer_->OnDamage();

		// �v���C���[�������Ă����烊�X�|�[��
		if (pPlayer_->IsLive())
		{
			// �v���C���[����1�ԋ߂����̍��W�̎擾
			VECTOR restartPos = pRoadManager_->GetClosestRoadPos(pPlayer_->GetPos());

			// �v���C���[�̃��X�|�[������
			pPlayer_->Respawn(restartPos);

			// �t�F�[�h�C���̊J�n
			StartFadeIn();

			// Update��ʏ��Update�ɕύX
			updateFunc_ = &TestScene::NormalUpdate;
		}
		// ����ł�����V�[���J��
		else
		{
			manager_.ChangeScene(new DebugScene(manager_));
		}
		return;
	}

	// 1�x�����t�F�[�h�A�E�g���������s
	if (!IsFadeing())
	{
		// �t�F�[�h�A�E�g�̊J�n
		StartFadeOut(255);
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �v���C���[�̗����������܂Ƃ߂�����
void TestScene::PlayerFallProcess()
{
	// �v���C���[�����̏�ɂ��邩
	bool isPlayerOnTheRoad = JudgePlayerOnTheRoad();

#if false
	if (!isPlayerOnTheRoad)
	{
		pPlayer_->A();
	}
#else
	// ������v���C���[�܂ł̋���������̋����𒴂��Ă��邩
	bool isOverLimitPlayerHeight = OverLimitPlayerHeight();

	// �v���C���[�����̏�ɂȂ�������v���C���[������
	if (!isPlayerOnTheRoad)
	{
		pPlayer_->Fall(normal_player_fall_speed);
	}
	// ������v���C���[�܂ł̋���������̋����𒴂��Ă�����v���C���[������
	if (isOverLimitPlayerHeight)
	{
		pPlayer_->Fall(special_player_fall_speed);
	}

	// �v���C���[���������S����̍����܂ŗ��������ǂ���
	if (pPlayer_->IsDeathJudgHeight())
	{
		// �����Ă�����Update���v���C���[�������S����Update�ɕύX
		updateFunc_ = &TestScene::DeathFallPlayerUpdate;
		return;
	}
#endif
}

// �n�ʂ̐��̕`��
void TestScene::GroundLineDraw()
{
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	VECTOR pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	VECTOR pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
	for (int i = 0; i <= lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.x += lineAreaSize / lineNum;
		pos2.x += lineAreaSize / lineNum;
	}

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	for (int i = 0; i < lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.z += lineAreaSize / lineNum;
		pos2.z += lineAreaSize / lineNum;
	}
}

// ���̖����X�N���[��
void TestScene::RoadInfiniteScroll()
{
	HITRESULT_LINE firstRoadResult{}, firstRoadResult2{};
	HITRESULT_LINE secondRoadResult{}, secondRoadResult2{};

	// 1�ڂ̓��A2�ڂ̓��̃C���X�^���X�̎擾
	auto firstRoad = pRoadManager_->GetRoads()[0];
	auto secondRoad = pRoadManager_->GetRoads()[1];

	// �摜�̍����̎擾
	float imgHeight = firstRoad->GetImageHeight();

	// 1�ڂ̓��A2�ڂ̓��̈ʒu���W�̎擾
	VECTOR firstRoadPos = firstRoad->GetPos();
	VECTOR secondRoadPos = secondRoad->GetPos();

	// �v���C���[���牺�ɐL�т���Ɠ��̓����蔻��
	CollisionRoadAndPlayer(firstRoad, firstRoadResult, firstRoadResult2);
	CollisionRoadAndPlayer(secondRoad, secondRoadResult, secondRoadResult2);

	// ���Ɠ����������ǂ���
	bool isHitFirstRoad = firstRoadResult.HitFlag || firstRoadResult2.HitFlag;
	bool isHitSecondRoad = secondRoadResult.HitFlag || secondRoadResult2.HitFlag;

	// 2�ڂ̓��̏�ɏ������1�ڂ̓���2�ڂ̓��̑O�ɍ��W�ύX
	// ���𖳌��X�N���[�������邽��
	if (nextRoad_ == 1 && isHitSecondRoad)
	{
		firstRoad->SetPos({ secondRoadPos.x, secondRoadPos.y, secondRoad->GetPos().z + imgHeight * 2 });
		nextRoad_ = 0;
	}
	else if (nextRoad_ == 0 && isHitFirstRoad)
	{
		secondRoad->SetPos({ firstRoadPos.x, firstRoadPos.y, firstRoad->GetPos().z + imgHeight * 2 });
		nextRoad_ = 1;
	}
}

// �v���C���[����L�΂������Ƃ��̐��ɓ����������܂ł̋���������̋����𒴂��Ă��邩
bool TestScene::OverLimitPlayerHeight()
{
	// �v���C���[���牺�ɐL�т���Ɠ��̓����蔻��
	HITRESULT_LINE result{}, result2{};
	CollisionAllRoadAndPlayer(result, result2);

	// 1�ł��|���S���ƃv���C���[���牄�΂��������������Ă�����`�F�b�N
	if (result.HitFlag || result2.HitFlag)
	{
		// �v���C���[���瓖���������܂ł̍����̋����̎擾
		float distanceFromPlayerToRoad = 0.0f;
		(result.HitFlag) ?
			(distanceFromPlayerToRoad = fabs((result.Position.y - pPlayer_->GetPos().y))) :
			(distanceFromPlayerToRoad = fabs((result2.Position.y - pPlayer_->GetPos().y)));

		// �����̋���������̋����𒴂��Ă�����true
		if (rise_possible_height < distanceFromPlayerToRoad)
		{
			return true;
		}
	}
	return false;
}

// �v���C���[�����̏�ɂ��邩����
bool TestScene::JudgePlayerOnTheRoad()
{
	// �v���C���[���牺�ɐL�т���Ɠ��̓����蔻��
	HITRESULT_LINE result{}, result2{};
	CollisionAllRoadAndPlayer(result, result2);

	// 1�ł��|���S���ƃv���C���[���牄�΂��������������Ă����瓹�̏�ɂ��锻��
	if (result.HitFlag || result2.HitFlag)
	{
		return true;
	}
	return false;
}

// �v���C���[���牺�ɐL�тĂ�����Ɠ��Ƃ̓����蔻��
void TestScene::CollisionRoadAndPlayer(std::shared_ptr<Road> pRoad, HITRESULT_LINE& result, HITRESULT_LINE& result2)
{
	// �v���C���[�̌��݈ʒu���牺�����ɐ���L�΂��Ēn�ʂ̃|���S���Ɠ������Ă��邩�m���߂�
	VECTOR leftTop = pRoad->GetVertex()[0].pos;
	VECTOR rightTop = pRoad->GetVertex()[1].pos;
	VECTOR leftBottom = pRoad->GetVertex()[2].pos;
	VECTOR rightBottom = pRoad->GetVertex()[3].pos;

	// ���C����L�΂��J�n�ʒu
	VECTOR startLinePos = pPlayer_->GetPos();

	// ���C���̏I���ʒu
	VECTOR endLinePos = VGet(pPlayer_->GetPos().x, pPlayer_->GetPos().y - 10000.0f, pPlayer_->GetPos().z);

	// ����2�̃|���S���ŕ`�悵�Ă���̂�2�Ƃ��`�F�b�N
	result = HitCheck_Line_Triangle(startLinePos, endLinePos, leftTop, rightTop, leftBottom);
	result2 = HitCheck_Line_Triangle(startLinePos, endLinePos, rightBottom, leftBottom, rightTop);
}

void TestScene::CollisionAllRoadAndPlayer(HITRESULT_LINE& result, HITRESULT_LINE& result2)
{
	// �S�Ă̓��̐��`�F�b�N
	for (auto& road : pRoadManager_->GetRoads())
	{
		CollisionRoadAndPlayer(road, result, result2);

		// 1�ł��|���S���ƃv���C���[���牄�΂��������������Ă����珈���I��
		// ����������������𑱂���ƌ��ʂ������ς���Ă��܂�����
		if (result.HitFlag || result2.HitFlag)
		{
			break;
		}
	}
}
