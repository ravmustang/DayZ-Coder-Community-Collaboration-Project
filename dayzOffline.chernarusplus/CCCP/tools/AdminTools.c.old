/*
	DayZ SA CCCP Mod
	
	FileName: AdminTools.c
	
	Usage:
	FNR:Description

	Authors: DaOne
	Modified by: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/   
    //------------------------------Admin quick Commands (Chatbased)-------------------
    int TeleportAllPlayersTo(PlayerBase PlayerAdmin) {
        //Collectes Arry of players
        array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );
        vector AdminPos;
        AdminPos = PlayerAdmin.GetPosition();
        for (int i = 0; i < players.Count(); i++) {
            PlayerBase Target = players.Get(i);
            Target.SetPosition( AdminPos );
        }
        return i;
    }

    void oSpawnItemFunc(bool ground, PlayerBase player, string ClassName) {
        EntityAI item;
        ItemBase itemBs;
        vector NewPosition;
        vector OldPosition;
        if (ground) {
            OldPosition = player.GetPosition();
            NewPosition[0] = OldPosition[0] + 1.5;
            NewPosition[1] = OldPosition[1] + 0.1;
            NewPosition[2] = OldPosition[2] + 1.5;
            item = GetGame().CreateObject( ClassName, NewPosition, false, true );
        } else {
            item = player.GetInventory().CreateInInventory(ClassName);
            itemBs = ItemBase.Cast(item);    
            itemBs.SetQuantity(1);
        }
    }

    vector SnapToGround(vector pos) {
        float pos_x = pos[0];
        float pos_z = pos[2];
        float pos_y = GetGame().SurfaceY(pos_x, pos_z);
        vector tmp_pos = Vector(pos_x, pos_y, pos_z);
        tmp_pos[1] = tmp_pos[1] + pos[1];
        return tmp_pos;
    }
    
    void addMags(PlayerBase player, string mag_type, int count) {
        if (count < 1)
            return;
        EntityAI mag;
        for (int i = 0; i < count; i++) {
            mag = player.GetInventory().CreateInInventory(mag_type);
        }
        player.SetQuickBarEntityShortcut(mag, 1, true);
    }
    
    vector GetCursorPos(PlayerBase player) {
        vector rayStart = GetGame().GetCurrentCameraPosition();
        vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
        vector hitPos;
        vector hitNormal;
        int hitComponentIndex;
        DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, player);
        return hitPos;
    }
    
    void teleport(PlayerBase player, vector pos) {
        float distance = vector.Distance(player.GetPosition(), pos);
        if (distance < 5000) {
            EntityAI oVehicle = player.GetDrivingVehicle();
            if(oVehicle) {
                string msg = "Get out of the vehicle first!";
                Param1<string> Msgparam = new Param1<string>(msg);
                GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
            } else {
                player.SetPosition( pos );
            }
        } else {
            msg = "Distance for teleportation is too far!";
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
        }
    }
    
    void AdminMessaging(PlayerBase requestingAdmin, string AdminMsg) {
        Param1<string> m_MessageParam = new Param1<string>(AdminMsg);
        if(AllAdminsMsg) {
            for (int ad = 0; ad < admins.Count(); ad++) {
                Man currAdmin = admins.Get(ad);
                if( currAdmin ) GetGame().RPCSingleParam(currAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, currAdmin.GetIdentity());
            }
        } else {
            GetGame().RPCSingleParam(requestingAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, requestingAdmin.GetIdentity());
        }
    }
    
    Man FindSelectedPlayer(string pName) {
        PlayerBase selectedPlayer = NULL;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );
        for (int zm = 0; zm < players.Count(); zm++) {
            PlayerBase currentPlayer = players.Get(zm);
            PlayerIdentity selectedIdentity = currentPlayer.GetIdentity();
            if (selectedIdentity.GetName() == pName) {
                selectedPlayer = currentPlayer;
                zm=100;
            }
        }
        return selectedPlayer;
    }
	
	void TempGodMode()
	{
		for ( int i = 0; i < admins.Count(); ++i )
		{
			PlayerBase player = admins.Get(i);
			if( player.IsAlive() || player == NULL || !player )
			{
				string PlayerUID = player.GetIdentity().GetId();
				g_Game.GetProfileString("adminGodMode"+PlayerUID, godMode);
				if(debugTempGodMode) PrintFormat( "[DonkeyAHAT] - init.c TempGodMode godMode:%1 admins:%2", godMode, admins );
				if ( godMode == "true" )
				{
					player.SetHealth( player.GetMaxHealth( "", "" ) );
					player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
					player.SetHealth("","Shock", player.GetMaxHealth("","Shock") );
					
					float energy = player.GetStatEnergy().Get();
					player.GetStatEnergy().Add( energy + 1.25 );
					
					float water = player.GetStatWater().Get();
					player.GetStatWater().Add( water + 1.25 );
					
					player.GetStatStamina().Set(1000); // Not sure if working or not
					
					player.GetStatWet().Add( -1 );
					
					
					//player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
					//TREMOR_TRESHOLD = 35.5
					//player.GetStatTremor().Set( TREMOR_TRESHOLD - player.GetStatTemperature().Get() );
					//Make a player tremor, set their temperature to lower than 35.5
					
					if(debugTempGodMode) PrintFormat( "[DonkeyAHAT] - init.c TempGodMode energy:%1 water:%2", energy, water );
										
					EntityAI CurrentWeapon = player.GetHumanInventory().GetEntityInHands();
					if( CurrentWeapon )
					{
						Magazine foundMag = ( Magazine ) CurrentWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
						if( foundMag && foundMag.IsMagazine())
						{
							foundMag.ServerSetAmmoMax();
						}

						Object Suppressor = ( Object ) CurrentWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
						if( Suppressor )
						{
							Suppressor.SetHealth( Suppressor.GetMaxHealth( "", "" ) );
						}
					}
				}
			}else{
				admins.Remove(i);
			}
		}
	}
    
    override void OnEvent(EventType eventTypeId, Param params) {
        PlayerIdentity identity;
        PlayerBase player;
        ItemBase item;
        int counter = 0;
            
        switch(eventTypeId) {
            //--------------
            case ChatMessageEventTypeID:
                ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );

                // Print("Channel: "+chat_params.param1+" From: "+chat_params.param2+ " Message: "+chat_params.param3+ " Color: "+chat_params.param4);
                //PrintFormat("[DonkeyDebug] - missionServer.c ChatMessageEventTypeID chat_params:%1 - param1:%2 - param2:%3 - param3:%4 ", chat_params, chat_params.param1, chat_params.param2, chat_params.param3);
                
                array<Man> players = new array<Man>;
                GetGame().GetPlayers(players);

                string strMessage;
                string strs[10];
                string lineIn = chat_params.param3;
                lineIn.ParseString(strs);
                string commandIn = strs[0] + strs[1];
                string locationName;
                vector ofixPlayerPos;
                
                //if (strs[0] == "/") { Print("Command Sent: " + strs[0] + strs[1] + strs[2] + strs[3] + strs[4] + strs[5] + strs[6] + strs[7] + strs[8] + strs[9] + "."); } //Debugging

                PlayerBase selectedPlayer;
                PlayerIdentity selectedIdentity;

                Param1<string> Msgparam;

                for (int i = 0; i < admins.Count(); i++) {
                    PlayerBase playerAdmin = admins.Get(i);
                    if( (playerAdmin == NULL) || !playerAdmin )
                        return;
                    PlayerIdentity identityT = playerAdmin.GetIdentity();
                    string PlayerUID = identityT.GetId();
                    string playerAdminName = playerAdmin.GetIdentity().GetName();
                    // Check player name to sent chat name
                    if (playerAdminName == chat_params.param2 && m_AdminList.Contains(PlayerUID)) {
						if (strs[0] != "" && strs[1] != "") {
                            switch (commandIn) {
                                // -------------------------------------------------------------------------------------------------------------------------- START OF COMMANDS =================
                                // strs[0] + strs[1] = commandIn    (command sent by user)
                                // strs[2]                          (parameter 1 for commands)
                                // strs[3]                          (parameter 2 for commands)
                                // strs[4]                          (parameter 3 for commands)
                                // strs[5]                          (parameter 4 for commands)
                                // strs[6]                          (parameter 5 for commands)
                                // strs[7]                          (parameter 6 for commands)
                                // strs[8]                          (parameter 7 for commands)
                                // strs[9]                          (parameter 8 for commands)
                                // --------------------------------------------------------------------------------------------------------------------------
								
					
					//TELEPORT TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /tppm
                                case "/tppm":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            playerAdmin.SetPosition(selectedPlayer.GetPosition());
                                            strMessage = "Admin:" + playerAdminName + " was teleported to " + strs[2] + ".";
                                        } else {
                                            strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpto
                                case "/tpto":
                                    if (strs[2] != "") {
                                        locationName = strs[2];
                                        if (strs[3] != "") { locationName += " " + strs[3]; }
                                        if (strs[4] != "") { locationName += " " + strs[4]; }
                                        if (strs[5] != "") { locationName += " " + strs[5]; }
                                        if (strs[6] != "") { locationName += " " + strs[6]; }
                                        if (strs[7] != "") { locationName += " " + strs[7]; }
                                        if (strs[8] != "") { locationName += " " + strs[8]; }
                                        if (strs[9] != "") { locationName += " " + strs[9]; }
                                        Print(locationName);
                                        vector position = "0 0 0";
                                    
                                        if (m_TPLocations.Contains(locationName)) {
                                            m_TPLocations.Find(locationName, position);
                                            ofixPlayerPos[0] = position[0];
                                            ofixPlayerPos[2] = position[2];
                                            ofixPlayerPos = SnapToGround(ofixPlayerPos);
                                            playerAdmin.SetPosition(ofixPlayerPos);
                                            strMessage = "Teleported To Location: " + locationName + ".";
                                        } else {                                
                                            strMessage = "Teleport Failed! Location: " + locationName + " is not on the list!";
                                        }
                                    } else {
                                        strMessage = "Teleport Failed! Please enter a location first!";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpc
                                case "/tpc":
                                    if (strs[2] != "" && strs[3] != "") {
                                        vector tpPos;
                                        tpPos[0] = strs[2].ToFloat();
                                        tpPos[1] = 0;
                                        tpPos[2] = strs[3].ToFloat();
                                        tpPos = SnapToGround(tpPos);
                                        playerAdmin.SetPosition(tpPos);
                                        strMessage = "Teleported to: " + tpPos;
                                    } else {
                                        strMessage = "Teleport Failed. Make sure to provide 3 coordinates (middle doesn't matter).";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpp
                                case "/tpp":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            selectedPlayer.SetPosition(playerAdmin.GetPosition());
                                            strMessage = "Player " + strs[2] + " was teleported to Admin: " + playerAdminName + "'s location!";
                                        } else {
                                            strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tppto
                                case "/tppto":
                                    if (strs[2] != "") {
                                        locationName = strs[2];
                                        if (strs[3] != "") { locationName += " " + strs[3]; }
                                        if (strs[4] != "") { locationName += " " + strs[4]; }
                                        if (strs[5] != "") { locationName += " " + strs[5]; }
                                        if (strs[6] != "") { locationName += " " + strs[6]; }
                                        if (strs[7] != "") { locationName += " " + strs[7]; }
                                        if (strs[8] != "") { locationName += " " + strs[8]; }
                                        if (strs[9] != "") { locationName += " " + strs[9]; }
                                        Print(locationName);
                                        if (locationName != "") {
                                            selectedPlayer = FindSelectedPlayer(strs[2]);
                                            if (selectedPlayer != NULL) {
                                                if (m_TPLocations.Contains(locationName)) {
                                                    m_TPLocations.Find(locationName, position);
                                                    ofixPlayerPos[0] = position[0];
                                                    ofixPlayerPos[2] = position[2];
                                                    ofixPlayerPos = SnapToGround(ofixPlayerPos);
                                                    selectedPlayer.SetPosition(ofixPlayerPos);
                                                    strMessage = strs[2] + " has been teleported to: " + locationName + ".";
                                                } else {                                
                                                    strMessage = "Teleport Failed! Location: " + locationName + " is not on the list!";
                                                }
                                            } else {
                                                strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                            }
                                        } else {
                                            strMessage = "Please type a Location first.";
                                        }
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                    //HELPER TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /spi
                                case "/spi":
                                    if (strs[2] != "") {    
                                        oSpawnItemFunc(false, playerAdmin, strs[2]);
                                        strMessage = "Admin: " + playerAdminName + " spawned Item: " + strs[2] + " in their Inventory!";
                                    } else {
                                        strMessage = "Please type an item name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /spg
                                case "/spg":
                                    if (strs[2] != "") {
                                        oSpawnItemFunc(true,playerAdmin,strs[2]);
                                        strMessage = "Admin: " + playerAdminName + " spawned Item: " + strs[2] + " on the ground!";
                                    } else {
                                        strMessage = "Please type an item name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /heal
                                case "/heal":
                                    Msgparam = new Param1<string>("Player Healed!");
                                    GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    for (int bsRemoveLoop = 0; bsRemoveLoop < playerAdmin.GetBleedingManager().m_BleedingSources.Count(); bsRemoveLoop++) {
                                        player.GetBleedingManager().RemoveSingleBleedingSource();
                                    }
                                    playerAdmin.SetHealth(playerAdmin.GetMaxHealth("", ""));
                                    playerAdmin.SetHealth("", "Blood", playerAdmin.GetMaxHealth("", "Blood"));
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /healp
                                case "/healp":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            for (int pbsRemoveLoop = 0; pbsRemoveLoop < selectedPlayer.GetBleedingManager().m_BleedingSources.Count(); pbsRemoveLoop++) {
                                                selectedPlayer.GetBleedingManager().RemoveSingleBleedingSource();
                                            }
                                            selectedPlayer.SetHealth(selectedPlayer.GetMaxHealth("", ""));
                                            selectedPlayer.SetHealth("", "Blood", selectedPlayer.GetMaxHealth("", "Blood"));
                                            strMessage = "Player " + strs[2] + " was teleported to Admin: " + playerAdminName + "'s location!";
                                        } else {
                                            strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /healall
                                case "/healall":
                                    for (int ha = 0; ha < players.Count(); ha++) {
                                        PlayerBase Patient = players.Get(ha);
                                            for (int absRemoveLoop = 0; absRemoveLoop < Patient.GetBleedingManager().m_BleedingSources.Count(); absRemoveLoop++) {
                                                Patient.GetBleedingManager().RemoveSingleBleedingSource();
                                            }
                                        Patient.SetHealth(playerAdmin.GetMaxHealth("", ""));
                                        Patient.SetHealth("","Blood", playerAdmin.GetMaxHealth("", "Blood"));
                                    }
                                    strMessage = "All Player's Healed by Admin: " + playerAdminName;
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                    //PUNISHMENT TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /killp
                                case "/killp":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            selectedPlayer.SetHealth(0);
                                            strMessage = "Player: " + strs[2] + " was Killed by Admin: " + playerAdminName + ".";
                                        } else {
                                            strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /strip
                                case "/strip":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            selectedPlayer.RemoveAllItems();
                                            strMessage = "Player:" + strs[2] + " was striped down by Admin: " + playerAdminName + ".";
                                        } else {
                                            strMessage = "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /salty
                                case "/salty":
                                    if (strs[2] != "") {
                                        selectedPlayer = FindSelectedPlayer(strs[2]);
                                        if (selectedPlayer != NULL) {
                                            item = GetGame().CreateObject("M67Grenade", selectedPlayer.GetPosition(), false, true);
                                            item.Explode();
                                            strMessage = "Player:" + strs[2] + " was salted(Grenade) by Admin: " + playerAdminName + ".";
                                        } else {
                                            strMessage =  "Could not find player " + strs[2] + ". Please check the name and try again.";
                                        }
                                    } else {
                                        strMessage = "Please type a player name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /kill
                                case "/kill":
                                    playerAdmin.SetHealth(0);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /killall
                                case "/killall":
                                    for (int ig = 0; ig < players.Count(); ig++) {
                                        PlayerBase Target = players.Get(ig);
                                        if ((Target.GetIdentity()) == identityT) {
                                        } else {
                                            Target = players.Get(ig);
                                            Target.SetHealth(0);
                                        }
                                    }
                                    strMessage = "All Player's Killed by Admin: " + playerAdminName + ".";
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                    //DEVELOPER TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /cmd
                                case "/cmd":
                                    if (chat_params.param3.Contains("/execEnf")) {
                                        int sLength = chat_params.param3.Length();
                                        strCodeline = chat_params.param3.Substring(13, sLength);
                                        GetGame().ExecuteEnforceScript("void scConsMain() \n{\n" + strCodeline + "\n}\n", "scConsMain");
                                        Print("strCodeline: " + strCodeline);
                                        strMessage = "Command executed!";
                                    } else {
                                        strMessage = "Please learn how to use this correctly first!";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /build
                                case "/build":
                                    if (strs[2] != "") {
                                        vector admPOS = playerAdmin.GetPosition();
                                        vector buildPOS; 
                                        buildPOS[0] = admPOS[0] + 5;
                                        buildPOS[2] = admPOS[2] + 5;
                                        Object Buildable = GetGame().CreateObject(strs[2], buildPOS, false, false, true);
                                        //AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
                                        //Buildable.AttachTo( playerAdmin, "0, 5, 0", "0 0 1");
                                        strMessage = "Object " + strs[2] + " created successfully!";
                                    } else {
                                        strMessage = "Please type an Object name first.";
                                    }
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /list
                                case "/list":
                                    for (int list = 0; list < players.Count(); list++) {
                                        selectedPlayer = players.Get(list);
                                        selectedIdentity = selectedPlayer.GetIdentity();
                                        Msgparam = new Param1<string>("Player Name: " + selectedIdentity.GetName());
                                        GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    }
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpalltome
                                case "/tpalltome":
                                    int tpCount = TeleportAllPlayersTo(playerAdmin);
                                    strMessage = "All " + tpCount.ToString() + " Players Teleported to Admin: " + playerAdminName + "'s POS!";
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                        //ADMIN TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /godmode
                                case "/godmode":
                                    string currentMode;
                                    g_Game.GetProfileString("adminGodMode" + PlayerUID, currentMode);
                                    if (currentMode == "false") {
                                        g_Game.SetProfileString("adminGodMode" + PlayerUID, "true");
                                        Msgparam = new Param1<string>("You have turned Godmode ON");
                                    } else {
                                        g_Game.SetProfileString("adminGodMode" + PlayerUID, "false");
                                        Msgparam = new Param1<string>("You have turned Godmode OFF");
                                    }
                                    g_Game.SaveProfile();
                                    GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /freecam
                                case "/freecam":
                                    PlayerBase pBody = playerAdmin;
                                    if (m_FreeCamera) {
                                        GetGame().SelectPlayer(playerAdmin.GetIdentity(), pBody);
                                        m_FreeCamera = false;
                                        strMessage = "Admin: " + playerAdminName + " is Exiting FreeCam!";
                                        AdminMessaging(playerAdmin, strMessage);
                                    } else {
                                        GetGame().SelectPlayer(playerAdmin.GetIdentity(), NULL);
                                        GetGame().SelectSpectator(playerAdmin.GetIdentity(), "freedebugcamera", playerAdmin.GetPosition()); //staticcamera, freedebugcamera, dayzspectator
                                        m_FreeCamera = true;
                                        strMessage = "Admin: " + playerAdminName + " is Entering FreeCam!";
                                        AdminMessaging(playerAdmin, strMessage);
                                    }
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /remove
                                case "/remove":
                                    EntityAI CurrentInHands = playerAdmin.GetHumanInventory().GetEntityInHands();
                                    string displayName = CurrentInHands.ConfigGetString("displayName");
                                    GetGame().ObjectDelete(CurrentInHands);
                                    playerAdmin.OnItemInHandsChanged();
                                    strMessage = "Admin: " + playerAdminName + " have removed " + displayName + ".";
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /nighttime
                                case "/nighttime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 22, 0);
                                    Msgparam = new Param1<string>("NIGHT TIME!!");
                                    GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /daytime
                                case "/daytime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 12, 0);
                                    Msgparam = new Param1<string>("DAY TIME!!");
                                    GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /dawntime
                                case "/dawntime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 6, 0);
                                    Msgparam = new Param1<string>("Time Changed to 6 am");
                                    GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /gm
                                case "/gm":
                                    playerAdmin.RPCSingleParam(ERPCs.DEV_RPC_TOGGLE_GOD_MODE, NULL, true);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /debug
                                case "/debug":
                                    if (m_IsDebugRunning) {
                                        strMessage = "Admin: " + playerAdminName + " DeBug Monitor (Status Monitor) Disabled!";
                                        AdminMessaging(playerAdmin, strMessage);
                                        GetGame().SetDebugMonitorEnabled(0);
                                        m_IsDebugRunning = false;
                                    } else {
                                        strMessage = "Admin: " + playerAdminName + " DeBug Monitor (Status Monitor) Enabled!";
                                        AdminMessaging(playerAdmin, strMessage);
                                        GetGame().SetDebugMonitorEnabled(1);
                                        m_IsDebugRunning = true;
                                    }
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /spawncar
                                case "/spawncar":
                                    EntityAI MyV3S;
                                    vector NewPosition;
                                    vector OldPosition;
                                    OldPosition = playerAdmin.GetPosition();
                                    NewPosition[0] = OldPosition[0] + 1.5;
                                    NewPosition[1] = OldPosition[1] + 0.2;
                                    NewPosition[2] = OldPosition[2] + 1.5;
                                    MyV3S = GetGame().CreateObject("OffroadHatchback", NewPosition, false, true, true );
                                    MyV3S.GetInventory().CreateAttachment("HatchbackHood");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackTrunk");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackDoors_CoDriver");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("SparkPlug");
                                    MyV3S.GetInventory().CreateAttachment("EngineBelt");
                                    MyV3S.GetInventory().CreateAttachment("CarBattery");
                                    strMessage = "Admin: " + playerAdminName + " has spawned in a vehicle";
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- /bandit
                                case "/bandit":
                                    playerAdmin.RemoveAllItems();
                                    playerAdmin.GetInventory().CreateInInventory("BallisticHelmet_Black");
                                    playerAdmin.GetInventory().CreateInInventory("BagMountain_Green");
                                    playerAdmin.GetInventory().CreateInInventory("CombatBoots_Brown");
                                    playerAdmin.GetInventory().CreateInInventory("USMCPants_Woodland");
                                    playerAdmin.GetInventory().CreateInInventory("PlateCarrierComplete2");
                                    playerAdmin.GetInventory().CreateInInventory("SmershVestBackpack");
                                    playerAdmin.GetInventory().CreateInInventory("TortillaBag");
                                    playerAdmin.GetInventory().CreateInInventory("TacticalGloves_Green");
                                    playerAdmin.GetInventory().CreateInInventory("TacticalShirt_Black");
                                    playerAdmin.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
                                    playerAdmin.GetInventory().CreateInInventory("SpaghettiCan_Opened");
                                    playerAdmin.GetInventory().CreateInInventory("SardinesCan_Opened");
                                    playerAdmin.GetInventory().CreateInInventory("TunaCan_Opened");
                                    playerAdmin.GetInventory().CreateInInventory("PersonalRadio");
                                    EntityAI rifles = playerAdmin.GetHumanInventory().CreateInInventory("AKM");
                                    rifles.GetInventory().CreateAttachment("AK_RailHndgrd_Black");
                                    rifles.GetInventory().CreateAttachment("KobraOptic");
                                    rifles.GetInventory().CreateAttachment("AK_PlasticBttstck_Green");
                                    rifles.GetInventory().CreateAttachment("AK_Suppressor");
                                    addMags(playerAdmin, "Mag_AKM_Drum75Rnd_Black", 3);
                                    playerAdmin.LocalTakeEntityToHands(rifles);
                                    EntityAI entity_hands1 = playerAdmin.GetHumanInventory().GetEntityInHands();
                                    playerAdmin.SetQuickBarEntityShortcut(rifles, 0, true);
                                    strMessage = "Admin: " + playerAdminName + " has spawned the bandit loadout";
                                    AdminMessaging(playerAdmin, strMessage);
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- !Invalid Command!
                                default:
                                    if (strs[0] == "/") {
                                        //Invalid Command
                                        strMessage = "Invalid Command: " + commandIn + ".";
                                        AdminMessaging(playerAdmin, strMessage);
                                    }
                                    break;
                                // -------------------------------------------------------------------------------------------------------------------------- END OF COMMANDS =================
                            }
                        }
                    }
                }
                break;
            //-------------- Vanilla----------
            case PreloadEventTypeID:
                PreloadEventParams preloadParams;
                Class.CastTo(preloadParams, params);
                OnPreloadEvent(preloadParams.param1, preloadParams.param2, preloadParams.param3, preloadParams.param4, preloadParams.param5);
                break;
            case ClientNewEventTypeID:
                ClientNewEventParams newParams;
                Class.CastTo(newParams, params);
                player = OnClientNewEvent(newParams.param1, newParams.param2, newParams.param3);
                if (!player) {
                    Debug.Log("ClientNewEvent: Player is empty");
                    return;
                }
                identity = newParams.param1;
                InvokeOnConnect(player,identity );
                break;
            case ClientReadyEventTypeID:
                ClientReadyEventParams readyParams;
                Class.CastTo(readyParams, params);
                identity = readyParams.param1;
                Class.CastTo(player, readyParams.param2);
                if (!player) {
                    Debug.Log("ClientReadyEvent: Player is empty");
                    return;
                }
                OnClientReadyEvent(identity, player);
                InvokeOnConnect(player, identity);
                break;
            case ClientRespawnEventTypeID:
                ClientRespawnEventParams respawnParams;
                Class.CastTo(respawnParams, params);
                identity = respawnParams.param1;
                Class.CastTo(player, respawnParams.param2);
                if (!player) {
                    Debug.Log("ClientRespawnEvent: Player is empty");
                    return;
                }
                OnClientRespawnEvent(identity, player);
                break;
            case ClientDisconnectedEventTypeID:
                ClientDisconnectedEventParams discoParams;
                Class.CastTo(discoParams, params);
                identity = discoParams.param1;
                Class.CastTo(player, discoParams.param2);
                int discTime = discoParams.param3;
                bool authFailed = discoParams.param4;
                if (!player) {
                    Debug.Log("ClientDisconnectenEvent: Player is empty");
                    return;
                }
                InvokeOnDisconnect(player);
                OnClientDisconnectedEvent(identity, player, discTime, authFailed);
                break;
            case LogoutCancelEventTypeID:
                LogoutCancelEventParams logoutCancelParams;
                Class.CastTo(logoutCancelParams, params);
                Class.CastTo(player, logoutCancelParams.param1);
                identity = player.GetIdentity();
                if (identity) {
                    Print("[Logout]: Player " + identity.GetId() + " cancelled");
                } else {
                    Print("[Logout]: Player cancelled"); 
                }
                m_LogoutPlayers.Remove(player);
                break;
        }
    }