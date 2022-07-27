$NOMOD51

#define _ASSM_CODE_

;-----------------------------------------------------------------------------
$include (MC96FD316.h)

;-----------------------------------------------------------------------------

    NAME      ASM_FUNC                                            				; Name of the program module
    PUBLIC    Result_uu, Result_ss, _LdCaldr16u, _LdCaldr16s, _LdCaldr32u, _LdCaldr32s

;-----------------------------------------------------------------------------
    ?PR?Result_uu?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?Result_uu?ASM_FUNC                                 				; Program segment for Result_uu
; unsigned long Result_uu(void);
; input: void
; output: RRRR4(R4:R5:R6:R7)
; output = (unsigned long)MO[31:0]
Result_uu:
  MOV		A, CALDR
  MOV		R4, A
  MOV		A, CALDR
  MOV		R5, A
  MOV		A, CALDR
  MOV		R6, A
  MOV		A, CALDR
  MOV		R7, A
  RET

;-----------------------------------------------------------------------------
    ?PR?Result_ss?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?Result_ss?ASM_FUNC                                 				; Program segment for Result_ss
; signed long Result_ss(void);
; input: void
; output: RRRR4(R4:R5:R6:R7)
; output = (signed long)MO[31:0]
Result_ss:
  MOV		A, CALDR
  MOV		R4, A
  MOV		A, CALDR
  MOV		R5, A
  MOV		A, CALDR
  MOV		R6, A
  MOV		A, CALDR
  MOV		R7, A
  RET

;-----------------------------------------------------------------------------
    ?PR?LdCaldr16u?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?LdCaldr16u?ASM_FUNC                                 			; Program segment for LdCaldr16u
; void LdCaldr16u(unsigned int);
; input: unsigned int
; output: void
_LdCaldr16u:
	MOV		A,R6
  MOV		CALDR, A
	MOV		A,R7
  MOV		CALDR, A
  RET

;-----------------------------------------------------------------------------
    ?PR?LdCaldr16s?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?LdCaldr16s?ASM_FUNC                                 			; Program segment for LdCaldr16s
; void LdCaldr16s(signed int);
; input: signed int
; output: void
_LdCaldr16s:
	MOV		A,R6
  MOV		CALDR, A
	MOV		A,R7
  MOV		CALDR, A
  RET

;-----------------------------------------------------------------------------
    ?PR?LdCaldr32u?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?LdCaldr32u?ASM_FUNC                                 			; Program segment for LdCaldr32u
; void LdCaldr32u(unsigned long);
; input: unsigned long
; output: void
_LdCaldr32u:
	MOV		A,R4
  MOV		CALDR, A
	MOV		A,R5
  MOV		CALDR, A
	MOV		A,R6
  MOV		CALDR, A
	MOV		A,R7
  MOV		CALDR, A
  RET

;-----------------------------------------------------------------------------
    ?PR?LdCaldr32s?ASM_FUNC    SEGMENT    CODE
    RSEG    ?PR?LdCaldr32s?ASM_FUNC                                 			; Program segment for LdCaldr32s
; void LdCaldr32s(signed long);
; input: signed long
; output: void
_LdCaldr32s:
	MOV		A,R4
  MOV		CALDR, A
	MOV		A,R5
  MOV		CALDR, A
	MOV		A,R6
  MOV		CALDR, A
	MOV		A,R7
  MOV		CALDR, A
  RET

;-----------------------------------------------------------------------------
    END
