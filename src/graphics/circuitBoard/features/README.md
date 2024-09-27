When the project was in its earliest days all logic regarding the circuit board was contained in one large CircuitBoard class.

This had several problems, one of which was just readability and maintainability.
Another issue was, that I cannot just "disable" specific features. What if I don't want to allow the user to modify cables, maybe
because I added some sort of read only mode?

This is why I split the code up into several features. Some are simple, e.g. the CursorFeature which updates and renders the cursor.
Some are more complex, for example the moveFeature which handles movement of selected components.

They are designed to work to plug and play, although their instantiation is still a bit messy.
Some features depend on other features. For example the moveFeature uses the selectionFeature to get information about current selected features.
The goal is that these features are as loosely coupled as possible, but currently some features need other features in their constructor, which is not good.

This issue discusses the interface between the different features: https://github.com/Placeblock/BuildIT/issues/21