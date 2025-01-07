#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/Geode.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/LevelSettingsObject.hpp>
#include <Geode/binding/GJEffectManager.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/ColorAction.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJSpecialColorSelect.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <utils/Warn.hpp>

using namespace geode::prelude;

std::string shortTextForColorIdx(int channel) {
    switch (channel) {
        case 0: return "D";
        case 1000: return "BG";
        case 1001: return "G1";
        case 1002: return "L";
        case 1003: return "3DL";
        case 1004: return "Obj";
        case 1005: return "P1";
        case 1006: return "P2";
        case 1007: return "LBG";
        case 1008: return "C";  // what
        case 1009: return "G2";
        case 1010: return "";   // black
        case 1011: return "";   // white
        case 1012: return "Ltr";// lighter
        case 1013: return "MG";
        case 1014: return "MG2";
        default: return std::to_string(channel);
    }
}

std::string longTextForColorIdx(int channel) {
    if (0 < channel && channel < 1000) {
        return std::to_string(channel);
    }
    else {
        return GJSpecialColorSelect::textForColorIdx(channel);
    }
}

static std::array<int, 15> RECENT_COLOR_IDS {};
static constexpr std::array SPECIAL_CHANNEL_ORDER_SMALL {
    0,    1005, 1006, 
    1000, 1001, 1013, 
    1007, 1009, 1014,
    1004, 1002, 1003,
    1012, 1010, 1011,
};
static constexpr std::array SPECIAL_CHANNEL_ORDER_LARGE {
    0,    1005, 1006, 1012,
    1000, 1001, 1013, 1010,
    1007, 1009, 1014, 1011,
    1004, 1002, 1003,
};

class $modify(NewColorSelect, CustomizeObjectLayer) {
    struct Fields {
        // This makes sure that the first call to goToPage always actually 
        // generates the page content
        int page = 0;
        bool modified = false;
        bool initDone = false;
    };

    static size_t getChannelsOnPage() {
        if (Mod::get()->template getSettingValue<bool>("larger-color-menu")) {
            return 20;
        }
        else {
            return 35;
        }
    }

    static size_t getRecentColorCount() {
        if (Mod::get()->template getSettingValue<bool>("larger-color-menu")) {
            return 4;
        }
        else {
            return 15;
        }
    }

    static auto getSpecialChannelOrder() {
        if (Mod::get()->template getSettingValue<bool>("larger-color-menu")) {
            return SPECIAL_CHANNEL_ORDER_LARGE;
        }
        else {
            return SPECIAL_CHANNEL_ORDER_SMALL;
        }
    }

    void updateSprite(ColorChannelSprite* sprite) {
        auto channel = static_cast<int>(reinterpret_cast<intptr_t>(sprite->getUserData()));
        auto action = LevelEditorLayer::get()->m_levelSettings->m_effectManager->getColorAction(channel);

        sprite->updateValues(action);
        
        switch (channel) {
            case 1010: {
                sprite->setColor({ 0, 0, 0 });
            } break;

            case 0: {
                sprite->setOpacity(50);
            } break;

            case 1007: {
                sprite->setOpacity(120);
            } break;

            case 1008: {
                sprite->setOpacity(200);
            } break;
        }

        if (auto label = sprite->getChildByID("id-label")) {
            label->setVisible(!action->m_copyID);
        }
    }

    ColorChannelSprite* createSprite(int channel, bool recent) {
        auto spr = ColorChannelSprite::create();
        spr->setScale(.8f);
        // tag gets overwritten by CCMenuItemSpriteExtra :(
        spr->setUserData(reinterpret_cast<void*>(static_cast<intptr_t>(channel)));
        spr->setID("channel-sprite"_spr);

        if (recent && channel == 0) {
            spr->setOpacity(105);
            spr->setColor({ 20, 20, 20 });
        }
        else {
            auto selection = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
            selection->setScale(1.1f);
            selection->setID("selected-indicator");
            selection->setPosition(spr->getContentSize() / 2);
            selection->setVisible(false);
            spr->addChild(selection);

            auto label = CCLabelBMFont::create(
                shortTextForColorIdx(channel).c_str(),
                "bigFont.fnt"
            );
            label->limitLabelWidth(25.f, .4f, .2f);
            label->setPosition(spr->getContentSize() / 2);
            label->setID("id-label");
            spr->addChild(label);

            this->updateSprite(spr);
        }

        return spr;
    }

