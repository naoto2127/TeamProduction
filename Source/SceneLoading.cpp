#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGame.h"

//������
void SceneLoading::Initialize()
{
    //�X�v���C�g������
    sprite = new Sprite("Data/Sprite/loading.png");

    //�X���b�h�J�n�i��P�����ɃX���b�h�œ����������֐��A��Q�����Ɋ֐��̑�P�����j
    thread = new std::thread(LoadingThread,this);
}

//�I����
void SceneLoading::Finalize()
{
    //�X���b�h�I����
    if (thread != nullptr)
    {
        //�X���b�h���I������܂őҋ@
        thread->join();

        //�X���b�h��j��
        delete thread;

        //�X���b�h��null������
        thread = nullptr;
    }

    //�X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180;
    angle += speed * elapsedTime;

    //���̃V�[���̏���������������V�[����؂�ւ���
    if(nextScene->IsReady())
    {
        //�V�[���}�l�[�W���[�� nextScene �ɐ؂�ւ���
        SceneManager::Instance().ChangeScene(nextScene);

        //nextScene �� null �������
        nextScene = nullptr;
    }
}

//�`�揈��
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2D�X�v���C�g�`��
    {
        //��ʉE���Ƀ��[�f�B���O�A�C�R����`��
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        float positionX = 0;
        float positionY = 0;
        //�^�C�g���X�v���C�g�`��
        sprite->Render(dc,
            positionX, positionY, 1920, 720,
            0, 0, 1920, 720,
            0,
            1, 1, 1, 1);
    }
}

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
};