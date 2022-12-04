#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <windows.h> 
using namespace std;

string message = "(콘솔메세지)";
int nextPage = 0;
int currentPage = 0;
bool isKeepGoing = true;
vector<char> wholeFile;
vector<char> pageFile;
int arguInt1, arguInt2, arguInt3;
string arguStr1, arguStr2, arguStr3;

vector<string> split(string input, char delimiter) {
	vector<string> result;
	stringstream ss(input);
	string tmp;

	while (getline(ss, tmp, delimiter)) result.push_back(tmp);

	return result;
}

class Shape { 
public:
	virtual void draw() = 0;

	void printWholeFile(int page, string msg) {
		system("cls");
		for (int i = 1; i <= 20; i++) {
			if (i < 10) {
				cout << " " << i << "| ";
			}
			else {
				cout << i << "| ";
			}
			int j = 0;
			while (true) {
				if (page == wholeFile.size() - 1) { break; }
				else if (j > 74) { break; }
				cout << wholeFile[page];
				j++;
				page++;
			}
			cout << endl;
		}
		cout << "-------------------------------------------------------------------------------" << endl;
		cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		cout << msg << endl;
		cout << "-------------------------------------------------------------------------------" << endl;

	}
};
class StartRead : public Shape {
public:
	void draw() {
		ifstream myfile1("test.txt");
		char c;
		while (myfile1.get(c)) {
			wholeFile.push_back(c);		
		}
		printWholeFile(currentPage, message);
	}
};
class StoreExit : public Shape {
public:
	void draw() {
		ofstream myfile2("test.txt");
		for (int i = 0; i < wholeFile.size(); i++) {
			myfile2 << wholeFile[i];
		}
		isKeepGoing = false;
		system("cls");
		cout << "저장 후 종료합니다." << endl;
	}
};

class Insert : public Shape {
public:
	void draw() {
		if (arguInt1 > 20 || arguInt2 >= 75) {
			message = "너무 큰 값을 입력하였습니다. 다시 입력해주세요.";
		}
		else {
			int location = currentPage + (arguInt1 - 1) * 75 + arguInt2;
			const char* str = arguStr1.c_str();
			for (int k = 0; k < arguStr1.length(); k++) {
				wholeFile.insert(wholeFile.begin() + location, str[k]);
				location++;
			}
			message = to_string(arguInt1) + "번째 줄의 " + to_string(arguInt2) + "번째 위치에 " + arguStr1 + "을 삽입하였습니다.";
		}
		printWholeFile(currentPage, message);
	}
};

class Delete : public Shape {
public:
	void draw() {
		if (arguInt1 > 20 || arguInt2 > 75) {
			message = "너무 큰 값을 입력하였습니다. 다시 입력해주세요.";
		}
		else {
			int location = currentPage + (arguInt1 - 1) * 75 + arguInt2;
			for (int k = 0; k < arguInt3; k++) {
				wholeFile.erase(wholeFile.begin() + location);
			}
			message = to_string(arguInt1) + "번째 줄의 " + to_string(arguInt2) + "번째 위치에서 " + to_string(arguInt3) + "바이트 만큼 제거하였습니다.";
		}
		printWholeFile(currentPage, message);
	}
};

class Search : public Shape {
public:
	void draw() {
		string beforeMessage = message;
		int tempLocation = 0;
		bool isNotFound = true;
		for (int i = 0; i < wholeFile.size(); i++) {
			bool match = true;
			if (wholeFile[i] == arguStr1.at(0)) {
				int point = i + 1;
				for (int j = 1; j < arguStr1.length(); j++) {
					if (wholeFile[point] != arguStr1.at(j)) {
						match = false;
						break;
					}
					point++;
				}
				if (match) {
					message = arguStr1 + "의 위치로 이동하였습니다.";
					isNotFound = false;
					currentPage = i;
					break;
				}
			}
		}
		if (isNotFound) {
			message = "텍스트 내에서 " + arguStr1 + "와 일치하는 단어를 찾을 수 없었습니다.";
		}
		printWholeFile(currentPage, message);
	}
};

class Change : public Shape {
public:
	void draw() {
		string beforeMessage = message;
		bool isNotFound = true;
		for (int i = 0; i < wholeFile.size(); i++) {
			bool match = true;
			if (wholeFile[i] == arguStr1.at(0)) {
				int point = i + 1;
				for (int j = 1; j < arguStr1.length(); j++) {
					if (wholeFile[point] != arguStr1.at(j)) {
						match = false;
						break;
					}
					point++;
				}
				if (match) {
					message = arguStr1 + "을 " + arguStr2 + "로 변환하였습니다.";
					isNotFound = false;
					for (int j = 0; j < arguStr1.length(); j++) {
						wholeFile.erase(wholeFile.begin() + i);
					}
					int location = i;
					for (int j = 0; j < arguStr2.length(); j++) {
						wholeFile.insert(wholeFile.begin() + location, arguStr1.at(j));
						location++;
					}
				}
			}
		}
		if (isNotFound) {
			message = "텍스트 내에서 " + arguStr1 + "와 일치하는 단어를 찾을 수 없었습니다.";
		}
		printWholeFile(currentPage, message);

	}
};

