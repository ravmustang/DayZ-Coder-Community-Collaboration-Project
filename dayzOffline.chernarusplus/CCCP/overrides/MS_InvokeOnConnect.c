/*
	DayZ SA CCCP Mod
	
	FileName: MS_InvokeOnConnect.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		identityName = identity.GetName();
		identityID = identity.GetId();
		Debug.Log("InvokeOnConnect:"+this.ToString(),"Connect");

		if( player ) player.OnConnect();

		// Send list of players at all clients
		SyncEvents.SendPlayerList();

		g_Game.GetProfileString("JoinedServer"+identityID, joinedServer);
		if( joinedServer == "false") 
		{
			msg = "DirtyS JoinFeed: " + identityName + " just logged on!";
			ServerMessagingSystem(msg);
			
			g_Game.SetProfileString("joinedServerMessageStatus"+identityID, "true");
			g_Game.SaveProfile();
		}else{
			string msg = "DirtyS JoinFeed: " + identityName + " has respawned!";
			ServerMessagingSystem(msg);
		}

		if ( m_AdminList.Contains( identityID ) ) admins.Insert(player);
		
		if( identityName == "Survivor" ) survivors.Insert(player);
	}
