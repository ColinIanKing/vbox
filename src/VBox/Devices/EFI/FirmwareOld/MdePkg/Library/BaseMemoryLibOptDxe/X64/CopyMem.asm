;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   CopyMem.asm
;
; Abstract:
;
;   CopyMem function
;
; Notes:
;
;------------------------------------------------------------------------------

    .code

;------------------------------------------------------------------------------
;  VOID *
;  EFIAPI
;  InternalMemCopyMem (
;    IN VOID   *Destination,
;    IN VOID   *Source,
;    IN UINTN  Count
;    );
;------------------------------------------------------------------------------
InternalMemCopyMem  PROC    USES    rsi rdi
    mov     rsi, rdx                    ; rsi <- Source
    mov     rdi, rcx                    ; rdi <- Destination
    lea     r9, [rsi + r8 - 1]          ; r9 <- Last byte of Source
    cmp     rsi, rdi
    mov     rax, rdi                    ; rax <- Destination as return value
    jae     @F                          ; Copy forward if Source > Destination
    cmp     r9, rdi                     ; Overlapped?
    jae     @CopyBackward               ; Copy backward if overlapped
@@:
    xor     rcx, rcx
    sub     rcx, rdi                    ; rcx <- -rdi
    and     rcx, 15                     ; rcx + rsi should be 16 bytes aligned
    jz      @F                          ; skip if rcx == 0
    cmp     rcx, r8
    cmova   rcx, r8
    sub     r8, rcx
    rep     movsb
@@:
    mov     rcx, r8
    and     r8, 15
    shr     rcx, 4                      ; rcx <- # of DQwords to copy
    jz      @CopyBytes
    movdqa  [rsp + 18h], xmm0           ; save xmm0 on stack
@@:
    movdqu  xmm0, [rsi]                 ; rsi may not be 16-byte aligned
    movntdq [rdi], xmm0                 ; rdi should be 16-byte aligned
    add     rsi, 16
    add     rdi, 16
    loop    @B
    mfence
    movdqa  xmm0, [rsp + 18h]           ; restore xmm0
    jmp     @CopyBytes                  ; copy remaining bytes
@CopyBackward:
    mov     rsi, r9                     ; rsi <- Last byte of Source
    lea     rdi, [rdi + r8 - 1]         ; rdi <- Last byte of Destination
    std
@CopyBytes:
    mov     rcx, r8
    rep     movsb
    cld
    ret
InternalMemCopyMem  ENDP

    END
