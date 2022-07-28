# 0layer
```asm
% fibonacci series to x
debut:
    mov x,95    % x-3 (up to 64bit integer overflow)
    mov p,0     % prev
    mov c,0     % current
    mov n,1     % next
    mov i,3
    # c
loop:
    add n,p     % next = current + prev
    mov p,c     % prev = current
    mov c,n     % current = next 
    # n
    add i,1
    bne i,x,loop
fin:
```