class NextRead : public Shape {
public:
	void draw() {
		currentPage += 1500;
		if (wholeFile.size() < currentPage + 1500) {
			currentPage = wholeFile.size() - 1500;
			message = "마지막 페이지입니다.";
		}
		else {
			message = "다음 페이지입니다.";
		}
		printWholeFile(currentPage, message);
	};
};

class BeforeRead : public Shape {
public:
	void draw() {
		int temp = currentPage - 1500;
		if (temp < 0) {
			message = "첫번째 페이지입니다.";
			currentPage = 0;
		}
		else {
			message = "이전 페이지입니다.";
			currentPage -= 1500;
		}
		printWholeFile(currentPage, message);
	};
};

class ErrorRead : public Shape {
public:
	void draw() {
		message = "잘못된 값을 입력하였습니다.";
		printWholeFile(currentPage, message);
	};
};

class AbstractFactory { 
public:
	virtual Shape* getType(string shapeType) = 0; 
};

class Factory : public AbstractFactory {
public:
	Shape* getType(string shapeType) {
		string instructionType = "x";
		if (shapeType.length() == 1) {
			if (shapeType.compare("t") == 0) {
				instructionType = "t";
			}
			else if (shapeType.compare("n") == 0) {
				instructionType = "n";
			}
			else if (shapeType.compare("p") == 0) {
				instructionType = "p";
			}
		}
		else {
			string iType = shapeType.substr(0, 1);
			if (iType.compare("i") == 0) {
				regex re("([i])([(])([0-9]{1,})([,])([0-9]{1,})([,])(.[^,]*)([)])");
				if (regex_match(shapeType, re)) {
					vector <string> result = split(shapeType, ',');
					stringstream ssInt(result[0].substr(2, result[0].length()));
					ssInt >> arguInt1;
					stringstream ssInt1(result[1]);
					ssInt1 >> arguInt2;
					arguStr1 = result[2].substr(0, result[2].length() - 1);
					instructionType = "i";
				}
			}
			else if (iType.compare("d") == 0) {
				regex re("([d])([(])([0-9]{1,})([,])([0-9]{1,})([,])([0-9]{1,})([)])");
				if (regex_match(shapeType, re)) {
					vector <string> result = split(shapeType, ',');
					stringstream ssInt(result[0].substr(2, result[0].length()));
					ssInt >> arguInt1;
					stringstream ssInt1(result[1]);
					ssInt1 >> arguInt2;
					stringstream ssInt2(result[2].substr(0, result[2].length() - 1));
					ssInt2 >> arguInt3;
					instructionType = "d";
				}
			}
			else if (iType.compare("s") == 0) {
				regex re("([s])([(])(.[^,]*)([)])");
				if (regex_match(shapeType, re)) {
					vector <string> result = split(shapeType, '(');
					arguStr1 = result[1].substr(0, result[1].length()-1);
					instructionType = "s";
				}
			}
			else if (iType.compare("c") == 0) {
				regex re("([c])([(])(.[^,]*)([,])(.[^,]*)([)])");
				if (regex_match(shapeType, re)) {
					vector <string> result = split(shapeType, ',');
					arguStr1 = result[0].substr(2, result[0].length());
					arguStr2 = result[1].substr(0, result[1].length()-1);
					instructionType = "c";
				}
			}
		}
		if (shapeType.compare("start") == 0) {
			return new StartRead();
		}
		else if (instructionType.compare("n") == 0) {
			return new NextRead();
		}
		else if (instructionType.compare("p") == 0) {
			return new BeforeRead();
		}
		else if (instructionType.compare("t") == 0) {
			return new StoreExit();
		}
		else if (instructionType.compare("i") == 0) {
			return new Insert();
		}
		else if (instructionType.compare("d") == 0) {
			return new Delete();
		}
		else if (instructionType.compare("s") == 0) {
			return new Search();
		}
		else if (instructionType.compare("c") == 0) {
			return new Change();
		}
		else {
			return new ErrorRead();
		}
	};
};

class FactoryProducer {
public:
	AbstractFactory* getFactory() {
		return new Factory();
	}
};

int main() {
	FactoryProducer* f = new FactoryProducer();
	AbstractFactory* shapeFactory = f->getFactory();
	Shape* firstShape = shapeFactory->getType("start");
	firstShape->draw();
	while (isKeepGoing) {
		cout << "입력 : ";
		string consoleMessage;
		cin >> consoleMessage;
		cout << "-------------------------------------------------------------------------------" << endl;
		Shape* shape = shapeFactory->getType(consoleMessage);
		shape->draw();
	}
	return 0;
}
