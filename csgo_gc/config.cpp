#include "stdafx.h"
#include "config.h"
#include "keyvalue.h"
#include "random.h"

constexpr const char *ConfigFilePath = "csgo_gc/config.txt";
constexpr const char *ProfileFilePath = "csgo_gc/profiles.txt";

GCConfig::GCConfig()
{
    KeyValue config{ "config" };

    if (!config.ParseFromFile(ConfigFilePath))
    {
        return;
    }

    const KeyValue *ranks = config.GetSubkey("ranks");
    if (ranks)
    {
        m_competitiveRank = static_cast<RankId>(ranks->GetNumber<int>("competitive_rank"));
        m_competitiveWins = ranks->GetNumber<int>("competitive_wins");

        m_wingmanRank = static_cast<RankId>(ranks->GetNumber<int>("wingman_rank"));
        m_wingmanWins = ranks->GetNumber<int>("wingman_wins");

        m_dangerZoneRank = static_cast<DangerZoneRankId>(ranks->GetNumber<int>("dangerzone_rank"));
        m_dangerZoneWins = ranks->GetNumber<int>("dangerzone_wins");
    }

    m_destroyUsedItems = config.GetNumber<int>("destroy_used_items") ? true : false;

    const KeyValue *rarityWeights = config.GetSubkey("rarity_weights");
    if (rarityWeights)
    {
        m_rarityWeights.reserve(rarityWeights->SubkeyCount());

        for (const KeyValue &subkey : *rarityWeights)
        {
            RarityWeight weight;
            weight.rarity = FromString<uint32_t>(subkey.Name());
            weight.weight = FromString<float>(subkey.String());
            m_rarityWeights.push_back(weight);
        }
    }

    m_vacBanned = config.GetNumber<int>("vac_banned") ? true : false;
    m_commendedFriendly = config.GetNumber<int>("cmd_friendly");
    m_commendedTeaching = config.GetNumber<int>("cmd_teaching");
    m_commendedLeader = config.GetNumber<int>("cmd_leader");
    m_level = config.GetNumber<int>("player_level");
    m_xp = config.GetNumber<int>("player_cur_xp");

    m_store_vault = config.GetNumber<int>("store_vault");
    m_country = config.GetString("country");

    m_enable_penalty = config.GetNumber<int>("enable_penalty") ? true : false;
    m_penalty_reason = config.GetNumber<int>("penalty_reason");
    m_penalty_time = config.GetNumber<int>("penalty_time");

    m_tournament_event = config.GetNumber<int>("tournament_event");
}

float GCConfig::GetRarityWeight(uint32_t rarity) const
{
    for (const RarityWeight &weight : m_rarityWeights)
    {
        if (weight.rarity == rarity)
        {
            return weight.weight;
        }
    }

    return 0;
}

bool GCConfig::DestroyUsedItems() const
{
    return m_destroyUsedItems;
}

int GCConfig::GetProfile(uint32_t profileID, const std::string &key)
{
    KeyValue profiles{ "profiles" };

    if (!profiles.ParseFromFile(ProfileFilePath))
    {
        assert(false);
        return 0;
    }

    const KeyValue *profile = profiles.GetSubkey(std::to_string(profileID));
    if (!profile)
    {
        profile = profiles.GetSubkey("0");
        KeyValue &newProfile = profiles.AddSubkey(std::to_string(profileID));
        newProfile.AddNumber("competitive_rank", profile->GetNumber<int>("competitive_rank"));
        newProfile.AddNumber("competitive_wins", profile->GetNumber<int>("competitive_wins"));
        newProfile.AddNumber("wingman_rank", profile->GetNumber<int>("wingman_rank"));
        newProfile.AddNumber("wingman_wins", profile->GetNumber<int>("wingman_wins"));
        newProfile.AddNumber("dangerzone_rank", profile->GetNumber<int>("dangerzone_rank"));
        newProfile.AddNumber("dangerzone_wins", profile->GetNumber<int>("dangerzone_wins"));
        newProfile.AddNumber("cmd_friendly", profile->GetNumber<int>("cmd_friendly"));
        newProfile.AddNumber("cmd_teaching", profile->GetNumber<int>("cmd_teaching"));
        newProfile.AddNumber("cmd_leader", profile->GetNumber<int>("cmd_leader"));
        newProfile.AddNumber("player_level", profile->GetNumber<int>("player_level"));
        newProfile.AddNumber("player_cur_xp", profile->GetNumber<int>("player_cur_xp"));
        profiles.WriteToFile(ProfileFilePath);
    }

    return profile->GetNumber<int>(key);
}