{--------------------------------------
  Fun: a minimal functional language
  -------------------------------------

  Fun is a minimal functional language designed to experiment with
  language interpreters and compilers. This module defines the 
  abstract syntax of the language:
   - the only primitive data are integers with primitives +, - and * 
   - if-zero-then-else conditional;
   - single argument lambdas; currying can be used for 
     multiple arguments;
   - single let-definitions; multiple definitions must be 
     translated to nested lets;
   - single recursive function definitions (for simplicity).

  Pedro Vasconcelos, 2008--2022
-}
module Fun where
import Data.List (union, delete)


-- abstract syntax for language terms
data Term = Var Ident               -- variables
          | Lambda Ident Term       -- abstraction
          | App Term Term           -- application
          | Const Int               -- constants 
          | Term :+ Term            -- arithmetic operators
          | Term :- Term
          | Term :* Term
          | IfZero Term Term Term   -- conditional
          | Let Ident Term Term     -- local definition
          | Fix Term                -- fixed-point operator
          | MyTrue                  -- Boolean true
          | MyFalse                 -- Boolean false
          | Pair Term Term          -- Pair
          | Fst Term                -- First term of pair
          | Snd Term                -- Snd Term of pair
          | Empty                   -- Representation of empty List
          | Term :$ Term            -- Representation of a cons list 
          | MyNull Term             -- yield 0 if it is null and 1 if it isn't
          | MyHead Term             -- yields the head of a list
          | MyTail Term             -- yields the tail of a list
            deriving Show

{- 
Definição Bool MyTrue e Bool e List Empty talvez estejam erradas
-}
-- indentifiers are just strings
type Ident = String

-- some syntactical definitions
-- list of identifiers with free occurrences in a term
fv :: Term -> [Ident]
fv (Var x)      = [x]
fv (Lambda x e) = delete x (fv e)
fv (App e1 e2)  =  (fv e1) `union` (fv e2)
fv (Const n)    = []
fv (e1 :+ e2)   = fv e1 `union` fv e2
fv (e1 :* e2)   = fv e1 `union` fv e2
fv (e1 :- e2)   = fv e1 `union` fv e2
fv (IfZero e1 e2 e3) = fv e1 `union` fv e2 `union` fv e3
fv (Pair e1 e2) = fv e1 `union` fv e2
fv (Fst e) = fv e 
fv (MyTail e) = fv e
fv (MyHead e) = fv e
fv (MyNull e) = fv e
fv (Empty) = []
fv (e1 :$ e2) = fv e1 `union` fv e2 
fv (Let x e1 e2) = fv e1 `union` delete x (fv e2)
fv (Fix e)       = fv e


-- end of file -------------------------------------------------

