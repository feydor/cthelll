# bf2c
brainfuck to C transpiler using the following optimizations:
- constant folding of consecutive ptr shifts or mutations, `++++----- = *ptr-=1`
- clear cell idiom, `[-] = *ptr=0`

```b
[obfuscated fibonacci series]
[i=9: 0 1 1 2 3 5 8 = E R g ]
>>>>>+>>>+++++++++<<<<<<<<++++++++[>>++++++<<-]>>.<<++++++++[>>------<<-]>>>>>>>>>++++++++++++++++++++++++++++++++.[-]
<<<<<<<<<++++++++[>>>>>++++++<<<<<-]>>>>>.<<<<<++++++++[>>>>>------<<<<<-]>>>>>>>>>++++++++++++++++++++++++++++++++.[-]
<[<<<[>+>+<<-]<<<[>+>+<<-]>[>>>+<<<-]>>>>[<<<<+>>>>-]<[<+>-]<<<[<+>-]<<[-]>>>[<<<+>>>-]>>>><<<<<<<<++++++++
[>>>>>++++++<<<<<-]>>>>>.<<<<<++++++++[>>>>>------<<<<<-]>>>>>>>>>++++++++++++++++++++++++++++++++.[-]<-]>++++++++++.[-]<
```
