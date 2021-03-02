#include "Builder.h"
#include "Struct.h"
#include "Union.h"
#include "Enum.h"
#include "TypeDef.h"
#include "Native.h"
#include "Exception.h"

using namespace std;

namespace AST {

RepositoryId* RepositoryId::cast (NamedItem* item) noexcept
{
	RepositoryId* p = nullptr;
	if (item) {
		switch (item->kind ()) {
			case Item::Kind::INTERFACE:
				p = static_cast <Interface*> (item);
				break;
			case Item::Kind::INTERFACE_DECL:
				p = static_cast <InterfaceDecl*> (item);
				break;
			case Item::Kind::STRUCT:
				p = static_cast <Struct*> (item);
				break;
			case Item::Kind::STRUCT_DECL:
				p = static_cast <StructDecl*> (item);
				break;
			case Item::Kind::UNION:
				p = static_cast <Union*> (item);
				break;
			case Item::Kind::UNION_DECL:
				p = static_cast <UnionDecl*> (item);
				break;
			case Item::Kind::ENUM:
				p = static_cast <Enum*> (item);
				break;
			case Item::Kind::TYPEDEF:
				p = static_cast <TypeDef*> (item);
				break;
			case Item::Kind::NATIVE:
				p = static_cast <Native*> (item);
				break;
			case Item::Kind::EXCEPTION:
				p = static_cast <Native*> (item);
				break;
		}
	}
	return p;
}

bool RepositoryId::check_prefix (Builder& builder, const NamedItem& item, const Location& loc) const
{
	if (Definition::ID != definition_) {
		const string& pref = builder.prefix ();
		if (prefix_or_id_ != pref) {
			builder.message (loc, Builder::MessageType::ERROR, item.qualified_name () + " is already declared with different prefix \"" + prefix_or_id_
				+ "\". Current prefix is \"" + pref + "\".");
			builder.message (item, Builder::MessageType::MESSAGE, "See previous declaration.");
			return false;
		}
	} else
		return true;
}
/*
bool RepositoryId::pragma_id (Builder& builder, const NamedItem& item, const std::string& id, const Location& loc)
{
	switch (definition_) {
		case Definition::VERSION:
			builder.message (loc, Builder::MessageType::ERROR, "");
			return false;
		case Definition::ID:
			return prefix_or_id_ == id;
	}
	prefix_or_id_ = id;
	definition_ = Definition::ID;
	pragma_loc_ = loc;
	return true;
}
*/
}
