#include "solocraft/SoloCraft.h"

#include "Util/Util.h"
#include "Globals/SharedDefines.h"
#include "Groups/Group.h"
#include "Chat/Chat.h"

INSTANTIATE_SINGLETON_1(Solocraft);

Solocraft::Solocraft() { }

Solocraft::~Solocraft() { }

bool Solocraft::Initialize()
{
    return sSolocraftConfig.Initialize();
}

void Solocraft::OnLogin(Player* player)
{
    if (sSolocraftConfig.enabled && sSolocraftConfig.SoloCraftAnnounceModule)
    {
        ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00SoloCraft |rmodule.");
    }

    OnMapChanged(player);
}

void Solocraft::OnLogout(Player* player)
{
    if (sSolocraftConfig.enabled)
    {
        std::map<ObjectGuid, float>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetObjectGuid());
        if (unitDifficultyIterator != _unitDifficulty.end())
        {
            _unitDifficulty.erase(unitDifficultyIterator);
        }

        std::map<ObjectGuid, float>::iterator unitBuffIterator = _unitBuff.find(player->GetObjectGuid());
        if (unitBuffIterator != _unitBuff.end())
        {
            _unitBuff.erase(unitBuffIterator);
        }
    }
}

void Solocraft::OnMapChanged(Player* player)
{
    if (sSolocraftConfig.enabled)
    {
        Map* map = player->GetMap();
        
        float difficulty = CalculateDifficulty(map);
        uint32 dunLevel = CalculateDungeonlevel(map);
        uint32 numInGroup = GetNumInGroup(player);
        uint32 classBalance = GetClassBalance(player);

        ApplyBuffs(player, map, dunLevel, difficulty, numInGroup, classBalance);
    }
}

// Set the instance difficulty
float Solocraft::CalculateDifficulty(Map* map)
{
    if (map)
    {
#if defined(SOLOCRAFT_TBC) || defined(SOLOCRAFT_WOTLK)

        if (map->Is25ManRaid())
        {
            if (map->IsHeroic() && map->GetId() == 649)
                return sSolocraftConfig.D649H25;
            else if (sSolocraftConfig.diff_Multiplier_Heroics.find(map->GetId()) == sSolocraftConfig.diff_Multiplier_Heroics.end())
                return sSolocraftConfig.D25;
            else
                return sSolocraftConfig.diff_Multiplier_Heroics[map->GetId()];
        }

        if (map->IsHeroic())
        {
            if (map->GetId() == 649)
                return sSolocraftConfig.D649H10;
            else if (sSolocraftConfig.diff_Multiplier_Heroics.find(map->GetId()) == sSolocraftConfig.diff_Multiplier_Heroics.end())
                return sSolocraftConfig.D10;
            else
            return sSolocraftConfig.diff_Multiplier_Heroics[map->GetId()];
        }

#endif
        if (sSolocraftConfig.diff_Multiplier.find(map->GetId()) == sSolocraftConfig.diff_Multiplier.end())
        {
            if (map->IsRaid())
                return sSolocraftConfig.D40;
            else if (map->IsDungeon())
                return sSolocraftConfig.D5;
        }
        else
            return sSolocraftConfig.diff_Multiplier[map->GetId()];
    }

    return 0;
}

uint32 Solocraft::CalculateDungeonlevel(Map* map)
{
    if (sSolocraftConfig.dungeons.find(map->GetId()) == sSolocraftConfig.dungeons.end())
        return sSolocraftConfig.SolocraftDungeonLevel;
    else
        return sSolocraftConfig.dungeons[map->GetId()];
}

// Get the group's size
uint32 Solocraft::GetNumInGroup(Player* player)
{
    int numInGroup = 1;
    Group* group = player->GetGroup();
    if (group)
    {
        Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
        numInGroup = groupMembers.size();
    }
    return numInGroup;
}

uint32 Solocraft::GetClassBalance(Player* player)
{
    int classBalance = 100;

    if (sSolocraftConfig.classes.find(player->getClass()) == sSolocraftConfig.classes.end())
    {
        return classBalance;
    }

    else if (sSolocraftConfig.classes[player->getClass()] >= 0 && sSolocraftConfig.classes[player->getClass()] <= 100)
            return sSolocraftConfig.classes[player->getClass()];
        else
            return classBalance;
}

// Resets buffers
void Solocraft::ClearBuffs(Player* player, Map* map)
{
    std::map<ObjectGuid, float>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetObjectGuid());
    if (unitDifficultyIterator != _unitDifficulty.end())
    {
        int difficulty = unitDifficultyIterator->second;
        _unitDifficulty.erase(unitDifficultyIterator);

        if (sSolocraftConfig.SoloCraftAnnounceModule)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Left to %s (removing difficulty = %f)",
                        map->GetMapName(), difficulty);
        }

        for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) 
        {
            player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, difficulty * sSolocraftConfig.SoloCraftStatsMult, false);
        }

        // Set player health
        // Defined in Unit.h line 1524
        player->SetHealth(player->GetMaxHealth());

        // Spellcaster Stat modify
        if (player->GetPowerType() == POWER_MANA || player->getClass() == CLASS_DRUID)
        {
            // Buff the player's mana
            player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            
#ifdef SOLOCRAFT_WOTLK
            // Buff Spellpower
            // Debuffed characters do not get spellpower
            if (difficulty > 0)
            {
                SpellPowerBonus = static_cast<int>((player->GetBaseSpellPowerBonus() * sSolocraftConfig.SoloCraftSpellMult) * difficulty);
                player->ApplySpellPowerBonus(SpellPowerBonus, false);
            }
#endif
        }
    }
}

// Apply the player buffs
void Solocraft::ApplyBuffs(Player* player, Map* map, uint32 dunLevel, float difficulty, uint32 numInGroup, uint32 classBalance)
{
    ClearBuffs(player, map);
    if (difficulty > 0)
    {
        int SpellPowerBonus = 0;

        // If a player is too high level for dungeon don't buff but if in a group will count towards the group offset balancing.
        if (player->GetLevel() <= dunLevel + sSolocraftConfig.SolocraftLevelDiff)
        {
            // Current Dungeon offset not exceeded - Buff player
            // Group difficulty and ClassBalance Adjustment
            difficulty = (((float)classBalance / 100) * difficulty) / numInGroup;
            // Float variables suck - two decimal rounding
            difficulty = roundf(difficulty * 100) / 100;

            if (sSolocraftConfig.SoloCraftAnnounceModule)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Entered %s (difficulty = %f, numInGroup = %d)",
                        map->GetMapName(), difficulty, numInGroup);
            }

            _unitDifficulty[player->GetObjectGuid()] = difficulty;
            // Modify Player Stats
            // STATS defined/enum in SharedDefines.h
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                // Buff the player
                // Unitmods enum UNIT_MOD_STAT_START defined in Unit.h line 391
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, difficulty * sSolocraftConfig.SoloCraftStatsMult, true);
            }

            // Set player health
            // Defined in Unit.h line 1524
            player->SetHealth(player->GetMaxHealth());

            // Spellcaster Stat modify
            if (player->GetPowerType() == POWER_MANA || player->getClass() == CLASS_DRUID)
            {
                // Buff the player's mana
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

#ifdef SOLOCRAFT_WOTLK
                // Buff Spellpower
                // Debuffed characters do not get spellpower
                if (difficulty > 0)
                {
                    SpellPowerBonus = static_cast<int>((player->GetBaseSpellPowerBonus() * sSolocraftConfig.SoloCraftSpellMult) * difficulty);
                    player->ApplySpellPowerBonus(SpellPowerBonus, true);
                }
#endif
            }
        }
    }    
}