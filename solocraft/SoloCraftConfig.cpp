#include "solocraft/SoloCraftConfig.h"

#include "SystemConfig.h"
#include "Globals/SharedDefines.h"
#include "Log/Log.h"

#include<stdio.h>

INSTANTIATE_SINGLETON_1(SolocraftConfig);

SolocraftConfig::SolocraftConfig():enabled(false) { }

bool SolocraftConfig::Initialize()
{
    sLog.outString("Initializing Solocraft.");
    sLog.outString(SYSCONFDIR);
    if (!config.SetSource(SYSCONFDIR"SoloCraft.conf", "SoloCraft_"))
    {
#ifdef _SOLOCRAFT_CONFIG
        char buff[255];
        snprintf(buff, sizeof(buff), "%s", _SOLOCRAFT_CONFIG);
        sLog.outString(buff);
        if (!config.SetSource(_SOLOCRAFT_CONFIG, "SoloCraft_"))
        {
            sLog.outString("Solocraft is Disabled. Unable to open configuration file solocreaft.conf");
            return false;
        }
#else
        sLog.outString("Solocraft is Disabled. Unable to open configuration file solocreaft.conf");
        return false;
#endif
    }

    enabled = config.GetBoolDefault("Solocraft.Enabled", false);
    if (!enabled)
    {
        sLog.outString("Solocraft is Disabled in SoloCraft.conf");
        return false;
    }

    SoloCraftAnnounceModule = config.GetBoolDefault("Solocraft.Announce", true);

    //Balancing
    SoloCraftSpellMult = config.GetFloatDefault("SoloCraft.Spellpower.Mult", 2.5);
    SoloCraftStatsMult = config.GetFloatDefault("SoloCraft.Stats.Mult", 100.0);

    classes =
    {
        {CLASS_WARRIOR, config.GetIntDefault("SoloCraft.WARRIOR", 100) },
        {CLASS_PALADIN, config.GetIntDefault("SoloCraft.PALADIN", 100) },
        {CLASS_HUNTER, config.GetIntDefault("SoloCraft.HUNTER", 100) },
        {CLASS_ROGUE, config.GetIntDefault("SoloCraft.ROGUE", 100) },
        {CLASS_PRIEST, config.GetIntDefault("SoloCraft.PRIEST", 100) },
        {CLASS_SHAMAN, config.GetIntDefault("SoloCraft.SHAMAN", 100) },
        {CLASS_MAGE, config.GetIntDefault("SoloCraft.MAGE", 100) },
        {CLASS_WARLOCK, config.GetIntDefault("SoloCraft.WARLOCK", 100) },
        {CLASS_DRUID, config.GetIntDefault("SoloCraft.DRUID", 100) },
#ifdef SOLOCRAFT_WOTLK
        {CLASS_DEATH_KNIGHT, config.GetIntDefault("SoloCraft.DEATH_KNIGHT", 100) },
#endif
    };

    //Level Thresholds
    SolocraftLevelDiff = config.GetIntDefault("Solocraft.Max.Level.Diff", 10);

    //Catch All Dungeon Level Threshold
    SolocraftDungeonLevel = config.GetIntDefault("Solocraft.Dungeon.Level", 80);

    // Dungeon Base Level
    dungeons =
    {
        // Wow Classic
        {33, config.GetIntDefault("Solocraft.ShadowfangKeep.Level", 15) },
        {34, config.GetIntDefault("Solocraft.Stockades.Level", 22) },
        {36, config.GetIntDefault("Solocraft.Deadmines.Level", 18) },
        {43, config.GetIntDefault("Solocraft.WailingCaverns.Level", 17) },
        {47, config.GetIntDefault("Solocraft.RazorfenKraulInstance.Level", 30) },
        {48, config.GetIntDefault("Solocraft.Blackfathom.Level", 20) },
        {70, config.GetIntDefault("Solocraft.Uldaman.Level", 40) },
        {90, config.GetIntDefault("Solocraft.GnomeragonInstance.Level", 24) },
        {109, config.GetIntDefault("Solocraft.SunkenTemple.Level", 50) },
        {129, config.GetIntDefault("Solocraft.RazorfenDowns.Level", 40) },
        {189, config.GetIntDefault("Solocraft.MonasteryInstances.Level", 35) },                  // Scarlet Monastery
        {209, config.GetIntDefault("Solocraft.TanarisInstance.Level", 44) },                     // Zul'Farrak
        {229, config.GetIntDefault("Solocraft.BlackRockSpire.Level", 55) },
        {230, config.GetIntDefault("Solocraft.BlackrockDepths.Level", 50) },
        {249, config.GetIntDefault("Solocraft.OnyxiaLairInstance.Level", 60) },
        {289, config.GetIntDefault("Solocraft.SchoolofNecromancy.Level", 55) },                  // Scholomance
        {309, config.GetIntDefault("Solocraft.Zul'gurub.Level", 60) },
        {329, config.GetIntDefault("Solocraft.Stratholme.Level", 55) },
        {349, config.GetIntDefault("Solocraft.Mauradon.Level", 48) },
        {389, config.GetIntDefault("Solocraft.OrgrimmarInstance.Level", 15) },                   // Ragefire Chasm
        {409, config.GetIntDefault("Solocraft.MoltenCore.Level", 60) },
        {429, config.GetIntDefault("Solocraft.DireMaul.Level", 48) },
        {469, config.GetIntDefault("Solocraft.BlackwingLair.Level", 40) },
        {509, config.GetIntDefault("Solocraft.AhnQiraj.Level", 60) },                            // Ruins of Ahn'Qiraj
        {531, config.GetIntDefault("Solocraft.AhnQirajTemple.Level", 60) },

#ifdef SOLOCRAFT_TBC
        // BC Instances
        {269, config.GetIntDefault("Solocraft.CavernsOfTime.Level", 68) },                       // The Black Morass
        {532, config.GetIntDefault("Solocraft.Karazahn.Level", 68) },
        {534, config.GetIntDefault("Solocraft.HyjalPast.Level", 70) },                           // The Battle for Mount Hyjal - Hyjal Summit
        {540, config.GetIntDefault("Solocraft.HellfireMilitary.Level", 68) },                    // The Shattered Halls
        {542, config.GetIntDefault("Solocraft.HellfireDemon.Level", 68) },                       // The Blood Furnace
        {543, config.GetIntDefault("Solocraft.HellfireRampart.Level", 68) },
        {544, config.GetIntDefault("Solocraft.HellfireRaid.Level", 68) },                        // Magtheridon's Lair
        {545, config.GetIntDefault("Solocraft.CoilfangPumping.Level", 68) },                     // The Steamvault
        {546, config.GetIntDefault("Solocraft.CoilfangMarsh.Level", 68) },                       // The Underbog
        {547, config.GetIntDefault("Solocraft.CoilfangDraenei.Level", 68) },                     // The Slavepens
        {548, config.GetIntDefault("Solocraft.CoilfangRaid.Level", 70) },                        // Serpentshrine Cavern
        {550, config.GetIntDefault("Solocraft.TempestKeepRaid.Level", 70) },                     // The Eye
        {552, config.GetIntDefault("Solocraft.TempestKeepArcane.Level", 68) },                   // The Arcatraz
        {553, config.GetIntDefault("Solocraft.TempestKeepAtrium.Level", 68) },                   // The Botanica
        {554, config.GetIntDefault("Solocraft.TempestKeepFactory.Level", 68) },                  // The Mechanar
        {555, config.GetIntDefault("Solocraft.AuchindounShadow.Level", 68) },                    // Shadow Labyrinth
        {556, config.GetIntDefault("Solocraft.AuchindounDemon.Level", 68) },                     // Sethekk Halls
        {557, config.GetIntDefault("Solocraft.AuchindounEthereal.Level", 68) },                  // Mana-Tombs
        {558, config.GetIntDefault("Solocraft.AuchindounDraenei.Level", 68) },                   // Auchenai Crypts
        {560, config.GetIntDefault("Solocraft.HillsbradPast.Level", 68) },                       // Old Hillsbrad Foothills
        {564, config.GetIntDefault("Solocraft.BlackTemple.Level", 70) },
        {565, config.GetIntDefault("Solocraft.GruulsLair.Level", 70) },
        {568, config.GetIntDefault("Solocraft.ZulAman.Level", 68) },
        {580, config.GetIntDefault("Solocraft.SunwellPlateau.Level", 70) },
        {585, config.GetIntDefault("Solocraft.Sunwell5ManFix.Level", 68) },                      // Magister's Terrace
#endif

#ifdef SOLOCRAFT_WOTLK
        // WOTLK Instances
        {533, config.GetIntDefault("Solocraft.StratholmeRaid.Level", 78) },                      // Naxxramas
        {574, config.GetIntDefault("Solocraft.Valgarde70.Level", 78) },                          // Utgarde Keep
        {575, config.GetIntDefault("Solocraft.UtgardePinnacle.Level", 78) },
        {576, config.GetIntDefault("Solocraft.Nexus70.Level", 78) },                             // The Nexus
        {578, config.GetIntDefault("Solocraft.Nexus80.Level", 78) },                             // The Occulus
        {595, config.GetIntDefault("Solocraft.StratholmeCOT.Level", 78) },                       // The Culling of Stratholme
        {599, config.GetIntDefault("Solocraft.Ulduar70.Level", 78) },                            // Halls of Stone
        {600, config.GetIntDefault("Solocraft.DrakTheronKeep.Level", 78) },                      // Drak'Tharon Keep
        {601, config.GetIntDefault("Solocraft.Azjol_Uppercity.Level", 78) },                     // Azjol-Nerub
        {602, config.GetIntDefault("Solocraft.Ulduar80.Level", 78) },                            // Halls of Lighting
        {603, config.GetIntDefault("Solocraft.UlduarRaid.Level", 80) },                          // Ulduar
        {604, config.GetIntDefault("Solocraft.GunDrak.Level", 78) },
        {608, config.GetIntDefault("Solocraft.DalaranPrison.Level", 78) },                       // Violet Hold
        {615, config.GetIntDefault("Solocraft.ChamberOfAspectsBlack.Level", 80) },               // The Obsidian Sanctum
        {616, config.GetIntDefault("Solocraft.NexusRaid.Level", 80) },                           // The Eye of Eternity
        {619, config.GetIntDefault("Solocraft.Azjol_LowerCity.Level", 78) },                     // Ahn'kahet: The Old Kingdom
        {631, config.GetIntDefault("Solocraft.IcecrownCitadel.Level", 80) },                     // Icecrown Citadel
        {632, config.GetIntDefault("Solocraft.IcecrownCitadel5Man.Level", 78) },                 // The Forge of Souls
        {649, config.GetIntDefault("Solocraft.ArgentTournamentRaid.Level", 80) },                // Trial of the Crusader
        {650, config.GetIntDefault("Solocraft.ArgentTournamentDungeon.Level", 80) },             // Trial of the Champion
        {658, config.GetIntDefault("Solocraft.QuarryOfTears.Level", 78) },                       // Pit of Saron
        {668, config.GetIntDefault("Solocraft.HallsOfReflection.Level", 78) },                   // Halls of Reflection
        {724, config.GetIntDefault("Solocraft.ChamberOfAspectsRed.Level", 80) },                 // The Ruby Sanctum
#endif
    };

    // Dungeon Difficulty | Catch alls
    D5 = config.GetFloatDefault("Solocraft.Dungeon", 5.0);
    D10 = config.GetFloatDefault("Solocraft.Heroic", 10.0);
    D25 = config.GetFloatDefault("Solocraft.Raid25", 25.0);
    D40 = config.GetFloatDefault("Solocraft.Raid40", 40.0);

    diff_Multiplier =
    {
        // WOW Classic Instances
        {33, config.GetFloatDefault("Solocraft.ShadowfangKeep", 5.0) },
        {34, config.GetFloatDefault("Solocraft.Stockades", 5.0) },
        {36, config.GetFloatDefault("Solocraft.Deadmines", 5.0) },
        {43, config.GetFloatDefault("Solocraft.WailingCaverns", 5.0) },
        {47, config.GetFloatDefault("Solocraft.RazorfenKraulInstance", 5.0) },
        {48, config.GetFloatDefault("Solocraft.Blackfathom", 5.0) },
        {70, config.GetFloatDefault("Solocraft.Uldaman", 5.0) },
        {90, config.GetFloatDefault("Solocraft.GnomeragonInstance", 5.0) },
        {109, config.GetFloatDefault("Solocraft.SunkenTemple", 5.0) },
        {129, config.GetFloatDefault("Solocraft.RazorfenDowns", 5.0) },
        {189, config.GetFloatDefault("Solocraft.MonasteryInstances", 5.0) },                     // Scarlet
        {209, config.GetFloatDefault("Solocraft.TanarisInstance", 5.0) },                        // Zul'Farrak
        {229, config.GetFloatDefault("Solocraft.BlackRockSpire", 10.0) },
        {230, config.GetFloatDefault("Solocraft.BlackrockDepths", 5.0) },
        {249, config.GetFloatDefault("Solocraft.OnyxiaLairInstance", 40.0) },
        {289, config.GetFloatDefault("Solocraft.SchoolofNecromancy", 5.0) },                     // Scholo
        {309, config.GetFloatDefault("Solocraft.Zul'gurub", 20.0) },
        {329, config.GetFloatDefault("Solocraft.Stratholme", 5.0) },
        {349, config.GetFloatDefault("Solocraft.Mauradon", 5.0) },
        {389, config.GetFloatDefault("Solocraft.OrgrimmarInstance", 5.0) },                      // Ragefire
        {409, config.GetFloatDefault("Solocraft.MoltenCore", 40.0) },
        {429, config.GetFloatDefault("Solocraft.DireMaul", 5.0) },
        {469, config.GetFloatDefault("Solocraft.BlackwingLair", 40.0) },
        {509, config.GetFloatDefault("Solocraft.AhnQiraj", 20.0) },
        {531, config.GetFloatDefault("Solocraft.AhnQirajTemple", 40.0) },

#ifdef SOLOCRAFT_TBC
        // BC Instances
        {269, config.GetFloatDefault("Solocraft.CavernsOfTime", 5.0) },                          // Black Morass
        {532, config.GetFloatDefault("Solocraft.Karazahn", 10.0) },
        {534, config.GetFloatDefault("Solocraft.HyjalPast", 25.0) },                             // Mount Hyjal
        {540, config.GetFloatDefault("Solocraft.HellfireMilitary", 5.0) },                       // The Shattered Halls
        {542, config.GetFloatDefault("Solocraft.HellfireDemon", 5.0) },                          // The Blood Furnace
        {543, config.GetFloatDefault("Solocraft.HellfireRampart", 5.0) },
        {544, config.GetFloatDefault("Solocraft.HellfireRaid", 25.0) },                          // Magtheridon's Lair
        {545, config.GetFloatDefault("Solocraft.CoilfangPumping", 5.0) },                        // The Steamvault
        {546, config.GetFloatDefault("Solocraft.CoilfangMarsh", 5.0) },                          // The Underbog
        {547, config.GetFloatDefault("Solocraft.CoilfangDraenei", 5.0) },                        // The Slavepens
        {548, config.GetFloatDefault("Solocraft.CoilfangRaid", 25.0) },                          // Serpentshrine Cavern
        {550, config.GetFloatDefault("Solocraft.TempestKeepRaid", 25.0) },                       // The Eye
        {552, config.GetFloatDefault("Solocraft.TempestKeepArcane", 5.0) },                      // The Arcatraz
        {553, config.GetFloatDefault("Solocraft.TempestKeepAtrium", 5.0) },                      // The Botanica
        {554, config.GetFloatDefault("Solocraft.TempestKeepFactory", 5.0) },                     // The Mechanar
        {555, config.GetFloatDefault("Solocraft.AuchindounShadow", 5.0) },                       // Shadow Labyrinth
        {556, config.GetFloatDefault("Solocraft.AuchindounDemon", 5.0) },                        // Sethekk Halls
        {557, config.GetFloatDefault("Solocraft.AuchindounEthereal", 5.0) },                     // Mana-Tombs
        {558, config.GetFloatDefault("Solocraft.AuchindounDraenei", 5.0) },                      // Auchenai Crypts
        {560, config.GetFloatDefault("Solocraft.HillsbradPast", 5.0) },                          // Old Hillsbrad Foothills
        {564, config.GetFloatDefault("Solocraft.BlackTemple", 25.0) },
        {565, config.GetFloatDefault("Solocraft.GruulsLair", 25.0) },
        {568, config.GetFloatDefault("Solocraft.ZulAman", 5.0) },
        {580, config.GetFloatDefault("Solocraft.SunwellPlateau", 25.0) },
        {585, config.GetFloatDefault("Solocraft.Sunwell5ManFix", 5.0) },                         // Magister's Terrace
#endif

#ifdef SOLOCRAFT_WOTLK
        // WOTLK Instances
        {533, config.GetFloatDefault("Solocraft.StratholmeRaid", 10.0) },                        //  Nax 10
        {574, config.GetFloatDefault("Solocraft.Valgarde70", 5.0) },                             // Utgarde Keep
        {575, config.GetFloatDefault("Solocraft.UtgardePinnacle", 5.0) },
        {576, config.GetFloatDefault("Solocraft.Nexus70", 5.0) },                                // The Nexus
        {578, config.GetFloatDefault("Solocraft.Nexus80", 5.0) },                                // The Occulus
        {595, config.GetFloatDefault("Solocraft.StratholmeCOT", 5.0) },                          // The Culling of Stratholme
        {599, config.GetFloatDefault("Solocraft.Ulduar70", 5.0) },                               // Halls of Stone
        {600, config.GetFloatDefault("Solocraft.DrakTheronKeep", 5.0) },                         // Drak'Tharon Keep
        {601, config.GetFloatDefault("Solocraft.Azjol_Uppercity", 5.0) },                        // Azjol-Nerub
        {602, config.GetFloatDefault("Solocraft.Ulduar80", 5.0) },                               // Halls of Lighting
        {603, config.GetFloatDefault("Solocraft.UlduarRaid", 10.0) },                            // Ulduar 10
        {604, config.GetFloatDefault("Solocraft.GunDrak", 5.0) },
        {608, config.GetFloatDefault("Solocraft.DalaranPrison", 5.0) },                          // Violet Hold
        {615, config.GetFloatDefault("Solocraft.ChamberOfAspectsBlack", 10.0) },                 // The Obsidian Sanctum 10
        {616, config.GetFloatDefault("Solocraft.NexusRaid", 10.0) },                             // The Eye of Eternity 10
        {619, config.GetFloatDefault("Solocraft.Azjol_LowerCity", 5.0) },                        // Ahn'kahet: The Old Kingdom
        {631, config.GetFloatDefault("Solocraft.IcecrownCitadel", 10.0) },                       // Icecrown Citadel 10
        {632, config.GetFloatDefault("Solocraft.IcecrownCitadel5Man", 5.0) },                    // The Forge of Souls
        {649, config.GetFloatDefault("Solocraft.ArgentTournamentRaid", 10.0) },                  // Trial of the Crusader 10
        {650, config.GetFloatDefault("Solocraft.ArgentTournamentDungeon", 5.0) },                // Trial of the Champion
        {658, config.GetFloatDefault("Solocraft.QuarryOfTears", 5.0) },                          // Pit of Saron
        {668, config.GetFloatDefault("Solocraft.HallsOfReflection", 5.0) },                      // Halls of Reflection
        {724, config.GetFloatDefault("Solocraft.ChamberOfAspectsRed", 10.0) },                   // The Ruby Sanctum 10
#endif
    };

    // Heroics
    diff_Multiplier_Heroics =
    {
#ifdef SOLOCRAFT_TBC
        // TBC Instances Heroics
        {269, config.GetFloatDefault("Solocraft.CavernsOfTimeH", 5.0) },                         // Black Morass H
        {540, config.GetFloatDefault("Solocraft.HellfireMilitaryH", 5.0) },                      // The Shattered Halls H
        {542, config.GetFloatDefault("Solocraft.HellfireDemonH", 5.0) },                         // The Blood Furnace H
        {543, config.GetFloatDefault("Solocraft.HellfireRampartH", 5.0) },                       // Heroic
        {545, config.GetFloatDefault("Solocraft.CoilfangPumpingH", 5.0) },                       // The Steamvault
        {546, config.GetFloatDefault("Solocraft.CoilfangMarshH", 5.0) },                         // The Underbog
        {547, config.GetFloatDefault("Solocraft.CoilfangDraeneiH", 5.0) },                       // The Slavepens H
        {552, config.GetFloatDefault("Solocraft.TempestKeepArcaneH", 5.0) },                     // The Arcatraz H
        {553, config.GetFloatDefault("Solocraft.TempestKeepAtriumH", 5.0) },                     // The Botanica H
        {554, config.GetFloatDefault("Solocraft.TempestKeepFactoryH", 5.0) },                    // The Mechanar H
        {555, config.GetFloatDefault("Solocraft.AuchindounShadowH", 5.0) },                      // Shadow Labyrinth H
        {556, config.GetFloatDefault("Solocraft.AuchindounDemonH", 5.0) },                       // Sethekk Halls H
        {557, config.GetFloatDefault("Solocraft.AuchindounEtherealH", 5.0) },                    // Mana-Tombs H
        {558, config.GetFloatDefault("Solocraft.AuchindounDraeneiH", 5.0) },                     // Auchenai Crypts H
        {560, config.GetFloatDefault("Solocraft.HillsbradPastH", 5.0) },                         // Old Hillsbrad Foothills H
        {568, config.GetFloatDefault("Solocraft.ZulAmanH", 5.0) },                               // Zul'Aman H
        {585, config.GetFloatDefault("Solocraft.Sunwell5ManFixH", 5.0) },                        // Magister's Terrace H
#endif

#ifdef SOLOCRAFT_WOTLK
        // WOTLK Instances Heroics
        {533, config.GetFloatDefault("Solocraft.StratholmeRaidH", 25.0) },                       // Naxxramas 25
        {574, config.GetFloatDefault("Solocraft.Valgarde70H", 5.0) },                            // Utgarde Keep H
        {575, config.GetFloatDefault("Solocraft.UtgardePinnacleH", 5.0) },                       // Utgarde Pinnacle H
        {576, config.GetFloatDefault("Solocraft.Nexus70H", 5.0) },                               // The Nexus H
        {578, config.GetFloatDefault("Solocraft.Nexus80H", 5.0) },                               // The Occulus H
        {595, config.GetFloatDefault("Solocraft.StratholmeCOTH", 5.0) },                         // The Culling of Stratholme H
        {599, config.GetFloatDefault("Solocraft.Ulduar70H", 5.0) },                              // Halls of Stone H
        {600, config.GetFloatDefault("Solocraft.DrakTheronKeepH", 5.0) },                        // Drak'Tharon Keep H
        {601, config.GetFloatDefault("Solocraft.Azjol_UppercityH", 5.0) },                       // Azjol-Nerub H
        {602, config.GetFloatDefault("Solocraft.Ulduar80H", 5.0) },                              // Halls of Lighting H
        {603, config.GetFloatDefault("Solocraft.UlduarRaidH", 25.0) },                           // Ulduar 25
        {604, config.GetFloatDefault("Solocraft.GunDrakH", 5.0) },                               // Gundrak H
        {608, config.GetFloatDefault("Solocraft.DalaranPrisonH", 5.0) },                         // Violet Hold H
        {615, config.GetFloatDefault("Solocraft.ChamberOfAspectsBlackH", 25.0) },                // The Obsidian Sanctum 25
        {616, config.GetFloatDefault("Solocraft.NexusRaidH", 25.0) },                            // The Eye of Eternity 25
        {619, config.GetFloatDefault("Solocraft.Azjol_LowerCityH", 5.0) },                       // Ahn'kahet: The Old Kingdom H
        {631, config.GetFloatDefault("Solocraft.IcecrownCitadelH", 25.0) },                      // Icecrown Citadel 25
        {632, config.GetFloatDefault("Solocraft.IcecrownCitadel5ManH", 5.0) },                   // The Forge of Souls
        {649, config.GetFloatDefault("Solocraft.ArgentTournamentRaidH", 25.0) },                 // Trial of the Crusader 25
        {650, config.GetFloatDefault("Solocraft.ArgentTournamentDungeonH", 5.0) },               // Trial of the Champion H
        {658, config.GetFloatDefault("Solocraft.QuarryOfTearsH", 5.0) },                         // Pit of Saron H
        {668, config.GetFloatDefault("Solocraft.HallsOfReflectionH", 5.0) },                     // Halls of Reflection H
        {724, config.GetFloatDefault("Solocraft.ChamberOfAspectsRedH", 25.0) },                  // The Ruby Sanctum 25
#endif
    };

    //Unique Raids beyond the heroic and normal versions of themselves
    D649H10 = config.GetFloatDefault("Solocraft.ArgentTournamentRaidH10", 10.0);  //Trial of the Crusader 10 Heroic
    D649H25 = config.GetFloatDefault("Solocraft.ArgentTournamentRaidH25", 25.0);

    return true;
}