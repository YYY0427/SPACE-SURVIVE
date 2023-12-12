#include "TestScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../UI/Warning.h"
#include "../UI/HpBar.h"
#include "../Util/DrawFunctions.h"
#include "../Easing.h"
#include <DxLib.h>

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager),
	imgPos_({0, common::screen_height / 2})
{
	imgHandle_ = my::MyLoadGraph("Data/Image/Test.png");

	pWarning_ = std::make_shared<Warning>(600);
	pCamera_ = std::make_shared<Camera>();

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffect(laserEffectHandle_, EffectID::test, {0, 0, 0}, {30, 30, 30}, 1.0, {0.0, DX_PI_F, 0.0});

//	pHpBar_ = std::make_shared<HpBar>(50);
}

// �f�X�g���N�^
TestScene::~TestScene()
{
	// �����Ȃ�
}

// �X�V
void TestScene::Update()
{
	// �e�N���X�̍X�V
//	pHpBar_->Update(30);
//	pWarning_->Update();
	static int count = 0;
	count++;

	// �C�[�W���O�֐����g���ĉ摜�̈ʒu��ύX
	imgPos_.x = Easing::EaseOutInExpo(count, 0, static_cast<float>(common::screen_width) / 2, common::screen_width, 180);
//	imgPos_.x = Easing::EaseOutBack(count, 0, static_cast<float>(common::screen_width) / 2, 180);
	Debug::Log("�摜�̈ʒu", imgPos_.x);
	pCamera_->Update();

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (InputState::IsTriggered(InputType::UP))
	{
		effectManager.SetDynamicEffectParam(laserEffectHandle_, 0, 1.0f);
	}
	float param = effectManager.GetDynamicEffectParam(laserEffectHandle_, 0);
	Debug::Log("���[�U�[�̂Ȃ񂩂̒l", param);

	{
		// �E�X�e�B�b�N�̓��͏��̎擾
		int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
		int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
		int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
		int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

		Debug::Log("up", up);
		Debug::Log("down", down);
		Debug::Log("right", right);
		Debug::Log("left", left);
	}

	Vector2 p1{ 10, 10 }, p2{ 100, 10 }, p3{ 100, 100};
	DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, GetColor(255, 0, 0), false);
	Vector2 add = p1 + p2 + p3;
	add /= 3;
	DrawCircle(70.0f, 40.0f, GetColor(0, 255, 0), true);
//	DrawCircle(add.x, add.y, GetColor(0, 255, 0), true);
	
	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
	}

	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		// �G�t�F�N�g��S�Ď~�߂�
		Effekseer3DEffectManager::GetInstance().StopAllEffect();

		// PushScene����̂ŃV�[�����c�邽�߃t�F�[�h�C���̐ݒ�
		StartFadeIn();

		// �V�[���J��
		manager_.PushScene(new PauseScene(manager_));
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void TestScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("TestScene");

	DrawRotaGraph(imgPos_.x, imgPos_.y, 0.1f, 0.0f, imgHandle_, true);

	// �e�N���X�̕`��
//	pWarning_->Draw();
//	pHpBar_->Draw(50, 20, 50);

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}