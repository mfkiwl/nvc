//
//  Copyright (C) 2019  Nick Gasson
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include "util.h"
#include "vcode.h"
#include "util/printer.hpp"

#include <vector>
#include <map>

class Machine {
public:
   Machine(const char *name, int num_regs, int result_reg, int sp_reg,
           int word_size);
   Machine(const Machine&) = default;
   Machine(Machine&&) = default;
   virtual ~Machine() {}

   const char *name() const { return name_; }
   int num_regs() const { return num_regs_; }
   int result_reg() const { return result_reg_; }
   int sp_reg() const { return sp_reg_; }
   int word_size() const { return word_size_; }

   int32_t read_i32(const uint8_t *p) const;
   int16_t read_i16(const uint8_t *p) const;

   virtual const char *fmt_reg(int reg) const;

private:
   const char *const name_;
   const int         num_regs_;
   const int         result_reg_;
   const int         sp_reg_;
   const int         word_size_;
};

class InterpMachine : public Machine {
public:
   static const InterpMachine& get();

   static const int NUM_REGS = 32;
   static const int WORD_SIZE = 4;
   static const int SP_REG = NUM_REGS - 1;

private:
   InterpMachine();
};

class Bytecode {
public:
   enum OpCode : uint8_t {
      NOP   = 0x00,     // Do nothing
      MOVW  = 0x01,     // Move 32-bit literal to register
      RET   = 0x02,     // Return from function
      ADD   = 0x03,     // Add two registers
      MOV   = 0x04,     // Move register to another register
      ADDW  = 0x05,     // Add 32-bit immediate to register
      STR   = 0x06,     // Store register to memory (indirect)
      LDR   = 0x07,     // Load register from memory (indirect)
      MUL   = 0x08,     // Multiply 32-bit registers
      CMP   = 0x09,     // Compare two registers
      CSET  = 0x0a,     // Set register based on flags
      JMP   = 0x0b,     // Jump to address
      // Unused 0x0c
      // Unused 0x0d
      MOVB  = 0x0e,     // Move 8-bit literal to register
      ADDB  = 0x0f,     // Add 8-bit immediate to register
      JMPC  = 0x10,     // Jump if condition code set
      SUB   = 0x11,     // Subtract two registers
      ANDB  = 0x12,     // Bitwise and with sign-extended 8-bit immediate
      ANDW  = 0x13,     // Bitwise and 32-bit immediate
      TESTB = 0x14,     // Mask 8-bit immediate and set flags
      TESTW = 0x15,     // Mask 32-bit immediate and set flags
      MULB  = 0x16,     // Multiply register with 8-bit immediate
      MULW  = 0x17,     // Multiply register with 8-bit immediate
   };

   enum Condition : uint8_t {
      Z = 0x01, NZ = 0x02, GT = 0x04, LT = 0x08, GE = 0x10, LE = 0x20,
      EQ = Z, NE = NZ
   };

   struct Register {
      int num;

      inline bool operator==(const Register& r) const { return r.num == num; }
      inline bool operator!=(const Register& r) const { return r.num != num; }
   };

   static inline Register R(int num) {
      return Register { num };
   }

   class Assembler;

   class Label {
   public:
      Label() = default;
      Label(const Label&) = delete;
      Label(Label&&) = default;
      ~Label();

      void bind(Assembler *owner, unsigned target);
      void add_patch(unsigned offset);
      bool bound() const { return bound_ != -1; }
      unsigned target() const;

   private:
      int bound_ = -1;
      std::vector<unsigned> patch_list_;
   };

   class Assembler {
   public:
      explicit Assembler(const Machine& m);

      Bytecode *finish();
      void set_frame_size(unsigned bytes);
      unsigned code_size() const { return bytes_.size(); }
      void patch_branch(unsigned offset, unsigned abs);
      void bind(Label& label);

      void comment(const char *fmt, ...)
         __attribute__((format(printf, 2, 3)));

      void mov(Register dst, Register src);
      void mov(Register dst, int64_t value);
      void add(Register dst, Register src);
      void add(Register dst, int64_t value);
      void sub(Register dst, Register src);
      void str(Register indirect, int16_t offset, Register src);
      void ldr(Register dst, Register indirect, int16_t offset);
      void ret();
      void cmp(Register lhs, Register rhs);
      void cset(Register dst, Condition cond);
      void jmp(Label& label);
      void jmp(Label& target, Condition cond);
      void mul(Register dst, Register rhs);
      void mul(Register dst, int64_t value);
      void nop();
      void andr(Register dst, int64_t value);
      void test(Register dst, int64_t value);

      Register sp() const { return Register{ machine_.sp_reg() }; };

   private:
      void emit_reg(Register reg);
      void emit_u8(uint8_t byte);
      void emit_i32(int32_t value);
      void emit_i16(int16_t value);
      void emit_branch(unsigned offset, Label& target);

      Assembler(const Assembler &) = delete;
      Assembler(Assembler &&) = default;

      std::vector<uint8_t> bytes_;
      const Machine machine_;
      unsigned frame_size_ = 0;

#if DEBUG
      std::map<int, char *> comments_;
#endif
   };

   ~Bytecode();

   const uint8_t *bytes() const { return bytes_; }
   size_t length() const { return len_; }
   const Machine& machine() const { return machine_; }
   unsigned frame_size() const { return frame_size_; }

   void dump(Printer&& printer = StdoutPrinter(), int mark_bci=-1) const;
   void dump(Printer& printer, int mark_bci=-1) const;

#if DEBUG
   const char *comment(const uint8_t *bptr) const;
#endif

private:
   explicit Bytecode(const Machine& m, const uint8_t *bytes, size_t len,
                     unsigned frame_size);
   Bytecode(const Bytecode&) = delete;
   Bytecode(const Bytecode&&) = delete;

#if DEBUG
   void move_comments(std::map<int, char*>&& comments);
#endif

   uint8_t *const  bytes_;
   const size_t    len_;
   const unsigned  frame_size_;
   const Machine   machine_;

#if DEBUG
   std::map<int, char *> comments_;
#endif
};