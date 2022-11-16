#include "StdAfx.h"
#include "uistartform.h"
#include "uiform.h"
#include "uitextbutton.h"
#include "uiformmgr.h"
#include "uiprogressbar.h"
#include "uilabel.h"
#include "netchat.h"
#include "packetcmd.h"
#include "gameapp.h"
#include "uigrid.h"
#include "character.h"
#include "uiheadsay.h"
#include "uimenu.h"
#include "uilist.h"
#include "uigrid.h"
#include "UIGlobalVar.h"
#include "UIMisLogForm.h"
#include "UICozeForm.h"
#include "NetGuild.h"
#include "uiguildmgr.h"
#include "worldscene.h"
#include "uititle.h"
#include "uiboxform.h"
#include "shipfactory.h"
#include "uiboatform.h"
#include "arearecord.h"
#include "isskilluse.h"
#include "ui3dcompent.h"
#include "smallmap.h"
#include "mapset.h"
#include "uiequipform.h"
#include "uiTradeForm.h"
#include "uiFindTeamForm.h"
#include "uistoreform.h"
#include "uidoublepwdform.h"
#include "uiitemcommand.h"
#include "uiminimapform.h"
#include "uibankform.h"
#include "uiboothform.h"
#include "uitradeform.h"
#include "UIChat.h"
#include "UITeam.h"
#include "AutoAttack.h"

#include "NPCHelper.h"

// Add by lark.li 20080811 begin
#include "UITeam.h"
// End

using namespace GUI;


static CForm*		frmSelectOriginRelive	= NULL;

//---------------------------------------------------------------------------
// class CStartMgr
//---------------------------------------------------------------------------
CMenu*	      CStartMgr::mainMouseRight = NULL;
CTextButton*  CStartMgr::btnQQ			= NULL;
CCharacter2D* CStartMgr::pMainCha		= NULL;
CCharacter2D* CStartMgr::pTarget		= NULL;
CCharacter* CStartMgr::pLastTarget = NULL;		
CCharacter* CStartMgr::pChaPointer = NULL;

static char szBuf[32] = { 0 };
bool sortcol( const vector<int>& v1, 
               const vector<int>& v2 ) { 
    return v1[1] < v2[1]; 
} 


void CStartMgr::SetDropsInfo (CCharacter* pTargetCha) {
		CChaRecord		*charInfo = GetChaRecordInfo(pTargetCha->getMobID());
		CItemRecord 	*tInfo;
		CItemRow		*pRow(NULL);
		CItemCommand	*ComItem(NULL);
		CItem			*content;
		float			GLOBAL_RATE = 2.0;
		std::vector< std::vector<int> > vect(defCHA_INIT_ITEM_NUM, vector<int> (2, 0));
		listMobDrops->GetList()->Clear();
		int max = 15;
		char buf[64];
		listMobDrops->GetList()->GetScroll()->Reset();
		for(int i = 0; i < defCHA_INIT_ITEM_NUM; i++){
					
			vect[i][0] = ((int)charInfo->lItem[i][0]);
			vect[i][1] = (int)charInfo->lItem[i][1];
			if(GetItemRecordInfo(vect[i][0]) == NULL) {
						max = i;
						break;
			}
		}
		
		sort(vect.begin(), vect.end() - (15-max) ,sortcol); 

		for(int i = 0; i < max; i++){
			tInfo = GetItemRecordInfo(vect[i][0]);
			sprintf(buf, charInfo->szName);
			labMobDropsItemName->SetCaption(buf);
			listMobDrops->_IsShowHint = true;
			listMobDrops->GetList()->SetIsChangeColor(false);
			pRow = listMobDrops->GetList()->GetItems()->NewItem();

			listMobDrops->pItem[i] = new CItemCommand(tInfo);
			content = new CItem;
			pRow->SetIndex(0, listMobDrops->pItem[i]);
			pRow->SetIndex(1, content);
			listMobDrops->pItem[i]->SetParent(listMobDrops);
			
			float per =  GLOBAL_RATE * (10000/float(vect[i][1]));
			if (per > 100) per = 100;
			if(per <= 100 && per >= 5 ){
				sprintf(buf, "%s \n%0.2f%%", tInfo->szName, per);
				listMobDrops->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetColor(0xff000000);			// Black
			}
			else if(per <= 5 && per >= 1 ){
				sprintf(buf, "%s \n%0.2f%%", tInfo->szName, per);
				listMobDrops->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetColor(0xff0652DD);			//Blue
			}
			else if(per <= 1 && per >= 0.5  ){
				sprintf(buf, "%s \n%0.2f%%", tInfo->szName, per);
				listMobDrops->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetColor(0xffEB2F06);			// Red
			}
			else if(per <= 0.5 ){
				sprintf(buf, "%s \n???", tInfo->szName, per);
				listMobDrops->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetColor(0xffF79F1F);			// Yellow
			}
			listMobDrops->GetList()->GetItems()->GetItem(i)->GetIndex(1)->SetString(buf);			
					}
		frmMobDrops->Refresh();
		frmMobDrops->Show();
	
}

void CStartMgr::SetMobInfo (CCharacter* pTargetCha){
	CChaRecord		*charInfo = GetChaRecordInfo(pTargetCha->getMobID());
	float GLOBAL_RATE = 2.0;
	char buf[64];
	sprintf(buf, "%d", charInfo->lMxHp);
	LabMObHP->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lMxSp);
	LabMObSP->SetCaption(buf);
	sprintf(buf, "%.0f",charInfo->lCExp * GLOBAL_RATE);
	LabMObexp->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lStr);
	LabMObStr->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lCon);
	LabMObCon->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lDex);
	LabMObAcc->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lSta);
	LabMObSpr->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lAgi);
	LabMObAgi->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lMSpd);
	LabMObMSpeed->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lFlee);
	LabMObDodge->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lHit);
	LabMObHitRate->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lDef);
	LabMObDef->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lPDef);
	LabMObPR->SetCaption(buf);
	sprintf(buf, "%d", charInfo->lASpd);
	LabMObAtSpeed->SetCaption(buf);
	sprintf(buf, "%d/%d", charInfo->lMnAtk, charInfo->lMxAtk );
	LabMObAttack->SetCaption(buf);

	frmMobInfo->Refresh();
	frmMobInfo->Show();
}

