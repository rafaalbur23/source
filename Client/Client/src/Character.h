#pragma once
#include "Scene.h"
#include "SceneNode.h"
#include "MPCharacter.h"
#include "CompCommand.h"
#include "CharacterModel.h"
#include "publicfun.h"
#include "CharacterAction.h"
#include "BoolSet.h"
#include "CharacterRecord.h"
#include "Actor.h"
#include "FindPath.h"
#include "GlobalVar.h"

const int	AutoMountHeight = 75;
const int	ITEM_FACE_MAX =	4;			// 锟斤拷锟斤拷前4锟斤拷锟斤拷锟斤拷为锟斤拷鄹锟斤拷锟?

class CShadeEff;
class CEffectObj;
class CGameScene;
class CActor;
class CSkillRecord;
class CChaRecord;
class CSkillStateRecord;
struct stSkillState;
class CEvent;
struct xShipInfo;
class CSceneHeight;

namespace GUI
{
	class CHeadSay;
}

struct LoadChaInfo
{
    LoadChaInfo()
    {
        memset( this, 0, sizeof(LoadChaInfo) );

    }

    DWORD cha_id;
    char bone[32];
    char part[5][32];
    char prop[4][32];
};

#define   ERROR_POSE_HEIGHT 9999
#define   MAX_CANCELSTATE   3

// 锟斤拷锟斤拷锟斤拷锟斤拷
enum eMainChaType
{
	enumMainNone = 0,		// 锟斤拷锟斤拷锟斤拷
	enumMainPlayer,			// 为锟斤拷锟角碉拷锟斤拷锟?
	enumMainBoat,			// 为锟斤拷锟角的达拷
};

enum eChaState
{
	enumChaStateMove=1,		// 锟角凤拷锟斤拷贫锟?
	enumChaStateAttack,		// 锟角凤拷锟斤拷锟绞癸拷锟斤拷锟酵?锟斤拷锟斤拷
	enumChaStateUseSkill,	// 锟角凤拷锟斤拷锟绞癸拷眉锟斤拷锟?
	enumChaStateTrade,		// 锟角凤拷锟斤拷越锟斤拷锟?
	enumChaStateUseItem,	// 锟角凤拷锟斤拷锟绞癸拷锟斤拷锟狡?

	enumChaStateNoHide,		// 锟斤拷锟斤拷
	enumChaStateNoDizzy,	// 锟斤拷眩
	enumChaStateNoAni,		// 锟角凤拷锟叫讹拷锟斤拷效锟斤拷

	enumChaStateNoShop,		// 锟斤拷前锟斤拷锟斤拷锟节帮拷摊状态
};

enum eChaPkState
{
	enumChaPkSelf=1,		// 锟皆硷拷锟斤拷PK锟斤拷锟斤拷
	enumChaPkScene,			// 锟角凤拷为锟斤拷锟斤拷锟斤拷,锟斤拷锟角凤拷锟斤拷圆锟斤拷锟絇K锟斤拷锟斤拷
};

class CCharacter : public CSceneNode, public CCharacterModel
{
	friend class CHeadSay;
	friend class CChaStateMgr;

private:
	virtual BOOL	_Create(int nCharTypeID, int nType);

public:	
	virtual bool    GetRunTimeMatrix(MPMatrix44* mat, DWORD dummy_id);
	//stNetChangeChaPart look;
public:	
	void			InitState();						// 锟斤拷状态锟斤拷始锟斤拷

    void            ForceMove(int nTargetX, int nTargetY);       // 锟斤拷锟侥变方锟斤拷锟斤拷贫锟?
	void			MoveTo( int x, int y );
	void 			MoveW();
	void 			MoveA();
	void 			MoveS();
	void 			MoveD();
	int			    FaceTo( int yaw );
	int 			FaceTo( int x, int y )	{ return FaceTo(_GetTargetAngle(x, y));	    }
	int				GetTargetDistance();

    void			StopMove();
	CActor*			GetActor()				{ return _pActor;	    }

	bool			GetIsArrive()						{ return _isArrive;		}
	bool			GetIsFaceTo()						{ return !_nTurnCnt;	}

	bool			UpdataItem( int nItem, DWORD nLink  );					// 锟斤拷锟铰碉拷锟斤拷
    void            UpdataFace( stNetChangeChaPart& stPart );

