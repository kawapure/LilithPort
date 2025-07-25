#include "stdafx.h"
#include "OptionForm.h"
#include "MainForm.h"

using namespace LilithPort;

void OptionForm::SaveOption(bool apply){
	MainForm^ parent = safe_cast<MainForm^>(this->Owner);

	try{
		FileVersionInfo^ info = FileVersionInfo::GetVersionInfo(textBoxGameExe->Text);
		
		//TODO: FIX HACK
		if (info->LegalCopyright != "(C)2001 ENTERBRAIN,INC / OUTBACK" && info->FileDescription != "２Ｄ格闘ツクール2nd." && info->FileDescription != "２Ｄ格闘ツクール９５" && info->FileDescription != "Fighting is Magic: Aurora"){
			throw gcnew Exception;
		}
	}
	catch(Exception^){
		textBoxGameExe->Text = gcnew String("Invalid File. Choose a Fighter Maker executable.");
	}

	IntPtr mp;
	// Path
	// パス
	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxGameExe->Text);
	_tcscpy_s(MTOPTION.GAME_EXE, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxReplayFolder->Text);
	_tcscpy_s(MTOPTION.REPLAY_FOLDER, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxVSSound->Text);
	_tcscpy_s(MTOPTION.VS_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxNoticeSound->Text);
	_tcscpy_s(MTOPTION.NOTICE_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxEnterSound->Text);
	_tcscpy_s(MTOPTION.ENTER_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxNameSound->Text);
	_tcscpy_s(MTOPTION.NAME_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxTalkSound->Text);
	_tcscpy_s(MTOPTION.TALK_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxSeekSound->Text);
	_tcscpy_s(MTOPTION.SEEK_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxKeywordSoundPath->Text);
	_tcscpy_s(MTOPTION.KEYWORD_SOUND, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxKeyword->Text);
	_tcscpy_s(MTOPTION.KEYWORD, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);
	

	MTOPTION.ENTER_SOUND_ENABLE   = checkBoxEnterSound->Checked;
	MTOPTION.VS_SOUND_ENABLE      = checkBoxVSSound->Checked;
	MTOPTION.NOTICE_SOUND_ENABLE  = checkBoxNoticeSound->Checked;
	MTOPTION.NAME_SOUND_ENABLE    = checkBoxNameSound->Checked;
	MTOPTION.TALK_SOUND_ENABLE    = checkBoxTalkSound->Checked;
	MTOPTION.SEEK_SOUND_ENABLE    = checkBoxSeekSound->Checked;
	MTOPTION.KEYWORD_SOUND_ENABLE = checkBoxKeywordSound->Checked;

	MTOPTION.GET_IP_ENABLE        = checkBoxGetIP->Checked;
	MTOPTION.SHOW_GAME_OPTION     = checkBoxShowGameOption->Checked;
	MTOPTION.SHOW_RESULT          = checkBoxShowResult->Checked;
	MTOPTION.LOG_CLEAR_WITHOUT_WELCOME = checkBoxLogClearWithoutWelcome->Checked;

	// Comment
	// コメント
	if(textBoxComment->Text != gcnew String(MTOPTION.COMMENT)){
		mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(textBoxComment->Text);
		_tcscpy_s(MTOPTION.COMMENT, static_cast<PTCHAR>(mp.ToPointer()));
		Runtime::InteropServices::Marshal::FreeHGlobal(mp);

		parent->ChangeComment(textBoxComment->Text);
	}

	// Game
	// ゲーム
	MTOPTION.MAX_STAGE      = (UINT)numericUpDownMaxStage->Value;
	MTOPTION.STAGE_SELECT   = (UINT)numericUpDownStageSelect->Value;
	MTOPTION.ROUND          = (UINT)numericUpDownRound->Value;
	MTOPTION.TIMER          = (UINT)numericUpDownTimer->Value;
	MTOPTION.SIMULATE_DELAY = (UINT)numericUpDownSimDelay->Value;

	MTOPTION.TEAM_ROUND_HP     = checkBoxTeamRoundHP->Checked;
	MTOPTION.DISPLAY_NAME      = checkBoxName->Checked;
	MTOPTION.DISPLAY_VERSUS    = checkBoxVersus->Checked;
	MTOPTION.DISPLAY_FRAMERATE = checkBoxFramerate->Checked;
	MTOPTION.DISPLAY_RAND      = checkBoxRand->Checked;

	MTOPTION.REPLAY_DIVIDE      = checkBoxDivide->Checked;
	MTOPTION.CHANGE_WINDOW_SIZE = checkBoxWindowSize->Checked;

	MTOPTION.MAX_CONNECTION = (UINT)numericUpDownMaxConnection->Value;
	MTOPTION.BGM_VOLUME     = (UINT)trackBarBGM->Value * 5;
	MTOPTION.SE_VOLUME      = (UINT)trackBarSE->Value * 5;

	if(radioButtonReplayVersion2->Checked){
		MTOPTION.REPLAY_VERSION = 2;
	}
	else{
		MTOPTION.REPLAY_VERSION = 1;
	}

	// Hit judge
	// 当たり判定
	if(MTOPTION.HIT_JUDGE != checkBoxHitJudge->Checked){
		MTOPTION.HIT_JUDGE = checkBoxHitJudge->Checked;

		// Immediate change if the game is running
		// ゲーム起動中なら即時変更
		if(MTINFO.INITIALIZED && MTINFO.PROCESS != NULL){
			DWORD b = MTOPTION.HIT_JUDGE;

			if(MTINFO.KGT2K){
				WriteProcessMemory(MTINFO.PROCESS, (LPVOID)HIT_JUDGE, &b, 4, NULL);
			}
			else{
				WriteProcessMemory(MTINFO.PROCESS, (LPVOID)HIT_JUDGE_95, &b, 4, NULL);
			}
		}
	}

	// Advanced settings
	// Log file save format
	// 詳細設定
	// ログファイル保存形式
	if(radioButtonLogRTF->Checked){
		MTOPTION.LOG_FORMAT_RTF = true;
	}else{
		MTOPTION.LOG_FORMAT_RTF = false;
	}

	// Flash window on speech
	// 発言でウィンドウを点滅
	MTOPTION.TALK_FLASH = checkBoxTalkFlash->Checked;
	// Flash window when name is called
	// 名前が呼ばれたらウィンドウを点滅
	MTOPTION.NAME_FLASH = checkBoxNameFlash->Checked;

	// Colours
	//　色
	MTCOLOR.SERVER_NAME    = buttonServerName->ForeColor.ToArgb();
	MTCOLOR.HOST_NAME      = buttonHostName->ForeColor.ToArgb();
	MTCOLOR.CLIENT_NAME    = buttonClientName->ForeColor.ToArgb();
	MTCOLOR.REST_STATE     = buttonRestState->BackColor.ToArgb();
	MTCOLOR.VS_STATE       = buttonVSState->BackColor.ToArgb();
	MTCOLOR.WATCH_STATE    = buttonWatchState->BackColor.ToArgb();
	MTCOLOR.SEEK_STATE     = buttonSeekState->BackColor.ToArgb();

	MTCOLOR.SYSTEM_MESSAGE = buttonSystemMessage->ForeColor.ToArgb();
	MTCOLOR.ERROR_MESSAGE  = buttonErrorMessage->ForeColor.ToArgb();
	MTCOLOR.DEBUG_MESSAGE  = buttonDebugMessage->ForeColor.ToArgb();
	MTCOLOR.NOTICE_BACK    = buttonNoticeBack->BackColor.ToArgb();
	MTCOLOR.COMMENT_BACK   = buttonCommentBack->BackColor.ToArgb();
	MTCOLOR.SECRET         = buttonSecret->ForeColor.ToArgb();

	// Apply colours
	// 色を反映させる
	parent->ResetColor();

	// Check value
	// 値のチェック
	CheckMTOption();

	if(apply){
		textBoxGameExe->Text          = gcnew String(MTOPTION.GAME_EXE);
		textBoxReplayFolder->Text     = gcnew String(MTOPTION.REPLAY_FOLDER);
		textBoxVSSound->Text          = gcnew String(MTOPTION.VS_SOUND);
		textBoxNoticeSound->Text      = gcnew String(MTOPTION.NOTICE_SOUND);
		textBoxEnterSound->Text       = gcnew String(MTOPTION.ENTER_SOUND);
		textBoxNameSound->Text        = gcnew String(MTOPTION.NAME_SOUND);
		textBoxTalkSound->Text        = gcnew String(MTOPTION.TALK_SOUND);
		textBoxSeekSound->Text        = gcnew String(MTOPTION.SEEK_SOUND);
		textBoxKeywordSoundPath->Text = gcnew String(MTOPTION.KEYWORD_SOUND);
	}

	SaveMTOption();
}
void OptionForm::CloseOption(){
	MainForm^ parent = safe_cast<MainForm^>(this->Owner);
	parent->ChangeProfileEnabled();
}
bool OptionForm::CheckTextProfileName(String^ buf){
	// Validation of profile name
	// プロファイル名バリデーション
	String^ mes;
	if(buf == gcnew String(MTOPTION.PROFILE)){
		return true;
	}
	if(buf->Length == 0){
		mes = "Your Profile name is blank.";
	}
	if(buf->Contains(",") || buf->Contains("[") || buf->Contains("]")){
		mes = "The profile name contains invalid characters";
	}
	for(int i=0; i < Profile::SystemSection->Length; i++){
		if(buf == Profile::SystemSection[i]){
			mes = "The profile name is not available.";
		}
	}
	for(int i=0; i < Profile::ProfileList->Count; i++){
		if(buf == Profile::ProfileList[i]){
			mes = "That profile name already exists.";
		}
	}
	if(mes != nullptr){
		MessageBox::Show(mes, "Save Profile", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		return true;
	}
	return false;
}
void OptionForm::DeleteProfile(String^ buf){
	// Delete profile
	// プロファイル削除
	TCHAR bufProfile[MAX_ARRAY];
	IntPtr mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(buf);
	_tcscpy_s(bufProfile, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	DeleteSection(bufProfile);
	SaveProfileOption();
}
void OptionForm::OverWriteProfile(String^ buf){
	// Overwrite save
	// 上書き保存
	DeleteSection(MTOPTION.PROFILE);

	IntPtr mp = Runtime::InteropServices::Marshal::StringToHGlobalAuto(buf);
	_tcscpy_s(MTOPTION.PROFILE, static_cast<PTCHAR>(mp.ToPointer()));
	Runtime::InteropServices::Marshal::FreeHGlobal(mp);

	SaveMTOption();
}