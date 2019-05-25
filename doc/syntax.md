# Skitter VM Syntax

Below is a summary of the VM syntax followed by examples.

## Example

```
.TEXT
_start:
	call @main
	mov %bx %rx
	hlt

main:
	push $x
	call @square
	ret

square:
	mov %ax [%bp - 4]
	mul %ax %ax
	mov %rx %ax
	ret

.DATA
x: dd #5
```