void CStartMgr::UpdateBackDrop(){
	CCharacter* pMain = CGameScene::GetMainCha();
	int nArea = CGameApp::GetCurScene()->GetTerrain()->GetTile( pMain->GetCurX()/100, pMain->GetCurY()/100 )->getIsland();
	CWorldScene* world = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	
	if(nArea ==  world->GetOldMainChaInArea()){
		return;
	}
	
	world->SetOldMainChaInArea(nArea);
	
	char buf[64];
	
	if( nArea){
		sprintf(buf,"texture/ui/corsairs/npcBackdrop/%d.tga",nArea);
	}else{
		sprintf(buf,"texture/ui/corsairs/npcBackdrop/sea.tga"); 
	}
	CCompent* imgBackDropPlayer = dynamic_cast<CCompent*>(g_stUIStart.frmDetail->Find("imgBackDropPlayer"));
	//CCompent* imgBackDropTarget = dynamic_cast<CCompent*>(g_stUIStart.frmTargetInfo->Find("imgBackDropTarget"));
	CCompent* teamBackDrops[4];
	
	for(int i = 0;i<4;i++){
		char formName[32];
		char imgName[32];
		sprintf(formName,"frmTeamMenber%d",i+1);
		sprintf(imgName,"imgBackDropTeam%d",i+1);
		teamBackDrops[i] = dynamic_cast<CCompent*>(g_stUIStart._FindForm(formName)->Find(imgName));
	}
	
		 //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c/25450408#25450408 # 3
	if(GetFileAttributes(buf) == INVALID_FILE_ATTRIBUTES){
		imgBackDropPlayer->GetImage()->LoadImage("texture/ui/corsairs/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		//imgBackDropTarget->GetImage()->LoadImage("texture/ui/corsairs/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		for(int i = 0;i<4;i++){
			teamBackDrops[i]->GetImage()->LoadImage("texture/ui/corsairs/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		}
	}else{
		imgBackDropPlayer->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		//imgBackDropTarget->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		for(int i = 0;i<4;i++){
			teamBackDrops[i]->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		}
	}
}

bool CStartMgr::Init()
{
	_IsNewer = false;
	_IsCanTeam = true;
		{
			frmMobDrops = _FindForm("frmMobDrops");
			if(frmMobDrops) {
			frmMobDrops->Refresh();
			listMobDrops = dynamic_cast<CListView*> (g_stUIStart.frmMobDrops->Find("listMobDrops"));
			labMobDropsItemName = dynamic_cast<CLabel*>(g_stUIStart.frmMobDrops->Find("labMobDropsItemName"));
			
			frmMobDrops->evtClose = _evtOnClose;
			
			}

	}
	{
			frmMobInfo = _FindForm("frmMobInfo");
			if(frmMobInfo) {
			frmMobInfo->Refresh();
			LabMObHP = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObHP"));
			LabMObSP = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObSP"));
			LabMObexp = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObexp"));
			LabMObStr = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObStr"));
			LabMObCon = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObCon"));
			LabMObAcc = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObAcc"));
			LabMObSpr = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObSPR"));
			LabMObAgi = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObAgi"));
			LabMObMSpeed = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObMSpeed"));
			LabMObDodge = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObDodge"));
			LabMObHitRate = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObHitRate"));
			LabMObDef = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObDef"));
			LabMObPR = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObPR"));
			LabMObAtSpeed = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObAtSpeed"));
			LabMObAttack = dynamic_cast<CLabelEx*>(g_stUIStart.frmMobInfo->Find("LabMObAttack"));
			
			frmMobInfo->evtClose = _evtOnClose;

			}

	}
	{
		frmDetail = _FindForm("frmDetail");
		if( frmDetail ) 
		{
			frmDetail->Refresh();
			proMainHP = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainHP1") );
			if( !proMainHP ) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainHP1" );   
			proMainHP->SetPosition(0.0f);

			proMainSP = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainSP") );
			if( !proMainSP ) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainSP" );   
			proMainSP->SetPosition(0.0f);

			proMainExp = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainEXP") );
			if( !proMainExp ) 
			{
				Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainEXP" );   
			}
			else
			{
				proMainExp->SetPosition(0.0f);
			}

			labMainName = dynamic_cast<CLabel *>( frmDetail->Find("labMainID") );
			if ( !labMainName ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "labMainID");

			labMainLevel = dynamic_cast<CLabel *>( frmDetail->Find("labMainLv"));
			if ( !labMainLevel ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "labMainLv");

			imgLeader = dynamic_cast<CImage*>( frmDetail->Find("imgLeader"));
			if ( !imgLeader ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "imgLeader");

			C3DCompent* d3dSelfDown = dynamic_cast<C3DCompent*>( frmDetail->Find("d3dSelfDown") );
			if (!d3dSelfDown) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "d3dSelfDown" );
			//d3dSelfDown->SetRenderEvent( _MainChaRenderEvent );
			d3dSelfDown->evtMouseDown = _evtSelfMouseDown;
			d3dSelfDown->SetMouseAction( enumMA_Skill );
			

			C3DCompent* p3D = dynamic_cast<C3DCompent*>( frmDetail->Find("d3dSelf") );
			if (!p3D) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "d3dSelf" );
            
			p3D->SetRenderEvent( _MainChaRenderEvent );
			//p3D->evtMouseDown = _evtSelfMouseDown;
			//p3D->SetMouseAction( enumMA_Skill );	
            
			RECT rt;
			rt.left = p3D->GetX();
			rt.right = p3D->GetX2();
			rt.top = p3D->GetY();
			rt.bottom = p3D->GetY2();
            
			pMainCha = new CCharacter2D;
			pMainCha->Create( rt );
		}

		// 点自己头像的右键菜单
		mnuSelf = CMenu::FindMenu("selfMouseRight");
		if (!mnuSelf)  return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "selfMouseRight" );
		mnuSelf->evtListMouseDown=_OnSelfMenu;
	}

	// frmMain800表单
	{
		frmMain800 = _FindForm("frmMain800");
		frmMain800->evtEntrustMouseEvent = _evtTaskMouseEvent;

		tlCity = dynamic_cast<CTitle*>(frmMain800->Find("tlCity"));
		tlField = dynamic_cast<CTitle*>(frmMain800->Find("tlField"));

		//grdHeart = dynamic_cast<CGrid*>(frmMain800->Find("grdHeart"));
		//if( !grdHeart )	return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "grdHeart" );
		//grdHeart->evtSelectChange = _evtChaHeartChange;

		//grdAction = dynamic_cast<CGrid*>(frmMain800->Find("grdAction"));
		//if( !grdAction ) return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "grdAction" );
		//grdAction->evtSelectChange = _evtChaActionChange;
	}



	// 角色属性
	//proMainHP1 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP1") );
	//if( !proMainHP1 ) return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "proMainHP1" );   
	//proMainHP1->SetPosition(10.0f );
	//
	//proMainHP2 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP2") );
	//if( !proMainHP2 ) return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "proMainHP2" );   
	//proMainHP2->SetPosition(10.0f );

	//proMainHP3 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP3") );
	//if( !proMainHP3 ) return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "proMainHP3" );   
	//proMainHP3->SetPosition(10.0f );

	//proMainSP =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainSP") );
	//if( !proMainSP ) return Error( "msgui.clu界面<%s>上找不到控件<%s>", frmMain800->GetName(), "proMainSP" );
 //  	proMainSP->SetPosition (10.0f );

	//_pShowExp = dynamic_cast<CLabel*>(frmMain800->Find( "labMainEXP" ) );
	//_pShowLevel = dynamic_cast<CLabel*>(frmMain800->Find( "labMainLV" ) );

	//frmMainFun表单
	{
		frmMainFun = _FindForm("frmMainFun");
		if(!frmMainFun) return false;

		frmMainFun->evtEntrustMouseEvent = _evtStartFormMouseEvent;

		// QQ
		/*FORM_CONTROL_LOADING_CHECK(btnQQ,frmMainFun,CTextButton,"msgui.clu","btnQQ");
		btnQQ->GetImage()->LoadImage("texture/ui/main800.tga",32,32,4,136,201);*/

		// 升级帮助，会闪烁
		btnLevelUpHelp = dynamic_cast<CTextButton*>(frmMainFun->Find("btnLevelUpHelp"));
		//FORM_CONTROL_LOADING_CHECK(btnLevelUpHelp, frmMainFun, CTextButton, "msgui.clu", "btnLevelUpHelp");
		if(btnLevelUpHelp)	btnLevelUpHelp->SetFlashCycle();
	}

	
	// 右键菜单
	mainMouseRight=CMenu::FindMenu("mainMouseRight");
	if (!mainMouseRight)
	{
		return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "mainMouseRight" );
	}
	mainMouseRight->evtListMouseDown=_evtPopMenu;

	// 主角复活表单
	frmMainChaRelive = _FindForm("frmRelive");
	if(!frmMainChaRelive) return false;
	frmMainChaRelive->evtEntrustMouseEvent = _evtReliveFormMouseEvent;

	//船只航行时的界面
	frmShipSail = _FindForm("frmShipsail");  //属性表单 
	if( !frmShipSail )		return false;

	labCanonShow	 =	(CLabelEx*)frmShipSail->Find("labCanonShow1");
	labSailorShow	 =	(CLabelEx*)frmShipSail->Find("labSailorShow1");
	labLevelShow	 =  (CLabelEx*)frmShipSail->Find("labLvship");
	labExpShow		 =  (CLabelEx*)frmShipSail->Find("labExpship");

	proSailor		 =	(CProgressBar*)frmShipSail->Find("proSailor");				//耐久滚动条
	proCanon		 =	(CProgressBar*)frmShipSail->Find("proCanon");				//补给滚动条
	frmShipSail->SetIsShow(false);
	
	//	Modify by alfred.shi 20080828
	CTextButton* btn1 = (CTextButton*)frmShipSail->Find("btnShip");
	if (!btn1) return false;
	btn1->evtMouseClick = _evtShowBoatAttr;

	// 自动跟随
	frmFollow = _FindForm("frmFollow");
	if( !frmFollow ) return false;

	labFollow = dynamic_cast<CLabel*>( frmFollow->Find("labFollow") );
	if( !labFollow ) return Error( g_oLangRec.GetString(45), frmFollow->GetName(), "labFollow" );

	// 宠物界面
	frmMainPet = _FindForm("frmMainPet");
	if( !frmMainPet )		return false;

	frmMainPet->Hide();

	labPetLv = dynamic_cast<CLabel*>( frmMainPet->Find("labPetLv") );
	if( !labPetLv ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "labPetLv" );

	imgPetHead = dynamic_cast<CImage*>( frmMainPet->Find("imgPetHead") );
	if( !imgPetHead ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "imgPetHead" );

	proPetHP = dynamic_cast<CProgressBar*>( frmMainPet->Find("proPetHP") );
	if( !proPetHP ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "proPetHP" );

	proPetSP = dynamic_cast<CProgressBar*>( frmMainPet->Find("proPetSP") );
	if( !proPetSP ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "proPetSP" );

	//
	// 新手帮助界面
	//
	frmHelpSystem = CFormMgr::s_Mgr.Find("frmHelpSystem");
	if( !frmHelpSystem ) return Error( g_oLangRec.GetString(45), "frmHelpSystem", "frmHelpSystem" );

	lstHelpList   = dynamic_cast<CList*>(frmHelpSystem->Find("lstHelpList"));
	if(! lstHelpList) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), "lstHelpList" );
	lstHelpList->evtSelectChange =  _evtHelpListChange;

	frmHelpSystem->evtEntrustMouseEvent = _evtStartFormMouseEvent;

	char szName[64] = {0};
	for(int i = 0; i < HELP_PICTURE_COUNT; ++i)
	{
		sprintf(szName, "imgHelpShow%d_1", i + 1);
		imgHelpShow1[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow1[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_2", i + 1);
		imgHelpShow2[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow2[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_3", i + 1);
		imgHelpShow3[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow3[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_4", i + 1);
		imgHelpShow4[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow4[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		if(i > 0)
		{
			imgHelpShow1[i]->SetIsShow(false);
			imgHelpShow2[i]->SetIsShow(false);
			imgHelpShow3[i]->SetIsShow(false);
			imgHelpShow4[i]->SetIsShow(false);
		}
		else
		{
			imgHelpShow1[i]->SetIsShow(true);
			imgHelpShow2[i]->SetIsShow(true);
			imgHelpShow3[i]->SetIsShow(true);
			imgHelpShow4[i]->SetIsShow(true);
		}
	}

	//
	// 背包按钮界面
	//
	frmBag = CFormMgr::s_Mgr.Find("frmBag");
	if(! frmBag) return Error(g_oLangRec.GetString(45), "frmBag", "frmBag");
	frmBag->evtEntrustMouseEvent = _evtStartFormMouseEvent;

	//
	// 社交按钮面板
	//
	frmSociliaty = CFormMgr::s_Mgr.Find("frmSociliaty");
	if(! frmSociliaty) return Error(g_oLangRec.GetString(45), "frmSociliaty", "frmSociliaty");
	frmSociliaty->evtEntrustMouseEvent = _evtStartFormMouseEvent;
	
	//
	// NPC指引面板 add by alfred.shi 20080710 begin
	//
	strMapName = "";

	frmNpcShow = CFormMgr::s_Mgr.Find("frmNpcShow");
	if(! frmNpcShow) return Error(g_oLangRec.GetString(45) , "frmNpcShow", "frmNpcShow");

	lstNpcList = dynamic_cast<CList*>(frmNpcShow->Find("lstNpcList"));
	assert(lstNpcList != NULL);	
	lstMonsterList = dynamic_cast<CList*>(frmNpcShow->Find("lstMonsterList"));
	assert(lstMonsterList != NULL);

	chkID  =( CCheckBox *)frmNpcShow->Find( "chkID" );
	chkID->SetIsChecked(true);

	lstNpcList->evtSelectChange =  _evtNPCListChange;
	lstMonsterList->evtSelectChange =  _evtNPCListChange;

	lstCurrList = lstNpcList;

	listPage = dynamic_cast<CPage*>(frmNpcShow->Find("pgeSkill"));
	assert(listPage != NULL);
	listPage->evtSelectPage = _evtPageIndexChange;

	return true;
}
void CStartMgr::ShowNPCHelper(const char * mapName,bool isShow)
{
	string strCurMap = g_pGameApp->GetCurScene()->GetTerrainName();

    if(strCurMap == "garner")
		strCurMap = g_oLangRec.GetString(56);
    else if(strCurMap == "magicsea")
        strCurMap = g_oLangRec.GetString(57);
	else if(strCurMap == "darkblue")
		strCurMap = g_oLangRec.GetString(58);
	else  if(strCurMap == "winterland")
		strCurMap = "Winter Land";
	else  if(strCurMap == "jialebi")
		strCurMap = "Carribean";


	strMapName =  strCurMap.c_str();
	{
		CListItems* items = lstCurrList->GetItems();
		items->Clear();
		lstCurrList->Refresh();

		int nTotalIndex = NPCHelper::I()->GetLastID() + 1;
		for(int i = 0; i < nTotalIndex ; ++ i)
		{
			NPCData * p = GetNPCDataInfo(i);
			if(p && strcmp(p->szMapName,strMapName) == 0) 
			{
				char buff[1024];
				std::string strName(p->szName);
				while (strName.length() < 18)
				{
					strName += " ";
				}
				std::string strAreaName = std::string("<") + std::string(p->szArea) + std::string(">");
				while (strAreaName.length() < 16)
				{
					strAreaName += " ";
				}
				//_snprintf_s( buff, _countof( buff ), _TRUNCATE, "%s%s<%d,%d>",strName.c_str(),strAreaName.c_str(),p->dwxPos0,p->dwyPos0);
				sprintf(buff, "%s%s<%d,%d>", strName.c_str(),strAreaName.c_str(),p->dwxPos0,p->dwyPos0);
				lstCurrList->Add(buff);
				lstCurrList->Refresh();
			}
		}
	}

	frmNpcShow->SetIsShow(isShow);
}	
void CStartMgr::_evtPageIndexChange(CGuiData *pSender)
{
	NPCHelper* pNPCHelper = NPCHelper::I();
	SAFE_DELETE(pNPCHelper);
	pNPCHelper = new NPCHelper(0,1000);

	int index = g_stUIStart.listPage->GetIndex();
	if(index == 0) //npc
	{
		pNPCHelper->LoadRawDataInfo("scripts/table/NPCList", FALSE);
		g_stUIStart.lstCurrList = g_stUIStart.lstNpcList;
	}
	else if(index == 1)
	{
		pNPCHelper->LoadRawDataInfo("scripts/table/MonsterList", FALSE);
		g_stUIStart.lstCurrList = g_stUIStart.lstMonsterList;
	}
	g_stUIStart.ShowNPCHelper(g_stUIStart.GetCurrMapName(),true);
}
void CStartMgr::_evtNPCListChange(CGuiData *pSender)
{
	CListItems* pItems = g_stUIStart.lstCurrList->GetItems();
	int nIndex = pItems->GetIndex(g_stUIStart.lstCurrList->GetSelectItem());

	CItemRow* itemrow = g_stUIStart.lstCurrList->GetSelectItem();
	CItemObj* itemobj = itemrow->GetBegin();

	std::string itemstring(itemobj->GetString());

	int pos = (int)itemstring.find(">") + 1;
	int pos1 = (int)itemstring.find("<",pos);
	int pos2 = (int)itemstring.find(",",pos);
	int pos3 = (int)itemstring.find(">",pos);

	if(pos1 >= 0 && pos2 > pos1 && pos3 > pos2 && pos3 <= (int)itemstring.length())
	{
		string xStr = itemstring.substr(pos1 + 1,pos2 - pos1 - 1);
		string yStr = itemstring.substr(pos2 + 1,pos3 - pos2 - 1);

		g_stUIMap.ShowRadar(xStr.c_str(),yStr.c_str());
	}
	g_stUIStart.ShowNPCHelper(g_stUIStart.GetCurrMapName(),false);
}

void CStartMgr::ShowShipSailForm(bool isShow /* = true  */)
{
	UpdateShipSailForm();
	frmShipSail->SetIsShow(isShow);
}
void CStartMgr::UpdateShipSailForm()
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if(!pMain || !pMain->IsBoat())
		return;

	SGameAttr* pAttr = pMain->getGameAttr();

	char buf[128];

	sprintf(buf, "%d/%d", pAttr->get(ATTR_HP), pAttr->get(ATTR_MXHP));
	labSailorShow->SetCaption(buf);
	proSailor->SetRange((float)0, (float)(pAttr->get(ATTR_MXHP)));
	proSailor->SetPosition((float)(pAttr->get(ATTR_HP)));

	sprintf(buf, "%d/%d", pAttr->get(ATTR_SP), pAttr->get(ATTR_MXSP));
	labCanonShow->SetCaption(buf);
	proCanon->SetRange((float)0, (float)(pAttr->get(ATTR_MXSP)));
	proCanon->SetPosition((float)(pAttr->get(ATTR_SP)));

	labLevelShow->SetCaption(itoa(pAttr->get(ATTR_LV), buf, 10));
	labExpShow->SetCaption(itoa(pAttr->get(ATTR_CEXP), buf, 10));

}

void CStartMgr::End()
{
	//delete pMainCha;
	//pMainCha = NULL;
	SAFE_DELETE(pMainCha); // UI当机处理
	SAFE_DELETE(pTarget); // UI当机处理
}

void CStartMgr::ShowQueryReliveForm( int nType, const char* str )
{
	stSelectBox* pOriginRelive = g_stUIBox.ShowSelectBox( _evtOriginReliveFormMouseEvent, str, false );
	frmSelectOriginRelive = pOriginRelive->frmDialog;
	frmSelectOriginRelive->nTag = nType;
}

void CStartMgr::_evtOriginReliveFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	frmSelectOriginRelive = NULL;
	if( pSender->GetForm()->nTag==enumEPLAYER_RELIVE_ORIGIN )
	{
		if( nMsgType==CForm::mrYes )
		{
			CS_DieReturn(enumEPLAYER_RELIVE_ORIGIN);		
			g_stUIStart.frmMainChaRelive->SetIsShow(false);
		}
		else
		{
			CS_DieReturn(enumEPLAYER_RELIVE_NORIGIN);		
		}
	}
	else
	{
		if( nMsgType==CForm::mrYes )
		{
			CS_DieReturn(enumEPLAYER_RELIVE_MAP);		
			g_stUIStart.frmMainChaRelive->SetIsShow(false);
		}
		else
		{
			CS_DieReturn(enumEPLAYER_RELIVE_NOMAP);		
		}
	}
}

void CStartMgr::_evtReliveFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	//if( name=="btnReCity" )
	{
		CS_DieReturn(enumEPLAYER_RELIVE_CITY);
		pSender->GetForm()->SetIsShow(false);
		if( frmSelectOriginRelive )
		{
			frmSelectOriginRelive->SetIsShow(false);
			frmSelectOriginRelive = NULL;
		}
	}
}

void CStartMgr::_evtStartFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name=pSender->GetName();

	if( name=="btnChat" )	// Eve add: toggle button (chat and battle mode)
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmMainFun" );
		CTextButton* btnChat = dynamic_cast<CTextButton*>(f->Find("btnChat"));

		CForm* fchat = CFormMgr::s_Mgr.Find( "frmMain800" );
		CEdit* edtSay = dynamic_cast<CEdit*>(fchat->Find("edtSay"));

		if(!g_pGameApp->m_keyMoveMode)
		{
			g_pGameApp->m_keyMoveMode = true;
			btnChat->LoadImage("texture/ui/shield.png",23,22,0,0,true);
			edtSay->SetIsEnabled(false);
		}
		else
		{
			g_pGameApp->m_keyMoveMode = false;
			btnChat->LoadImage("texture/ui/sword.png",23,22,0,0,true);
			edtSay->SetIsEnabled(true);
		}
		edtSay->Refresh();
		btnChat->Refresh();
	}
	else if( name=="btnState" )	// 显示属性界面e
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmState" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}	
	//else if( name=="btnItem" )	// 显示道具界面
	//{
	//	CForm* f = CFormMgr::s_Mgr.Find( "frmItem" );
	//	if( f )
	//	{		
	//		f->SetIsShow( !f->GetIsShow() );
	//	}
	//	return;
	//}		
	else if( name=="btnSkill" )	// 显示技能界面
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmSkill" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	else if( name=="btnMission" )	// 显示任务界面
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmMission" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );	
		}
		return;
	}
	//else if( name=="btnGuild" )	    // 显示工会界面
	//{		
	//	CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
	//	if( f ) 
	//	{
	//		f->SetIsShow( !f->GetIsShow() );	
	//	}		
	//	return;
	//}
	else if( name=="btnHelp" )		
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmStartHelp" );
		if( f ) 
		{
			f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	//else if( name=="btnShip" )	// 显示帮助界面	Modify by alfred.shi 20080822
	//{
 //       CForm* f = CFormMgr::s_Mgr.Find("frmStartHelp");
	//	if( f ) 
	//	{
	//		f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
	//		f->SetIsShow( !f->GetIsShow() );
	//	}

 //       return;
	//}

	// 显示帮助界面		Add by alfred.shi 20080822	beign
	else if( name=="btnShip1" )	
	{
        CForm* f = CFormMgr::s_Mgr.Find("frmStartHelp");
		if( f ) 
		{
			f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
			f->SetIsShow( !f->GetIsShow() );
		}

        return;
	}
	//	End

	//if( name=="btnOpenHelp" )	
	//{	
	//	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	//	if( frm ) 
	//	{
	//		frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
	//		frm->nTag = 0;
	//		frm->ShowModal();
	//	}
	//	return;
	//}

	//	Add by alfred.shi 20080822	begin
	if( name=="btnShip2" )		
	{	
		CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
		if( frm ) 
		{
			frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
			frm->nTag = 0;
			frm->ShowModal();
		}
		return;
	}
	//	End

	else if( name=="btnSystem" )	// 显示功能界面
	{
		CForm* f = CFormMgr::s_Mgr.Find("frmSystem");
		if(f)		f->SetIsShow(!f->GetIsShow());			
		return;
	}
	else if( name=="btnQQ" )	
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmQQ" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	else if( name == "btnLevelUpHelp" )	// 玩家角色升级帮助
	{
		SGameAttr* pAttr = CGameScene::GetMainCha()->getGameAttr();

		int nLevel = pAttr->get(ATTR_LV);
		g_stUIStart.ShowHelpSystem(true, nLevel + HELP_LV1_BEGIN - 1);

		g_stUIStart.ShowLevelUpHelpButton(false);
	}
	else if( name == "btnInfoCenter" )	// 介绍新手的一些玩法的界面
	{
		bool bShow = g_stUIStart.frmHelpSystem->GetIsShow();
		g_stUIStart.ShowHelpSystem(!bShow);
	}
	else if( name == "btnOpenBag" )	// 打开背包按钮界面
	{
		g_stUIEquip.GetItemForm()->SetIsShow(!g_stUIEquip.GetItemForm()->GetIsShow());
		//g_stUIStart.ShowBagButtonForm(! g_stUIStart.frmBag->GetIsShow());
		//g_stUIStart.ShowSociliatyButtonForm(false);
		
		//g_stUIStart.frmBag->SetIsShow(! g_stUIStart.frmBag->GetIsShow());
		//g_stUIStart.frmSociliaty->SetIsShow(false);
	}
	else if( name == "btnOpenSociliaty" ) // 打开社交按钮界面
	{
		//g_stUIStart.ShowSociliatyButtonForm(! g_stUIStart.frmSociliaty->GetIsShow());
		//g_stUIStart.ShowBagButtonForm(false);

		//g_stUIStart.frmSociliaty->SetIsShow(! g_stUIStart.frmSociliaty->GetIsShow());
		//g_stUIStart.frmBag->SetIsShow(false);
		
		CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
		if( f ) 
		{
			bool a =f->GetIsShow();
			f->SetIsShow( !a );
		//	Add by alfred.shi 20080905	begin
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha->getGuildID()<=0)
		{
			g_pGameApp->MsgBox("You are not in a guild.");
		}
		
		}		
		return;
		
		
		
	}
	//else if( name == "btnOpenItem")	// 显示道具界面
	//{
	//	CForm* f = CFormMgr::s_Mgr.Find( "frmItem" );
	//	if( f )
	//	{
	//		f->SetIsShow( !f->GetIsShow() );
	//	}
	//	return;
	//}
	else if( name == "btnOpenTempBag" ) // 打开临时背包
	{
		//g_stUIStore.ShowTempKitbag();
	}
	else if( name == "btnOpenStore" )	// 打开商城
	{
		// 商城未开启,打开网页
		//g_stUIStore.ShowStoreWebPage();

		// 打开商城输入二次密码（暂时不上内置商城）
		g_stUIDoublePwd.SetType(CDoublePwdMgr::STORE_OPEN_ASK);
		g_stUIDoublePwd.ShowDoublePwdForm();
	}
	else if( name == "btnOpenGuild" )	// 显示公会
	{
	
		CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
		if( f ) 
		{
			bool a =f->GetIsShow();
			f->SetIsShow( !a );
		//	Add by alfred.shi 20080905	begin
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha->getGuildID()<=0)
		{
			g_pGameApp->MsgBox("You are not in a guild.");
		}
		//	End.
		}		
		return;
	}
	else if( name == "btnOpenTeam" )	// 志愿者
	{
		CCharacter* pMainCha = CGameScene::GetMainCha();

		// 角色 8 级以下禁止组队	Modify by alfred.shi 20080902	begin
		if(g_stUIFindTeam.IsShowFom())
			g_stUIFindTeam.ShowFindTeamForm(false);
		else if(pMainCha && !pMainCha->IsBoat() && pMainCha->getGameAttr()->get(ATTR_LV) >= 8)
		{
			CS_VolunteerOpen((short) CFindTeamMgr::FINDTEAM_PAGE_SIZE);
		}	//	End
		else
		{
			if(pMainCha->IsBoat())
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(888));
			}
			else
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(866));
				g_pGameApp->MsgBox(g_oLangRec.GetString(866));	//	Add by alfred.shi 20080905
			}
		}
	}

	return;
}

