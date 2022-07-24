# λ
church-encoding primitives and higher-order functions

```
=== church-encoded logic primitives ===
False(T, F) = F
True(T, F) = T
Not(True(T, F)) = F
And(True(T,F), True(T,F)) = T
And(True(T,F), False(T,F)) = F
And(False(T,F), True(T,F)) = F
And(False(T,F), False(T,F)) = F
Or(True(T,F), True(T,F)) = T
Or(True(T,F), False(T,F)) = T
Or(False(T,F), True(T,F)) = T
Or(False(T,F), False(T,F)) = F
Xor(True(T,F), True(T,F)) = F
Xor(True(T,F), False(T,F)) = T
Xor(False(T,F), True(T,F)) = T
Xor(False(T,F), False(T,F)) = F

===== church-encoded reals =====
===== C(f, x) => f(x)
Nil() = 0
C(S, Nil()) = 1
C(S, C(S, Nil())) = 2
C(S, C(S, C(S, Nil()))) = 3
C(S, C(S, C(S, C(S, Nil())))) = 4
C(S, C(S, C(S, C(S, C(S, Nil()))))) = 5
C(S, C(S, C(S, C(S, C(S, C(S, Nil())))))) = 6
C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil()))))))) = 7
C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil())))))))) = 8
C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil()))))))))) = 9
...
using the general definition:
=== n = λf.λx.f∘n x
CN(S, Nil(), 333) = 333

===== arithmetics on church-encoded reals =====
===== plus = λm.λn.λf.λx.f∘m f∘n x
Plus(CN, CN(S, Nil(), 2), CN(S, Nil(), 3), Nil()) = 5
Succ(x:1) = Plus(CN, x:1, C(S, Nil()), Nil()) = 2
Succ(CN, Nil(), CN(S, Nil(), 30)) = 31
Cons(33, 11) = (33, 11)
Car(Cons(33, 11)) = 33
Cdr(Cons(33, 11)) = 11
```
