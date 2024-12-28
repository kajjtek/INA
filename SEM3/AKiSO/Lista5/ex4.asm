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
	jmp start_hex_converter

;converts int to hex - first we convert ascii into int 

start_hex_converter:
	push esi
	call ascii_to_int ; int w eax
	xor ebx, ebx
	jmp hex_loop

hex_loop:
	cmp ax, 0
	je hex_loop_done

	inc ebx
	mov ecx , 16
	div ecx
	push edx

	jmp hex_loop

hex_loop_done: ; mamy teraz w stosie odwrocone wartosci - kazda bedziemy popowac i 
	xor ecx, ecx
	xor eax, eax
	xor edx, edx
	jmp retrieve_hexagon

retrieve_hexagon:
	cmp ecx, ebx
	je end_hex_convert

	inc ecx
	pop eax
	push ebx
	push ecx
	call print_int_to_ascii
	pop ecx
	pop ebx
	jmp retrieve_hexagon


end_hex_convert:
	mov eax, 4
	mov edx, ebx
	mov ebx, 1
	lea ecx, [newline]
	int 0x80
	jmp end_program	

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
	xor ebx, ebx
	mov ebp, esp
	
	cmp eax, 10
	jl convert_digit

	jmp convert_number

convert_digit:
	mov ebx, 1
	add eax, '0'	
	jmp convert_done

convert_number:
	mov ebx, 1
	add eax, 55
	jmp convert_done

convert_done:
	xor ecx, ecx
	xor edx, edx
	jmp add_to_result

add_to_result:
	mov [result+ecx], eax
	jmp end

end:
	mov eax, 4
	mov edx, ebx
	mov ebx, 1
	mov ecx, result
	int 0x80

	;mov eax, 4
	;mov edx, ebx
	;mov ebx, 1
	;lea ecx, [newline]
	;int 0x80

	mov esp, ebp
	pop ebp
	ret
; END of function printing int value as ASCII
end_program:
	mov eax, 1
	mov ebx, 0
	int 0x80
