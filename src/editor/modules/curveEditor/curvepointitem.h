#pragma once
#include <qgraphicsitem.h>

#include "utils/Event.h"
#include "utils/ViewModule.h"
#include "models/Curve.h"

#include "CurvePointEditor.h"

class QStyleOptionGraphicsItem;

namespace Idogep
{
    class TimelineArea;

    class CurveSegment;

    class CurvePointItem : public QGraphicsItem, public View
    {
    public:
        //CurvePointItem(QGraphicsItem* parent = nullptr);
        explicit CurvePointItem(Grafkit::Animation::Key key, size_t index, QGraphicsItem* parent = nullptr);

        // may be useful when copy and paste
        //CurvePointItem(const CurvePointItem& other);

        ~CurvePointItem();

        // Operations

        void RecalculatePosition(TimelineArea const* area);

        // Properties
        //float GetTime() const { return m_key.m_time; }
        //void SetTime(const float t) { m_key.m_time = t; }

        //float GetValue() const { return m_key.m_value; }
        //void SetValue(const float v) { m_key.m_value = v; }

        QPointF GetCoord() const { return { m_key.m_time, m_key.m_value }; }

        void SetCoord(QPointF c)
        {
            m_key.m_time = c.x();
            m_key.m_value = c.y();
        }

        //float GetRadius();
        //void SetRadius(float  radius);
        //float GetAngle();
        //void SetAngle(float  angle);
        //void SetTangent(float angle, float radius);
        //QPointF GetTangent();

        // --- 

        Grafkit::Animation::Key GetKey() const { return m_key; }
        void SetKey(const Grafkit::Animation::Key& key) { m_key = key; }

        // TODO protected: + Friend CurveEditor
        Grafkit::Animation::Key GetOriginalKey() const { return m_originalKey; }
        void SetOriginalKey(const Grafkit::Animation::Key& originalKey) { m_originalKey = originalKey; }

        uint32_t GetId() const { return m_id; }
        void SetId(const uint32_t id) { m_id = id; }

        // events
        KeyEditEvent onEditKey;
        KeyEditEvent onStartEdit;
        KeyEditEvent onCommitEdit;

        // --- 
        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        void FakeMouseMoveEvent(QGraphicsSceneMouseEvent* event);

        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

        void keyReleaseEvent(QKeyEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;

        void EditPosition(QGraphicsSceneMouseEvent* event);

        void ToggleTangentEditing();
        void EditTangent(QGraphicsSceneMouseEvent* event);

        void RefreshView(bool force) override;

    private:
        float /*m_radix, */m_lastRadix;
        float /*m_angle, */m_lastAngle;

        //QPointF m_angle; // Will take care of it later

        bool m_showTangent;

        uint32_t m_id;

        Grafkit::Animation::Key m_key;
        Grafkit::Animation::Key m_originalKey;

        uint32_t m_nodeType;
        uint32_t m_color;

        QSizeF m_areaScaling;

        Ref<CurveSegment> m_previousSegment;
        Ref<CurveSegment> m_nextSegment;
    };


    class CurveSegment : public Referencable
    {
    public:
        void RecalculateSegment(TimelineArea const* area);

    private:
        QPixmap * m_segment;

        Grafkit::Animation::ChannelRef m_channel;

        uint32_t m_startId;
        uint32_t m_endId;
    };
}
