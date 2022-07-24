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

=== church-encoded reals ===
Nil() = 0
S(Nil()) = 1
S(S(Nil())) = 2
S(S(S(Nil()))) = 3
S(S(S(S(Nil())))) = 4
S(S(S(S(S(Nil()))))) = 5
S(S(S(S(S(S(Nil())))))) = 6
S(S(S(S(S(S(S(Nil()))))))) = 7
S(S(S(S(S(S(S(S(Nil())))))))) = 8
S(S(S(S(S(S(S(S(S(Nil()))))))))) = 9
S(S(S(S(S(S(S(S(S(S(Nil())))))))))) = 10
```
