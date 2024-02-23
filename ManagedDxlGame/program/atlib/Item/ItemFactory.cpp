#include "ItemFactory.h"
#include "../atlib/Utilities/AtlRandom.h"
#include "ItemAction.h"

namespace atl {

    Shared<Item> ItemFactory::createItem(e_ItemList createItemName) {
        Shared<Item> newItem = std::make_shared<Item>();

        // アイテム情報の初期化
        newItem->initialize(createItemName);

        // アイテムの効果を設定
        auto itemAction = createItemActionFromItemName(createItemName);
        newItem->setItemAction(itemAction);

        return newItem;
    }

    Shared<Item> ItemFactory::createRandomItem() {
        // 1 ~ アイテム数 - 1 の間で数値を取得 ( 0 はNONEで外れ値 )
        int itemID = mtRandomRangeInt(1, static_cast<int>(e_ItemList::ITEM_MAX) - 1);
        // 数値からアイテム名のenum列挙子に変換
        e_ItemList itemName = static_cast<e_ItemList>(itemID);
        
        return createItem(itemName);
    }

    Shared<IE_ItemAction> ItemFactory::createItemActionFromItemName(e_ItemList itemName) {
        // 範囲外チェック、早期リターン
        if (itemName == e_ItemList::NONE || itemName == e_ItemList::ITEM_MAX) { return nullptr; }

        Shared<IE_ItemAction> returnItemAction = nullptr;

        switch (itemName) {
        case e_ItemList::HealHerb:
            returnItemAction = std::make_shared<HealHerbAction>();
            break;
        case e_ItemList::HealPotion:
            returnItemAction = std::make_shared<HealPotionAction>();
            break;
        case e_ItemList::ThunderStone:
            returnItemAction = std::make_shared<ThundeStoneAction>();
            break;
        case e_ItemList::ThunderScroll:
            returnItemAction = std::make_shared<ThundeScrollAction>();
            break;
        case e_ItemList::MagicInc:
            returnItemAction = std::make_shared<MagicIncAction>();
            break;
        case e_ItemList::HealRune:
            returnItemAction = std::make_shared<HealRuneAction>();
            break;
        case e_ItemList::FireRune:
            returnItemAction = std::make_shared<FireRuneAction>();
            break;
        case e_ItemList::StoneRune:
            returnItemAction = std::make_shared<StoneRuneAction>();
            break;

        default:
            return nullptr;
            break;
        }

        return returnItemAction;
    }

}