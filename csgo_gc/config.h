#pragma once

#include "gc_const_csgo.h"

struct RarityWeight
{
    uint32_t rarity;
    float weight;
};

class GCConfig
{
public:
    GCConfig();

    RankId CompetitiveRank() const { return m_competitiveRank; }
    int CompetitiveWins() const { return m_competitiveWins; }
    RankId WingmanRank() const { return m_wingmanRank; }
    int WingmanWins() const { return m_wingmanWins; }
    DangerZoneRankId DangerZoneRank() const { return m_dangerZoneRank; }
    int DangerZoneWins() const { return m_dangerZoneWins; }

    bool DestroyUsedItems() const;

    bool VacBanned() const { return m_vacBanned; }
    int CommendedFriendly() const { return m_commendedFriendly; }
    int CommendedTeaching() const { return m_commendedTeaching; }
    int CommendedLeader() const { return m_commendedLeader; }
    int Level() const { return m_level; }
    int Xp() const { return m_xp; }
    
    int StoreVault() const { return m_store_vault; }
    std::string Country() const { return m_country; }

    int isPenaltyEnabled() const { return m_enable_penalty; }
    int PenaltyReason() const { return m_penalty_reason; }
    int PenaltyTime() const { return m_penalty_time; }

    int TournamentEvent() const { return m_tournament_event; }

    float GetRarityWeight(uint32_t rarity) const;

    int GetProfile(uint32_t profileID, const std::string &key);

private:
    RankId m_competitiveRank{ RankNone };
    int m_competitiveWins{ 0 };
    RankId m_wingmanRank{ RankNone };
    int m_wingmanWins{ 0 };
    DangerZoneRankId m_dangerZoneRank{ DangerZoneRankNone };
    int m_dangerZoneWins{ 0 };

    bool m_destroyUsedItems{ true };

    bool m_vacBanned{ false };
    int m_commendedFriendly{ 0 };
    int m_commendedTeaching{ 0 };
    int m_commendedLeader{ 0 };
    int m_level{ 0 };
    int m_xp{ 0 };

    int m_store_vault{ 2 };
    std::string m_country{ "FI" };

    bool m_enable_penalty{ false };
    int m_penalty_reason{ 0 };
    int m_penalty_time{ 0 };

    int m_tournament_event{ 20 };

    std::vector<RarityWeight> m_rarityWeights;
};
