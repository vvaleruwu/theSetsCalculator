// Calculator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
#include <set>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

set<variant<int, string>> Intersection(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b);

set<variant<int, string>> Subtraction(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b);

set<variant<int, string>> Union(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b);

set<variant<int, string>> Complement(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& universalSet);

set<variant<int, string>> SymmetricSubtraction(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b);

void enterSet(set<variant<int, string>>&newSet, map <int, set<variant<int, string>>> &sets, int &count);
int pickOperation();
void showSets(const map<int, set<variant<int, string>>>& sets);
void showOperations();
bool checkCorrectOfTheExpressionByOperations(
	const string& expression,
	const map<int, set<variant<int, string>>>& sets);
void showTheExpression();
set<variant<int, string>> calculateExpression(
	const string& expression,
	map<int, set<variant<int, string>>>& sets
);
int getPriority(char operation);

// Универсальное множество = объединение всех введённых множеств
set<variant<int, string>> getUniversalSet(
	const map<int, set<variant<int, string>>>& sets);

void applyOperation(
	stack<set<variant<int, string>>>& values,
	char operation,
	const set<variant<int, string>>& universalSet
);

int main()
{
	setlocale(LC_ALL, "ru");
	map <int, set<variant<int, string>>> sets;
	int count = 1;
	while (true)
    {
		int choice=0;
		cout << "Введите множество: ";
		set<variant<int, string>>set;
		enterSet(set, sets, count);
			cout << "Хотите ввести еще одно множество?\n1.Да\n2.Нет\nПри вводе иных вариантов вы автоматически закончите ввод множеств\n";
			cin >> choice;
			if (choice == 1) {}
			else {
				break;
			}
    }

	string expression;

	while (true) {
		showSets(sets);
		showOperations();
		cout << "Введите выражение с множествами в строку, используя для обозначения множеств вышепоказанные числа и обозначения операций.\n";
		cout << "Пример выражения: (1+2)*3-4^(!5+6)"<<endl;
		// Читаем всю строку целиком, чтобы выражение можно было вводить с пробелами
		if (cin.peek() == '\n') cin.ignore();
		getline(cin, expression);
		string withoutSpaces;
		for (char symbol : expression)
		{
			if (symbol != ' ' && symbol != '\t' && symbol != '\r')
			{
				withoutSpaces += symbol;
			}
		}
		expression = withoutSpaces;
		if (!checkCorrectOfTheExpressionByOperations(expression, sets)){
			cout << "Некорректный ввод операций, попробуйте снова!\n";
		}
		else {
			break;
		}
	}

	set<variant<int, string>> result =
		calculateExpression(expression, sets);

	cout << "\nРезультат: { ";

	for (const auto& element : result)
	{
		if (holds_alternative<int>(element))
		{
			cout << get<int>(element) << " ";
		}
		else
		{
			cout << get<string>(element) << " ";
		}
	}

	cout << "}\n";
}

set<variant<int, string>> Intersection(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b)
{
	set<variant<int, string>> result;
	unordered_map <variant<int, string>, int> currentResult;
	for (const auto& element : a)
	{
		currentResult.insert({ element, 1 });
	}
	for (const auto& element : b) {

		if (currentResult.contains(element))
		{
			currentResult[element] = 0;
		}
	}
	for (const auto& element : currentResult) {

		if (element.second == 0) {
			result.insert(element.first);
		}
	}

	return result;
}
set<variant<int, string>> Subtraction(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b) {
	set<variant<int, string>> result;
	unordered_map <variant<int, string>, int> currentResult;
	for (const auto& element : a)
	{
		currentResult.insert({ element, 1 });
	}
	for (const auto& element : b) {

		if (currentResult.contains(element))
		{
			currentResult[element] = 0;
		}
	}
	for (const auto& element : currentResult) {

		if (element.second == 1) {
			result.insert(element.first);
		}
	}

	return result;
}

set<variant<int, string>> Union(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b) {
	set<variant<int, string>> result;
	for (const auto& element : a)
	{
		result.insert(element);
	}
	for (const auto& element : b) {

		result.insert(element);
	}

	return result;
}

