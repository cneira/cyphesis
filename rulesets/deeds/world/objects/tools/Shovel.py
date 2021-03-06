#This file is distributed under the terms of the GNU General Public license.
#Copyright (C) 1999 Aloril (See the file COPYING for details).
#Copyright (C) 2004 Al Riddoch (See the file COPYING for details).

import server
from atlas import *

class Shovel(server.Thing):
    """This is a shovel for light digging"""
    def cut_operation(self, op):
        ent=Entity(self.id,status=self.status-0.01)
        to_ = op[0].id
        return Oplist(Operation("set",ent,to=self),Operation("dig",op[0],to=to_))
