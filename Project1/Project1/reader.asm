.code

GetPEB proc
			mov rax, qword ptr gs:[60h]
			ret
GetPEB endp

IsDebugged proc stdcall
			xor rax, rax
			call GetPEB
			movzx rax, byte ptr[rax+2h]
			ret
IsDebugged endp

end 
		