??    W      ?     ?      ?  [   ?     ?  9   ?  (   3     \     i  	   y     ?  	   ?     ?     ?     ?  (   ?     	     -	     M	     h	     ?	     ?	     ?	     ?	     ?	  ,   ?	     
  :   6
  1   q
  9   ?
  6   ?
       "   .  )   Q     {  %   ?  #   ?  +   ?  +     1   /  1   a  %   ?  +   ?  1   ?  1     /   I     y     ?     ?     ?  .   ?  +   	     5     O  !   j  5   ?  "   ?  0   ?          &     F  )   c     ?     ?  %   ?  &   ?          &  !   :     \  !   w  !   ?  2   ?  2   ?  2   !  0   T  -   ?  ;   ?  	   ?     ?                     /     G     b      w     ?     ?  y  ?  \   >     ?  <   ?  +   ?          &     <      I     j     z     ?     ?  (   ?     ?  5         6  #   T  #   x  "   ?     ?     ?     ?  /   ?       =   ;  5   y  <   ?  -   ?  +     9   F  4   ?     ?  $   ?  %   ?  1     %   I  ,   o  ,   ?  (   ?  &   ?  *     *   D  B   o     ?     ?     ?     	  ?   (  ?   h  ,   ?     ?  $   ?  7     '   O  E   w     ?  0   ?  /   ?  :   -  '   h     ?  (   ?  '   ?  "   ?  !   "     D     _  $   ~  $   ?  <   ?  9     <   ?  7   |  4   ?  M   ?     7      D      R      g      }      ?   ,   ?      ?   !   ?      !     0!     P   ;   J          >       ?   I           D   H   -      .   2          :   <   !           F   5   "            7   V           8          4            1      T   *   K       %                    Q                 L   U   )   &           ,      +   
           S   3      C                      G       	                 B   /                  A          #   =   @       $          O      E       6   R       M           (              0   N   9   W       '                  
The following ARM specific disassembler options are supported for use with
the -M switch:
 # <dis error: %08x> # internal disassembler error, unrecognised modifier (%c) # internal error, undefined modifier(%c) $<undefined> %02x		*unknown* (unknown) *unknown operands type: %d* *unknown* <function code %d> <illegal precision> <internal disassembler error> <internal error in opcode table: %s %s>
 <unknown register %d> Address 0x%x is out of bounds.
 Bad case %d (%s) in %s:%d
 Bad immediate expression Bad register in postincrement Bad register in preincrement Bad register name Don't understand %x 
 Hmmmm %x Illegal limm reference in last instruction!
 Internal disassembler error Internal error:  bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Internal error: bad sparc-opcode.h: "%s" == "%s"
 Internal error: bad sparc-opcode.h: "%s", %#.8lx, %#.8lx
 Internal: Non-debugged code (test-case missing): %s:%d Label conflicts with `Rx' Label conflicts with register name Small operand was not an immediate number Unknown error %d
 Unrecognised disassembler option: %s
 Unrecognised register name set: %s
 Unrecognized field %d while building insn.
 Unrecognized field %d while decoding insn.
 Unrecognized field %d while getting int operand.
 Unrecognized field %d while getting vma operand.
 Unrecognized field %d while parsing.
 Unrecognized field %d while printing insn.
 Unrecognized field %d while setting int operand.
 Unrecognized field %d while setting vma operand.
 attempt to set y bit when using + or - modifier bad instruction `%.50s' bad instruction `%.50s...' branch operand unaligned branch to odd offset branch value not in range and to an odd offset branch value not in range and to odd offset branch value out of range can't cope with insert %d
 displacement value is not aligned displacement value is not in range and is not aligned displacement value is out of range ignoring least significant bits in branch offset illegal bitmask immediate value is out of range immediate value must be even immediate value not in range and not even index register in load range invalid conditional option invalid register for stack adjustment invalid register operand when updating jump hint unaligned junk at end of line missing mnemonic in syntax string offset not a multiple of 4 offset not between -2048 and 2047 offset not between -8192 and 8191 operand out of range (%ld not between %ld and %ld) operand out of range (%ld not between %ld and %lu) operand out of range (%lu not between %lu and %lu) operand out of range (%lu not between 0 and %lu) syntax error (expected char `%c', found `%c') syntax error (expected char `%c', found end of instruction) undefined unknown unknown	0x%02x unknown	0x%04lx unknown	0x%04x unknown constraint `%c' unknown operand shift: %x
 unknown pop reg: %d
 unrecognized form of instruction unrecognized instruction value out of range Project-Id-Version: opcodes 2.12.91
POT-Creation-Date: 2002-07-23 15:55-0400
PO-Revision-Date: 2003-04-28 08:32+0300
Last-Translator: Eugen Hoanca <eugenh@urban-grafx.ro>
Language-Team: Romanian <translation-team-ro@lists.sourceforge.net>
MIME-Version: 1.0
Content-Type: text/plain; charset=ISO-8859-2
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=(n != 1);
 
Op?iunile ARM de dezasamblor specifice urm?toare sunt permise cu folosirea
switch-ului -M:
 # <eroare dez: %08x> # eroare intern? de dezasamblor, modificator necunoscut (%c) # eroare intern?, modificator nedefinit(%c) $<nedefinit> %02x		*necunoscut(?)* (necunoscut) *tip necunoscut de operanzi: %d* *necunoscut(?)* <cod func?ie %d> <precizie ilegal?> <eroare intern? de dezasamblor> <eroare intern? ?n tabel opcode: %s %s>
 <registru necunoscut %d> Adresa 0x%x este ?n afara limitelor (out of bounds).
 Caz gre?it %d (%s) in %s: %d
 Expresie direct?(immediate) gre?it? Registru gre?it ?n postincrementare Registru gre?it ?n preincrementare Nume registru gre?it Nu ?n?eleg %x 
 Hmmmm %x referin?? limm ilegal? ?n ultima instruc?iune!
 Eroare intern? de dezasamblor Eroare intern?:  opcode.h sparc gre?it: "%s", %#.8lx, %#.8lx
 Eroare intern?:  opcode.h sparc gre?it: "%s" == "%s"
 Eroare intern?: opcode.h sparc gre?it: "%s", %#.8lx, %#.8lx
 Intern: cod ne-depanat (caz test lips?) %s:%d Eticheta(label) se afl? ?n conflict cu `Rx' Eticheta(label) se afl? ?n conflict cu numele de registru Operandul redus nu a fost un num?r direct(immediate) Eroare necunoscut? %d
 Op?iune dezasamblor necunoscut?: %s
 Setare nume registru necunoscut?: %s
 C?mp necunoscut %d ?n construire(building) insn.
 C?mp necunoscut %d ?n decodare insn.
 C?mp necunoscut %d ?n preluare operand int.
 C?mp necunoscut %d ?n preluare operand vma.
 C?mp necunoscut %d ?n analiz?(parsing).
 C?mp necunoscut %d la tip?rirea insn.
 C?mp necunoscut %d ?n setare operand int.
 C?mp necunoscut %d ?n setare operand vma.
 se ?ncearc? setarea bitului y ?n folosirea modificatorilor + sau - instruc?iune gre?it? `%.50s' instruc?iune gre?it? `%.50s...' ramur? operand nealiniat? ramur?(branch) la offset impar valoare ramur?(branch) ?n afara intervalului ?i la offset impar valoare ramur?(branch) ?n afara intervalului ?i la offset impar valoare ramur?(branch) ?n afara intervalului nu fac fa?? la inserarea %d
 valoarea deplas?rii nu este aliniat? valoarea deplas?rii ?n afara intervalului ?i nealiniat? valoare deplasare ?n afara intervalului se ignor? cei mai pu?in semnifican?i bi?i ?n offsetul ramurii(branch) bitmask ilegal valoare direct?(immediate) ?n afara intervalului valoarea direct?(immediate) trebuie s? fie par? valoare direct?(immediate) ?n afara intervalului ?i impar? registru index ?n interval de ?nc?rcare op?iune condi?ional? invalid? registru invalid pentru modificare stiv? registru de operand invalid ?n updatare sugestie s?ritur?(jump) nealiniat? resturi(junk) la sf?r?it de linie mnemonic? lips? ?n sintax? offsetul nu este multiplu de 4 offsetul nu este ?ntre -2048 ?i 2047 offsetul nu este ?ntre -8192 ?i 8191 operand ?n afara intervalului (%ld nu este ?ntre %ld ?i %ld) operand ?n afara limitelor (%ld nu este ?ntre %ld ?i %lu) operand ?n afara intervalului (%lu nu este ?ntre %lu ?i %lu) operand ?n afara limitelor (%lu nu este ?ntre 0 ?i %lu) eroare de sintax? ( se a?tepta %c', s-a primit `%c') eroare de sintax? (s-a a?teptat char `%c' s-a primit sf?r?it de instruc?iune) nedefinit(?) necunoscut(?) necunoscut(?)	0x%02x necunoscut(?)	0x%04lx necunoscut(?)	0x%04x constr?ngere necunoscut? `%c' schimbare(shift) de oberand necunoscut?: %x
 pop reg necunoscut: %d
 form? de instruc?iune necunoscut? instruc?iune necunoscut? valoare ?n afara intervalului 