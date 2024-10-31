#include "Graphics/Graphics.h"
#include "GameOver.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Input/Input.h"

extern int count;

//������
void GameOver::Initialize()
{
    //�X�v���C�g������
    moji = new Sprite("Data/fonts/font4.png");
    sprite = new Sprite("Data/Sprite/result.png");

    ResultMusic = Audio::Instance().LoadAudioSource("Data/music/result.wav");

    ResultMusic->Play(true);
}

//�I����
void GameOver::Finalize()
{
    //�X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
    if (moji != nullptr)
    {
        delete moji;
        moji = nullptr;
    }
    ResultMusic->Stop();
}

//�X�V����
void GameOver::Update(float eapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //�����{�^������������Q�[���V�[���֐؂�ւ���
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;

    Mouse& mouse = Input::Instance().GetMouse();
    if (gamePad.GetButtonDown() & anyButton|| mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
    }

}

//�`�揈��
void GameOver::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        //�^�C�g���X�v���C�g�`��
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        moji->textout(dc, "score:",
            200, 300,
            100, 100,
            1, 1, 1, 1);
        moji->textout(dc, std::to_string(count),
            900, 300,
            100, 100,
            1, 1, 1, 1);
    }
}