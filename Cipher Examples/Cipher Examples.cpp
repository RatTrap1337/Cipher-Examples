#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

static std::string g_Alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?:;#'*+-/*§$%&()=");
static std::string g_ReversedAlphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?:;#'*+-/*§$%&()=");

struct Input_t {
	std::string m_Input = "";
	int m_Shift = 0, m_Key = 0;
};

class ITransformation {

public:
	virtual char operator()(char Char) = 0;

public:
	std::string m_Name;
};

class CCaesar : public ITransformation {

private:
	int m_Shift;

public:
	CCaesar(int Shift) : m_Shift(Shift) {
	
		m_Name = "Caesar";
	}

	char operator()(char Char) {

		if (isspace(Char)) {

			return ' ';
		}

		std::string::size_type found = g_Alphabet.find(Char);

		int shiftPos = (static_cast<int>(found) + m_Shift) % g_Alphabet.length();

		if (shiftPos < 0) {

			shiftPos = g_Alphabet.length() + shiftPos;
		}

		char shifted = g_Alphabet[shiftPos];

		return shifted;		
	}
};

class CXor : public ITransformation {

private:
	int m_Key;

public:
	CXor(int Key) : m_Key(Key) {

		m_Name = "Xor";
	}

	char operator()(char Char) {

		if (isspace(Char)) {

			return ' ';
		}

		auto xored = Char ^ m_Key;

		return xored;
	}
};

class CChao : public ITransformation {

private:
	bool m_Mode;
	std::string m_Left, m_Right;

public:
	//true == enc | false == dec
	CChao(bool Mode) : m_Mode(Mode), m_Left(g_Alphabet), m_Right(g_ReversedAlphabet) {

		m_Name = "Chao";
	}

	char operator()(char Char) {

		std::ranges::next_permutation(m_Left.begin(), m_Left.end());
		std::ranges::next_permutation(m_Right.begin(), m_Right.end());

		if (isspace(Char)) {

			return ' ';
		}

		if (m_Mode) {

			std::size_t pos = m_Right.rfind(Char);

			return m_Left[pos];
		}
			
		std::size_t pos = m_Left.rfind(Char);

		return m_Right[pos];
	}

};

//basicly std::transform template is unable to use abstract classes for some reason
template<typename Iterator>
Iterator Transform(Iterator First, Iterator Last, Iterator Output, ITransformation* Transformation) {

	while (First != Last) {

		*Output++ = Transformation->operator()(*First++);
	}

	return Output;
}
void ExecuteTransformation(const std::string& Input, ITransformation* Transformation) {

	std::string temp = Input;
	Transform(temp.begin(), temp.end(), temp.begin(), Transformation);
	std::cout << "Encrypted " << Transformation->m_Name << ":\n";
	std::cout << temp << std::endl;
	delete Transformation;
}

void ShowPermutation() {

	std::cout << "You can now enter a string for permutation.\n>>";

	std::string perm;  
	std::cin >> perm;

	do {

		std::cout << "Permutated: [" << perm << "]\n";

	} while (std::ranges::next_permutation(perm.begin(), perm.end()).found);
}

void BruteforcePassword() {

	std::cout << "You can now enter a random string with the following letters: abcdefghijk\nto see how you can use permutation for simple bruteforcing.\n>>"; 

	std::string password; //getchar() bugging?
	std::cin >> password;

	std::string alphabet = "abcdefghijk"; //shorten alphabet else it takes for ever
	auto length = password.length();
	bool found = false;

	do {

		std::cout << "Next Permutation: [" << alphabet << "]\n";

		auto temp = alphabet;

		std::vector<std::string> subStr;
		for (int i = 0; i <= temp.length() - length; i++) {

			subStr.push_back(temp.substr(i, length));
		}

		for (const auto& str : subStr) {

			if (!str.compare(password)) {

				found = true;
				std::cout << "Found password: " << str << "\n";
			}
		}

	} while (std::ranges::next_permutation(alphabet.begin(), alphabet.end()).found && !found);

	system("pause");
}

Input_t ParseInput() {

	Input_t temp;

	std::cout << "Input something you want to encrypt?\n>>";
	getline(std::cin, temp.m_Input);

	std::cout << "Shift-Amount for Caesar? (0 - " << g_Alphabet.length() << ")\n>>";
	std::cin >> temp.m_Shift;

	std::cout << "Key for Xor? (-128 - 127)\n>>";
	std::cin >> temp.m_Key;

	std::cout << "Using default declaration for Chao.\n";

	std::cout << "Plaintext:\n" << temp.m_Input << std::endl;

	return temp;
}

int main() {

	std::reverse(g_ReversedAlphabet.begin(), g_ReversedAlphabet.end());

	auto input = ParseInput();

	ExecuteTransformation(input.m_Input, new CCaesar(input.m_Shift));
	ExecuteTransformation(input.m_Input, new CXor(input.m_Key));
	ExecuteTransformation(input.m_Input, new CChao(true));

	ShowPermutation();
	BruteforcePassword();

	return EXIT_SUCCESS;
}
