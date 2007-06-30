#This file is distributed under the terms of the GNU General Public license.
#Copyright (C) 2006 Al Riddoch (See the file COPYING for details).

from atlas import *
from physics import *
from Quaternion import Quaternion
from Vector3D import Vector3D

from cyphesis.Thing import Thing

class Slaughter(Thing):
    """ A task for cutting a log into boards."""
    def cut_operation(self, op):
        """ Op handler for cut op which activates this task """
        print "Slaughter.cut"

        if len(op) < 1:
            sys.stderr.write("Slaughter task has no target in cut op")

        # FIXME Use weak references, once we have them
        self.target = op[0].id
        self.tool = op.to

    def tick_operation(self, op):
        """ Op handler for regular tick op """
        print "Slaughter.tick"

        target=self.character.world.get_object(self.target)
        if not target:
            # print "Target is no more"
            self.irrelevant()
            return

        if square_distance(self.character.location, target.location) > target.location.bbox.square_bounding_radius():
            self.rate = 0
            # print "Too far away"
            return self.next_tick(1.75)
        else:
            self.progress += self.rate * 1.75

        self.rate = 0.1 / 1.75

        if self.progress < 1:
            # print "Not done yet"
            return self.next_tick(1.75)

        res=Message()

        set=Operation("set", Entity(target.id, bbox=new_bbox), to=target)
        res.append(set)

        chunk_loc = target.location.copy()
        meat_type = 'meat'

        if hasattr(target, 'meat'):
            meat_type = target.meat

        chunk_loc.coordinates = target.location.coordinates

        chunk_loc.orientation = target.location.orientation

        create=Operation("create",
                         Entity(name = meat_type,
                                type = meat_type,
                                location = chunk_loc), to = target)
        res.append(create)

        if width - self.width > self.width:
            self.progress = 0
        else:
            # FIXME Integrate with other set op.
            # Add create to convert remaining fragment into a board
            set=Operation("set", Entity(target.id, status=-1), to=target)
            res.append(set)

        res.append(self.next_tick(1.75))

        return res
