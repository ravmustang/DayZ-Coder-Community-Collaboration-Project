/**MIT License

Copyright(c) 2018 Clinton Butler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**--------------------------------------------------------------------------------------------------------------------\
| ButlerZ Custom Loadout Functions                                                                                     |
\--------------------------------------------------------------------------------------------------------------------**/

/**
\brief Adds Item(s) to Player and set to Hotbar
    \param EntityAI itemEnt
    \param ItemBase itemBs
    \param PlayerBase player
    \param string item_name
    \param int count        \p Number of Stacks
    \param int stackSize    \p 0 = max; above 0 changes the stack size
    \param int invSlot      \p Hotbar Slot # (-1 for no slot)
    @code
        PlayerBase player;
        EntityAI itemEnt;
        ItemBase itemBs;
        
        addItem(itemEnt, itemBs, player, "Pear", 2, 0, -1); // Gives player 2 Pears at full quantity and does not assign an inventory slot.
    @endcode
*/
void addItem(EntityAI itemEnt, ItemBase itemBs, PlayerBase player, string item_name, int count, int stackSize, int invSlot) {
    for (int i = 0; i < count; i++) {
        itemEnt = player.GetInventory().CreateInInventory(item_name);
        itemBs = ItemBase.Cast(itemEnt);
        if (stackSize > 0) {
            itemBs.SetQuantity(stackSize);
        }
        if (invSlot >= 0 || invSlot <= 9) {
            player.SetQuickBarEntityShortcut(itemEnt, invSlot, true);
        }
    }
}

/**
\brief Adds Clothes to Player
    \param EntityAI itemEnt
    \param ItemBase itemBs
    \param PlayerBase player
    \param TStringArray playerClothes
    @code
        PlayerBase player;
        EntityAI itemEnt;
        ItemBase itemBs;
        TStringArray playerClothes {
            "ThinFramesGlasses",
            "Armband_Blue",
            "GP5GasMask",
            "NBCHoodGray",
            "NBCGlovesGray",
            "NBCBootsGray",
            "NBCJacketGray",
            "NBCPantsGray",
            "SmershVestBackpack",
            "TortillaBag",
        };
    
        addClothes(itemEnt, itemBs, player, playerClothes);
    @endcode
*/
void addClothes(EntityAI itemEnt, ItemBase itemBs, PlayerBase player, TStringArray playerClothes) {
    for (int i = 0; i < playerClothes.Count(); i++) {
        addItem(itemEnt, itemBs, player, playerClothes.Get(i), 1, 0, -1);
    }
}

/**
\brief Sets Player's Primary Weapon
    \param PlayerBase player
    \param string oName             \p if set to "" this function does nothing
    \param string oButtstock        \p "" to send none
    \param string oHandguard        \p "" to send none
    \param string oMuzzle           \p "" to send none
    \param string oOpticName        \p "" to send none
    \param bool oBattery            \p true/false
    \param string oGhillieWrap      \p "" to send none
    \param string oMagName          \p "" to send none
    \param int oHotbarSlot          \p -1 to disable
    \param int oMagHotbarSlot       \p -1 to disable
    @code
        PlayerBase player;
        Weapon_Base primary;
        primary = primaryWeapon(player, itemBs, gun, itemEnt, optic_attach, "AKM", "AK_WoodBttstck_Camo", "AK_WoodHndgrd_Camo", "AK_Suppressor", "PSO1Optic", true, "", "Mag_AKM_30Rnd", -1, -1);
    @endcode
*/
static Weapon_Base primaryWeapon(PlayerBase oPlayer, string oName="", string oButtstock="", string oHandguard="", string oMuzzle="", string oOpticName="", bool oBattery=false, string oGhillieWrap="", int oHotbarSlot=-1) {
    if (oName != "") {
        Weapon_Base oWeapon = Weapon_Base.Cast(oPlayer.GetHumanInventory().CreateInHands(oName));
        if (oHandguard != "") { oWeapon.GetInventory().CreateAttachment(oHandguard); }
        if (oButtstock != "") { oWeapon.GetInventory().CreateAttachment(oButtstock); }
        if (oMuzzle != "") { oWeapon.GetInventory().CreateAttachment(oMuzzle); }
        if (oOpticName != "") {
            EntityAI oOptic = oWeapon.GetInventory().CreateAttachment(oOpticName);
            if (oBattery) {
                oOptic.GetInventory().CreateAttachment("Battery9V");
            }
        }
        if (oGhillieWrap != "") { oWeapon.GetInventory().CreateAttachment(oGhillieWrap); }
        if (oHotbarSlot > -1) { oPlayer.SetQuickBarEntityShortcut(oWeapon, 1, true); }
        return oWeapon;
    } else {
        return null;
    }
}

static Magazine LoadMag( PlayerBase oPlayer, Weapon_Base oWpn, string oMagName, int hotbarSlot ) {
    if (oMagName != "") {
        Magazine oMag = Magazine.Cast(oPlayer.GetInventory().CreateInInventory( oMagName ));
        oPlayer.GetWeaponManager().AttachMagazine( oMag );
        if (hotbarSlot > -1) { oPlayer.SetQuickBarEntityShortcut(oMag, hotbarSlot, true); }
        return oMag;
    } else {
        return null;
    }
}