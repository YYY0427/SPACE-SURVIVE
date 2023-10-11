#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Image3D.h"
#include "../Image3DManager.h"
#include "../common.h"

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// �I�u�W�F�N�g�̔z�u�f�[�^�̓ǂݍ���
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();
	auto playerData = pDataReader_->GetData().find("Player")->second.front();
	auto rockData = pDataReader_->GetData().find("Rock")->second;
	auto meteorData = pDataReader_->GetData().find("Meteor")->second;
	auto sunData = pDataReader_->GetData().find("Sun")->second;
	auto earthData = pDataReader_->GetData().find("Earth")->second;
	auto roadData = pDataReader_->GetData().find("Stage")->second;

	pImg3DManager_ = std::make_shared<Image3DManager>(roadData);
	pPlayer_ = std::make_shared<Player>(playerData);
	pRockManager_ = std::make_shared<RockManager>(rockData, meteorData, pPlayer_);
	pPlanetManager_ = std::make_shared<PlanetManager>(sunData, earthData);
	pCamera_ = std::make_shared<Camera>(pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>();

	// �R���X�g���N�^�œn���Ȃ��|�C���^�̐ݒ�
	pPlayer_->SetCameraPointer(pCamera_);
}

//  �f�X�g���N�^
TestScene::~TestScene()
{
	// �����Ȃ�
}

// �����o�֐��|�C���^�̍X�V
void TestScene::Update()
{
	(this->*updateFunc_)();
}

// �`��
void TestScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "TestScene", 0xffffff, true);

	// �e�N���X�̕`��
	pSkyDome_->Draw();
	pImg3DManager_->Draw();
	pRockManager_->Draw();
	pPlanetManager_->Draw();
	pPlayer_->Draw();

#ifdef _DEBUG
	// �n�ʂ̊p�̈ʒu�\��
	for (auto& road : pImg3DManager_->GetRoads())
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
	pCamera_->Update();
	pPlayer_->Update();
	pRockManager_->Update();
	pPlanetManager_->Update();

	// �v���C���[�����̏�ɂ��邩
	if (!JudgePlayerOnTheRoad())
	{
		// ���̏�ɂ��Ȃ������̂ŗ���
		pPlayer_->Fall();

		// �v���C���[���������S����̍����܂ŗ��������ǂ���
		if (pPlayer_->IsDeathJudgHeight())
		{
			// �����Ă�����Update���v���C���[�������S����Update�ɕύX
			updateFunc_ = &TestScene::DeathFallPlayerUpdate;
			return;
		}
	}

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
		// �S�ẴG�t�F�N�g�̍Đ����~
		Effekseer3DEffectManager::GetInstance().StopAllEffect();

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
	pCamera_->Update();

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
	// �t�F�[�h�A�E�g���I�������烊�X�|�[��
	if (IsStartFadeOutAfterFadingOut())
	{
		// �_���[�W����
		pPlayer_->OnDamage();

		// �v���C���[�������Ă����烊�X�|�[��
		if (pPlayer_->IsLive())
		{
			// �v���C���[����1�ԋ߂����̍��W�̎擾
			VECTOR restartPos = pImg3DManager_->GetClosestRoadPos(pPlayer_->GetPos());

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

// �v���C���[�����̏�ɂ��邩����
bool TestScene::JudgePlayerOnTheRoad()
{
	// ���̏�ɂ��Ȃ�������v���C���[���������鏈��
	bool isPlayerOnTheRoad = false;
	for (auto& road : pImg3DManager_->GetRoads())
	{
		// �v���C���[�̌��݈ʒu���牺�����ɐ���L�΂��Ēn�ʂ̃|���S���Ɠ������Ă��邩�m���߂�
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		// ���C����L�΂��J�n�ʒu
		VECTOR startLinePos = pPlayer_->GetPos();

		// ���C���̏I���ʒu
		VECTOR endLinePos = VGet(pPlayer_->GetPos().x, pPlayer_->GetPos().y - 10000.0f, pPlayer_->GetPos().z);

		// ����2�̃|���S���ŕ`�悵�Ă���̂�2�Ƃ��`�F�b�N
		HITRESULT_LINE result = HitCheck_Line_Triangle(startLinePos, endLinePos, leftTop, rightTop, leftBottom);
		HITRESULT_LINE result2 = HitCheck_Line_Triangle(startLinePos, endLinePos, rightBottom, leftBottom, rightTop);

		// 1�ł��|���S���ƃv���C���[���牄�΂��������������Ă�����v���C���[�͗������Ȃ�
		if (result.HitFlag || result2.HitFlag)
		{
			isPlayerOnTheRoad = true;
			break;
		}
	}
	return isPlayerOnTheRoad;
}