//-----------------------------------------------------------------------------
//プレイヤ
//-----------------------------------------------------------------------------
#include "Task_Player.h"


//-----------------------------------------------------------------------------
//タスク内共有リソース
//-----------------------------------------------------------------------------
int				Player::refCnt;//消すな
DG::cMesh::SP	Player::mesh;
void Player::ResourceLoad( )
{
	if(refCnt == 0){
		//共有リソースはここで読み込む
		//--------------------------------------------------
		mesh = DG::cMesh::Create(ge->sobPath);
		//--------------------------------------------------
	}
	++refCnt;
}
void Player::ResourceRelease( )
{
	--refCnt;
	if(refCnt == 0){
		//共有リソースはここで解放する
		//--------------------------------------------------
		mesh.reset( );
		//--------------------------------------------------
	}
}
//-----------------------------------------------------------------------------
//初期化処理
//機能概要：タスク生成時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
Player::Player(ML::Vec3& p_):BChara(_T("プレイヤ") )
{
	ResourceLoad( );//消すな
	//drawPriority = 100.0f;	2D描画なし意味なし
	pos = p_;
	dirY = 0;
	dirX = 0;
	walkSpeed = 5.0f;

	UpDate_DrawInfo( );
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：タスク終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
Player::~Player( )
{
	ResourceRelease( );//消すな
}

//-----------------------------------------------------------------------------
//実行処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void Player::Step( )
{
	//方向キー
	if(ge->in1->Check( DI::X_M, DI::On)){	dirY -= (3.0f * D3DX_PI) / 180.0f;}
	if(ge->in1->Check( DI::X_P, DI::On)){	dirY += (3.0f * D3DX_PI) / 180.0f;}
	if(ge->in1->Check( DI::Y_M, DI::On)){	dirX -= (3.0f * D3DX_PI) / 180.0f;}
	if(ge->in1->Check( DI::Y_P, DI::On)){	dirX += (3.0f * D3DX_PI) / 180.0f;}
	UpDate_DrawInfo( );
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：描画処理を行う
//-----------------------------------------------------------------------------
void Player::Draw3D( )
{
	mesh->Draw(matW);
}
