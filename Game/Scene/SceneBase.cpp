#include "SceneBase.h"
#include "../common.h"
#include <cmath>
#include "../Util/Range.h"

namespace
{
	// �ʏ�̃t�F�[�h�̑��x
	constexpr int fade_normal_speed = 8;

	// ���U�C�N�p�����[�^�[�̍ő�l
	constexpr int gauss_max_value = 1400;
}

// �R���X�g���N�^
SceneBase::SceneBase(SceneManager& manager) :
	manager_(manager),
	isFadeOut_(false),
	fadeColor_(GetColor(0, 0, 0)),
	fadeBright_(255),
	fadeSpeed_(-fade_normal_speed),
	fadeBrightUpperLimitValue_(255)
{
	// ���U�C�N�����p�̃O���t�B�b�N�̍쐬
	gaussScreen_ = MakeScreen(common::screen_width, common::screen_height);
}

// �f�X�g���N�^
SceneBase::~SceneBase()
{
	DeleteGraph(gaussScreen_);
}

void SceneBase::LoadAsync()
{
	SetUseASyncLoadFlag(TRUE);

	LoadAsync2();

	SetUseASyncLoadFlag(FALSE);
}

void SceneBase::LoadAsync2()
{
	// �����Ȃ�
}

void SceneBase::UpdateLoadAsync()
{
	// 1�ł��ǂݍ��ݒ��̃f�[�^����������
	if (GetASyncLoadNum() > 0)
	{
		return;
	}
}

void SceneBase::DrawLoadingScreen()
{
	// 1�ł��ǂݍ��ݒ��̃f�[�^����������
	if (GetASyncLoadNum() > 0)
	{
		int num = GetASyncLoadNum();
		DrawFormatString(100, 200, 0xffffff, "�Ȃ���[�ł���", num);
		return;
	}
}

// �t�F�[�h�̍X�V
void SceneBase::UpdateFade()
{
	// �t�F�[�h�̖��邳�̍X�V
	fadeBright_ += fadeSpeed_;

	// �t�F�[�h�̖��邳�̉����l�Ə���l�̐ݒ�
	const Range<int> fadeBrightRange(0, fadeBrightUpperLimitValue_);
	
	// �t�F�[�h�̖��邳���ݒ肵���͈͂𒴂�����t�F�[�h���~�߂�
	if (!fadeBrightRange.IsInside(fadeBright_))	fadeSpeed_ = 0;

	// �t�F�[�h�̖��邳��ݒ肵���͈͓��ɃN�����v����
	fadeBright_ = fadeBrightRange.Clamp(fadeBright_);
}

// �t�F�[�h�̕`��
void SceneBase::DrawFade(bool isPlay)
{
	if (!isPlay) return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, common::screen_width, common::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ���U�C�N�t�F�[�h�̕`��
void SceneBase::DrawGaussFade(bool isPlay)
{
	if (!isPlay) return;

	// 0~255�͈̔͂�0~���U�C�N�p�����[�^�[�̍ő�l�ɕϊ�
	int gaussParameter = fadeBright_ * gauss_max_value / 255;

	// ���U�C�N�摜�̍쐬
	GetDrawScreenGraph(0, 0, common::screen_width, common::screen_height, gaussScreen_);
	GraphFilter(gaussScreen_, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, gaussScreen_, true);
}

// �t�F�[�h�A�E�g�̊J�n
void SceneBase::StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// �t�F�[�h�A�E�g���ǂ̂��炢�܂ōs���̂��l��ݒ�
	// 0(�t�F�[�h���Ȃ�)�`255(�Ō�܂Ńt�F�[�h���s��)
	fadeBrightUpperLimitValue_ = fadeBrightUpperLimitValue;

	// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
	isFadeOut_ = true;

	// �t�F�[�h���x�̐ݒ�
	fadeSpeed_ = abs(fadeSpeed);;
}

// �t�F�[�h�C���̊J�n
void SceneBase::StartFadeIn(int fadeSpeed)
{
	// ������
	isFadeOut_ = false;

	// �t�F�[�h���x�̐ݒ�
	fadeSpeed_ = -abs(fadeSpeed);
}

// �t�F�[�h�C�������ǂ���
bool SceneBase::IsFadingIn() const
{
	return (fadeSpeed_ < 0);
}

// �t�F�[�h�A�E�g�����ǂ���
bool SceneBase::IsFadingOut() const
{
	return (fadeSpeed_ > 0);
}

// �t�F�[�h�����ǂ���
bool SceneBase::IsFadeing() const
{
	return IsFadingIn() || IsFadingOut();
}

// �t�F�[�h�A�E�g�X�^�[�g��Ƀt�F�[�h�A�E�g���ł͂Ȃ��� 
bool SceneBase::IsStartFadeOutAfterFadingOut()
{
	return !IsFadingOut() && isFadeOut_;
}

// �t�F�[�h�̖��邳�̎擾
int SceneBase::GetFadeBright() const
{
	return fadeBright_;
}

// �t�F�[�h�̖��邳�̐ݒ�
void SceneBase::SetFadeBright(int fadeBright)
{
	fadeBright_ = fadeBright;
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
