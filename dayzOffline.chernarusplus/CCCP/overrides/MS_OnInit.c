/*
	DayZ SA CCCP Mod
	
	FileName: MS_OnInit.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/
	override void OnInit()
	{		
		AdminTool();
        Print("!!calling AdminTool extentions!!");
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(NumPlayersOnServer, 60 * 1000, true);  // 60 seconds
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CustomServerMessages, 1800 * 1000, true);  // 30 Min = 1800
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CustomServerMessages, 150 * 1000, false);	// 2.5 mins dont repeat
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SurvivorNameChange, 30 * 1000, true);  // 30 seconds
		//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SurvivorNameGoFuckYourself, 2 * 1000, true);  // 2 seconds
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TempGodMode, 1 * 1000, true);  // 1 seconds
	}