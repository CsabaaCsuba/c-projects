#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <stdint.h>

/*
Specification
1. uses ; to close lines
2. uses {} to parse code scopes
3. can define your own keywords
4. uses [] for arrays
(chat is this real) -> if
(chat what about this) -> else if
(bruh chat) -> else
(nocap) -> true
(cap) -> false
yap -> print
skibidi -> switch
on god -> case
sigma -> const
aura -> int
small aura -> short int
infinite aura -> long int
bussin aura -> unsigned int
forreal -> bool
yippiti yap-> string
gyatt -> float
kys -> program terminates
hold up -> program sleeps
mew -> function
goated -> class
hawk -> try
tuah -> catch
be -> =
frfr -> ==
flex -> for
edge until -> while
foreach (val) in iterable -> flex aura val only in Ohio iterable;
slay -> return
gamble -> random number
goon -> import
mog -> > (greater than)
aint -> ! (reverse condition)
diddy -> break
epstein island -> void (function that returns nothing)
*/

typedef enum {
	if_kw,
	elif_kw,
	else_kw,
	true_kw,
	false_kw,
	switch_kw,
	case_kw,
	break_kw,
	void_kw,
	short_kw,
	number_kw,
	double_kw,
	function_kw,
	class_kw,
	try_kw,
	catch_kw,

	identifier,

	constant_val, //like a number, or a string written into the code
	
	assignment_op,
	equality_check_expr,
	greater_than_expr,
	smaller_than_expr,
	reverse_condition, //!
	

	scope_opener, // {
	scope_closer,
	open_parenthesis,
	closing_parenthesis,
	open_squarebracket,
	closing_squarebracket,




} Type;

typedef union { //for tokens that need to carry a value
	char* str_val;
	int64_t int_val;
	double real_val;
} Value;

typedef struct {
	Value tokenval;
	Type tokentype;
} Token;

int main() {


	return 0;
}