#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectLiveOverlay.hpp>

using namespace geode::prelude;

class $modify(CSLO, ColorSelectLiveOverlay) {
    struct Fields {
        CCSprite* m_dragButton;
        CCPoint m_dragStartPos;
        CCPoint m_layerStartPos;
        bool m_isDragging;
    };

    bool init(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object) {
        if (!ColorSelectLiveOverlay::init(baseAction, detailAction, object)) return false;
        
        m_fields->m_dragButton = CCSprite::create("GJ_colorThumbSBtn.png");
        if (m_fields->m_dragButton) {
            m_fields->m_dragButton->setPosition(ccp(220, 170));
            this->addChild(m_fields->m_dragButton, 10);
        }

        m_fields->m_isDragging = false;

        return true;
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!m_fields->m_dragButton) return false;
        
        CCPoint touchPos = this->convertTouchToNodeSpace(touch);
        if (m_fields->m_dragButton->boundingBox().containsPoint(touchPos)) {
            m_fields->m_dragStartPos = touchPos;
            m_fields->m_layerStartPos = this->getPosition();
            m_fields->m_isDragging = true;
            return true;
        }
        return false;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        if (!m_fields->m_isDragging) return;
        
        CCPoint touchPos = this->convertTouchToNodeSpace(touch);
        CCPoint delta = ccpSub(touchPos, m_fields->m_dragStartPos);
        this->setPosition(ccpAdd(m_fields->m_layerStartPos, delta));
    }

    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        m_fields->m_isDragging = false;
    }

    void ccTouchCancelled(CCTouch* touch, CCEvent* event) {
        m_fields->m_isDragging = false;
    }

    void registerWithTouchDispatcher() {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
    }
};
