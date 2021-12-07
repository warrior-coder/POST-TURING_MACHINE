#include <iostream>
#include <fstream>
#define TP_LENGTH 64
#define TP_ZERO TP_LENGTH / 2

using namespace std;

class Ex
{
	const char* info;
public:
	Ex(const char* info) : info(info) {}
	const char* what() const { return info; }
};

class Tape
{
	int head_pos;
	char tape[TP_LENGTH + 1];

public:
	Tape(const char* fname)
	{
		ifstream ifl(fname);

		if (ifl)
		{
			head_pos = 0;
			memset(tape, '0', TP_LENGTH);

			char s[TP_LENGTH];
			ifl.getline(s, TP_LENGTH, '\n');
			while (s[head_pos] != 'V') head_pos++;
			head_pos += TP_ZERO;

			ifl.getline(s, TP_LENGTH, '\n');
			int offset = 0;
			while (s[offset] == '0') offset++;
			head_pos -= offset;

			for (int i = 0; s[i + offset]; i++)
			{
				tape[TP_ZERO + i] = s[i + offset];
			}
			tape[TP_LENGTH] = '\0';
		}
		else
		{
			throw Ex("tape input");
		}
	}
	Tape& operator++()
	{
		++head_pos;
		if (head_pos >= TP_LENGTH) throw Ex("tape overflow");
		return *this;
	}
	Tape& operator--()
	{
		--head_pos;
		if (head_pos < 0) throw Ex("tape underflow");
		return *this;
	}
	char get_head() { return tape[head_pos]; }
	void set_head()
	{
		tape[head_pos] = '1';
	}
	void unset_head()
	{
		tape[head_pos] = '0';
	}
	void print_state(int radius = 8)
	{
		for (int i = 0; i < 2 + radius; i++) cout.put(' ');
		cout << "V" << endl;
		cout << "..";
		for (int i = head_pos - radius; i < head_pos + radius; i++)
		{
			cout.put(tape[i]);
		}
		cout << ".." << endl;
	}
	void save(const char* fname)
	{
		ofstream ofl(fname);

		for (int i = 0; i < head_pos; i++) ofl.put(' ');
		ofl << "V" << endl;
		ofl << tape << endl;
	}
};

class Command
{
	int i;
	int j1;
	int j2;
	char K;

public:
	Command(const char* str)
	{
		int l = 0;
		char wrd[8];
		int k;

		k = 0;
		while (isdigit(str[l]))
		{
			wrd[k++] = str[l++];
		}
		wrd[k] = '\0';
		i = atoi(wrd);

		K = str[l++];

		k = 0;
		while (isdigit(str[l]))
		{
			wrd[k++] = str[l++];
		}
		wrd[k] = '\0';
		j1 = atoi(wrd);

		if (str[l] == ',')
		{
			l++;

			k = 0;
			while (isdigit(str[l]))
			{
				wrd[k++] = str[l++];
			}
			wrd[k] = '\0';
			j2 = atoi(wrd);
		}
		else
		{
			j2 = -1;
		}
	}

	int get_i() { return i; }
	int get_j1() { return j1; }
	int get_j2() { return j2; }
	char get_K() { return K; }
};

class CommandList
{
	int size;
	Command** commands;

public:
	CommandList(const char* fname)
	{
		ifstream ifl(fname);

		char s[16];
		size = 0;
		ifl.getline(s, 16, '\n');
		while (strlen(s) > 0)
		{
			ifl.getline(s, 16, '\n');
			size++;
		}

		ifl.close();
		ifl.open(fname);

		commands = new Command*[size];
		for (int i = 0; i < size; i++)
		{
			ifl.getline(s, 16, '\n');
			commands[i] = new Command(s);
		}
	}
	Command* operator[](int i)
	{
		if (i < size && i >= 0) return commands[i];
		else return nullptr;
	}
	~CommandList()
	{
		for (int i = 0; i < size; i++)
		{
			delete[] commands[i];
			commands[i] = nullptr;
		}
		delete[] commands;
		commands = nullptr;
	}
};

class PostMachine
{
	Tape& tp;
	CommandList& cl;
	bool log;

public:
	PostMachine(Tape& tp, CommandList& cl, bool log = false) : tp(tp), cl(cl), log(log) {}

	void run()
	{
		int j = 0;

		if (log)
		{
			cout << "Tape states:" << endl;
			tp.print_state();
		}

		for (Command* cmd = cl[j]; cmd->get_K() != '!'; cmd = cl[j])
		{
			switch (cmd->get_K())
			{
			case '>':
			{
				++tp;
				j = cmd->get_j1();
			}
			break;

			case '<':
			{
				--tp;
				j = cmd->get_j1();
			}
			break;

			case 'V':
			{
				tp.set_head();
				j = cmd->get_j1();
			}
			break;

			case 'X':
			{
				tp.unset_head();
				j = cmd->get_j1();
			}
			break;

			case '?':
			{
				if (tp.get_head() == '0') j = cmd->get_j1();
				else j = cmd->get_j2();
			}
			break;
			}

			if (log) tp.print_state();

		}
	}
};


int main()
{
	Tape tp("tape.txt");
	CommandList cl("command_list.txt");
	PostMachine pm(tp, cl, true);

	pm.run();

	tp.save("result.txt");
	

	return 0;
}