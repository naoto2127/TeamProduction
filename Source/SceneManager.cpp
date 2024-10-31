#include"SceneManager.h"

//�X�V����
void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//�Â��V�[�����I������
		Clear();
		//currentScene->Finalize();���ꂾ�����ƌ��X�͂��Ă���A�h���X���J������Ă��Ȃ�
		//�V�����V�[����ݒ�
		currentScene = nextScene;
		nextScene = nullptr;

		//�V�[������������
		//���������I����ĂȂ�������
		if (!currentScene->IsReady())
		{
			currentScene->Initialize();
		}
		
		
	}

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//�`�揈��
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene -> Render();
	}
}

//�V�[���N���A
void SceneManager::Clear()
{

	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
	//�V�����V�[����ݒ�
	nextScene = scene;
}