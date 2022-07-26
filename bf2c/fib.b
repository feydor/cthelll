[ith terms of fibonacchi series]
cell 1 = inner loop counter
cell 2 = p
cell 3 = c
cell 4 = c'
cell 5 = c''
cell 6 = n
cell 7 = n'
cell 8 = n''
cell 9 = i
cell 10 = inner loop counter & special char printer

each iteration:
p' = c
c' = n
n' = c plus n

>                       p=0
> > >                   c=c'=c''=0
> + > >                 n=1 n'=n''=0
> ++++ +++ ++           i=9

print the first numbers in the series
note: always return to cell 9 (i)
print c using cell 1 as counter
< < < < < <
<< ++++ ++++
[
    >> +++ +++
    << -
]
>> .
<< ++++ ++++
[
    >> --- ---
    << -
]
>>
> > > > > >

print space and clear cell
>++++++++ ++++++++ ++++++++ ++++++++.[-]<

print n using cell 1 as counter
< < <
<<<<< ++++ ++++
[
    >>>>> +++ +++
    <<<<< -
]
>>>>> .
<<<<< ++++ ++++
[
    >>>>> --- ---
    <<<<< -
]
>>>>>
> > >
    
print space and clear cell
>++++++++ ++++++++ ++++++++ ++++++++.[-]<
[ 
    copy n to n' and n'' clearing and returning to n
    < < <
    [
        > + > +
        < < -
    ]

    copy c to c' and c'' clearing and returning to c
    < < <
    [
        > + > +
        < < -
    ]

    add c' into n' clearing and returning to c'
    >
    [
        > > > +
        < < < -
    ]
    n' = n plus c
    
    add n'' into c' clearing and returning to n''
    > > > >
    [
        < < < < +
        > > > > -
    ]
    c' = n

    n = 0 // n'=n plus c // n'' = 0 // c = 0 // c' = n // c'' = c = p
    now we need to move the results back to their original registers
    add n' into n
    add c' into c
    clear p then add c'' into p
    
    add n' into n and return to n'
    <
    [
        < +
        > -
    ]

    add c' into c and return to c'
    < < <
    [
        < +
        > -
    ]
    
    move to p / clear it / then add c'' into p returning to c''
    < < [-]
    > > >
    [
        < < < +
        > > > -
    ]
    
    c'' to i
    > > > >
    
    print n using cell 1 as counter
    '0' to '9' is 48 to 57
    < < <
    <<<<< ++++ ++++
    [
        >>>>> +++ +++
        <<<<< -
    ]
    >>>>> .
    <<<<< ++++ ++++
    [
        >>>>> --- ---
        <<<<< -
    ]
    >>>>>
    > > >
    
    print space and clear cell
    >++++++++ ++++++++ ++++++++ ++++++++.[-]<
     
    -
]
print\n
>++++++++++.[-]<
