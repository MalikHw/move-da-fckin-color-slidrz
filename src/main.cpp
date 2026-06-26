#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectLiveOverlay.hpp>

using namespace geode::prelude;

// yes i had this prob so why not make it to a mod
class DragHandleItem : public CCMenuItem {
public:
    ColorSelectLiveOverlay* m_targetLayer;
    CCSprite* m_sprite;
    CCPoint m_startTouchPos;
    CCPoint m_startLayerPos;
    bool m_isDragging;

    static DragHandleItem* create(CCSprite* sprite, ColorSelectLiveOverlay* target) {
        auto ret = new DragHandleItem();
        if (ret && ret->init(sprite)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(CCSprite* sprite) {
        if (!CCMenuItem::init()) return false;
        m_sprite = sprite;
        m_targetLayer = nullptr;
        m_isDragging = false;
        this->setContentSize(sprite->getContentSize());
        this->addChild(sprite);
        sprite->setPosition(m_obContentSize / 2);
        return true;
    }

    void setTargetLayer(ColorSelectLiveOverlay* layer) {
        m_targetLayer = layer;
    }

    virtual void activate() {}

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
    };

    bool init(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object) {
        if (!ColorSelectLiveOverlay::init(baseAction, detailAction, object)) return false;
        
        m_fields->m_dragMenu = CCMenu::create();
        m_fields->m_dragMenu->setPosition(ccp(0, 0));
        this->addChild(m_fields->m_dragMenu, 10);
        auto dragBtn = CCSprite::create("GJ_colorThumbSBtn.png");
        auto dragItem = DragHandleItem::create(dragBtn, this);
        dragItem->setTargetLayer(this);
        dragItem->setPosition(ccp(220, 170));
        m_fields->m_dragMenu->addChild(dragItem);

        return true;
    }
};