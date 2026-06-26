#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectLiveOverlay.hpp>
#include <Geode/modify/HSVLiveOverlay.hpp>

using namespace geode::prelude;

// yes i had this prob so why not make it to a mod
class DragHandleItem : public CCMenuItemSpriteExtra {
public:
    CCNode* m_targetLayer;
    CCPoint m_startTouchPos;
    CCPoint m_startLayerPos;
    bool m_isDragging;

    static DragHandleItem* create(const char* sprite, CCNode* target) {
        auto ret = new DragHandleItem();
        if (ret && ret->init(sprite, target)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(const char* sprite, CCNode* target) {
        auto spr = CCSprite::create(sprite);
        if (!CCMenuItemSpriteExtra::init(spr, spr, nullptr, nullptr)) return false;
        m_targetLayer = target;
        m_isDragging = false;
        return true;
    }

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!m_bEnabled || !m_bVisible || !m_targetLayer) return false;
        
        CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(touch);
        if (this->boundingBox().containsPoint(touchPos)) {
            m_startTouchPos = m_targetLayer->convertTouchToNodeSpace(touch);
            m_startLayerPos = m_targetLayer->getPosition();
            m_isDragging = true;
            return true;
        }
        return false;
    }

    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        if (m_isDragging) {
            CCPoint touchPos = m_targetLayer->convertTouchToNodeSpace(touch);
            CCPoint delta = ccpSub(touchPos, m_startTouchPos);
            m_targetLayer->setPosition(ccpAdd(m_startLayerPos, delta));
        }
    }

    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        m_isDragging = false;
    }

    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) {
        m_isDragging = false;
    }
};

class $modify(CSLO, ColorSelectLiveOverlay) {
    struct Fields {
        CCMenu* m_dragMenu;
        DragHandleItem* m_dragItem;
    };

    bool init(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object) {
        if (!ColorSelectLiveOverlay::init(baseAction, detailAction, object)) return false;
        
        m_fields->m_dragMenu = CCMenu::create();
        m_fields->m_dragMenu->setPosition(ccp(0, 0));
        m_fields->m_dragMenu->setTouchEnabled(true);
        m_fields->m_dragMenu->setTouchPriority(-128);
        this->addChild(m_fields->m_dragMenu, 10);
        m_fields->m_dragItem = DragHandleItem::create("GJ_colorThumbSBtn.png", this);
        m_fields->m_dragItem->setPosition(ccp(220, 170));
        m_fields->m_dragMenu->addChild(m_fields->m_dragItem);

        return true;
    }
};

class $modify(HLO, HSVLiveOverlay) {
    struct Fields {
        CCMenu* m_dragMenu;
        DragHandleItem* m_dragItem;
    };

    bool init(GameObject* object, cocos2d::CCArray* objects) {
        if (!HSVLiveOverlay::init(object, objects)) return false;
        
        m_fields->m_dragMenu = CCMenu::create();
        m_fields->m_dragMenu->setPosition(ccp(0, 0));
        m_fields->m_dragMenu->setTouchEnabled(true);
        m_fields->m_dragMenu->setTouchPriority(-128);
        this->addChild(m_fields->m_dragMenu, 10);
        m_fields->m_dragItem = DragHandleItem::create("GJ_colorThumbSBtn.png", this);
        m_fields->m_dragItem->setPosition(ccp(220, 170));
        m_fields->m_dragMenu->addChild(m_fields->m_dragItem);

        return true;
    }
};