	bool			LoadBoat( stNetChangeChaPart& stPart );
	static xShipInfo*	ConvertPartTo8DWORD( stNetChangeChaPart& stPart, DWORD* dwBuf );

	bool			IsTeamLeader()				{ return _nLeaderID!=0 && _nLeaderID==getHumanID();		}	// 锟角凤拷锟角队筹拷
	long			GetTeamLeaderID()			{ return _nLeaderID;	}	// 锟斤拷锟斤拷锟斤拷锟斤拷拥亩映锟斤拷桑锟?,没锟叫讹拷锟斤拷为0

	void			SetTeamLeaderID( long v )	{ _nLeaderID=v;			}

	void			RefreshShadow();
	void			SetHide(BOOL bHide);

	//void			SetHieght(float fhei);
public: // 锟斤拷锟斤拷锟角的硷拷锟斤拷锟斤拷锟?
	bool			ChangeReadySkill( int nSkillID );

	static CSkillRecord*	GetReadySkillInfo()		{ return _pReadySkillInfo;		}

    static void             SetDefaultSkill( CSkillRecord* p )  { _pDefaultSkillInfo = p;       }
	static CSkillRecord*	GetDefaultSkillInfo()	            { return _pDefaultSkillInfo;	}

	static bool			    IsDefaultSkill();
	static void			    ResetReadySkill();

	static void				SetIsShowEffects( bool v )	{ _ShowEffects = v;		}
	static void				SetIsShowApparel( bool v )	{ _ShowApparel = v;		}
	static void				SetIsShowShadow( bool v )	{ _IsShowShadow = v;		}
	static bool				GetIsShowShadow()			{ return _IsShowShadow;		}
	static bool				GetIsShowApparel()			{ return _ShowApparel;		}

	CChaRecord*		GetDefaultChaInfo()		{ return _pDefaultChaInfo;		}
	void			SetDefaultChaInfo( CChaRecord* pInfo );

	xShipInfo*		GetShipInfo()			{ return _pShipInfo;			}

	CBoolSet&		GetPK()					{ return _PK;					}
	bool			GetIsPK()				{ return _PK.IsAny();			}
	
public:
	void			ActionKeyFrame(DWORD key_id);

	bool			ItemEffect(int nEffectID, int nItemDummy, int nAngle=999 );
	CEffectObj*		SelfEffect(int nEffectID, int nDummy=-1, bool isLoop=false, int nSize=-1, int nAngle=999 );

	void			OperatorEffect( char oper, int x, int y );

	// 锟酵放凤拷锟斤拷锟斤拷效:锟接憋拷锟斤拷Dummy锟缴筹拷,锟斤拷锟斤拷锟劫讹拷,锟斤拷锟斤拷锟侥衬匡拷锟斤拷pTarget,追锟斤拷目锟斤拷锟斤拷锟絥TargetChaID
	CEffectObj*		SkyEffect(int nEffectID, int nBeginDummy=2, int nItemDummy=0, int nSpeed=400, D3DXVECTOR3* pTarget=NULL, int nTargetChaID=-1, CSkillRecord* pSkill=NULL );	

    // 锟角凤拷为锟缴诧拷锟斤拷状态,锟斤拷锟斤拷锟斤拷,锟斤拷锟阶刺?锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷时,锟斤拷锟斤拷锟斤拷锟斤拷
    bool			IsEnabled()             { return GetActor()->IsEnabled();   }
	//////////////////////////////////////////////
	void SetPlayerVip(bool vip) {
		IsVip = vip;
	}
	bool GetPlayerVip() { return IsVip; }
	/////////////////////
    // 锟角凤拷锟斤拷锟斤拷锟绞撅拷锟叫★拷锟酵?
    bool            IsInMiniMap()           { return IsValid() && GetActor()->IsEnabled() && !IsHide(); }			     

	CSceneItem*		GetAttackItem();		// 锟矫碉拷锟斤拷锟斤拷锟斤拷系牡锟斤拷锟?
    
    void            PlayAni( char* pAni, int nMax );	// 锟斤拷锟脚讹拷锟斤拷
	void			StopAni();							// 停止锟斤拷锟脚讹拷锟斤拷

    int             GetPose( int pose );

