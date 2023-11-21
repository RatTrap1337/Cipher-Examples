#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

static std::string g_Alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?:;#'*+-/*�$%&()=");
static std::string g_ReversedAlphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?:;#'*+-/*�$%&()=");

class ITransformation {

public:
	virtual char operator()(char Char) = 0;
};

class Caesar : public ITransformation {

private:
	int m_Shift;

public:
	Caesar(int Shift) : m_Shift(Shift) { }

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

class Xor : public ITransformation {

private:
	int m_Key;

public:
	Xor(int Key) : m_Key(Key) { }

	char operator()(char Char) {

		if (isspace(Char)) {

			return ' ';
		}

		auto xored = Char ^ m_Key;

		return xored;
	}
};

class Chao : public ITransformation {

private:
	bool m_Mode;
	std::string m_Left, m_Right;

public:
	//true == enc | false == dec
	Chao(bool Mode) : m_Mode(Mode), m_Left(g_Alphabet), m_Right(g_ReversedAlphabet) { }

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

int main() {

	std::reverse(g_ReversedAlphabet.begin(), g_ReversedAlphabet.end());

	std::string input;
	int shift = 0, key = 0;

	std::cout << "Which text is to be encrypted?\n";
	getline(std::cin, input);

	std::cout << "shift amount for caesar? (0-" << g_Alphabet.length() << ")\n";
	std::cin >> shift;

	std::cout << "key for xor?\n";
	std::cin >> key;

	std::cout << "before encryption:\n" << input << std::endl;

	auto caesar = input;
	std::transform(caesar.begin(), caesar.end(), caesar.begin(), Caesar(shift));
	std::cout << "encrypted caesar:\n";
	std::cout << caesar << std::endl;

	auto xored = input;
	std::transform(xored.begin(), xored.end(), xored.begin(), Xor(key));
	std::cout << "encrypted xor:\n";
	std::cout << xored << std::endl;

	auto chao = input;
	std::transform(chao.begin(), chao.end(), chao.begin(), Chao(true));
	std::cout << "encrypted chao:\n";
	std::cout << chao << std::endl;

	std::string perm;
	std::cout << "You can now enter a string for permutation.\n";

	std::cin >> perm;
	do {

		std::cout << "Permutated: [" << perm << "]\n";

	} while (std::ranges::next_permutation(perm.begin(), perm.end()).found);

	std::cout << "Using permutation for bruteforcing.\n";

	std::string password = "unsafe";
	std::string alphabet = "aefklnurs"; //shorten else it takes for ever
	auto length = 6;
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
				std::cout << "Found password: " << str;
			}
		}

	} while (std::ranges::next_permutation(alphabet.begin(), alphabet.end()).found && !found);

	return 0;
}
