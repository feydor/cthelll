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

                        x=0
>                       p=0
> > >                   c=c'=c''=0
> + > >                 n=1 n'=n''=0
> ++++ +++ ++            i=9

print the first three numbers in the series

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
    
>++++++++ ++++++++ ++++++++ ++++++++.[-]<       print space and clear cell

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
    
>++++++++ ++++++++ ++++++++ ++++++++.[-]<       print space and clear cell

[ 
    copy n to n' and n'' clearing n
    < < <
    [
        > + > +
        < < -
    ]
    n' and n'' are copies of n
    at n

    copy c to c' and c'' clearing c
    < < <
    [
        > + > +
        < < -
    ]
    at c (cell 3)

    add c' into n' clearing c'
    >
    [
        > > > +
        < < < -
    ]
    n' = n plus c
    at c'
    
    add n'' into c' clearing n''
    > > > >
    [
        < < < < +
        > > > > -
    ]
    c' = n
    at n''

    n = 0 // n'=n plus c // n'' = 0 // c = 0 // c' = n // c'' = c = p
    now we need to move these numbers back to their original registers
    add n' into n
    add c' into c
    clear p then add c'' into p

    < at n'
    [
        < +
        > -
    ]
    at n'

    < < <  at c'  
    [
        < +
        > -
    ]
    at c'
    
    clear p
    < < [-]
    > > >     at c''
    [
        < < < +
        > > > -
    ]
    at c''
    
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
    
    >++++++++ ++++++++ ++++++++ ++++++++.[-]<       print space and clear cell
     
    -
]
>++++++++++.[-]<       print \n then clear cell
