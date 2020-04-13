#include "IdentifiersTable.h"

using namespace std;

size_t IdentifiersTable::size() const {
	return identifiers.size();
}

Identifier* IdentifiersTable::getByName(const string& identName) {
	for (size_t i = 0; i < identifiers.size(); i++)
		if (identifiers[i].getName() == identName)
			return &identifiers[i];
	
	return nullptr;
}

void IdentifiersTable::addByName(const string& identName) {
	if (getByName(identName) == nullptr)
		identifiers.push_back(Identifier(IdentifierType::identifier_unknown, identName));
}

ostream &operator<<(ostream &os, const IdentifiersTable &table) {
	for (size_t i = 0; i < table.identifiers.size(); i++) {
		os << "name: " << table.identifiers[i].getName();
		os << ", value: " << table.identifiers[i].getValue();
		os << ", type: " << table.identifiers[i].printType();
		os << ", declared: " << (table.identifiers[i].getDeclared() ? "yes" : "no") << endl;
	}

	return os;
}