The gui directory contains code for the simple self developed GUI system.

It contains several widgets, for example text widgets or list containers, that can be put into each other.
In the end a buffer with all the relevant data to render the gui is generated. This buffer is only updated if widgets are added or removed.
Just because widgets move the buffer does not have to get regenerated and position updates of widgets can be updated in the buffer using subbuffer gl calls,
which is really convenient.