// LilithPort.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "MainForm.h"
#include <math.h>

using namespace LilithPort;

// グローバル
// Globals
MT_SP_INFORMATION  MTINFO;
MT_SP_OPTION       MTOPTION;
MT_SP_WINDOW_STATE MTWS;
MT_SP_COLOR        MTCOLOR;

CRITICAL_SECTION CS_CAPTION;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	// Before making any controls, enable Windows XP visual effects
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true);

	// 自プログラムのパス
	// Path to own process
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR];

	GetModuleFileName(NULL, MTOPTION.PATH, _MAX_PATH);
	_tsplitpath_s(MTOPTION.PATH, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
	_stprintf_s(MTOPTION.PATH, _T("%s%s"), drive, dir);

	// コマンドライン引数
	// Command line arguments
	if(args->Length > 0){
		if(args[0] == "-s"){
			MTINFO.SERVER_MODE = true;
			if(args->Length > 1){
				try{
					MTINFO.SERVER_MODE_PORT = (UINT)Convert::ToInt32(args[1]);
				}
				catch(Exception^){
					MTINFO.SERVER_MODE_PORT = 0;
				}
			}else{
				MTINFO.SERVER_MODE_PORT = 0;
			}
		}
		
	}

#ifdef _DEBUG
	MTINFO.DEBUG = false;
#endif

	LoadMTOption();
	CheckMTOption();

	// 乱数の初期化＆まぜまぜ
	// Random number generator initialisation and mixing
	XorShift((UINT)DateTime::Now.Ticks);
	XorShift();
	XorShift();
	XorShift();

	// 音量(dB)をあらかじめ計算しておく
	// Calculate the volume (dB) in advance
	MTINFO.VOLUME[0] = -10000;

	for(int i = 1; i < 21; i++){
		// -6dB is half the volume, which is -600 in DirectSound terms
		// 19.931568f = -6.0f / log10(0.5f); 
		//
		// -6dBで音量半減、DirectSound的には-600
		// 19.931568f = -6.0f / log10(0.5f); 
		MTINFO.VOLUME[i] = (INT32)(19.931568f*log10((float)(i*5)/100.0f)*100.0f);
	}

	InitializeCriticalSection(&CS_CAPTION);

	// 未処理の例外をキャッチ
	// Catch nhandled exceptions
	Application::ThreadException += gcnew ThreadExceptionEventHandler(ApplicationThreadException);
	Threading::Thread::GetDomain()->UnhandledException += gcnew UnhandledExceptionEventHandler(ApplicationUnhandledException);

	// メイン ウィンドウを作成して、実行します
	// Create and show main window
	Application::Run(gcnew MainForm());

	DeleteCriticalSection(&CS_CAPTION);

	SaveMTOption();

	return 0;
}
