#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


#define L_LENGTH 64
#define L_ZERO L_LENGTH / 2

void parse_command(char* command, char& cmd, int& second, int& third)
{
	int i = 0;
	char wrd[8];
	int k;


	{
		k = 0;
		while (isdigit(command[i]))
		{
			i++;
		}
	}

	{
		cmd = command[i];
		i++;
	}

	{
		k = 0;
		while (isdigit(command[i]))
		{
			wrd[k] = command[i];
			k++;
			i++;
		}
		wrd[k] = '\0';
		second = atoi(wrd);
	}

	if (command[i] == ',')
	{
		i++;

		k = 0;
		while (isdigit(command[i]))
		{
			wrd[k] = command[i];
			k++;
			i++;
		}
		wrd[k] = '\0';
		third = atoi(wrd);
	}
	else third = -1;

	cout << cmd << endl;
	cout << second << endl;
	cout << third << endl;

}

class Ex
{
	const char* info;
public:
	Ex(const char* info) : info(info) {}
	const char* what() const { return info; }
};



int main()
{
	// 1 read line
	ifstream ifl;

	ifl.open("line.txt");

	int cr_pos = 0;
	char line[L_LENGTH + 1];
	memset(line, '0', L_LENGTH);

	char buf[64];

	ifl.getline(buf, 64, '\n');
	while (buf[cr_pos] != 'V') cr_pos++;
	cr_pos += L_ZERO;

	ifl.getline(buf, 64, '\n');
	int offset = 0;
	while (buf[offset] == '0') offset++;
	cr_pos -= offset;

	for (int i = 0; buf[i+offset] != '\0'; i++)
	{
		line[L_ZERO + i] = buf[i+offset];
	}
	line[L_LENGTH] = '\0';

	ifl.close();

	// 2 read commands
	ifl.open("commands.txt");

	int size = 0;
	ifl.getline(buf, 64, '\n');
	while (strlen(buf) > 0)
	{
		ifl.getline(buf, 64, '\n');
		size++;
	}

	ifl.close();
	ifl.open("commands.txt");

	char** commands = new char* [size];
	for (int i = 0; i < size; i++)
	{
		commands[i] = new char[16];
		ifl.getline(commands[i], 16, '\n');
		cout << commands[i] << endl;
	}

	// 3 execute
	int first, second, third;
	char cmd = NULL;
	bool loop = true;

	first = 0;
	while (loop)
	{
		parse_command(commands[first], cmd, second, third);

		if (cmd == '>')
		{
			cr_pos++;
			first = second;
		}
		else if (cmd == '<')
		{
			cr_pos--;
			first = second;
		}
		else if (cmd == 'V')
		{
			line[cr_pos] = '1';
			first = second;
		}
		else if (cmd == 'X')
		{
			line[cr_pos] = '0';
			first = second;
		}
		else if (cmd == '?')
		{
			if (line[cr_pos] == '0') first = second;
			else first = third;
		}
		else
		{
			loop = false;
		}

		if (cr_pos < 0 || cr_pos >= L_LENGTH) throw Ex("out of line");
		
	}

	// 4 print result
	ofstream ofl("res.txt");
	memset(buf, ' ', 64);
	buf[cr_pos] = 'V';
	buf[cr_pos+1] = '\0';

	ofl << buf << endl;
	ofl << line << endl;



	return 0;
}