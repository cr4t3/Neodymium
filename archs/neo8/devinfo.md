# Neodymium Devinfo

## CPU

### REGISTERS

The CPU contains 8 normal accesible registers (\$a-\$h) which are accesible per using byte `0x00` for \$a and `0x07` for \$h. 
But there is also a semi-constant flag called "always zero", which as  the name implies, is always zero. This one uses byte `0xff`, and it's mostly dedicated for using in addresses.

## vRAM

The vRAM is forced to be 64KB (0x0-0xffff).

### DEFINITIONS
- \$x/\$y - Any register
- #0 - Any immediate number (unsigned 8-bit)
- [*] - vRAM Address
- () - Optional

### OPCODES
INSTRUCTION | OPCODE | EXPLANATION |
|-|-|-|
NOP             | 0x00  | -
MOV \$x, \$y      | 0x01  | \$y -> \$x
MOV \$x, #0      | 0x02  | #0 -> \$x
MOV \$x, [#0]    | 0x03  | [#0] -> \$x   
MOV \$x, [\$y,$z] | 0x04  | [\$y,$z] -> \$x 
NOT \$x          | 0x10  | !$x -> \$x
AND \$x, \$y      | 0x11  | \$x & \$y -> \$x
AND \$x, #0      | 0x12  | \$x & #0 -> \$x
JMP [#0]        | 0x20  | -
JMP [\$x,\$y]   | 0x21  | -
CMP \$x, #0      | 0x22  | \$x - #0
CMP \$x, \$y      | 0x23  | \$x - \$y
JZ [#0]         | 0x24  | -
JZ [\$x,$y]      | 0x25  | -
JNZ [#0]        | 0x26  | -
JNZ [\$x,$y]     | 0x27  | -
JU [#0]         | 0x28  | -
JU [\$x,\$y]    | 0x29  | -
JNU [#0]        | 0x2a  | -
JNU [\$x,\$y]   | 0x2b  | -
JO [#0]         | 0x2c  | -
JO [\$x,\$y]    | 0x2d  | -
JNO [#0]        | 0x2e  | -
JNO [\$x,\$y]   | 0x2f  | -
PUSH \$x         | 0x30  | -
PUSH #0         | 0x31  | -
POP \$x          | 0x32  | -
ADD \$x, #0      | 0x40  | \$x + #0 -> \$x
ADD \$x, \$y      | 0x41  | \$x + \$y -> \$x
INC \$x          | 0x42  | \$x++
SUB \$x, #0      | 0x43  | \$x - #0 -> \$x
SUB \$x, \$y      | 0x44  | \$x - \$y -> \$x
DEC \$x          | 0x45  | \$x--
MUL \$x, #0      | 0x46  | \$x * #0 -> \$x
MUL \$x, \$y      | 0x47  | \$x * \$y -> \$x
DIV \$x, #0      | 0x48  | \$x * #0 -> \$x
DIV \$x, \$y      | 0x49  | \$x * \$y -> \$x
MOD \$x, \$y          | 0x4e  | \$x % \$y -> \$x
MOD \$x, #0          | 0x4f  | \$x % #0 -> \$x
CALL [#0]       | 0x50  | -
CALL [\$x, \$y]   | 0x51  | -
RET             | 0x52  | -
STORE [\$x, \$y], $z  | 0x60  | \$z -> [\$x,\$y]
STORE [#0], $x      | 0x61  | $x -> [#0]
STORE [\$x, \$y], #0  | 0x62  | #0 -> [\$x,\$y]
STORE [#0], #1      | 0x63  | #1 -> [#0]
FLUSH           | 0x70  | -
HALT \$x         | 0xfd  | -
HALT #0         | 0xfe  | -
HALT            | 0xff  | -

### Stack

The stack is part of the vRAM, using address 0xCF00 to 0xCFFF (256 bytes).