void CStartMgr::_evtSelfMouseDown(CGuiData *pSender,int x,int y ,DWORD key)
{
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if( !pScene ) return;

	CCharacter* pMain = CGameScene::GetMainCha();
	if( !pMain ) return;

	if( key & Mouse_LDown )
	{
		CSkillRecord* pSkill = pMain->GetReadySkillInfo();
		if( pSkill && g_SkillUse.IsUse( pSkill, pMain, pMain ) )
		{
			pScene->GetMouseDown().ActAttackCha( pMain, pSkill, pMain );
		}
	}
	else if( (key & Mouse_RDown) && (pMain->GetTeamLeaderID()!=0) )
	{		
		pSender->GetForm()->PopMenu( g_stUIStart.mnuSelf, x, y );
	}
}

void CStartMgr::MainChaDied()
{
	if( frmMainChaRelive )
	{
		int nLeft = ( g_pGameApp->GetWindowHeight() - frmMainChaRelive->GetWidth() ) / 2;
		int nTop = ( g_pGameApp->GetWindowHeight() - frmMainChaRelive->GetHeight() ) / 2;
		nTop-=80;
		frmMainChaRelive->SetPos( nLeft, nTop );
		frmMainChaRelive->Refresh();
		
		static CLabel* pInfo = dynamic_cast<CLabel*>( frmMainChaRelive->Find( "labReCity" ) );	
		CCharacter* pCha = CGameScene::GetMainCha();
		bool IsShow = true;
		if( pInfo && pCha )
		{
			if( pCha->IsBoat() )
			{
				pInfo->SetCaption( g_oLangRec.GetString(761) );				
			}
			else
			{
				pInfo->SetCaption( g_oLangRec.GetString(762) );

				if( CGameScene* pScene = CGameApp::GetCurScene() )
				{
					if( CMapInfo *pInfo = pScene->GetCurMapInfo() )
					{
						// Modify by lark.li 20080719 begin
						//if( stricmp( pInfo->szDataName, "teampk" )==0 )
						if( stricmp( pInfo->szDataName, "teampk" )==0 || stricmp( pInfo->szDataName,"starena1") == 0 
							|| stricmp( pInfo->szDataName,"starena2") == 0 || stricmp( pInfo->szDataName,"starena3") == 0)
							IsShow = false;
						// End
					}
				}
			}
		}

		// add by Philip.Wu  角色死亡后执行一次移动，用于关闭窗体
		CUIInterface::MainChaMove();

		// add by Philip.Wu  2006-07-05  角色死亡后关闭交易界面
		// BUG规避：TEST-32  由交易后触发的替死娃娃功能bug
		g_stUITrade.CloseAllForm();

		// add by Philip.Wu  2006-07-12  角色死亡后关闭出海界面
		CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
		if(pWorldScene && pWorldScene->GetShipMgr())
		{
			pWorldScene->GetShipMgr()->CloseForm();
		}


		if( IsShow ) frmMainChaRelive->Show();
	}
}

