#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectLiveOverlay.hpp>

using namespace geode::prelude;

// yes i had this prob so why not make it to a mod
class DragHandleItem : public CCMenuItemSpriteExtra {
public:
    ColorSelectLiveOverlay* m_targetLayer;
    CCPoint m_startTouchPos;
    CCPoint m_startLayerPos;
    bool m_isDragging;

    static DragHandleItem* create(CCNode* normalSprite, ColorSelectLiveOverlay* target) {
        auto ret = new DragHandleItem();
        if (ret && ret->init(normalSprite, nullptr, nullptr)) {
            ret->m_targetLayer = target;
            ret->m_isDragging = false;
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!m_bEnabled || !m_bVisible) return false;
        
        CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(touch);
        if (this->boundingBox().containsPoint(touchPos)) {
            m_startTouchPos = m_targetLayer->convertTouchToNodeSpace(touch);
            m_startLayerPos = m_targetLayer->getPosition();
            m_isDragging = true;
            return true;
        }
        return CCMenuItemSpriteExtra::ccTouchBegan(touch, event);
    }

    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        if (m_isDragging) {
            CCPoint touchPos = m_targetLayer->convertTouchToNodeSpace(touch);
            CCPoint delta = ccpSub(touchPos, m_startTouchPos);
            m_targetLayer->setPosition(ccpAdd(m_startLayerPos, delta));
        } else {
            CCMenuItemSpriteExtra::ccTouchMoved(touch, event);
        }
    }

    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        m_isDragging = false;
        CCMenuItemSpriteExtra::ccTouchEnded(touch, event);
    }

    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) {
        m_isDragging = false;
        CCMenuItemSpriteExtra::ccTouchCancelled(touch, event);
    }
};

class $modify(CSLO, ColorSelectLiveOverlay) {
    struct Fields {
        CCMenu* m_dragMenu;
    };

    bool init(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object) {
        if (!ColorSelectLiveOverlay::init(baseAction, detailAction, object)) return false;
        
        m_fields->m_dragMenu = CCMenu::create();
        m_fields->m_dragMenu->setPosition(ccp(0, 0));
        this->addChild(m_fields->m_dragMenu, 10);
        auto dragBtn = CCSprite::create("GJ_colorThumbSBtn.png");
        auto dragItem = DragHandleItem::create(dragBtn, this);
        dragItem->setPosition(ccp(220, 170));
        m_fields->m_dragMenu->addChild(dragItem);

        return true;
    }
};