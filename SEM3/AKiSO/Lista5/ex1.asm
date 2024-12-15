global _start

section .data
newline db 0xA

section .bss
	buffer resb 100;
	result resb 100;

section .text
	global _start

_start:
	mov eax, 3
	mov ebx, 0
	mov ecx, buffer
	mov edx, 100
	int 0x80

	mov esi, buffer
	xor edx, edx
loop:
	mov al, [esi]
	cmp al, 10
	je loop_done

	sub al, '0'
	add dl, al
	inc esi
	jmp loop

loop_done:
	mov eax, edx
	xor ebx, ebx

convert_ascii:
	call divideby10
	push edx
	test eax, eax
	jz convert_done

	xor ecx, ecx
	jmp convert_ascii


convert_done:
	xor ecx, ecx
	xor edx, edx
	jmp retriever

divideby10:
	push ebp
	mov ebp, esp
	add ebx, 1

	mov ecx, 10
	xor edx, edx

	div ecx
	add dl, '0'

	mov esp, ebp
	pop ebp
	ret

retriever:
	cmp ecx, ebx
	je retriever_done

	pop edx
	mov [result+ecx], edx
	inc ecx
	jmp retriever

retriever_done:
	mov edx, 10
	inc ecx
	inc ebx
	mov [result+ecx], edx
	jmp end
end:
	mov eax, 4
	mov edx, ebx
	mov ebx, 1
	mov ecx, result
	int 0x80

	mov eax, 4
	mov edx, ebx
	mov ebx, 1
	lea ecx, [newline]
	int 0x80

	mov eax, 1
	mov ebx, 0
	int 0x80
