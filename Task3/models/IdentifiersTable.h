#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Identifier.h"

class IdentifiersTable {
	std::vector<Identifier> identifiers; // вектор идентификаторов

public:
	size_t size() const; // получение размера таблицы

	void addByName(const std::string& identName); // добавление идентификатора по имени
	Identifier* getByName(const std::string& identName); // получение указателя на идентификатор по имени

	friend std::ostream &operator<<(std::ostream &stream, const IdentifiersTable &identifiersTable); // вывод таблицы в стандартный поток
};