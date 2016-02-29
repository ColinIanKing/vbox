; $Id$
;; @file
; BS3Kit - bs3TestSendCmdWithU32.
;

;
; Copyright (C) 2007-2016 Oracle Corporation
;
; This file is part of VirtualBox Open Source Edition (OSE), as
; available from http://www.virtualbox.org. This file is free software;
; you can redistribute it and/or modify it under the terms of the GNU
; General Public License (GPL) as published by the Free Software
; Foundation, in version 2 as it comes in the "COPYING" file of the
; VirtualBox OSE distribution. VirtualBox OSE is distributed in the
; hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
;
; The contents of this file may alternatively be used under the terms
; of the Common Development and Distribution License Version 1.0
; (CDDL) only, as it comes in the "COPYING.CDDL" file of the
; VirtualBox OSE distribution, in which case the provisions of the
; CDDL are applicable instead of those of the GPL.
;
; You may elect to license modified versions of this file under the
; terms and conditions of either the GPL or the CDDL or both.
;

%include "bs3kit-template-header.mac"
%include "VBox/VMMDevTesting.mac"

BS3_EXTERN_DATA16 g_fbBs3VMMDevTesting
TMPL_BEGIN_TEXT

;;
; @cproto   BS3_DECL(void) bs3TestSendCmdWithU32_c16(uint32_t uCmd, uint32_t uValue);
;
BS3_PROC_BEGIN_CMN bs3TestSendCmdWithU32
        BS3_CALL_CONV_PROLOG 2
        push    xBP
        mov     xBP, xSP
        push    xAX
        push    xDX
        push    xSI

        cmp     byte [g_fbBs3VMMDevTesting], 0
        je      .no_vmmdev

        ; The command (uCmd).
        mov     dx, VMMDEV_TESTING_IOPORT_CMD
        mov     eax, [xBP + xCB*2]
        out     dx, eax

        ; The value (uValue).
        mov     dx, VMMDEV_TESTING_IOPORT_DATA
        mov     eax, [xBP + xCB*2 + sCB]
        out     dx, eax

.no_vmmdev:
        pop     xSI
        pop     xDX
        pop     xAX
        leave
        BS3_CALL_CONV_EPILOG 2
        ret
BS3_PROC_END_CMN   bs3TestSendCmdWithU32