void CStartMgr::CheckMouseDown( int x, int y  )
{
	//if( frmMainFun->GetIsShow() )
	//{
	//	if( !frmMainFun->InRect(x,y) )
	//	{
	//		//frmMainFun->SetIsShow(false);	
	//	}
	//}

	//if ( grdAction->GetIsShow() )
	//{
	//	if ( !grdAction->InRect(x,y) )
	//		grdAction->SetIsShow(false);
	//}		

	//if ( grdHeart->GetIsShow() )
	//{
	//	if ( !grdHeart->InRect(x,y) )
	//		grdHeart->SetIsShow(false);		
	//}	

	//if ( g_stUICoze.GetFaceGrid()->GetIsShow() )
	//{
	//	if ( !g_stUICoze.GetFaceGrid()->InRect(x,y) )
	//		g_stUICoze.GetFaceGrid()->SetIsShow(false);		
	//}
}

void CStartMgr::_evtTaskMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name=pSender->GetName();

	if( name=="btnStart" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmMainFun" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );			
		}
		return;
	}	
	//else if( name=="btnAction" )
	//{
	//	g_stUIStart.grdHeart->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( false );
	//	g_stUIStart.grdAction->SetIsShow(!g_stUIStart.grdAction->GetIsShow() );			
	//	return;
	//}	
	//else if( name=="btnBrow" )	
	//{
	//	g_stUIStart.grdAction->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( false );
	//	g_stUIStart.grdHeart->SetIsShow( !g_stUIStart.grdHeart->GetIsShow() );
	//	return;
	//}	
	//else if( name=="btnChatFace" )
	//{
	//	g_stUIStart.grdAction->SetIsShow( false );
	//	g_stUIStart.grdHeart->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( !g_stUICoze.GetFaceGrid()->GetIsShow() );
	//	return;
	//}

	// 快捷键的点击 
	if( pSender->nTag > 10 )
	{
		CCharacter* c = CGameScene::GetMainCha();
        if( !c )		return;

		c->ChangeReadySkill( pSender->nTag );
	}
	return;
}

