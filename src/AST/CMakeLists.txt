add_subdirectory(Builder)
target_sources(idlfe PRIVATE
    Container.cpp
    Fixed.cpp
    Identifier.cpp
    Interface.cpp
    Item.cpp
    ItemScope.cpp
    ItemWithId.cpp
    Member.cpp
    NamedItem.cpp
    Root.cpp
    ScopedName.cpp
    Symbols.cpp
    Type.cpp
    ValueType.cpp
    Variant.cpp
    )