.386
.model flat, stdcall
option casemap :none

include masm32\include\windows.inc
include masm32\include\kernel32.inc
include masm32\include\masm32.inc
include masm32\include\user32.inc
include masm32\include\msvcrt.inc
includelib masm32\lib\kernel32.lib
includelib masm32\lib\masm32.lib
includelib masm32\lib\user32.lib
includelib masm32\lib\msvcrt.lib

.DATA
;===User Data================================================================================
	_Aaa_	dw	0
	_Bbb_	dw	0
	_Xxx_	dw	0
	_Yyy_	dw	0

	DivErrMsg	db	13, 10, "Division: Error: division by zero", 0
	ModErrMsg	db	13, 10, "Mod: Error: division by zero", 0

;===Addition Data============================================================================
	_gc				dd	?
	hConsoleInput	dd	?
	hConsoleOutput	dd	?
	endBuff			db	5 dup (?)
	msg1310			db	13, 10, 0

	CharsReadNum	dd	?
	InputBuf		db	15 dup (?)
	OutMessage		db	"%hd", 0
	ResMessage		db	20 dup (?)

.CODE
start:
invoke AllocConsole
invoke GetStdHandle, STD_INPUT_HANDLE
mov hConsoleInput, eax
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hConsoleOutput, eax
xor ebx,ebx

	call Input_
	mov _Aaa_, ax
	call Input_
	mov _Bbb_, ax
	push _Aaa_
	push _Bbb_
	call Add_
	call Output_
	push _Aaa_
	push _Bbb_
	call Sub_
	call Output_
	push _Aaa_
	push _Bbb_
	call Mul_
	call Output_
	push _Aaa_
	push _Bbb_
	call Div_
	call Output_
	push _Aaa_
	push _Bbb_
	call Mod_
	call Output_
	push _Aaa_
	push _Bbb_
	call Sub_
	push word ptr 10
	call Mul_
	push _Aaa_
	push _Bbb_
	call Add_
	push word ptr 10
	call Div_
	call Add_
	pop _Xxx_
	push _Xxx_
	push _Xxx_
	push word ptr 10
	call Mod_
	call Add_
	pop _Yyy_
	push _Xxx_
	call Output_
	push _Yyy_
	call Output_
exit_label:
invoke WriteConsoleA, hConsoleOutput, ADDR msg1310, SIZEOF msg1310 - 1, 0, 0
invoke ReadConsoleA, hConsoleInput, ADDR endBuff, 5, 0, 0
invoke ExitProcess, 0


;===Procedure Add============================================================================
Add_ PROC
	mov ax, [esp + 6]
	add ax, [esp + 4]
	mov [esp + 6], ax
	pop ecx
	pop ax
	push ecx
	ret
Add_ ENDP
;============================================================================================


;===Procedure Div============================================================================
Div_ PROC
	pushf
	pop cx

	mov ax, [esp + 4]
	cmp ax, 0
	jne end_check
	invoke WriteConsoleA, hConsoleOutput, ADDR DivErrMsg, SIZEOF DivErrMsg - 1, 0, 0
	jmp exit_label
end_check:
	mov ax, [esp + 6]
	cmp ax, 0
	jge gr
lo:
	mov dx, -1
	jmp less_fin
gr:
	mov dx, 0
less_fin:
	mov ax, [esp + 6]
	idiv word ptr [esp + 4]
	push cx
	popf

	mov [esp + 6], ax
	pop ecx
	pop ax
	push ecx
	ret
Div_ ENDP
;============================================================================================


;===Procedure Input==========================================================================
Input_ PROC
	invoke ReadConsoleA, hConsoleInput, ADDR InputBuf, 13, ADDR CharsReadNum, 0
	invoke crt_atoi, ADDR InputBuf
	ret
Input_ ENDP
;============================================================================================


;===Procedure Mod============================================================================
Mod_ PROC
	pushf
	pop cx

	mov ax, [esp + 4]
	cmp ax, 0
	jne end_check
	invoke WriteConsoleA, hConsoleOutput, ADDR ModErrMsg, SIZEOF ModErrMsg - 1, 0, 0
	jmp exit_label
end_check:
	mov ax, [esp + 6]
	cmp ax, 0
	jge gr
lo:
	mov dx, -1
	jmp less_fin
gr:
	mov dx, 0
less_fin:
	mov ax, [esp + 6]
	idiv word ptr [esp + 4]
	mov ax, dx
	push cx
	popf

	mov [esp + 6], ax
	pop ecx
	pop ax
	push ecx
	ret
Mod_ ENDP
;============================================================================================


;===Procedure Mul============================================================================
Mul_ PROC
	mov ax, [esp + 6]
	imul word ptr [esp + 4]
	mov [esp + 6], ax
	pop ecx
	pop ax
	push ecx
	ret
Mul_ ENDP
;============================================================================================


;===Procedure Output=========================================================================
Output_ PROC value: word
	invoke wsprintf, ADDR ResMessage, ADDR OutMessage, value
	invoke WriteConsoleA, hConsoleOutput, ADDR ResMessage, eax, 0, 0
invoke WriteConsoleA, hConsoleOutput, ADDR msg1310, SIZEOF msg1310 - 1, 0, 0
	ret 2
Output_ ENDP
;============================================================================================


;===Procedure Sub============================================================================
Sub_ PROC
	mov ax, [esp + 6]
	sub ax, [esp + 4]
	mov [esp + 6], ax
	pop ecx
	pop ax
	push ecx
	ret
Sub_ ENDP
;============================================================================================
end start
