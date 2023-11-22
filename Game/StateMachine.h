#pragma once
#pragma once
#include <functional>
#include <map>

// �X�e�[�g�}�V��
template <class TState> class StateMachine
{
public:
	typedef std::function<void(void)> Delegate;

private:
	class StateDelegateSet
	{
	public:
		TState state;
		Delegate enter, update, exit;

		// �ϐ��������x�����o��̂ŁA�f�t�H���g�R���X�g���N�^�ŏ�����
		StateDelegateSet() : state(TState()) {}
	};

	TState currentState = {};
	std::map<TState, StateDelegateSet> stateFuncMap;
	bool isInitialized = false;

public:
	// �X�e�[�g�̒ǉ�
	void AddState(TState state, Delegate enter, Delegate update, Delegate exit)
	{
		// FIXME: �d���l��
		StateDelegateSet set{};
		set.state = state;
		set.enter = enter;
		set.update = update;
		set.exit = exit;
		stateFuncMap.emplace(state, set);
	}

	/// <summary>
	/// �X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state">�ݒ肵�����X�e�[�g</param>
	void SetState(TState state)
	{
		// �����enter�����Ă�
		if (isInitialized == false)
		{
			isInitialized = true;
			currentState = state;
			stateFuncMap[currentState].enter();
		}
		// ���O��state��exit���Ă�ŃX�e�[�g���X�V
		else if (currentState != state)
		{
			stateFuncMap[currentState].exit();
			currentState = state;
			stateFuncMap[currentState].enter();
		}
	}

	// �X�V
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	// �X�e�[�g�̎擾
	TState GetCurrentState() const { return currentState; }
};