void  CStartMgr::_evtChaActionChange(CGuiData *pSender)
{
	CCharacter* pCha = g_pGameApp->GetCurScene()->GetMainCha();
	if( !pCha ) return;
	
	pSender->SetIsShow( false );

	CGrid *p = dynamic_cast<CGrid*>(pSender);
	if( !p ) return;
	CGraph * r = p->GetSelect();
	int nIndex = p->GetSelectIndex();
    if( r )
	{		
		pCha->GetActor()->PlayPose( r->nTag, true, true);
	}	
}

void  CStartMgr::_evtChaHeartChange(CGuiData *pSender)
{
	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha ) return;  
	pSender->SetIsShow( false );

	CGrid *p = dynamic_cast<CGrid*>(pSender);
	if( !p ) return;
	CGraph * r = p->GetSelect();
	int nIndex = p->GetSelectIndex();
    if( r )
	{
		pCha->GetHeadSay()->SetFaceID(nIndex);
		char szFaceID[10] = {0} ;
		sprintf( szFaceID , "***%d" ,nIndex);	
		CS_Say( szFaceID);		
	}
}

void CStartMgr::RefreshMainLifeNum( long num, long max )
{	
	////HP的变化
	//char szHP[32] = { 0 };
	//if ( num < 0 )	num = 0;
	//sprintf( szHP,"%d/%d", num, max);
	//szHP[sizeof(szHP)-1] = '\0';

	//float f = (float) num /(float) max;
	//CProgressBar* pBar = NULL;
	//if( f < 0.334 )
	//{
	//	pBar = proMainHP3;
	//	proMainHP2->SetIsShow(false);
	//	proMainHP1->SetIsShow(false);
	//}
	//else if( f > 0.666 )
	//{
	//	pBar = proMainHP1;
	//	proMainHP2->SetIsShow(false);
	//	proMainHP3->SetIsShow(false);
	//}
	//else
	//{
	//	pBar = proMainHP2;
	//	proMainHP1->SetIsShow(false);
	//	proMainHP3->SetIsShow(false);
	//}

	//pBar->SetIsShow(true);
	//pBar->SetPosition( 10.0f * f ) ;

	if (proMainHP)
	{
		proMainHP->SetRange(0.0f, (float)max);
		proMainHP->SetPosition((float)num);
	}
}

void CStartMgr::RefreshMainExperience(long num, long curlev, long nextlev)
{
	LG("exp", g_oLangRec.GetString(763), num, curlev, nextlev, 100.0f * (float)(num - curlev) / (float)(nextlev - curlev));
	
	//// EXP的变化
	//long max = nextlev - curlev;
	//num = num - curlev;
	//if ( num < 0 ) num = 0;

	//if (max!=0)
	//	sprintf( szBuf , "%4.2f%%" , num*100.0f/max );	
	//else 
	//	sprintf( szBuf , "0.00%");
	//szBuf[sizeof(szBuf)-1] = '\0';

	//_pShowExp->SetCaption( szBuf );
	if ( proMainExp )
	{
		proMainExp->SetRange(0, 1);
		proMainExp->SetPosition((float)(num - curlev) / (float)(nextlev - curlev));
	}
}

void CStartMgr::RefreshMainName(const char* szName )
{
	if ( labMainName )
	{
		labMainName->SetCaption(szName);
	}
}


void CStartMgr::RefreshLv( long l )
{
	if (labMainLevel)
	{
		sprintf( szBuf, "%d", l );
		labMainLevel->SetCaption( szBuf );
	}
}

