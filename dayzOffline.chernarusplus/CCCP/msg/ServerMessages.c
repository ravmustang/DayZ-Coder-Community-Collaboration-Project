/*
	DayZ SA CCCP Mod
	
	FileName: ServerMessages.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/

	void ServerMessagingSystem(string ServerMsg)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		for ( int i = 0; i < players.Count(); ++i )
		{
			Man player = players.Get(i);
			if( player )
			{
				Param1<string> m_MessageParam = new Param1<string>(ServerMsg);
				GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
			}
		}
		if(debugServerMsgSys) PrintFormat("[DonkeyDebug] init.c ServerMessagingSystem ServerMsg:%1 players.Count:%2 players:%", ServerMsg, players.Count(), players);
	}
	
	void CustomServerMessages()
	{
		string messages[6] = {"Welcome to DonkeyPunch Community Gaming.", "Discord at https://discord.me/eXpoch", "Go there for a Free Customized Loadout(limited time)", "If you need help join the discord and chime an admin","",""};
		for ( int m = 0; m < 6; ++m )
		{
			ServerMessagingSystem(messages[m]);
			if(debugCustomServerMsgs) PrintFormat("[DonkeyDebug] init.c CustomServerMessages msg:%1", messages[m]);
		}
		
	}
	
	void NumPlayersOnServer()
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		msg = "Players on the server: " + players.Count().ToString();
		ServerMessagingSystem(msg);
		if(debugNumPlyrMsg) PrintFormat("[DonkeyDebug] init.c NumPlayersOnServer count:%1 msg:%2", players.Count(), msg);
	}
	
	void SurvivorNameChange()
	{
		for ( int s = 0; s < survivors.Count(); ++s )
		{
			Man player = survivors.Get(s);
			if((player)&&(player.IsAlive()))
			{
				msg = "Please change your name by adding -name=YourGamerName in your steam dayz sa startup parameters";
				Param1<string> m_MessageParam = new Param1<string>(msg);
				GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
			}
			else
			{
				survivors.Remove(s);
			}
			if(debugNameWarningMsg) PrintFormat("[DonkeyDebug] init.c SurvivorNameChange survivors:%1", survivors);
		}
	}
	
	//This is optional annoyance for players with Survivor Name
	//This will remove whatever is in their hands every (default:2 second check)
	//rename SurvivorNameHassleProtection
	void SurvivorNameGoFuckYourself()
	{
		for ( int s = 0; s < survivors.Count(); ++s )
		{
			//Man douche = survivors.Get(s);
			Man douche = survivors.Get(s);
			EntityAI CurrentInHands = douche.GetHumanInventory().GetEntityInHands();
			if(CurrentInHands)
			{
				string displayName = CurrentInHands.ConfigGetString("displayName");
				GetGame().ObjectDelete(CurrentInHands);
				PrintFormat( "[DonkeyAdminLog] - init.c %1 had his %2 removed from his hands", douche, displayName );
				msg = "Don't you want to have fun? Change your name and you will stop losing your"+displayName;
				Param1<string> m_MessageParam = new Param1<string>(msg);
				GetGame().RPCSingleParam(douche, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, douche.GetIdentity());
			}
		}
	}