    bool            IsMainCha();
    void            CheckIsFightArea();

	int				GetSkillSelectType();

	void			setReliveTime(short sTime)	{	_sReliveTime = sTime;			}
	short			getReliveTime()				{	return _sReliveTime;			}
	int				getPatrolX()				{   return _nPatrolX;				}
	int				getPatrolY()				{   return _nPatrolY;				}
	void			setPatrol(int x, int y)		{   _nPatrolX = x, _nPatrolY = y;	}

	bool			GetIsFight()				{	return this->_InFight;			}
	void			FightSwitch(bool isFight)	{   _FightSwitch(isFight);			}

	bool			GetIsPet();	// 锟角凤拷锟角筹拷锟斤拷
	bool			GetIsFly(); // 锟角凤拷锟斤拷锟?
	bool			GetIsOnMount(); // 锟角凤拷锟斤拷锟?
	bool			GetCanPkOnMount();
	
	int				GetSpecialAppID(SItemGrid app);
	bool			IsSameEquip(stNetChangeChaPart part);
	

private:		// 锟狡讹拷
	void			_CalPos(float fProgressRate);
	int				_GetTargetAngle(int nTargetX, int nTargetY, BOOL bBack = FALSE);
    void            _DetachAllItem();
    void            _FightSwitch( bool isFight );

	float			_fStepProgressRate;
	float			_fProgressYaw;
	float			_fStep;
	int				_nAngleStep;
	int				_nTurnCnt;
	int				_nTargetX, _nTargetY;
	int				_nLastPosX, _nLastPosY;
	bool			IsVip;
	bool			_isArrive;
    bool            _isStopMove;
	float			_fMapHeight;	    // 锟节碉拷图锟较碉拷锟杰高度ｏ拷锟斤拷位锟斤拷锟斤拷
	CSceneHeight*	_pSceneHeight;

	static			bool _IsShowName;
    
private:
	static void	    _SetReadySkill( CSkillRecord* p );

	static CSkillRecord*    _pDefaultSkillInfo;

	static CSkillRecord*   _pReadySkillInfo;	// 锟酵伙拷锟斤拷准锟斤拷使锟矫的硷拷锟斤拷

	CChaRecord*     _pDefaultChaInfo;
	CActor*         _pActor;

private:
	int			_ulChaID;

    char			_szName[33];		// 锟斤拷锟斤拷
	char			_szHumanName[33];	// 锟斤拷一锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷
	char			_szGuildName[33];	// 锟斤拷锟斤拷锟斤拷锟斤拷
	char			_szGuildMotto[101];	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	int				_nGuildID;			// 锟斤拷锟斤拷ID
	int				_nGuildCircleColour;
	int				_chGuildIcon;
	int				_nGuildPermission;
	DWORD			_dwGuildColor;		// 锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷锟斤拷锟斤拷色
	DWORD			_dwNameColor;		// 锟斤拷锟斤拷锟斤拷色
	char			_szShopName[33];	// 锟教碉拷锟斤拷锟斤拷

	char			_szPreName[16];		// 前缀锟斤拷锟街ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷
	DWORD			_szPreColor;

	long			_lSideID;			// 锟斤拷锟斤拷锟斤拷一锟斤拷,锟届方,锟教凤拷,锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷战时使锟斤拷
	CShadeEff*		_pSideShade;		// 锟斤拷锟斤拷锟斤拷示锟届方,锟教凤拷

public:	// 应锟矫诧拷
	void			setSideID( long v );
	long			getSideID()							{ return _lSideID;					}
//zizo here
	void			setIsPlayerCha(bool v){_isPlayerCha = v;};
	bool			getIsPlayerCha(){return _isPlayerCha;};
	bool			_isPlayerCha;
	
	void			RefreshSelfEffects();
	void			setName(const char *pszName);
	const char*		getName()							{ return _szName;					}

	void			setGuildID( int nGuildID );
	void			setGuildCircle( int colour,int icon );
	
	void			setGuildIcon( int icon ){_chGuildIcon = icon;};
	char			getGuildIcon(){return _chGuildIcon;};
	
	int				getGuildID()						{ return _nGuildID;					}
	
