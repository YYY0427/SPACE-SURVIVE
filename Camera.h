#pragma once

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera();

	// �f�X�g���N�^
	virtual ~Camera();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
};