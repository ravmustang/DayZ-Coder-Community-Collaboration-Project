/*
	DayZ SA CCCP Mod
	
	FileName: Init.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/
	
void main()
{
	//Load map structures-before Hive Init
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\MapStructuresCfg.c"
	
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();
	
	//Use this for gathering your loot positions from your building additions
    #include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\tools\ExportProxyData.c"
	
	//Weather Configuration
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\configs\WeatherCfg.c"
}

class CustomMission: MissionServer
{
	//PreInit mission and systems within this file
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\PreInit.c"
	
	//This loads after everything is ready above
	#include "$CurrentDir:\mpmissions\dayzOffline.chernarusplus\CCCP\overrides\MS_OnInit.c"
	
};
  
Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
/*
bool preStart = false;
int keyPressTimeout = 0;
int loopDelay = 100;
bool hacksOn = false;
ref PlayerBase dp_player;

	void MainMissionLoop()
	{
		PrintFormat("[DonkeyDebug] g_Game.GetMissionState():%1 dp_player:%2 name:%3",g_Game.GetMissionState(), dp_player, dp_player.GetIdentity().GetName());
		int keydelay = 250; //ms between keypresses, default half a second.
		//DayZGame.MISSION_STATE_GAME) 
		//while(g_Game.GetMissionState() == 1)
		//{
			//if(player) 
			//{
				if(keyPressTimeout >= keydelay) 
				{
					onKeyPress();
				}
			//}
			if(keyPressTimeout <= keydelay)
			{
				keyPressTimeout+=loopDelay;
			}
			if(hacksOn) 
			{
				//TempGodMode();
			}
		//}
	}
	
	void onKeyPress()
	{
		string PlayerUID = dp_player.GetIdentity().GetId();
		string currentMode;
		Param1<string> Msgparam;
		if(KeyState(KeyCode.KC_T)) 
		{
			vector hitPos = GetCursorPos();
			teleport(hitPos);
			keyPressTimeout = 0;
		}
		if(KeyState(KeyCode.KC_INSERT))
		{
			//toggleFreeCamera();
			keyPressTimeout = 0;
		}
		if(KeyState(KeyCode.KC_HOME))
		{
			g_Game.GetProfileString("adminGodMode"+PlayerUID, currentMode);
			if ( currentMode == "false")
			{
				g_Game.SetProfileString("adminGodMode"+PlayerUID, "true");
				Msgparam = new Param1<string>( "You have turned Godmode ON" );
			}else{
				g_Game.SetProfileString("adminGodMode"+PlayerUID, "false");
				Msgparam = new Param1<string>( "You have turned Godmode OFF" );
			}
			g_Game.SaveProfile();
			GetGame().RPCSingleParam(dp_player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, dp_player.GetIdentity());
			keyPressTimeout = 0;
		}
		if(KeyState(KeyCode.KC_R)) 
		{
			refillGun();
			keyPressTimeout = 0;
		}
	}
	
	vector GetCursorPos() 
	{
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
		vector hitPos;
		vector hitNormal;
		int hitComponentIndex;
		DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, dp_player);

		return hitPos;
	}
	
	void teleport(vector pos) 
	{
		float distance = vector.Distance( dp_player.GetPosition(), pos );
		if ( distance < 5000 ) 
		{
			EntityAI oVehicle = dp_player.GetDrivingVehicle();
			if( oVehicle ) 
			{
				dp_player.MessageStatus("Get out of the vehicle first!");
			}
			else 
			{
				dp_player.SetPosition( pos );
			}
		}
		else
		{
			dp_player.MessageStatus( "Distance for teleportation is too far!" );
		}
	}
	
	void refillGun()
	{
		//infinite ammo update
		EntityAI oWeapon = dp_player.GetHumanInventory().GetEntityInHands();
		if( oWeapon )
		{
			Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
			if( oMag && oMag.IsMagazine())
			{
				oMag.LocalSetAmmoMax();
			}
			Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
			if( oSupressor ) 
			{
				oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
			}
		}
	}
	
	//void eXpandedPlayerStats() 
	//{
	//	FileHandle file = OpenFile(playersave, FileMode.WRITE);
	//}*/