#include "Scene.h"
#include "../Game.h"
#include <cmath>

namespace
{
	constexpr int fade_normal_speed = 8;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager"></param>
Scene::Scene(SceneManager& manager) :
	manager_(manager)
{
	isFadeOut_ = false;
	fadeColor_ = GetColor(0, 0, 0);
	fadeBright_ = 255;
	fadeSpeed_ = -fade_normal_speed;
}

/// <summary>
/// �t�F�[�h�̍X�V
/// </summary>
void Scene::UpdateFade()
{
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

/// <summary>
/// �t�F�[�h�̕`��
/// </summary>
void Scene::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// �t�F�[�h�A�E�g�̊J�n
/// </summary>
void Scene::StartFadeOut()
{
	fadeSpeed_ = fade_normal_speed;
}

/// <summary>
/// �t�F�[�h�C�������ǂ���
/// </summary>
/// <returns>true : �t�F�[�h�C�����Afalse : �t�F�[�h�C�����Ă��Ȃ�</returns>
bool Scene::IsFadingIn() const
{
	if (fadeSpeed_ < 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �t�F�[�h�A�E�g�����ǂ���
/// </summary>
/// <returns>true : �t�F�[�h�A�E�g���Afalse : �t�F�[�h�A�E�g���Ă��Ȃ�</returns>
bool Scene::IsFadingOut() const
{
	if (fadeSpeed_ > 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �t�F�[�h�����ǂ���
/// </summary>
/// <returns>true : �t�F�[�h���Afalse : �t�F�[�h���ĂȂ�</returns>
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
void Scene::SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright)
{
	// �t�F�[�h�������ݒ�������Ȃ�Ȃ�
	if (!IsFadeing()) return;

	// �t�F�[�h�C�����t�F�[�h�A�E�g���Ńt�F�[�h���x�̒l��ύX����
	int absoluteFadeSpeed = abs(fadeSpeed);
	if (IsFadingIn())	absoluteFadeSpeed *= -1;

	// ���肦�Ȃ��l������Ȃ��悤�ɐ���
	if (fadeBright > 255)	fadeBright = 255;
	if (fadeBright < 0)		fadeBright = 0;
	if (fadeColor.x > 255)	fadeColor.x = 255;
	if (fadeColor.x < 0)	fadeColor.x = 0;
	if (fadeColor.y > 255)	fadeColor.y = 255;
	if (fadeColor.y < 0)	fadeColor.y = 0;
	if (fadeColor.z > 255)	fadeColor.z = 255;
	if (fadeColor.z < 0)	fadeColor.z = 0;

	// �l�̐ݒ�
	fadeSpeed_ = absoluteFadeSpeed;
	fadeColor_ = GetColor(static_cast<int>(fadeColor.x), static_cast<int>(fadeColor.y), static_cast<int>(fadeColor.z));
	fadeBright_ = fadeBright;
}

/// <summary>
/// �t�F�[�h�̖��邳���擾
/// </summary>
/// <returns>0(�t�F�[�h���Ă��Ȃ�)�`255(�^����)</returns>
int Scene::GetFadeBright() const
{
	return fadeBright_;
}