void CStartMgr::RefreshMainSP( long num , long max )
{
	//SP的变化
	if ( proMainSP )
	{
		proMainSP->SetRange(0.0f, (float)max);
		proMainSP->SetPosition( (float)num );  
	}
}

void CStartMgr::_evtPopMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	mainMouseRight->SetIsShow(false);
	CMenuItem* pItem=mainMouseRight->GetSelectMenu();
	if (!pItem) return;
	string str=pItem->GetString();
	if (str==g_oLangRec.GetString(764))	// ���뽻��
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && pCha!=pMain && pCha->IsEnabled() && pMain->IsEnabled() 
			&& ( ( !pCha->IsBoat() && !pMain->IsBoat() )
				|| ( pCha->IsBoat() && pMain->IsBoat() ) ) )
		{
			if(pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 6)
			{
				CS_RequestTrade( mission::TRADE_CHAR, mainMouseRight->nTag );
			}
			else
			{
				// ��ɫ�ȼ�6�����½�ֹ�������뽻��
				g_pGameApp->SysInfo(g_oLangRec.GetString(864));
			}
		}
		else
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(765) );	// ����ʱ����˫��������ң����Ǵ�
		}
	}
	else if (str==g_oLangRec.GetString(482))	// ���Ӻ���
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if(!pCha || !pMain)  return;

		if(pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 7)
		{
			CS_Frnd_Invite( pCha->getHumanName());
		}
		else
		{
			// ��ɫ�ȼ�7�����½�ֹ�������Ӻ���
			g_pGameApp->SysInfo(g_oLangRec.GetString(865));
			
		}
	}
	else if (str==g_oLangRec.GetString(484))	// �������
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if(!pCha || !pMain)  return;

		if( (pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 8) &&
			 (pCha->IsBoat() ||  pCha->getGameAttr()->get(ATTR_LV) >= 8))
		{
			CS_Team_Invite( pCha->getHumanName());
		}
		else
		{
			// ��ɫ�ȼ�8�����½�ֹ�����������
			g_pGameApp->SysInfo(g_oLangRec.GetString(866));
		}

		return;	
	}
	else if (str==g_oLangRec.GetString(483))	// �뿪����
	{
		CS_Team_Leave();
		return;
	}
	else if (str==g_oLangRec.GetString(481))	// ����Է�
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if(pCha)
		{
			CCozeForm::GetInstance()->OnPrivateNameSet(pCha->getHumanName());
		}
		return;
	}
	else if (str==g_oLangRec.GetString(766))	// ���ս���
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && pCha->IsBoat() && pMain->IsBoat() )
		{
			CS_RequestTrade( mission::TRADE_BOAT, mainMouseRight->nTag );
		}
		else
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(767) );
		}
		return;
	}
	else if( str==g_oLangRec.GetString(768) )	// �鿴̯λ
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha && !pCha->IsMainCha() )
		{
			CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
			if( pScene )
			{
				pScene->GetMouseDown().ActShop( CGameScene::GetMainCha(), pCha );
			}
		}
		return;
	}
	else if( str==g_oLangRec.GetString(769) )	// ���鵥��
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha ) CS_TeamFightAsk( pCha->getAttachID(), pCha->lTag, enumFIGHT_TEAM );
		return;
	}
	else if( str==g_oLangRec.GetString(770) )	// ��ҵ���
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha ) CS_TeamFightAsk( pCha->getAttachID(), pCha->lTag, enumFIGHT_MONOMER );
		return;
	}
	else if(str == g_oLangRec.GetString(855))	// �����ʦ
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && !pCha->IsBoat() && !pMain->IsBoat() )
		{
			const char* szName = pCha->getHumanName();
			CS_MasterInvite(pCha->getHumanName(), mainMouseRight->nTag);
		}
		else
		{
			// ��ʦ����ͽʱ����˫���������
			g_pGameApp->SysInfo(g_oLangRec.GetString(888));
		}
	}
	else if(str == g_oLangRec.GetString(859))	// ������ͽ
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && !pCha->IsBoat() && !pMain->IsBoat() )
		{
			const char* szName = pCha->getHumanName();
			CS_PrenticeInvite(pCha->getHumanName(), mainMouseRight->nTag);
		}
		else
		{
			// ��ʦ����ͽʱ����˫���������
			g_pGameApp->SysInfo(g_oLangRec.GetString(888));
		}
	}else if (str=="Check Eq"){
		//TODO  - Move this to a different form ?.
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		g_stUIEquip.UpdataEquipSpy( pCha->GetPart(), pCha );
	}else if (str=="Follow")
	{
		CWorldScene* pScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if(pCha){ 
		pScene->GetMouseDown().GetAutoAttack()->Follow(g_pGameApp->GetCurScene()->GetMainCha(), pCha);
		}
	}
	else if(str == "Block")
	{
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked);
		if(!pTeam->FindByName(pCha->getHumanName()))
			pTeam->Add(-1, pCha->getHumanName(), "", 9);
	}
	else if(str == "Unblock")
	{
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->DelByName(pCha->getHumanName());
	}else if(str == "Manage"){
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
	CS_RequestTalk( pCha->getAttachID(), 0 );
	}
	
	else if(str == "View Info"){
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		pChaPointer = pCha;
		g_stUIStart.SetMobInfo(pCha);
	}
	else if(str == "View Drops"){
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		pChaPointer = pCha;
		g_stUIStart.SetDropsInfo(pCha);
	}
	
	g_stUIStart.frmMain800->Refresh();
}

void CStartMgr::AskTeamFight( const char* str )
{
	g_stUIBox.ShowSelectBox( _evtAskTeamFightMouseEvent, str, false );
}

void CStartMgr::_evtAskTeamFightMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CS_TeamFightAnswer( nMsgType==CForm::mrYes );
}