	int				getGuildCircleColour()						{ return _nGuildCircleColour;					}
	DWORD			getGuildColor()						{ if(_nGuildCircleColour!=-1){return _nGuildCircleColour;}return _dwGuildColor;				}
	//DWORD			getGuildColor()						{ return _dwGuildColor;				}

	int			getGuildPermission()	{ return _nGuildPermission;}
	void			setGuildPermission( int guildPermission )	{ _nGuildPermission = guildPermission;	}
	void			setGuildName( const char* pszName )	{ strncpy( _szGuildName, pszName, sizeof(_szGuildName) );	}
	const char*		getGuildName()						{ return _szGuildName;				}
	
	int		getMobID()						{ return _ulChaID;				}
	void		setMobID(int ID)						{ _ulChaID = ID;				}

	void			setGuildMotto( const char* pszName ){ strncpy( _szGuildMotto, pszName, sizeof(_szGuildMotto) );	}
	const char*		getGuildMotto()						{ return _szGuildMotto;				}

	void			setHumanName(const char *pszName)	{ strncpy( _szHumanName, pszName, sizeof( _szHumanName ) );	}
	const char*		getHumanName()						{ return _szHumanName;				}

	void			setShopName(const char *pszName)	{ strncpy( _szShopName, pszName, sizeof( _szShopName ) );	}
	const char*		getShopName()						{ return _szShopName;				}

	void			setNameColor(DWORD dwColor);
	DWORD			getNameColor()						{	return _dwNameColor;		    }

	const char*		GetPreName()						{ return _szPreName;				}
	DWORD			GetPreColor()						{ return _szPreColor;				}

	// Game Attrib Shortcut
    void			setMoveSpeed( long lSpeed )			{	_Attr.set(ATTR_MSPD, lSpeed);   }
	long			getMoveSpeed()	 					{	return _Attr.get(ATTR_MSPD);    }
    void			setHPMax( long lValue )				{	_Attr.set(ATTR_MXHP, lValue);   }
    long			getHPMax()							{	return _Attr.get(ATTR_MXHP);    }
    void			setHP( long lValue )				{   _Attr.set(ATTR_HP, lValue);	    }
    long			getHP()								{	return _Attr.get(ATTR_HP);	    }
    void			setAttackSpeed( long lValue )	    {   _Attr.set(ATTR_ASPD, lValue);	}
    long			getAttackSpeed()					{	return _Attr.get(ATTR_ASPD);	}
    long			getLv()					{	return _Attr.get(ATTR_LV);	}

    void            setChaModalType( int type )         {   _eChaModalType = (EChaModalType)type;}
    EChaModalType   getChaModalType()                   {   return _eChaModalType;				 }

    void            setChaCtrlType( int type );
    EChaCtrlType    getChaCtrlType()                    {   return _eChaCtrlType;           }

	int				GetDangeType()						{ return _nDanger;					}

	int				getNpcType()						{   return _nNpcType;				}
	void			setNpcType( int type )				{   _nNpcType = type;				}

	void			setGMLv( char v )					{   _chGMLv = v;					}
	char			getGMLv()							{   return _chGMLv;					}

public:
	CCharacter();
	virtual ~CCharacter();
	
    void            LoadingCall();

	virtual	void	FrameMove(DWORD dwTimeParam);
	virtual void	Render();
	void			RefreshUI(int nParam = 0);
	void			RefreshLevel( int nMainLevel );
    void            RefreshItem( bool isFirst=false );

	void			EnableAI(BOOL bEnable)			{ _bEnableAI = bEnable;}
	void			ResetAITick()					{ _dwLastAITick = GetTickCount(); } 

	bool			PlayPose( DWORD pose, DWORD type = PLAY_ONCE, int time=-1, int fps = 30, bool isBlend=false );

public:	    
    void			setPos(int nX, int nY);
	void			setPos(int nX, int nY, int nZ);
	
    bool			IsBoat();
	bool			IsPlayer();
	bool			IsNPC();
	bool			IsMonster();
	bool			IsResource();

	void			SetMainType( eMainChaType v )	{ _eMainType = v;			}
	eMainChaType	GetMainType()					{ return _eMainType;		}
	
	CBoolSet*		GetChaState()					{ return &_ChaState;		}

	void			SetCircleColor(D3DCOLOR dwColor);

    int             DistanceFrom(CCharacter *pCha);
    BOOL            WieldItem( const lwSceneItemLinkInfo* info );

