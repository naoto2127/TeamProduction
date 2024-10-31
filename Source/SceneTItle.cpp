#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "Scenegame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include <string>

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    game = new Sprite("Data/Sprite/play.png");
    tutorial = new Sprite("Data/Sprite/rule.png");
  
    explain[0] = new Sprite("Data/Sprite/explain.png");
    explain[1] = new Sprite("Data/Sprite/explain2.png");
    explain[2] = new Sprite("Data/Sprite/explain3.png");
    explain[3] = new Sprite("Data/Sprite/explain4.png");
    next = new Sprite("Data/Sprite/next.png");

    TitleMusic = Audio::Instance().LoadAudioSource("Data/music/title.wav");

    TitleMusic->Play(true);

 }

//終了化
void SceneTitle::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
    if (game != nullptr)
    {
        delete game;
        game = nullptr;
    }
    if (tutorial != nullptr)
    {
        delete tutorial;
        tutorial = nullptr;
    }
    for (int i = 0; i < 4; i++)
    {
        if (explain[i] != nullptr)
        {
            delete explain[i];
            explain[i] = nullptr;
        }
    }
   
    if (next != nullptr)
    {
        delete next;
        next = nullptr;
    }
   
    TitleMusic->Stop();
}

//更新処理
void SceneTitle::Update(float eapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //何かボタンを押したらゲームシーンへ切り替える
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;

    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetPositionX() > 400 && mouse.GetPositionX() < 550 && mouse.GetPositionY() > 500 && mouse.GetPositionY() < 600&&gamefalg==false)
    {
        gameScale = 1.1f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {

            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame()));
        }
    }
    else if (mouse.GetPositionX() > 750 && mouse.GetPositionX() < 900 && mouse.GetPositionY() > 500 && mouse.GetPositionY() < 600&& tutorialfalg==false)
    {
        tutorialScale = 1.1f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
           /* explain_x = 1280;
            explain_y = 720;
            batuScale = 1.0f;   */      
            gamefalg = true;
            tutorialfalg = true;
        }
    }
    else if (mouse.GetPositionX() > 750 && mouse.GetPositionX() < 900 && mouse.GetPositionY() > 500 && mouse.GetPositionY() < 600&&nextfalg==false&&tutorialfalg==true)
    {
        nextScale = 1.1f;
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            
            SceneNum++;
            nextScale = 1.0f;
            if (SceneNum == 3)
            {
                nextfalg = true;
                gamefalg = false;
            }
        }
    }
    else if (mouse.GetPositionX() > 1050 && mouse.GetPositionX() < 1300 && mouse.GetPositionY() > 30 && mouse.GetPositionY() < 180)
    {
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            explain_x = 0.0f;
            explain_y = 0.0f;
            batuScale = 0.0f;
        }
    }
    
    else
    {
        gameScale = 1.0f;
        tutorialScale = 1.0f;
    }

   
}

//描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        if (tutorialfalg == false)
        {
            sprite->Render(dc,
                0, 0, screenWidth, screenHeight,
                0, 0, textureWidth, textureHeight,
                0,
                1, 1, 1, 1);
        }
        
        if (tutorialfalg == true)
        {
            explain[SceneNum]->Render(dc,
                0, 0, screenWidth, screenHeight,
                0, 0, 1920, 1080,
                0,
                1, 1, 1, 1);
        }
      
        if (nextfalg == false&&gamefalg==true)
        {
            next->Render(dc,
                750, 500, 150 * nextScale, 100 * nextScale,
                0, 0, 480, 180,
                0,
                1, 1, 1, 1);
        }
        if (gamefalg == false)
        {
            game->Render(dc,
                400, 500, 150 * gameScale, 100 * gameScale,
                0, 0, 480, 180,
                0,
                1, 1, 1, 1);
        }

        if (tutorialfalg == false)
        {
            tutorial->Render(dc,
                750, 500, 150 * tutorialScale, 100 * tutorialScale,
                0, 0, 480, 180,
                0,
                1, 1, 1, 1);
        }
        

    }
}