// 弹出右键菜单
void  CStartMgr::PopMenu( CCharacter* pCha )
{
	//g_pGameApp->GetCurScene()->GetTerrainName();

	if (pCha->IsPlayer() && !g_stUIStart.IsCanTeam())
		return;

	if( g_stUIBank.GetBankGoodsGrid()->GetForm()->GetIsShow())		// �����ĵ��޸ģ��������в��������Ҽ��˵�
		return;

	if( g_stUIBooth.GetBoothItemsGrid()->GetForm()->GetIsShow())	// �����ĵ��޸ģ���̯���������Ҽ��˵�
        return;

	if( g_stUITrade.IsTrading())// �����ĵ��޸ģ�����ʱ���������Ҽ��˵�
		return;


	if ( mainMouseRight && pCha && pCha->IsValid() && !pCha->IsHide() && (pCha->IsPlayer() || pCha->IsMonster()) ) // && (pCha->IsPlayer() || pCha->IsMonster())
	{
		mainMouseRight->nTag = pCha->getAttachID();
		mainMouseRight->SetPointer( pCha );

		CCharacter * pMain = CGameScene::GetMainCha();
		if( !pMain ) return;

		if( !pMain->IsValid() || pMain->IsHide() ) return;

		if( pCha->GetIsPet()) return;	// ���ﲻ��������Ҽ��˵�

		int nMainGuildID = pMain->getGuildID();
		int nChaGuildID  = pCha->getGuildID();
		if( nMainGuildID > 0 && nChaGuildID > 0)
		{
			if(g_stUIMap.IsGuildWar() && ((nMainGuildID <= 100 && nChaGuildID > 100) || (nMainGuildID > 100 && nChaGuildID <= 100))) return;	// ��ͬ��Ӫ
		}

		mainMouseRight->SetAllEnabled( false );
		const int nCount = mainMouseRight->GetCount();
		CMenuItem* pItem = NULL;
		const char *MapName = g_pGameApp->GetCurScene()->GetTerrainName(); // ȡ�����ﵱǰ���ڵ�ͼ Add by ning.yan 20080715
		//Add by sunny.sun20080820
		//Begin
		if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
			return;
		for( int i=0; i<nCount; i++ )
		{
			pItem = mainMouseRight->GetMenuItem( i );
			
			
			if( stricmp( pItem->GetString(), "Manage" )==0 ){
				if(!pCha->getIsPlayerCha() && !pCha->IsMonster()){
					pItem->SetIsHide( false );
					pItem->SetIsEnabled( true );
				}else{
					pItem->SetIsHide( true );
					pItem->SetIsEnabled( false );
				}
			}
			
		
			if(stricmp(pItem->GetString(), "View Info") == 0){
				if(pCha->IsMonster()) {
					pItem->SetIsHide(false);
					pItem->SetIsEnabled(true);
					
				}else{
					pItem->SetIsHide(true);
					pItem->SetIsEnabled(false);
				}
			}
			if(stricmp(pItem->GetString(), "View Drops") == 0){
				if(pCha->IsMonster()) {
					pItem->SetIsHide(false);
					pItem->SetIsEnabled(true);
					
				}else{
					pItem->SetIsHide(true);
					pItem->SetIsEnabled(false);
				}
			}
			//if(!pCha->getIsPlayerCha()){
			//	continue;
			//}
			
			if( stricmp( pItem->GetString(), g_oLangRec.GetString(764) )==0 )
			{
				if(pMain!=pCha && pMain->IsEnabled() && pCha->IsEnabled() && ((pMain->IsBoat() && pCha->IsBoat()) || (!pMain->IsBoat() && !pCha->IsBoat())) && !pCha->IsMonster())
				{
					pItem->SetIsEnabled( true );
					pItem->SetIsHide(false);
				}
				else
				{
					pItem->SetIsHide(pCha->IsMonster());
					pItem->SetIsEnabled( false );
				}
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(482) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(484) )==0 )
			{	// ��������ȡ����ӹ��� Add by ning.yan  20080715 Begin
				//if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
				//	pItem->SetIsEnabled( false );
				//// End
				//else			
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain!=pCha && ( pMain->GetTeamLeaderID()==0 || ( pMain->IsTeamLeader() && pCha->GetTeamLeaderID()!=pMain->GetTeamLeaderID() ) ) );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(483) )==0 )
			{	// ��������ȡ���뿪���鹦��  Add by ning.yan  20080715 Begin
				//if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
				//	pItem->SetIsEnabled( false );
				//// End
				//else
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain->GetTeamLeaderID()!=0 && pCha->GetTeamLeaderID()==pMain->GetTeamLeaderID() );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(481) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(766) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha && pMain->IsBoat() && pMain->IsEnabled() && pCha->IsBoat() && pCha->IsEnabled() );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(768) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha && pMain->IsEnabled() && !pMain->IsShop() && pCha->IsEnabled() && pCha->IsShop() );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(769) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain!=pCha && pMain->IsEnabled() && pMain->IsTeamLeader() && !pMain->IsShop()
					&& pCha->IsEnabled() && pCha->IsTeamLeader() && !pCha->IsShop()	);
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(770) )==0 )
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain!=pCha && pCha->IsPlayer() );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(855) )==0 )	// �����ʦ
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha && pCha->IsPlayer() && pMain->getGameAttr() && pMain->getGameAttr()->get(ATTR_LV) <= 40 );
							//&& pCha->getGameAttr()  && pCha->getGameAttr()->get(ATTR_LV) > 40 );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(859) )==0 )	// ������ͽ
			{
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled( pMain!=pCha && pCha->IsPlayer() && pMain->getGameAttr() && pMain->getGameAttr()->get(ATTR_LV) > 40 );
							//&& pCha->getGameAttr()  && pCha->getGameAttr()->get(ATTR_LV) <= 40 );
			}else if( stricmp( pItem->GetString(), "Check Eq" )==0 ){
				if (pCha->IsPlayer()){ // pMain->getGMLv()	 == 99  Enabling for Non-GMs Mdr.st May 2020 - FPO Alpha
					pItem->SetIsEnabled( pMain!=pCha );
					pItem->SetIsHide(false);
				}else{
					pItem->SetIsHide( true );
					pItem->SetIsEnabled(false);
				}
				
			}
			else if( stricmp( pItem->GetString(), "Follow") == 0){
				pItem->SetIsHide(pCha->IsMonster());
				pItem->SetIsEnabled(pMain!=pCha && pCha->IsPlayer());
			}
			else if( stricmp( pItem->GetString(), "Block" )==0 )
			{
				
				if ((!g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(pCha->getHumanName())) && !pCha->IsMonster()) 
				{

					pItem->SetIsHide( false );
					pItem->SetIsEnabled( pMain!=pCha );
				}
				else
				{
					pItem->SetIsHide( true );
					
				}
			}
			else if( stricmp( pItem->GetString(), "Unblock" )==0 )
			{
				if (g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(pCha->getHumanName()) && !pCha->IsMonster())
				{
					pItem->SetIsHide( false );
					pItem->SetIsEnabled( pMain!=pCha );
				}
				else
				{
					pItem->SetIsHide( true );
				}
			}
		}

		if( mainMouseRight->IsAllDisabled() )
		{
			mainMouseRight->SetIsShow( false );
			return;
		}

		int x=0, y=0;
		g_Render.WorldToScreen(pCha->GetPos().x , pCha->GetPos().y, pCha->GetPos().z, &x, &y );

		if( CForm::GetActive() && CForm::GetActive()->IsNormal() )
			CForm::GetActive()->PopMenu(mainMouseRight,x,y);
		else
			frmMain800->PopMenu(mainMouseRight,x,y);
	}
}


void CStartMgr::CloseForm()
{
	//if( frmMainFun->GetIsShow() )
	//	frmMainFun->Close();

	//grdAction->SetIsShow(false);
	//grdHeart->SetIsShow(false);
	//g_stUICoze.GetFaceGrid()->SetIsShow( false );
}

CTextButton* CStartMgr::GetShowQQButton()
{
	return btnQQ;
}

void CStartMgr::ShowBigText( const char* str )
{
	int nType = 0;
	CCharacter* pMain = CGameScene::GetMainCha();
	if( pMain && CGameApp::GetCurScene() && CGameApp::GetCurScene()->GetTerrain() )
	{
		int nArea = CGameApp::GetCurScene()->GetTerrain()->GetTile( pMain->GetCurX()/100, pMain->GetCurY()/100 )->getIsland();
		CAreaInfo* pArea = GetAreaInfo( nArea );
		if( pArea )
		{
			nType = pArea->chType;
		}
	}

	if( nType )
	{
		if( tlCity ) tlCity->SetIsShow( false );

		if( tlField )
		{
			tlField->SetCaption( str );
			tlField->SetIsShow( true );
		}
	}
	else
	{
		if( tlField ) tlField->SetIsShow( false );

		if( tlCity )
		{
			tlCity->SetCaption( str );
			tlCity->SetIsShow( true );
		}
	}
}

void CStartMgr::FrameMove(DWORD dwTime)
{
	static CTimeWork time(100);
	if( time.IsTimeOut( dwTime ) )
	{
		if (frmShipSail->GetIsShow())
		{
			UpdateShipSailForm();		
		}		
	}

	static CTimeWork pet_time(300);
	if( pet_time.IsTimeOut( dwTime ) ){
		if(g_stUIBoat.GetHuman()){
			SItemGrid Data = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRY];
			if(Data.IsValid() && CGameScene::GetMainCha() ){
				CItemRecord* pItemInfo = GetItemRecordInfo( Data.sID );
				if( !pItemInfo || pItemInfo->sType!=enumItemTypePet){
					if( frmMainPet->GetIsShow() ){			
						frmMainPet->Hide();
					}
				}else{
					proPetHP->SetRange( 0, float(Data.sEndure[1] / 50) );
					proPetHP->SetPosition( float(Data.sEndure[0] / 50) );
		
					proPetSP->SetRange( 0, Data.sEnergy[1] );
					proPetSP->SetPosition( Data.sEnergy[0] );
		
					static bool IsFlash;
					IsFlash = !IsFlash;
					if( IsFlash && Data.sEnergy[1]==Data.sEnergy[0] ){
						proPetSP->GetImage()->SetColor( 0xff00ff00 );
					}
					else{
						proPetSP->GetImage()->SetColor( 0xFFFFFFFF );
					}
					if( !frmMainPet->GetIsShow() ){
						frmMainPet->Show();
					}
				}
			}else if( frmMainPet->GetIsShow() ){			
				frmMainPet->Hide();
			}
		}else if( frmMainPet->GetIsShow() ){			
			frmMainPet->Hide();
		}
	}
}

void CStartMgr::_evtShowBoatAttr(CGuiData *pSender,int x,int y ,DWORD key)	// 显示船只属性
{
	xShipFactory* pkShip = ((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
	if (pkShip && pkShip->sbf.wnd->GetIsShow())
	{
		pkShip->sbf.wnd->SetIsShow(false);
	}
	else
	{
		CS_GetBoatInfo();
	}
}

void CStartMgr::SwitchMap()
{
	frmMainChaRelive->Close();
	CMenu::CloseAll();

	if( !(dynamic_cast<CWorldScene*>( CGameApp::GetCurScene() )) ) return;

	if( !_IsNewer ) return;

	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	if( frm ) 
	{
		frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
		frm->nTag = 1;
		frm->ShowModal();
	}
}

//~ 回调函数 =================================================================
void CStartMgr::_NewFrmMainMouseEvent(CCompent *pSender, int nMsgType, 
									  int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();
	if( name=="btnNo"  || name == "btnClose" )  
	{	
		if (pSender->GetForm()->nTag == 1)
			CBoxMgr::ShowMsgBox( _CloseEvent, g_oLangRec.GetString(771) );
		else
			pSender->GetForm()->Close();
	}
}

void CStartMgr::_HelpFrmMainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string name = pSender->GetName();  
	if( name=="btnOpenHelp" )
	{	
		CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
		if( frm ) 
		{
			frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
			frm->nTag = 0;
			frm->ShowModal();
		}
		return;
	}
}

void CStartMgr::_CloseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	if( frm ) 
	{
		frm->Close();
	}
}

