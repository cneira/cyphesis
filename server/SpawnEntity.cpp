// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2009 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#include "SpawnEntity.h"

#include "rulesets/Entity.h"
#include "rulesets/AreaProperty.h"

#include "common/log.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/RootEntity.h>
#include <Atlas/Objects/SmartPtr.h>

using Atlas::Message::ListType;
using Atlas::Message::MapType;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Entity::RootEntity;
using Atlas::Objects::Operation::Create;

SpawnEntity::SpawnEntity(Entity * e, const MapType & data) : m_ent(e)
{
    MapType::const_iterator I = data.find("character_types");
    if (I != data.end() && I->second.isList()) {
        m_characterTypes = I->second.List();
    }
}

static const int check_character_type(const std::string & type,
                                      const Atlas::Message::ListType & types)
{
    ListType::const_iterator I = types.begin();
    ListType::const_iterator Iend = types.end();
    for (; I != Iend; ++I) {
        if (*I == type) {
            return 0;
        }
    }
    return -1;
}
                                  

int SpawnEntity::spawnEntity(const std::string & type,
                             const RootEntity & dsc)
{
    if (m_ent.get() == 0) {
        return -1;
    }
    if (check_character_type(type, m_characterTypes) != 0) {
        return -1;
    }
    const AreaProperty * ap = m_ent->getPropertyClass<AreaProperty>("area");
    if (ap != 0) {
        log(NOTICE, "Spawn entity has area");
    } else {
        if (m_ent->hasAttr("area")) {
            log(WARNING, "Spawn entity has area but not an AreaProperty");
        }
        if (m_ent->m_location.bBox().isValid()) {
            // Locate in bbox
        }
    }
    // FIXME this is exactly the same location as the spawn entity
    dsc->setLoc(m_ent->m_location.m_loc->getId());
    ::addToEntity(m_ent->m_location.pos(), dsc->modifyPos());
    return 0;
}

int SpawnEntity::populateEntity(Entity * ent,
                                const RootEntity & dsc,
                                OpVector & res)
{
    // Hack in default objects
    // This needs to be done in a generic way
    Anonymous create_arg;
    create_arg->setParents(std::list<std::string>(1,"coin"));
    ::addToEntity(Point3D(0,0,0), create_arg->modifyPos());
    create_arg->setLoc(ent->getId());
    // FIXME We can probably send the same op 10 times, rather than create 10
    // FIXME alternatively we can set 10 args on one op
    for(int i = 0; i < 10; i++) {
        Create c;
        c->setTo(ent->getId());
        c->setArgs1(create_arg);
        res.push_back(c);
    }

    create_arg = create_arg.copy();
    create_arg->setParents(std::list<std::string>(1, "shirt"));
    Create c;
    c->setTo(ent->getId());
    c->setArgs1(create_arg);
    res.push_back(c);

    create_arg = create_arg.copy();
    create_arg->setParents(std::list<std::string>(1, "trousers"));
    c = Create();
    c->setTo(ent->getId());
    c->setArgs1(create_arg);
    res.push_back(c);

    create_arg = create_arg.copy();
    create_arg->setParents(std::list<std::string>(1, "cloak"));
    c = Create();
    c->setTo(ent->getId());
    c->setArgs1(create_arg);
    res.push_back(c);

    create_arg = create_arg.copy();
    create_arg->setParents(std::list<std::string>(1, "boots"));
    c = Create();
    c->setTo(ent->getId());
    c->setArgs1(create_arg);
    res.push_back(c);

    create_arg = create_arg.copy();
    create_arg->setParents(std::list<std::string>(1, "hat"));
    c = Create();
    c->setTo(ent->getId());
    c->setArgs1(create_arg);
    res.push_back(c);

    return 0;
}

int SpawnEntity::addToMessage(MapType & msg) const
{
    msg.insert(std::make_pair("character_types", m_characterTypes));
    return 0;
}