// Дополнение: !A = U \ A, где U - универсальное множество
// (все значения, которые были введены в программу)
set<variant<int, string>> Complement(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& universalSet) {
	set<variant<int, string>> result;
	unordered_map <variant<int, string>, int> currentResult;
	for (const auto& element : universalSet)
	{
		currentResult.insert({ element, 1 });
	}
	for (const auto& element : a) {


		if (currentResult.contains(element))
		{
			currentResult[element] = 0;
		}
	}
	for (const auto& element : currentResult) {

		if (element.second == 1) {
			result.insert(element.first);
		}
	}

	return result;
}

// Симметрическая разность: A ^ B = (A \ B) + (B \ A)
set<variant<int, string>> SymmetricSubtraction(
	const set<variant<int, string>>& a,
	const set<variant<int, string>>& b) {
	return Union(Subtraction(a, b), Subtraction(b, a));
}

set<variant<int, string>> getUniversalSet(
	const map<int, set<variant<int, string>>>& sets)
{
	set<variant<int, string>> universalSet;
	for (const auto& currentSet : sets)
	{
		universalSet = Union(universalSet, currentSet.second);
	}
	return universalSet;
}
void enterSet(
	set<variant<int, string>>& newSet,
	map<int, set<variant<int, string>>>& sets,
	int& count)
{
	cout << "\nВводите элементы множества через пробел." << endl;
	cout << "Для завершения ввода введите end:" << endl;

	string input;

	while (cin >> input)
	{
		if (input == "end")
		{
			break;
		}
		bool isNumber = true;
		int start = 0;
		if (input[0] == '-')
		{
			start = 1;
		}

		if (start == input.length())
		{
			isNumber = false;
		}
		for (int i = start; i < input.length(); i++)
		{
			if (!isdigit(input[i]))
			{
				isNumber = false;
				break;
			}
		}

		if (isNumber)
		{
			newSet.insert(stoi(input));
		}
		else
		{
			newSet.insert(input);
		}
	}

	sets[count] = newSet;

	cout << "Множество сохранено под номером " << count << endl;

	count++;
}

int getPriority(char operation)
{
	switch (operation)
	{
	case '!':
		return 3; 

	case '*':
		return 2; 

	case '+':
	case '-':
	case '^':
		return 1; 

	default:
		return 0;
	}
}

