// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2001 Alistair Riddoch

#ifndef SERVER_SERVER_ROUTING_H
#define SERVER_SERVER_ROUTING_H

#include <common/OOGThing.h>
#include <common/serialno.h>

class WorldRouter;
class Lobby;
class CommServer;

class ServerRouting : public OOGThing {
  private:
    BaseDict objects;
    const std::string svrRuleset;
    const std::string svrName;
    int numClients;
    WorldRouter & world;
  public:
    Lobby & lobby;

    ServerRouting(const std::string & ruleset, const std::string & name);
    ~ServerRouting();

    int idle();

    BaseEntity * addObject(BaseEntity * obj) {
        objects[obj->getId()] = obj;
        return obj;
    }

    int getSerialNo() {
        return opSerialNo();
    }

    const BaseDict & getObjects() const {
        return objects;
    }

    BaseEntity * getObject(const std::string & fid) const {
        BaseDict::const_iterator I = objects.find(fid);
        if (I == objects.end()) {
            return NULL;
        } else {
            return I->second;
        }
    }

    void incClients() { ++numClients; }
    void decClients() { --numClients; }

    WorldRouter & getWorld() { return world; }

    virtual void addToObject(Atlas::Message::Object::MapType &) const;
};

#endif // SERVER_SERVER_ROUTING_H
