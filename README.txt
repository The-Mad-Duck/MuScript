This is a low-level instructional parsing audio plugin made in JUCE framework.

If you want to run these files, you will have to make a new project in JUCE's Projucer under the name "MuScript" and include these files instead of its default files.

This is the first version, expect future commands. 

Commands are based on quaternary parsing. The last instruction of the command is always the store value.

agn a b c
Value a is not used, but assigns the value of b to c.

add a b c
c is set to the output of a + b.

sub a b c
c is set to the output of a - b.

mlt a b c
c is set to the output of a * b.

div a b c
c is set to the output of a / b.

mod a b c
c is set to the output of a modulo b.

abs a b c
Value a is not used, but assigns the absolute value of b to c.

sin a b c
Value a is not used, but assigns the sine of b to c.

cos a b c
Value a is not used, but assigns the cosine of b to c.

tan a b c
Value a is not used, but assigns the tangent of b to c.

pow a b c
c is set to the output of a to the power of b.

--== NEW AS OF 1.1 ==--

jil a b c
jumps to line c if a is less than b.

jig a b c
jumps to line c if a is greater than b.

Enjoy this... thing.
