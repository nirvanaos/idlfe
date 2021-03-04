#include "Builder/Builder.h"
#include "Struct.h"
#include "Union.h"
#include "Enum.h"
#include "TypeDef.h"
#include "Native.h"
#include "Exception.h"

using namespace std;
using namespace AST::Build;

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

RepositoryId::RepositoryId (const NamedItem& item, const Builder& builder) :
	item_ (item),
	RepositoryIdData (builder.prefix ())
{}

bool RepositoryId::check_prefix (Builder& builder, const Location& loc) const
{
	if (Definition::ID != definition_) {
		const string& pref = builder.prefix ();
		if (prefix_or_id_ != pref) {
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different prefix \"" + prefix_or_id_
				+ "\". Current prefix is \"" + pref + "\".");
			see_prev_declaration (builder, item ());
			return false;
		}
	}
	return true;
}

void RepositoryId::pragma_id (Builder& builder, const std::string& id, const Location& loc)
{
	switch (definition_) {
		case Definition::VERSION:
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version.");
			see_prev_declaration (builder, pragma_loc_);
			return;
		case Definition::ID:
			if (prefix_or_id_ != id) {
				builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma ID \"" + prefix_or_id_ + "\".");
				see_prev_declaration (builder, pragma_loc_);
			}
			return;
	}
	prefix_or_id_ = id;
	definition_ = Definition::ID;
	pragma_loc_ = loc;
}

void RepositoryId::pragma_version (Builder& builder, const Version v, const Location& loc)
{
	switch (definition_) {
		case Definition::VERSION:
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version.");
			see_prev_declaration (builder, pragma_loc_);
			return;
		case Definition::ID:
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma ID \"" + prefix_or_id_ + "\".");
			see_prev_declaration (builder, pragma_loc_);
			return;
	}
	version_ = v;
	definition_ = Definition::VERSION;
	pragma_loc_ = loc;
}

void RepositoryId::see_prev_declaration (Builder& builder, const Location& loc)
{
	builder.message (loc, Builder::MessageType::MESSAGE, "See previous declaration.");
}

}
