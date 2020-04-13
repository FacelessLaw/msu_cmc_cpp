#pragma once

enum class LexemeType {
	unknown = -1, // неизвестная лексема
	null, // пустота
	comment, // комментарий
	keyword, // ключевое слово
	delimeter, // разделитель
	identifier, // идентификатор

	// объявление типа данных
	datatype_int, // целый
	datatype_real, // вещественный
	datatype_string, // строковый
	datatype_boolean, // логический
	
	// константы
	constant_int, // целочисленная
	constant_real, // вещественная
	constant_string, // строковая
	constant_boolean, // логическая

	// полиз
	rpn_minus, // унарный минус
	rpn_go, // перехож
	rpn_fgo, // переход по лжи
	rpn_label, // метка
	rpn_address // адрес перехода
};

enum class IdentifierType {
	identifier_unknown, // неизвестный идентификатор
	identifier_int, // целочисленный идентификатор
	identifier_real, // вещественный идентификатор
	identifier_string, // строковый идентификатор
	identifier_boolean // логический идентификатор
};