void applyOperation(
	stack<set<variant<int, string>>>& values,
	char operation,
	const set<variant<int, string>>& universalSet
)
{
	// Отладочный вывод (при необходимости можно раскомментировать)
	// cout << "\nDEBUG applyOperation: " << operation << endl;
	// cout << "Количество множеств в стеке: " << values.size() << endl;

	// Дополнение - унарная операция, берёт из стека одно множество
	if (operation == '!')
	{
		if (values.empty())
		{
			cout << "ОШИБКА: нет множества для операции дополнения!" << endl;
			return;
		}

		auto a = values.top();
		values.pop();

		values.push(Complement(a, universalSet));
		return;
	}

	if (values.size() < 2)
	{
		cout << "ОШИБКА: в стеке меньше двух множеств!" << endl;
		return;
	}

	auto b = values.top();
	values.pop();

	auto a = values.top();
	values.pop();

	set<variant<int, string>> result;

	switch (operation)
	{
	case '+':
		result = Union(a, b);
		break;

	case '*':
		result = Intersection(a, b);
		break;

	case '-':
		result = Subtraction(a, b);
		break;

	case '^':
		result = SymmetricSubtraction(a, b);
		break;
	}

	values.push(result);
}
set<variant<int, string>> calculateExpression(
	const string& expression,
	map<int, set<variant<int, string>>>& sets
)
{
	stack<set<variant<int, string>>> values;
	stack<char> operations;
	const set<variant<int, string>> universalSet = getUniversalSet(sets);

	for (int i = 0; i < expression.length(); i++)
	{
		if (isdigit(expression[i]))
		{
			int number = 0;

			while (i < expression.length() && isdigit(expression[i]))
			{
				number = number * 10 + (expression[i] - '0');
				i++;
			}
			i--;

			values.push(sets[number]);
		}

		else if (expression[i] == '(')
		{
			operations.push('(');
		}

		else if (expression[i] == '!')
		{
			// Префиксная унарная операция: просто кладём в стек,
			// она применится к следующему за ней операнду
			operations.push('!');
		}

		else if (expression[i] == ')')
		{
			while (!operations.empty() && operations.top() != '(')
			{
				char operation = operations.top();
				operations.pop();

				applyOperation(values, operation, universalSet);
			}
			operations.pop();
		}

		else if (
			expression[i] == '+' ||
			expression[i] == '-' ||
			expression[i] == '*' ||
			expression[i] == '^'
			)
		{
			while (
				!operations.empty() &&
				operations.top() != '(' &&
				getPriority(operations.top()) >= getPriority(expression[i])
				)
			{
				char operation = operations.top();
				operations.pop();

				applyOperation(values, operation, universalSet);
			}

			operations.push(expression[i]);
		}
	}

	while (!operations.empty())
	{
		char operation = operations.top();
		operations.pop();
		applyOperation(values, operation, universalSet);
	}

	return values.top();
}
void showSets(const map<int, set<variant<int, string>>>& sets)
{
	cout << "\nДоступные множества:\n";

	for (const auto& currentSet : sets)
	{
		cout << currentSet.first << " = { ";

		for (const auto& element : currentSet.second)
		{
			if (holds_alternative<int>(element))
			{
				cout << get<int>(element);
			}
			else
			{
				cout << get<string>(element);
			}

			cout << " ";
		}

		cout << "}\n";
	}

	cout << endl;
}
void showOperations()
{
	cout << "\nДоступные операции:\n";
	cout << "+  - объединение\n";
	cout << "*  - пересечение\n";
	cout << "-  - разность\n";
	cout << "^  - симметрическая разность\n";
	cout << "!  - дополнение\n";
	cout << "( ) - изменение порядка выполнения операций\n\n";
}
bool checkCorrectOfTheExpressionByOperations(
	const string& expression,
	const map<int, set<variant<int, string>>>& sets)
{
	if (expression.empty())
	{
		return false;
	}

	int brackets = 0;
	// true - сейчас ожидается множество (число, '(' или '!'),
	// false - ожидается бинарная операция или ')'
	bool expectOperand = true;

	for (int i = 0; i < expression.length(); i++)
	{
		char symbol = expression[i];

		if (isdigit(symbol))
		{
			if (!expectOperand)
			{
				cout << "Пропущена операция перед множеством!\n";
				return false;
			}
			expectOperand = false;

			int number = 0;

			while (i < expression.length() && isdigit(expression[i]))
			{
				number = number * 10 + (expression[i] - '0');
				i++;
			}

			i--;

			if (sets.find(number) == sets.end())
			{
				cout << "Множества с номером " << number
					<< " не существует!\n";

				return false;
			}
		}

		else if (symbol == '(')
		{
			if (!expectOperand)
			{
				cout << "Пропущена операция перед скобкой!\n";
				return false;
			}
			brackets++;
		}

		else if (symbol == ')')
		{
			if (expectOperand)
			{
				cout << "Пустые скобки или операция без второго множества!\n";
				return false;
			}
			brackets--;
			if (brackets < 0)
			{
				cout << "Некорректно расставлены скобки!\n";
				return false;
			}
		}

		else if (symbol == '!')
		{
			// Дополнение пишется перед множеством: !1, !(1+2), !!3
			if (!expectOperand)
			{
				cout << "Дополнение '!' ставится перед множеством, например !1\n";
				return false;
			}
		}

		else if (
			symbol == '+' ||
			symbol == '-' ||
			symbol == '*' ||
			symbol == '^')
		{
			if (expectOperand)
			{
				cout << "Операции '" << symbol << "' не хватает множества слева!\n";
				return false;
			}
			expectOperand = true;
		}

		else
		{
			cout << "Недопустимый символ: " << symbol << endl;
			return false;
		}
	}

	if (brackets != 0)
	{
		cout << "Некорректно расставлены скобки!\n";
		return false;
	}

	if (expectOperand)
	{
		cout << "Выражение не может заканчиваться операцией!\n";
		return false;
	}

	return true;
}