    CCMenuItemSpriteExtra* createChannelButton(int channel, bool recent = false) {
        auto btn = CCMenuItemSpriteExtra::create(
            this->createSprite(channel, recent),
            this, menu_selector(CustomizeObjectLayer::onSelectColor)
        );
        btn->setTag(channel);
        btn->setID(fmt::format("channel-{}-button", channel));
        if (recent && channel == 0) {
            btn->setEnabled(false);
        }
        return btn;
    }

    void updateSpritesInMenu(CCNode* menu) {
        if (menu) {
            for (auto child : CCArrayExt<CCMenuItemSpriteExtra*>(menu->getChildren())) {
                if (auto spr = static_cast<ColorChannelSprite*>(child->getChildByID("channel-sprite"_spr))) {
                    this->updateSprite(spr);
                }
            }
        }
    }

    void updateSprites() {
        this->updateSpritesInMenu(m_mainLayer->getChildByID("channels-menu"));
        this->updateSpritesInMenu(m_mainLayer->getChildByID("special-channels-menu"));
        this->updateSpritesInMenu(m_mainLayer->getChildByID("recent-channels-menu"_spr));
    }

    $override
    void onSelectColor(CCObject* sender) {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::onSelectColor(sender);
        }
        
        // gotoPage removes the children in the channel menu, which causes the 
        // sender to be freed - this ensures it stays in memory for the 
        // duration of the call

        // funny note: this actually still results in a read from undefined 
        // memory! This is because Cocos2d is a terribly designed framework and 
        // every single `CCMenuItem::activate` call always results in at least 
        // one read from then-freed memory if the selector frees the menu item 
        // itself
        
        auto ref = Ref(sender);
        auto channel = sender->getTag();
        CustomizeObjectLayer::onSelectColor(sender);
        if (!channel) {
            channel = this->getActiveMode(true);
        }
        this->gotoPageWithChannel(channel);
        m_customColorChannel = channel;
        this->updateCustomColorLabels();
        m_fields->modified = true;
    }

    void highlightSelectedInMenu(CCNode* menu, int selected) {
        if (menu) {
            for (auto child : CCArrayExt<CCMenuItemSpriteExtra*>(menu->getChildren())) {
                if (auto spr = child->getChildByID("channel-sprite"_spr)) {
                    if (auto i = spr->getChildByID("selected-indicator")) {
                        i->setVisible(child->getTag() == selected);
                    }
                }
            }
        }
    }

    $override
    void highlightSelected(ButtonSprite* sprite) {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::highlightSelected(sprite);
        }
        auto selected = this->getActiveMode(true);
        this->highlightSelectedInMenu(m_mainLayer->getChildByID("channels-menu"), selected);
        this->highlightSelectedInMenu(m_mainLayer->getChildByID("special-channels-menu"), selected);
        this->highlightSelectedInMenu(m_mainLayer->getChildByID("recent-channels-menu"_spr), selected);
    }

// todo: hooking this virtual crashes on Android? probably a TulipHook or loader issue
#if !defined(GEODE_IS_ANDROID) && !defined(GEODE_IS_ARM_MAC)
    $override
    void colorSelectClosed(CCNode* target) {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::colorSelectClosed(target);
        }
        CustomizeObjectLayer::colorSelectClosed(target);
        this->updateSprites();
    }