    int             ReCreate( DWORD type_id );

public:
	GUI::CHeadSay*	GetHeadSay() { return _pHeadSay; }
    void            DestroyLinkItem();

    int             LoadCha( const LoadChaInfo* info );

    void            UpdateTileColor();
	char*			getLogName();
	
	void			setSecondName(const char *pszSecName) { strcpy(_szSecondName, pszSecName); }
	const char*     getSecondName()						  { return _szSecondName;              }
	void			ShowSecondName(BOOL bShow)			  { _bShowSecondName = bShow;          }
	BOOL			IsShowSecondName()					  { return _bShowSecondName;           }
	void			setPhotoID(short sID)			      { _sPhotoID = sID;				   }
	short			getPhotoID()						  { return _sPhotoID;				   }

	void			setHumanID( DWORD v )				  { _dwHumanID = v;					   }
	DWORD			getHumanID()						  { return _dwHumanID;				   }

    void            setNpcState( DWORD dwState );     // 锟斤拷锟捷憋拷识锟斤拷示锟斤拷前状态

	void			DieTime();						  // 锟斤拷锟斤拷时锟斤拷锟矫ｏ拷删锟斤拷锟斤拷锟斤拷锟斤拷效锟斤拷锟斤拷湛锟斤拷艿锟阶刺?锟斤拷锟斤拷眩锟斤拷

    void            SetIsForUI( bool v )                { _IsForUI = v;                 }
    bool            GetIsForUI()                        { return _IsForUI;              }

	void			setEvent( CEvent* pEvent )			{ _pEvent = pEvent;				}
	CEvent*			getEvent()							{ return _pEvent;				}

	void			SwitchFightPose();

	stNetChangeChaPart&		GetPart()					{ return _stChaPart;			}
	stNetChangeChaPart	_stChaPart;

	bool			IsUpdate()							{ return _bUpdate;				}

	bool			IsShop()							{ return _ChaState.IsFalse(enumChaStateNoShop);	}

	void			RefreshFog();

	CSceneItem*		GetHandItem( int nEquipPos );

	void			SetItemFace( unsigned int nIndex, int nItem );
	int				GetItemFace( unsigned int nIndex )  { return _ItemFace[nIndex];     }

	int				GetServerX()						{ return _nServerX;				}
	int				GetServerY()						{ return _nServerY;				}
	void			SetServerPos( int x, int y )		{ _nServerX=x; _nServerY=y;		}

	//add by ALLEN 2007-10-16
	bool			IsReadingBook();

public: // 锟斤拷锟斤拷锟斤拷锟斤拷状态    
	CChaStateMgr*	GetStateMgr()						{ return _pChaState;			}

    void            SynchroSkillState( stSkillState* pState, int nCount );
    void            HitEffect( int nAngle );

	void			RefreshShopShop();
	void			UnloadHandEff();

private:
	CChaStateMgr*	_pChaState;
	CBoolSet		_ChaState;			// 锟斤拷锟阶刺?锟斤拷锟斤拷

	struct stHit
	{
		stHit( int id, int dummy ) : nEffectID(id), nDummy(dummy) {}

		int		nEffectID;
		int		nDummy;
	};
	typedef vector<stHit>	hits;
	hits			_hits;

protected:
    virtual void	_UpdateYaw();
	virtual void	_UpdatePitch();
	virtual void	_UpdateRoll();
    virtual void    _UpdateHeight();
    virtual void    _UpdatePos();
	virtual void    _UpdateValid(BOOL bValid);

protected: 	
    BOOL			_bEnableAI;
	DWORD			_dwAIInterval;
	DWORD			_dwLastAITick;

	GUI::CHeadSay*	_pHeadSay;
    int             _nUIScale;		// 锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷示时, 锟斤拷锟脚诧拷锟斤拷

private:			// 锟斤拷效
	CBoolSet		_Special;		// 锟斤拷效锟斤拷锟街硷拷锟斤拷
	CBoolSet		_PK;			// PK锟斤拷锟斤拷

