global _start

section .data
newline db 0xA

section .bss
	buffer resb 100;
	result resb 100;

section .text
	global _start

;Reads from STDIN
_start:
	mov eax, 3
	mov ebx, 0
	mov ecx, buffer
	mov edx, 100
	int 0x80

	mov esi, buffer
	xor edx, edx
	jmp loop

next_char_base:
	jmp loop

skip_space_base:
	jmp next_char_base

;loop adding all elements from input
loop:
	mov al, [esi]
	mov ecx, esi
	cmp al, 10
	je loop_done
	cmp al, 0
	je loop_done

	cmp al, ' '
	je skip_space_base

	push ecx
	call ascii_to_int
	add edx, eax
	jmp next_char_base

next_digit:
	inc esi
	jmp loop

loop_done:
	mov eax, edx
	xor ebx, ebx
	xor ecx, ecx
	call print_int_to_ascii
	jmp diagonal_sum ;Jumps to fnc summing number on the diagonal

;Sum of diagonal elements
diagonal_sum:
	mov esi, buffer
	xor eax, eax
	xor edx, edx
	xor ecx, ecx
	xor ebx, ebx
	jmp diagonal_loop

diagonal_loop: ; we check for index 0, 4, 8
	mov al, [esi]
	mov ecx, esi
	cmp al, 10
	je diagonal_loop_done
	cmp al, 0
	je diagonal_loop_done
	cmp al, ' '
	je diag_skip_space_base


	push ecx
	call ascii_to_int

	cmp edi, 0
	je on_diagonal

	cmp edi, 4
	je on_diagonal

	cmp edi, 8
	je on_diagonal

	inc edi
	jmp diagonal_loop

on_diagonal:
	inc edi
	add edx, eax
	jmp diag_next_char_base

diag_next_char_base:
	jmp diagonal_loop

diag_skip_space_base:
	jmp diag_next_char_base


diagonal_loop_done:
	mov eax, edx
	xor ebx, ebx
	call print_int_to_ascii
	jmp end_program

;END of diagonal summing

; BEGIN of function converting ascii to int
; RETURNS VALUE IN eax

ascii_to_int:
	push ebp
	mov ebp, esp
	mov ebx, [ebp+8]
	xor eax, eax
	xor ecx, ecx

	jmp ascii_loop

next_char:
	inc ebx
	inc esi
	jmp ascii_loop

skip_space:
	jmp next_char

ascii_loop:
	mov cl, [ebx]
	cmp cl, ' '
	je ascii_to_int_done
	cmp cl, 10
	je ascii_to_int_done

	sub cl, '0'
	imul eax, eax, 10
	movzx ecx, cl
	add eax, ecx

	jmp next_char

ascii_to_int_done:
	inc esi
	mov esp, ebp
	pop ebp
	ret
; END of function converting ascii to int 

; BEGIN function prints int kept in eax
print_int_to_ascii:
	push ebp
	mov ebp, esp
	jmp convert_ascii

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

	mov esp, ebp
	pop ebp
	ret
; END of function printing int value as ASCII
end_program:
	mov eax, 1
	mov ebx, 0
	int 0x80
