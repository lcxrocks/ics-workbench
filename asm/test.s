	.file	"test.c"
	.text
	.globl	asm_memcpy
	.type	asm_memcpy, @function
asm_memcpy:
.LFB11:
	.cfi_startproc
	movq	%rdi, %rax
	testq	%rdx, %rdx
	je	.L2
	movl	$0, %ecx
.L3:
	movzbl	(%rcx), %esi
	movb	%sil, (%rax,%rcx)
	addq	$1, %rcx
	cmpq	%rdx, %rcx
	jne	.L3
.L2:
	ret
	.cfi_endproc
.LFE11:
	.size	asm_memcpy, .-asm_memcpy
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
