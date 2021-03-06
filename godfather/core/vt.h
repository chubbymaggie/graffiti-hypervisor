/*
  Copyright notice
  ================
  
  Copyright (C) 2010 - 2013
      Lorenzo  Martignoni <martignlo@gmail.com>
      Roberto  Paleari    <roberto.paleari@gmail.com>
      Aristide Fattori    <joystick@security.di.unimi.it>
      Mattia   Pagnozzi   <pago@security.di.unimi.it>
  
  This program is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.
  
  HyperDbg is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.
  
*/

#ifndef _VT_H
#define _VT_H

#include "types.h"
#include "msr.h"
#include "idt.h"

#define HVM_DELIVER_NO_ERROR_CODE (-1)

/* CR access types */
typedef enum {
  VT_CR_ACCESS_WRITE,
  VT_CR_ACCESS_READ,
  VT_CR_ACCESS_CLTS,
  VT_CR_ACCESS_LMSW,
} VtCrAccessType;

typedef enum {
  VT_REGISTER_RAX,
  VT_REGISTER_RBX,
  VT_REGISTER_RCX,
  VT_REGISTER_RDX,
  VT_REGISTER_RSP,
  VT_REGISTER_RBP,
  VT_REGISTER_RSI,
  VT_REGISTER_RDI,
  VT_REGISTER_R8,
  VT_REGISTER_R9,
  VT_REGISTER_R10,
  VT_REGISTER_R11,
  VT_REGISTER_R12,
  VT_REGISTER_R13,
  VT_REGISTER_R14,  
  VT_REGISTER_R15,
} VtRegister;

struct __attribute__((__packed__)) CPU_CONTEXT {
  struct __attribute__((__packed__)) {
    hvm_address rip;
    hvm_address resumerip;
    hvm_address rsp;
    hvm_address cs;
    hvm_address cr0;
    hvm_address cr3;
    hvm_address cr4;
    hvm_address rflags;

    hvm_address rax;
    hvm_address rbx;
    hvm_address rcx;
    hvm_address rdx;
    hvm_address rdi;
    hvm_address rsi;
    hvm_address rbp;
  } GuestContext;
};

struct HVM_X86_OPS {
  /* VT-related */
  hvm_bool      (*vt_cpu_has_support)(void);
  hvm_bool      (*vt_disabled_by_bios)(void);
  hvm_bool      (*vt_enabled)(void);
  hvm_status    (*vt_initialize)(void (*idt_initializer)(PIDT_ENTRY));
  hvm_status    (*vt_finalize)(void);
  void          (*vt_launch)(void);
  hvm_status    (*vt_hardware_enable)(void);
  hvm_status    (*vt_hardware_disable)(void);
  void          (*vt_hypercall)(Bit32u num) USESTACK;

  /* These will be removed in a near future... */
  hvm_status    (*vt_vmcs_initialize)(hvm_address guest_stack, hvm_address guest_return, hvm_address host_cr3);
  Bit32u        (*vt_vmcs_read)(Bit32u encoding) USESTACK;
  void          (*vt_vmcs_write)(Bit32u encoding, Bit32u value) USESTACK;
  
  void          (*vt_set_cr0)(hvm_address cr0);
  void          (*vt_set_cr3)(hvm_address cr3);
  void          (*vt_set_cr4)(hvm_address cr4);

  void          (*vt_trap_io)(hvm_bool enabled);
  void          (*vt_switch_mtf)(hvm_bool enabled);
  Bit32u        (*vt_get_exit_instr_len)(void);

  /* Memory management */
  void          (*mmu_tlb_flush)(void);

  /* HVM-related */
  void          (*hvm_handle_exit)(void);
  hvm_status    (*hvm_switch_off)(void);
  hvm_status    (*hvm_update_events)(void);
  void          (*hvm_inject_hw_exception)(Bit32u type, Bit32u error_code);
};

extern struct CPU_CONTEXT context asm("_context");
extern struct HVM_X86_OPS hvm_x86_ops;

#endif	/* _VT_H */
