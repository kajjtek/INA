global _start

section .data
newline db 0xA

section .bss
	buffer resb 100;
	result resb 4;

section .text
	global _start

;Reads from STDIN
_start:
	mov esi, 2
	mov eax, 2
	call print_int_to_ascii

main_loop:
	cmp esi, 100000  ; MAX value 1000
	je main_done
	inc esi

	push esi
	call isPrime ; jesli jest prime zwraca 1 jesli nie to zwraca 0 w edi
	cmp edi, 0
	je not_prime
	mov eax, esi
	call print_int_to_ascii

	jmp main_loop

not_prime:
	jmp main_loop

isPrime:
	push ebp
	mov ebp, esp

	mov ebx, [ebp+8]
	mov eax, ebx
	mov edx, ebx
	mov edi, ebx
	mov ecx, 2
	xor edx, edx
	div ecx ; eax trzyma quotient
	mov ebx, eax
	add ebx, 1
	mov eax, edx
	jmp check_loop

check_loop:
	cmp ecx, ebx
	jg check_done

	mov eax, edi
	xor edx, edx
	div ecx
	cmp edx, 0
	je isDivisible

	inc ecx
	jmp check_loop

isDivisible:
	mov edi, 0
	mov esp, ebp
	pop ebp
	ret

check_done:
	mov edi, 1
	mov esp, ebp
	pop ebp
	ret

; BEGIN function prints int kept in eax
print_int_to_ascii:
	push ebp
	mov ebp, esp
	xor ebx, ebx
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
main_done:
	mov eax, 1
	mov ebx, 0
	int 0x80