#endif

    $override
    void updateCustomColorLabels() {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::updateCustomColorLabels();
        }
        // prevent textChanged from firing
        auto delegate = m_customColorInput->m_delegate;
        m_customColorInput->setDelegate(nullptr);
        m_customColorInput->setString(longTextForColorIdx(m_customColorChannel));
        m_customColorInput->setDelegate(delegate);
        m_customColorInput->m_textField->detachWithIME();
    }

    $override
    void onUpdateCustomColor(CCObject* sender) {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::onUpdateCustomColor(sender);
        }
        auto order = getSpecialChannelOrder();
        // recreate to avoid the default clamp of 10..999 without needing to patch
        m_customColorSelected = true;
        int i = m_customColorChannel;
        // cycling to special colors is a blatantly useless feature that i added for some reason
        if (sender->getTag() == 1) {
            // don't allow crossing 999 by pressing up
            if (i < 999) {
                i += 1;
            }
            else if (i >= 1000) {
                // loop around to normal channels
                if (i == order.back()) {
                    i = 1;
                }
                else {
                    // find the current channel in special channels
                    auto it = std::find(order.begin(), order.end(), i);
                    // if it's invalid, just hop to start
                    if (it == order.end()) {
                        i = 1;
                    }
                    // otherwise get the next in sequence
                    // this is safe because we have already checked if i is the last element
                    else {
                        i = *std::next(it);
                    }
                }
            }
        }
        else {
            // don't allow crossing 999 by pressing down
            if (i >= 1000) {
                // special case for default
                if (i == 0) {
                    i = 1;
                }
                // first in order is default so we want second
                else if (i != order.at(1)) {
                    // find the current channel in special channels
                    auto it = std::find(order.begin(), order.end(), i);
                    // if it's invalid, just hop to start
                    if (it == order.end()) {
                        i = 1;
                    }
                    // otherwise get the previous in sequence
                    else {
                        i = *std::prev(it);
                    }
                }
            }
            else {
                if (i > 1) {
                    i -= 1;
                }
                // loop around to special colors
                else {
                    i = order.back();
                }
            }
        }
        m_customColorChannel = i;
        this->updateCustomColorLabels();
        this->updateSelected(i);
        m_customColorSelected = false;
        this->updateColorSprite();
        this->updateChannelLabel(i);
        this->highlightSelected(nullptr);
        this->gotoPageWithChannel(m_customColorChannel);
        m_fields->modified = true;
    }

    $override
    void textChanged(CCTextInputNode* input) {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return CustomizeObjectLayer::textChanged(input);
        }
        CustomizeObjectLayer::textChanged(input);
        this->gotoPageWithChannel(m_customColorChannel);
        m_fields->modified = true;
    }

    void saveColors() {
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return;
        }
        // add selected color to recent list if it's not there and it's not 0
        if (
            m_fields->modified && m_customColorChannel != 0 &&
            !ranges::contains(RECENT_COLOR_IDS, m_customColorChannel)
        ) {
            std::rotate(
                RECENT_COLOR_IDS.begin(),
                std::prev(RECENT_COLOR_IDS.end()),
                RECENT_COLOR_IDS.end()
            );
            *RECENT_COLOR_IDS.begin() = m_customColorChannel;
        }
    }

    $override
    void onClose(CCObject* sender) {
        saveColors();
        CustomizeObjectLayer::onClose(sender);
    }

    #ifdef GEODE_IS_MACOS // onClose is inlined into keyBackClicked, onBreakApart, and onLiveEdit on macOS
    $override
    void keyBackClicked() {
        saveColors();
        CustomizeObjectLayer::keyBackClicked();
    }

    $override
    void onBreakApart(CCObject* sender) {
        if (m_targetObject) saveColors();
        CustomizeObjectLayer::onBreakApart(sender);
    }

    $override
    void onLiveEdit(CCObject* sender) {
        if (m_targetObject) saveColors();
        CustomizeObjectLayer::onLiveEdit(sender);
    }
    #endif

    void gotoPageWithChannel(int channel) {
        if (0 < channel && channel < 1000) {
            this->gotoPage(static_cast<int>((channel - 1) / getChannelsOnPage()));
        }
    }

    BE_ALLOW_START
    BE_ALLOW_SHADOWING
    void gotoPage(int page) {
        if (!m_fields->initDone) {
            return;
        }
        int channelsPerPage = static_cast<int>(getChannelsOnPage());
        if (page < 0) {
            page = 0;
        }
        if (page > 999 / channelsPerPage) {
            page = 999 / channelsPerPage;
        }
        auto channelsMenu = m_mainLayer->getChildByID("channels-menu");
        if (!channelsMenu) {
            // some GD code may cause this to be called before IDs have been added
            return;
        }
        m_fields->page = page;
        channelsMenu->removeAllChildren();

        for (int channel = 1; channel <= channelsPerPage; channel++) {
            auto c = channel + page * channelsPerPage;
            if (c < 1000) {
                channelsMenu->addChild(this->createChannelButton(c));
            }
        }
        channelsMenu->updateLayout();

        if (auto menu = m_mainLayer->getChildByID("page-menu"_spr)) {
            static_cast<CCLabelBMFont*>(menu->getChildByID("current-page"))
                ->setString(fmt::format("Page {} / {}", page + 1, ceil(1000.f / channelsPerPage)).c_str());
        }

        this->highlightSelected(nullptr);
    }
    BE_ALLOW_END

    void onPage(CCObject* sender) {
        this->gotoPage(m_fields->page + sender->getTag());
    }

    $override
    bool init(GameObject* obj, CCArray* objs) {
        if (!CustomizeObjectLayer::init(obj, objs))
            return false;
        
        if (!Mod::get()->template getSettingValue<bool>("new-color-menu")) {
            return true;
        }

        auto winSize = CCDirector::get()->getWinSize();
        auto largeBtns = Mod::get()->template getSettingValue<bool>("larger-color-menu");

        // move the browse and copy paste menus to be inline with the popup because 
        // i will actually get diarrhea if they stay the way they are in vanilla
        if (auto menu = m_mainLayer->getChildByID("browse-menu")) {
            menu->setPositionY(winSize.height / 2 - 50.f);
        }
        if (auto menu = m_mainLayer->getChildByID("copy-paste-menu")) {
            menu->setPositionY(winSize.height / 2 + 50.f);
        }

        auto channelsMenu = m_mainLayer->getChildByID("channels-menu");
        auto specialsMenu = m_mainLayer->getChildByID("special-channels-menu");

        // remove existing color button UI; we will recreate our own from scratch
        m_colorButtons->removeAllObjects();
        channelsMenu->removeAllChildren();
        specialsMenu->removeAllChildren();

        // move and resize selected channel menu down a bit to fit the color buttons
        if (auto menu = m_mainLayer->getChildByID("selected-channel-menu")) {
            menu->setPosition(winSize.width / 2 + 125.f, winSize.height / 2 - 95.f);
            menu->setScale(.8f);
        }

        auto selectPos = CCPoint(winSize.width / 2 + 210.f, winSize.height / 2 - 15.f);
        m_mainLayer->getChildByID("select-channel-menu")->setPosition(selectPos);
        m_mainLayer->getChildByID("channel-input-bg")->setPosition(selectPos);
        m_mainLayer->getChildByID("channel-input")->setPosition(selectPos);
        m_customColorInput->m_maxLabelWidth = 38.f;
        
        // // remove the bg from custom color select
        // m_colorTabNodes->removeObject(m_customColorInputBG);
        // m_customColorInputBG->removeFromParent();

        // keeping both m_customColorInputBG and m_customColorButtonSprite 
        // in memory so there are no faultful accesses due to GD code

        // add bg. it's the special HJfod sauce that makes UI look good
        auto bg = CCScale9Sprite::create("square02_001.png", { 0, 0, 80, 80 });
        bg->setOpacity(80);
        bg->setContentSize({ 340, 150 });
        bg->setPosition(winSize / 2);
        bg->setID("bg"_spr);
        m_mainLayer->addChild(bg);
        m_colorTabNodes->addObject(bg); // this makes the bg be hidden in the text tab

        // move and resize the channel menus to be side-by-side horizontally
        // also reset layout to enforce the new one incase someone has altered it >:3

        // add special color channels
        specialsMenu->setContentSize({ 90.f, 125.f });
        specialsMenu->setPosition(winSize.width / 2 - 130.f, winSize.height / 2 - 7.5f);
        specialsMenu->setLayout(
            RowLayout::create()
                ->setCrossAxisOverflow(false)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
                ->setGap(4.f)
        );

        if (largeBtns) {
            specialsMenu->setContentSize({ 125.f, 125.f });
            specialsMenu->setPositionX(winSize.width / 2 - 102.5f);
        }

        for (auto channel : getSpecialChannelOrder()) {
            specialsMenu->addChild(this->createChannelButton(channel));
        }
        specialsMenu->updateLayout();

        auto specialTitle = CCLabelBMFont::create("Special", "bigFont.fnt");
        specialTitle->setID("special-channels-title"_spr);
        specialTitle->setScale(.35f);
        specialTitle->setPosition(
            specialsMenu->getPositionX(),
            specialsMenu->getPositionY() + 72.f
        );
        m_mainLayer->addChild(specialTitle);
        m_colorTabNodes->addObject(specialTitle);

        // add new color buttons to channel menu
        channelsMenu->setContentSize({ 170.f, 125.f });
        channelsMenu->setPosition(winSize.width / 2, winSize.height / 2 - 7.5f);
        channelsMenu->setLayout(
            RowLayout::create()
                ->setCrossAxisOverflow(false)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
                ->setGap(4.f)
        );
        channelsMenu->updateLayout();

        if (largeBtns) {
            channelsMenu->setContentSize({ 150.f, 125.f });
            channelsMenu->setPositionX(winSize.width / 2 + 47.5f);
        }

        auto pageMenu = CCMenu::create();
        pageMenu->setID("page-menu"_spr);
        pageMenu->ignoreAnchorPointForPosition(false);
        pageMenu->setPosition(
            channelsMenu->getPositionX(),
            channelsMenu->getPositionY() + 72.f
        );
        pageMenu->setContentSize({ 150, 20 });

        auto pageTitle = CCLabelBMFont::create("Page 1/X", "bigFont.fnt");
        pageTitle->setID("current-page");
        pageTitle->setScale(.35f);
        pageTitle->setPosition(70.f, 10.f);
        pageMenu->addChild(pageTitle);

        auto lastPageSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
        lastPageSpr->setScale(largeBtns ? .65f : .5f);
        auto lastPageBtn = CCMenuItemSpriteExtra::create(
            lastPageSpr, this, menu_selector(NewColorSelect::onPage)
        );
        lastPageBtn->setTag(-1);
        lastPageBtn->setPosition(10.f, 10.f);
        pageMenu->addChild(lastPageBtn);

        auto nextPageSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
        nextPageSpr->setScale(largeBtns ? .65f : .5f);
        auto nextPageBtn = CCMenuItemSpriteExtra::create(
            nextPageSpr, this, menu_selector(NewColorSelect::onPage)
        );
        nextPageBtn->setTag(1);
        nextPageBtn->setPosition(140.f, 10.f);
        pageMenu->addChild(nextPageBtn);

        m_mainLayer->addChild(pageMenu);
        m_colorTabNodes->addObject(pageMenu);

        // add separator line between items. it's the special HJfod UI sauce
        auto line = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
        line->setPosition(ccp(
            channelsMenu->getPositionX() - channelsMenu->getScaledContentSize().width / 2 - (largeBtns ? 7 : 4),
            channelsMenu->getPositionY()
        ));
        line->setScaleY(1.65f);
        line->setOpacity(100);
        line->setID("separator"_spr);
        m_mainLayer->addChild(line);
        m_colorTabNodes->addObject(line);

        // add second separator line between channels and recent
        auto line2 = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
        line2->setPosition(ccp(
            channelsMenu->getPositionX() + channelsMenu->getScaledContentSize().width / 2 + (largeBtns ? 7 : 4),
            channelsMenu->getPositionY()
        ));
        line2->setScaleY(1.65f);
        line2->setOpacity(100);
        line2->setID("separator-2"_spr);
        m_mainLayer->addChild(line2);
        m_colorTabNodes->addObject(line2);

        // add recently used channels menu
        auto recentMenu = CCMenu::create();
        recentMenu->ignoreAnchorPointForPosition(false);
        recentMenu->setID("recent-channels-menu"_spr);
        recentMenu->setContentSize({ 90.f, 125.f });
        recentMenu->setPosition(winSize.width / 2 + 130.f, winSize.height / 2 - 7.5f);
        recentMenu->setLayout(
            RowLayout::create()
                ->setCrossAxisOverflow(false)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
                ->setGap(4.f)
        );
        m_mainLayer->addChild(recentMenu);
        m_colorTabNodes->addObject(recentMenu);

        if (largeBtns) {
            recentMenu->setContentSize({ 30.f, 125.f });
            recentMenu->setPositionX(winSize.width / 2 + 150.f);
        }

        for (size_t i = 0; i < getRecentColorCount(); i += 1) {
            recentMenu->addChild(this->createChannelButton(RECENT_COLOR_IDS.at(i), true));
        }
        recentMenu->updateLayout();

        auto recentTitle = CCLabelBMFont::create("Recent", "bigFont.fnt");
        recentTitle->setID("recent-channels-title"_spr);
        recentTitle->setScale(.35f);
        recentTitle->setPosition(
            recentMenu->getPositionX(),
            recentMenu->getPositionY() + 72.f
        );
        m_mainLayer->addChild(recentTitle);
        m_colorTabNodes->addObject(recentTitle);

        m_fields->initDone = true;

        this->gotoPage(0);
        this->updateCustomColorLabels();

        return true;
    }
};

#ifdef GEODE_IS_MACOS // we need to manually hook the keyBackClicked virtual offset function
#define CustomizeObjectLayer_keyBackClicked_336 base::get() + GEODE_ARM_MAC(0x19ebec) GEODE_INTEL_MAC(0x1e77d0)
void CustomizeObjectLayer_keyBackClicked(void* self) {
    reinterpret_cast<NewColorSelect*>(reinterpret_cast<uintptr_t>(self) - 0x150)->saveColors();
    reinterpret_cast<void(*)(void*)>(CustomizeObjectLayer_keyBackClicked_336)(self);
}

$execute {
    (void) Mod::get()->hook(
        reinterpret_cast<void*>(CustomizeObjectLayer_keyBackClicked_336),
        &CustomizeObjectLayer_keyBackClicked,
        "CustomizeObjectLayer::keyBackClicked (+0x150)",
        tulip::hook::TulipConvention::Thiscall
    );
}
#endif
