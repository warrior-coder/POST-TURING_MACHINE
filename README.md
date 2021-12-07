# POST-TURING MACHINE

A Post-Turing machine is a type of abstract machine (very similar to Turing machine, but they were developed independently). The Post–Turing machine uses a binary alphabet, an infinite sequential binary storage, called a tape, and a primitive programming language with instructions for movement among the tape and changing values on it.


### Commands alphabet

* i>j     – head move right
* i<j     – head move left
* iVj     – set 1
* iXj     – set 0
* i?j1,j2 – if 0 goto j1, else goto j2
* i!      – end of program

Where i – current command number, j – next command number.


### Example of work
An example of a Post-Turing machine program that increments a number by one.

###### tape.txt
```TXT
        V
0000000011100000000
```


###### command_list.txt
```TXT
0>1
1?2,0
2V3
3!
```


###### project.cpp
```CPP
int main()
{
	Tape tp("tape.txt");
	CommandList cl("command_list.txt");
	PostMachine pm(tp, cl);

	pm.run();
	tp.save("result.txt");

	return 0;
}
```


###### result.txt
```TXT
                                   V
0000000000000000000000000000000011110000000000000000000000000000

```
