#include "ItemScope.h"
#include "Module.h"
#include "Interface.h"
#include "Struct.h"
#include "Union.h"
#include "Exception.h"
#include "Builder/Builder.h"

namespace AST {

ItemScope::ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
	NamedItem (kind, builder, name),
	prefix_ (builder.prefix ())
{}

ItemScope* ItemScope::cast (NamedItem* item) noexcept
{
	if (item) {
		switch (item->kind ()) {
			case Item::Kind::MODULE:
				return static_cast <Module*> (item);
			case Item::Kind::INTERFACE:
				return static_cast <Interface*> (item);
			case Item::Kind::STRUCT:
				return static_cast <Struct*> (item);
			case Item::Kind::UNION:
				return static_cast <Union*> (item);
			case Item::Kind::EXCEPTION:
				return static_cast <Exception*> (item);
		}
	}
	return nullptr;
}

}