void CStartMgr::SysLabel( const char *pszFormat, ... )
{
	char szBuf[2048] = { 0 };

	va_list list;
	va_start(list, pszFormat);           
	vsprintf(szBuf, pszFormat, list);
	va_end(list);

	labFollow->SetCaption( szBuf );
	frmFollow->Show();
}

void CStartMgr::SysHide()
{
	if( frmFollow->GetIsShow() ) 
		frmFollow->Hide();
}

void CStartMgr::_MainChaRenderEvent(C3DCompent *pSender, int x, int y)
{
	pMainCha->Render();
	//pTarget->Render();
}

void CStartMgr::RefreshMainFace( stNetChangeChaPart& stPart )
{
	if( pMainCha )
	{
		static stNetTeamChaPart stTeamPart;	
		stTeamPart.Convert( stPart );
		pMainCha->UpdataFace( stTeamPart );
	}
	
	
}

void CStartMgr::_OnSelfMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	CMenu* pMenu = dynamic_cast<CMenu*>( pSender );
	if( !pMenu ) return;

	pMenu->SetIsShow( false );

	CMenuItem* pItem= pMenu->GetSelectMenu();
	if (!pItem) return;

	string str = pItem->GetString();
	//Modify by sunny.sun20080820
	//Begin
	const char *MapName = g_pGameApp->GetCurScene()->GetTerrainName(); // 取得人物当前所在地图 
	if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
	{
		pItem->SetIsEnabled( false );
	}
	else
	{
		pItem->SetIsEnabled( true );
		if (str==g_oLangRec.GetString(483) )
		{
			CS_Team_Leave();
		}
	}
	//End
}

void CStartMgr::SetIsLeader( bool v )
{
	imgLeader->SetIsShow( v );
}

bool CStartMgr::GetIsLeader()
{
	return imgLeader->GetIsShow();
}

bool CStartMgr::IsCanTeamAndInfo()
{
	if( !_IsCanTeam ) 	g_pGameApp->SysInfo( g_oLangRec.GetString(772) );	
	return _IsCanTeam;
}

void CStartMgr::RefreshPet(){
	if(g_stUIBoat.GetHuman()){
		SItemGrid pGrid = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRY];
		SItemGrid pApp = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRYAPP];
	
		int ID = pGrid.sID;
		if(ID == 0){
			return;
		}
		if(g_stUIBoat.GetHuman()->GetIsShowApparel() && pApp.sID!=0){
			ID = pApp.sID;
		}
		CItemRecord* pInfo = GetItemRecordInfo( ID );
		int nLevel = pGrid.GetAttr(ITEMATTR_VAL_STR)
			+ pGrid.GetAttr(ITEMATTR_VAL_AGI) 
			+ pGrid.GetAttr(ITEMATTR_VAL_DEX) 
			+ pGrid.GetAttr(ITEMATTR_VAL_CON) 
			+ pGrid.GetAttr(ITEMATTR_VAL_STA);
	
		sprintf( szBuf, "%d", nLevel );
		labPetLv->SetCaption( szBuf );
	
		static CGuiPic* Pic = imgPetHead->GetImage();
		Pic->LoadImage( pInfo->GetIconFile(), 
			imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
			0, 
			4, 4 );
	
	}
}
void CStartMgr::RefreshPet( SItemGrid pGrid,SItemGrid pApp )
{
	int ID = pGrid.sID;
	if(g_stUIBoat.GetHuman()->GetIsShowApparel() && pApp.sID!=0){
		ID = pApp.sID;
	}
	CItemRecord* pInfo = GetItemRecordInfo( ID );
	if(pInfo){
		int nLevel = pGrid.GetAttr(ITEMATTR_VAL_STR)
			+ pGrid.GetAttr(ITEMATTR_VAL_AGI) 
			+ pGrid.GetAttr(ITEMATTR_VAL_DEX) 
			+ pGrid.GetAttr(ITEMATTR_VAL_CON) 
			+ pGrid.GetAttr(ITEMATTR_VAL_STA);
	
		sprintf( szBuf, "%d", nLevel );
		labPetLv->SetCaption( szBuf );
	
		static CGuiPic* Pic = imgPetHead->GetImage();
		Pic->LoadImage( pInfo->GetIconFile(), 
			imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
			0, 
			4, 4 );
	}
}

void CStartMgr::RefreshPet( CItemCommand* pItem )
{
	static CItemRecord* pInfo = NULL;
	pInfo = pItem->GetItemInfo();

	static SItemHint s_item;
	memset( &s_item, 0, sizeof(SItemHint) );
	s_item.Convert( pItem->GetData(), pInfo );

	// 更新宠物等级,头像
	int nLevel = s_item.sInstAttr[ITEMATTR_VAL_STR]
				+ s_item.sInstAttr[ITEMATTR_VAL_AGI] 
				+ s_item.sInstAttr[ITEMATTR_VAL_DEX] 
				+ s_item.sInstAttr[ITEMATTR_VAL_CON] 
				+ s_item.sInstAttr[ITEMATTR_VAL_STA];

	sprintf( szBuf, "%d", nLevel );
	labPetLv->SetCaption( szBuf );

	static CGuiPic* Pic = imgPetHead->GetImage();
	Pic->LoadImage( pInfo->GetIconFile(), 
		imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
		0, 
		4, 4 );
}


void CStartMgr::ShowHelpSystem(bool bShow, int nIndex)
{
	int nCount = g_stUIStart.lstHelpList->GetItems()->GetCount();

	if(0 > nIndex || nCount <= nIndex)
	{
		// 越界
		frmHelpSystem->SetIsShow(bShow);
		return;
	}

	for(int i = 0; i < nCount; ++i)
	{
		imgHelpShow1[i]->SetIsShow(nIndex == i);
		imgHelpShow2[i]->SetIsShow(nIndex == i);
		imgHelpShow3[i]->SetIsShow(nIndex == i);
		imgHelpShow4[i]->SetIsShow(nIndex == i);
	}

	frmHelpSystem->SetIsShow(bShow);
}


void CStartMgr::ShowLevelUpHelpButton(bool bShow)
{
	if(btnLevelUpHelp)
	{
		btnLevelUpHelp->SetIsShow(bShow);
	}
}

void CStartMgr::ShowInfoCenterButton(bool bShow)
{
	CTextButton* btnInfoCenter = dynamic_cast<CTextButton*>(frmMainFun->Find("btnInfoCenter"));
	if(btnInfoCenter)
	{
		btnInfoCenter->SetIsShow(bShow);
	}
}

void CStartMgr::_evtHelpListChange(CGuiData *pSender)
{
	//g_pGameApp->MsgBox("Index: %d\nName:  %s", nIndex, pSender->GetName());//g_stUIStart.lstHelpList->GetSelectItem()->
	CListItems* pItems = g_stUIStart.lstHelpList->GetItems();
	int nIndex = pItems->GetIndex(g_stUIStart.lstHelpList->GetSelectItem());

	//g_stUIStart.ShowLevelUpHelpButton(false);
	g_stUIStart.ShowHelpSystem(true, nIndex);
}

void CStartMgr::ShowBagButtonForm(bool bShow)
{
	if(frmBag)
	{
		frmBag->SetPos(frmMainFun->GetX2() - frmBag->GetWidth()-109, frmMainFun->GetY() - frmBag->GetHeight()+41);
		frmBag->Refresh();

		frmBag->SetIsShow(bShow);
	}
}

void CStartMgr::ShowSociliatyButtonForm(bool bShow)
{
	if(frmSociliaty)
	{
		frmSociliaty->SetPos(frmMainFun->GetX2() - frmSociliaty->GetWidth()-67, frmMainFun->GetY() - frmSociliaty->GetHeight()+39);
		frmSociliaty->Refresh();

		frmSociliaty->SetIsShow(bShow);
	}
}

void CStartMgr::_evtOnClose( CForm* pForm, bool& IsClose )  // �ر��û�����
	{
		pTarget = NULL;
		pLastTarget = NULL;
	}
