# 0layer
```asm
debut:
    mov a,1
to100:
    mov b,a
    mod a,2
    beq a,1,odd
even:
    mov a,b
    add a,1
    bne a,100,to100
    beq a,100,fin
odd:
    # b
    jmp even
fin:
```
