### Scheme Subset's Grammar

```bnf
<program> ::= (<top>)+
<top> ::= <variable> / <lambda> / <compute>
<variable> ::= "(" "define" / "let!" <identifier> <nestable> ")"
<lambda> ::= "(" "lambda" <params> <nestable> ")"
<params> ::= "(" (<identifier>)* ")"
<nestable> ::= <lambda> / <do> / <cond> / <compute>
<do> ::= "(" "do" (<top>)+ ")"
<cond> ::= "(" "cond" (<case>)+ ")"
<case> ::= "(" <compute> <compute> ")"
<compute> ::= "(" <identifier> (<compute>)+ ")" / <datum>
<datum> ::= <boolean> / <exact> / <real> / <string> / <identifier>
```