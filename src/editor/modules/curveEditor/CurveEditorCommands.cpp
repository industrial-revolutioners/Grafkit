#include "CurveEditorCommands.h"

#include "modules/curveEditor/CurvePointEditor.h"

using namespace Grafkit;
using namespace Idogep;


CurveKeyChangeCommand::CurveKeyChangeCommand(Animation::ChannelRef& channel, size_t keyId, Animation::Key oldKey, Animation::Key newKey, CurvePointEditor* pointEditor): m_channel(channel)
    , m_id(keyId)
    , m_newKey(newKey)
    , m_oldKey(oldKey)
    , m_controller(pointEditor) 
{
}

CurveKeyChangeCommand::~CurveKeyChangeCommand() = default;


void CurveKeyChangeCommand::Do()
{
    m_channel->SetKey(m_id, m_newKey);
    m_controller->UpdateKey(m_channel, m_id, m_newKey);
}

void CurveKeyChangeCommand::Undo()
{
    m_channel->SetKey(m_id, m_oldKey);
    m_controller->UpdateKey(m_channel, m_id, m_oldKey);
}