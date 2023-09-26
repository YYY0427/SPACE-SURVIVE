#include "Scene.h"
#include "../common.h"
#include <cmath>

namespace
{
	// �ʏ�̃t�F�[�h�̑��x
	constexpr int fade_normal_speed = 8;

	// ���U�C�N�p�����[�^�[�̍ő�l
	constexpr int gauss_max_value = 1400;
}

// �R���X�g���N�^
Scene::Scene(SceneManager& manager) :
	manager_(manager),
	isFadeOut_(false),
	fadeColor_(GetColor(0, 0, 0)),
	fadeBright_(255),
	fadeSpeed_(-fade_normal_speed)
{
	handle_ = MakeScreen(common::screen_width, common::screen_height);
}

// �f�X�g���N�^
Scene::~Scene()
{
	// �����Ȃ�
}

// �t�F�[�h�̍X�V
void Scene::UpdateFade()
{
	// �t�F�[�h�̖��邳�̍X�V
	fadeBright_ += fadeSpeed_;

	// �t�F�[�h�A�E�g�I������
	if (fadeBright_ >= 255)
	{
		fadeBright_ = 255;
		if (fadeSpeed_ > 0)
		{
			fadeSpeed_ = 0;
		}
	}
	// �t�F�[�h�C���I������
	if (fadeBright_ <= 0)
	{
		fadeBright_ = 0;
		if (fadeSpeed_ < 0)
		{
			fadeSpeed_ = 0;
		}
	}
}

// �t�F�[�h�̕`��
void Scene::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, common::screen_width, common::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ���U�C�N�t�F�[�h�̕`��
void Scene::DrawGaussFade()
{
	// 0~255�͈̔͂�0~���U�C�N�p�����[�^�[�̍ő�l�ɕϊ�
	int gaussParameter = fadeBright_ * gauss_max_value / 255;

	// ���U�C�N�摜�̍쐬
	GetDrawScreenGraph(0, 0, common::screen_width, common::screen_height, handle_);
	GraphFilter(handle_, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, handle_, true);
}

// �t�F�[�h�A�E�g�̊J�n
void Scene::StartFadeOut()
{
	fadeSpeed_ = fade_normal_speed;
}

// �t�F�[�h�C�������ǂ���
bool Scene::IsFadingIn() const
{
	if (fadeSpeed_ < 0)
	{
		return true;
	}
	return false;
}

// �t�F�[�h�A�E�g�����ǂ���
bool Scene::IsFadingOut() const
{
	if (fadeSpeed_ > 0)
	{
		return true;
	}
	return false;
}

// �t�F�[�h�����ǂ���
bool Scene::IsFadeing() const
{
	return IsFadingIn() || IsFadingOut();
}

/// <summary>
/// �t�F�[�h�ɂ��Ă̐ݒ�(�t�F�[�h���ł����ݒ�������Ȃ�Ȃ�)
/// </summary>
/// <param name="fadeSpeed">�t�F�[�h�̑��x</param>
/// <param name="fadeColor">�t�F�[�h�̐F(0~255)</param>
/// <param name="fadeBright">�t�F�[�h�̖��邳(0~255)</param>
//void Scene::SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright)
//{
//	// �t�F�[�h�������ݒ�������Ȃ�Ȃ�
//	if (!IsFadeing()) return;
//
//	// �t�F�[�h�C�����t�F�[�h�A�E�g���Ńt�F�[�h���x�̒l��ύX����
//	int absoluteFadeSpeed = abs(fadeSpeed);
//	if (IsFadingIn())	absoluteFadeSpeed *= -1;
//
//	// ���肦�Ȃ��l������Ȃ��悤�ɐ���
//	if (fadeBright > 255)	fadeBright = 255;
//	if (fadeBright < 0)		fadeBright = 0;
//	if (fadeColor.x > 255)	fadeColor.x = 255;
//	if (fadeColor.x < 0)	fadeColor.x = 0;
//	if (fadeColor.y > 255)	fadeColor.y = 255;
//	if (fadeColor.y < 0)	fadeColor.y = 0;
//	if (fadeColor.z > 255)	fadeColor.z = 255;
//	if (fadeColor.z < 0)	fadeColor.z = 0;
//
//	// �l�̐ݒ�
//	fadeSpeed_ = absoluteFadeSpeed;
//	fadeColor_ = GetColor(static_cast<int>(fadeColor.x), static_cast<int>(fadeColor.y), static_cast<int>(fadeColor.z));
//	fadeBright_ = fadeBright;
//}
