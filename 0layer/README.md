# 0layer
```asm
% map print on odd numbers in range 0..100
debut:
    mov a,0
    mov b,0
loopto100:
    mod a,2
    beq a,0,incr
    call odd
incr:
    add a,1
    add b,1
    bne b,100,loopto100
    beq b,100,fin
odd:
    # b
    ret
fin:
```