	// 锟斤拷转时锟斤拷时锟斤拷锟斤拷
	long			_nHelixCenterX, _nHelixCenterY;				
	int				_nHelixAngle;	// 锟斤拷锟斤拷锟角讹拷锟斤拷锟斤拷
	int				_nHelixRadii;

#ifdef _LOG_NAME_
	char			_szLogName[128];
public:
	void			setLogName( const char* str )	{ strncpy( _szLogName, str, sizeof(_szLogName) );	}

	static  bool    IsShowLogName;
#endif

private:
	float			_fMoveSpeed;
	char*			_pszFootMusic;
	char*			_pszWhoopMusic;
    float           _fMaxOpacity;       // 锟酵伙拷锟斤拷透锟斤拷锟斤拷锟斤拷锟街?,1.0f为实锟斤拷

    bool            _IsFightPose;
    bool            _InFight;           // 锟角凤拷锟斤拷锟斤拷锟斤拷战锟斤拷锟斤拷

    CSceneItem*     _pHandItem[ enumEQUIP_NUM ];
	CEffectObj*		_pHandItemEff[ enumEQUIP_NUM ];

	

    CSceneItem*     _pNpcStateItem;
	CSceneItem*		_pShopItem;

	int				_nNpcType;
    bool            _IsForUI;           // 锟斤拷锟斤拷锟缴?锟角伙拷锟斤拷UI锟较碉拷

	long			_nLeaderID;			// 锟斤拷锟斤拷锟斤拷拥锟絀D
	char			_szSecondName[41];	// 锟节讹拷锟斤拷锟斤拷锟斤拷,锟斤拷锟斤拷锟斤拷锟斤拷
	BOOL			_bShowSecondName;	// 锟角凤拷锟斤拷示锟节讹拷锟斤拷锟斤拷锟斤拷
	short			_sPhotoID;			// 头锟斤拷ID

    EChaModalType   _eChaModalType;		// 模锟斤拷锟斤拷锟斤拷
	EChaCtrlType	_eChaCtrlType;		// 锟斤拷锟斤拷锟斤拷锟斤拷
	eMainChaType	_eMainType;			// 锟斤拷锟斤拷锟斤拷锟斤拷
	int				_nDanger;			// 危锟秸筹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷选锟斤拷时锟斤拷锟斤拷危锟秸筹拷锟斤拷锟斤拷锟斤拷

	DWORD			_dwHumanID;			// 锟斤拷锟斤拷锟斤拷Group Server通锟脚碉拷ID

	char			_chGMLv;
	CEvent*			_pEvent;

	xShipInfo*		_pShipInfo;
	bool			_bUpdate;
	CEffectObj*		_pBoatFog;			// 锟斤拷锟斤拷锟斤拷锟斤拷,锟斤拷血锟叫癸拷

	CEffectObj*		_pItemFaceEff[ITEM_FACE_MAX];
	int				_ItemFace[ITEM_FACE_MAX];

private:
	bool			_IsMoveTimeType;	// 为false锟斤拷帧锟狡讹拷(锟斤拷锟斤拷锟斤拷锟斤拷),为true锟斤拷时锟斤拷锟狡讹拷
	D3DXVECTOR2		_vMoveStart, _vMoveEnd, _vMoveDir;
	DWORD 			_dwStartTime;
	float			_fMoveLen;
	
	// 锟斤拷锟斤拷诜锟斤拷锟斤拷锟揭?锟侥诧拷锟斤拷锟斤拷录-----------------------------------
	short			_sReliveTime; // 锟脚癸拷时锟斤拷锟斤拷锟斤拷录锟斤拷锟斤拷时锟斤拷
	int				_nPatrolX;	  // 巡锟竭碉拷x
	int				_nPatrolY;	  // 巡锟竭碉拷y
	// -----------------------------------------------------------

	int				_nServerX, _nServerY;

	static	bool	_IsShowShadow;
	static	bool	_ShowApparel;
	static	bool	_ShowEffects;


	// Added by clp
public:
	void linkTo( CCharacter *node, int boneID )
	{
		mParentNode = node;
		mParentBoneID = boneID;
	}
	void removeLink()
	{
		mParentNode = NULL;
	}
	
	void RemoveMount()
	{
		if (pMount)
		{
			pMount->SetValid(FALSE);
			pMount->removeLink();
			pMount = NULL;
			this->pMountID = 0;
			this->pMountBone = 0;
			this->MountHeight = 0;
			this->MountPose = 0;
			this->MountScale = 0;
			this->MountX = 0;
			this->MountY = 0;
			setHeightOff(GetDefaultChaInfo()->fHeight);
			PlayPose(POSE_WAITING);
			
		}
	}
	
