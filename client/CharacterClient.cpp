// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2001 Alistair Riddoch

#include "CharacterClient.h"

#include "ClientConnection.h"

#include "common/debug.h"

#include <Atlas/Objects/Operation/RootOperation.h>

static const bool debug_flag = false;

CharacterClient::CharacterClient(const std::string & id,
                                 const std::string & name,
                                 ClientConnection & c) :
                                 BaseMind(id, name), m_connection(c)
{
}

void CharacterClient::sightImaginaryOperation(const Sight &,
                                              Imaginary &, OpVector &)
{
}

void CharacterClient::soundTalkOperation(const Sound & , Talk &, OpVector &)
{
}

#if 0
//FIXME Dunno what all this trigger lark is about.
bad_type CharacterClient::add_trigger(bad_type event_name, bad_type func) {
    dictlist.add_value(CharacterClient::event_triggers,event_name, func);
}
bad_type CharacterClient::call_triggers(bad_type op) {
    event_name, sub_op = CharacterClient::get_op_name_and_sub(op);
    reply = Message();
    for (/*func in CharacterClient::event_triggers.get(event_name,[])*/) {
        reply = reply + func(op, sub_op);
    }
    return reply;
}
bad_type CharacterClient::set_from_op(bad_type op) {
    op.from_=self;
}
bad_type CharacterClient::set_from(bad_type msg) {
    CharacterClient::apply_to_operation(CharacterClient::set_from_op,msg);
}
#endif

void CharacterClient::send(RootOperation & op)
{
    op.setFrom(getId());
    m_connection.send(op);
}

inline bool CharacterClient::findRefnoOp(const RootOperation & op, long refno)
{
    if (refno == op.getRefno()) {
        return true;
    }
    return false;
}

inline bool CharacterClient::findRefno(const RootOperation & msg, long refno)
{
    return findRefnoOp(msg,refno);
}

OpVector CharacterClient::sendAndWaitReply(RootOperation & op)
{
    send(op);
    long no = op.getSerialno();
    while (true) {
        if (m_connection.pending()) {
            RootOperation * input=CharacterClient::m_connection.pop();
            if (input != NULL) {
                // What the hell is this!
                OpVector result;
                operation(*input, result);
                OpVector::const_iterator I;
                for (I=result.begin();I!=result.end();I++) {
                    send(*(*I));
                }
    
                if (findRefno(*input,no)) {
                    return OpVector(1,input);
                }
                delete input;
            }
        } else if (m_connection.wait()) {
            return OpVector();
        }
    }
}
