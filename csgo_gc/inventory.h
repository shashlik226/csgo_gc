#pragma once

#include "gc_const_csgo.h"
#include "item_schema.h"
#include "random.h"

class GCConfig;
class KeyValue;

using ItemMap = std::unordered_map<uint64_t, CSOEconItem>;

class Inventory
{
public:
    Inventory(uint64_t steamId, const GCConfig &config);
    ~Inventory();

    void BuildCacheSubscription(CMsgSOCacheSubscribed &message, int level, bool server);

    bool EquipItem(uint64_t itemId, uint32_t classId, uint32_t slotId, CMsgSOMultipleObjects &update);

    bool UseItem(uint64_t itemId,
        CMsgSOSingleObject &destroy,
        CMsgSOMultipleObjects &updateMultiple,
        CMsgGCItemCustomizationNotification &notification);

    bool UnlockCrate(uint64_t crateId,
        uint64_t keyId,
        CMsgSOSingleObject &destroyCrate,
        CMsgSOSingleObject &destroyKey,
        CMsgSOSingleObject &newItem,
        CMsgGCItemCustomizationNotification &notification);

    bool SetItemPositions(
        const CMsgSetItemPositions &message,
        std::vector<CMsgItemAcknowledged> &acknowledgements,
        CMsgSOMultipleObjects &update);

    bool ApplySticker(const CMsgApplySticker &message,
        CMsgSOSingleObject &update,
        CMsgSOSingleObject &destroy,
        CMsgGCItemCustomizationNotification &notification);

    bool ScrapeSticker(const CMsgApplySticker &message,
        CMsgSOSingleObject &update,
        CMsgSOSingleObject &destroy,
        CMsgGCItemCustomizationNotification &notification);

    bool IncrementKillCountAttribute(uint64_t itemId, uint32_t amount, CMsgSOSingleObject &update);

    bool NameItem(uint64_t nameTagId,
        uint64_t itemId,
        std::string_view name,
        CMsgSOSingleObject &update,
        CMsgSOSingleObject &destroy,
        CMsgGCItemCustomizationNotification &notification);

    bool NameBaseItem(uint64_t nameTagId,
        uint32_t defIndex,
        std::string_view name,
        CMsgSOSingleObject &create,
        CMsgSOSingleObject &destroy,
        CMsgGCItemCustomizationNotification &notification);

    bool RemoveItemName(uint64_t itemId,
        CMsgSOSingleObject &update,
        CMsgSOSingleObject &destroy,
        CMsgGCItemCustomizationNotification &notification);

    // returns the item id and adds the item to the provided CMsgSOMultipleObjects
    // on failure returns 0 and does nothing
    uint64_t PurchaseItem(uint32_t defIndex, std::vector<CMsgSOSingleObject> &update);

    bool DeleteItem(uint64_t itemId, CMsgSOSingleObject &destroy);

private:
    uint32_t AccountId() const;

    // allocates an empty item, sets id and account_id fields
    // pass zero as highItemId to generate a new one
    CSOEconItem &AllocateItem(uint32_t highItemId);

    // create a new item of a specific type
    CSOEconItem &CreateItem(const CSOEconItem &copyFrom);
    CSOEconItem &CreateItem(uint32_t defIndex, ItemOrigin origin, UnacknowledgedType unacknowledgedType);

    void ReadFromFile();
    void ReadItem(const KeyValue &itemKey, CSOEconItem &item) const;

    void WriteToFile() const;
    void WriteItem(KeyValue &itemKey, const CSOEconItem &item) const;

    // helper, only called via EquipItem
    bool UnequipItem(uint64_t itemId, CMsgSOMultipleObjects &update);
    void UnequipItem(uint32_t classId, uint32_t slotId, CMsgSOMultipleObjects &update);

    void DestroyItem(ItemMap::iterator iterator, CMsgSOSingleObject &message);

    // move this to the item schema maybe?
    void ItemToPreviewDataBlock(const CSOEconItem &item, CEconItemPreviewDataBlock &block);

    // helpers for serializing items to CMsgSOMultipleObjects and CMsgSOSingleObject
    void AddToMultipleObjects(CMsgSOMultipleObjects &message, SOTypeId type, const google::protobuf::MessageLite &object);
    void ToSingleObject(CMsgSOSingleObject &message, SOTypeId type, const google::protobuf::MessageLite &object);

    // helpers for above..
    void AddToMultipleObjects(CMsgSOMultipleObjects &message, const CSOEconItem &object)
    {
        AddToMultipleObjects(message, SOTypeItem, object);
    }

    void ToSingleObject(CMsgSOSingleObject &message, const CSOEconItem &object)
    {
        ToSingleObject(message, SOTypeItem, object);
    }

    void AddToMultipleObjects(CMsgSOMultipleObjects &message, const CSOEconDefaultEquippedDefinitionInstanceClient &object)
    {
        AddToMultipleObjects(message, SOTypeDefaultEquippedDefinitionInstanceClient, object);
    }

    void ToSingleObject(CMsgSOSingleObject &message, const CSOEconDefaultEquippedDefinitionInstanceClient &object)
    {
        ToSingleObject(message, SOTypeDefaultEquippedDefinitionInstanceClient, object);
    }

    const uint64_t m_steamId;
    const GCConfig &m_config;
    ItemSchema m_itemSchema;
    Random m_random;
    uint32_t m_lastHighItemId{};
    ItemMap m_items;
    std::vector<CSOEconDefaultEquippedDefinitionInstanceClient> m_defaultEquips;
};