	void MountFunction(int nItem);
	
	void SetBlockTargetCha(bool val) { BlockTargetCha = val; }
	bool GetBlockTargetCha() { return BlockTargetCha; }
	
protected:
	CCharacter *mParentNode;
	int mParentBoneID;
	CCharacter* pMount;
	int pMountID;
	int pMountBone;
	DWORD MountPose;
	int MountHeight; 
	float MountScale;
	int MountX;
	int MountY;
	bool BlockTargetCha;
private:
	void _computeLinkedMatrix();
};

inline void CCharacter::setNameColor(DWORD dwColor)
{
	_dwNameColor = dwColor;
}

inline void CCharacter::_UpdateYaw()
{
	SetYaw( Angle2Radian((float)_nYaw) );
    UpdateYawPitchRoll();
	_nTurnCnt = 0;

    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }

}

inline void CCharacter::_UpdatePitch()
{
	SetPitch(Angle2Radian((float)_nPitch));
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline void CCharacter::_UpdateRoll()
{
	SetRoll(Angle2Radian((float)_nRoll));
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline void CCharacter::_UpdatePos()
{
    _vPos.x = (float)_nCurX / 100.0f;
    _vPos.y = (float)_nCurY / 100.0f;
    SetPos((float*)&_vPos); // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷实锟斤拷, _vPos.z锟斤拷锟街诧拷锟斤拷
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline bool	CCharacter::IsPlayer()
{
	return _eChaCtrlType==enumCHACTRL_PLAYER;
}

inline bool CCharacter::IsBoat()
{
    return _eChaModalType==enumMODAL_BOAT; 
}

inline bool CCharacter::IsNPC()
{
    return enumCHACTRL_NPC==_eChaCtrlType;    
}

inline bool CCharacter::IsMonster()
{
    return enumCHACTRL_MONS==_eChaCtrlType;  
}

inline CSceneItem*	CCharacter::GetAttackItem()
{
	CSceneItem* item = GetLinkItem( LINK_ID_RIGHTHAND );
	if( !item ) item = GetLinkItem( LINK_ID_LEFTHAND );
	return item;
}

inline int CCharacter::DistanceFrom(CCharacter *pCha)
{
	return (int)GetDistance(GetCurX(), GetCurY(), pCha->GetCurX(), pCha->GetCurY());
}

inline bool CCharacter::IsDefaultSkill()		
{
	return _pReadySkillInfo==NULL || _pReadySkillInfo==_pDefaultSkillInfo;	
}

inline char* CCharacter::getLogName()					
{ 
#ifdef _LOG_NAME_
	return _szLogName;
#else
	return _szName;
#endif
}

inline void CCharacter::_CalPos(float fProgressRate)
{
    float fX = (float)_nLastPosX * (1.0f - fProgressRate) + (float)_nTargetX * fProgressRate;
    float fY = (float)_nLastPosY * (1.0f - fProgressRate) + (float)_nTargetY * fProgressRate;

    setPos((int)fX, (int)fY);
}

inline bool CCharacter::IsMainCha()
{
    return this==GetScene()->GetMainCha();
}

inline void CCharacter::_DetachAllItem()
{
    DetachItem( LINK_ID_LEFTHAND );
    DetachItem( LINK_ID_RIGHTHAND );
    DetachItem( 21 );
    DetachItem( 22 );
    DetachItem( 23 );
}

inline int CCharacter::ReCreate( DWORD type_id )           
{ 
    setTypeID(type_id); 
    return CCharacterModel::ReCreate( type_id);    
}

inline void CCharacter::_SetReadySkill( CSkillRecord* p )
{
	_pReadySkillInfo = p;
}

inline void CCharacter::HitEffect( int nAngle )
{
	if( !_hits.empty() )
    {
		for( hits::iterator it=_hits.begin(); it!=_hits.end(); ++it )
        {
            SelfEffect( it->nEffectID, it->nDummy, false, -1, nAngle );
        }
    }
}

inline void CCharacter::MoveTo(int nTargetX, int nTargetY)
{
    ForceMove( nTargetX, nTargetY );
    if( !_isArrive )  FaceTo(_GetTargetAngle(nTargetX, nTargetY));
}

inline void CCharacter::MoveW()
{
	ForceMove(_nCurX, _nCurY-50);
}
inline void CCharacter::MoveA()
{
	ForceMove(_nCurX-50, _nCurY);
}
inline void CCharacter::MoveS()
{
	ForceMove(_nCurX, _nCurY+50);
}
inline void CCharacter::MoveD()
{
	ForceMove(_nCurX+50, _nCurY);
}

inline void CCharacter::CheckIsFightArea()
{
    if( _pScene->GetTerrain() )
    {
		_InFight = !(_pScene->GetTerrain()->GetTile(GetCurX()/100, GetCurY()/100)->sRegion & enumAREA_TYPE_NOT_FIGHT );
	}
}

inline int CCharacter::GetPose( int pose )
{
	if( _InFight )
    {
        switch( pose )
        {
        case POSE_WAITING:
            return POSE_WAITING2;
        case POSE_RUN:
			return POSE_RUN2;
        }
    }
    else
    {
        switch( pose )
        {
        case POSE_WAITING2:
            return POSE_WAITING;
        case POSE_RUN2:
			//if(g_cFindPath.LongPathFinding())
			//	return POSE_RUN2;
			//else
			//	return POSE_RUN;
			return POSE_RUN2;
		case POSE_RUN:
			//if(g_cFindPath.LongPathFinding())
			//	return POSE_RUN2;
			//else
			//	return POSE_RUN;
			return POSE_RUN;
        }
    }
    return pose;
}

inline void	CCharacter::StopMove()              
{ 
	if( _IsMoveTimeType ) 
		_isArrive=true; 
	else 
		_isStopMove = true;   
}

inline void	CCharacter::RefreshShadow()
{
	if( _IsShowShadow && (GetDefaultChaInfo()->chTerritory!=defCHA_TERRITORY_SEA) && GetActor()->GetState()==enumNormal && !IsHide() )
	{
		SetShadeShow( SCENENODE_SHADOW, true );
	}
	else
	{
		SetShadeShow( SCENENODE_SHADOW, false );
	}
}


inline void CCharacter::StopAni()
{
    _Special.AllFalse();
	_nHeightOff = 0;
	_fMapHeight = 0.0f;
	_nPoseHeightOff = ERROR_POSE_HEIGHT;

	_nCurX = _nHelixCenterX;
	_nCurY = _nHelixCenterY;
}

inline bool CCharacter::IsResource()
{
	return _eChaCtrlType>=enumCHACTRL_MONS_TREE && _eChaCtrlType<=enumCHACTRL_MONS_DBOAT;
}

inline CSceneItem* CCharacter::GetHandItem( int nEquipPos )
{
	if( nEquipPos<0 || nEquipPos>=enumEQUIP_NUM ) return NULL;

	return _pHandItem[nEquipPos];
}

inline int CCharacter::GetTargetDistance()
{
	if( _isArrive ) return 0;
	else return GetDistance( _nCurX, _nCurY, _nTargetX, _nTargetY );
}

inline void CCharacter::setChaCtrlType( int type )          
{
	_eChaCtrlType = (EChaCtrlType)type;
	switch( type )
	{
	case enumCHACTRL_Mount:  _nDanger = 0; break;
	case enumCHACTRL_NONE: _nDanger = 0; break;
	case enumCHACTRL_PLAYER: _nDanger = 1; break;
	case enumCHACTRL_NPC: _nDanger = 2; break;

	case enumCHACTRL_NPC_EVENT: _nDanger = 7; break;
	case enumCHACTRL_MONS_TREE: _nDanger = 7; break;
	case enumCHACTRL_MONS_MINE: _nDanger = 7; break;
	case enumCHACTRL_MONS_FISH: _nDanger = 7; break;
	case enumCHACTRL_MONS_DBOAT: _nDanger = 7; break;

	case enumCHACTRL_PLAYER_PET: _nDanger = 8; break;
	case enumCHACTRL_MONS_REPAIRABLE: _nDanger = 9; break;

	case enumCHACTRL_MONS: _nDanger = 10; break;
	default: _nDanger = 0; break;
	}
}