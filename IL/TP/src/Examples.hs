{--------------------------------------
  Fun: a minimal functional language
  -------------------------------------
 
  This module contains some example programs.
  Pedro Vasconcelos, 2008--2009.
-}
module Examples where
import Fun

-- simple computations
ex1 = (Const 42 :+ Const 23) :* Const 5

ex1'= Const 5 :* (Const 42 :+ Const 23) 

-- the identity function
ex2 = Lambda "x" (Var "x")

-- the sucessor function
ex3 = Lambda "x" (Var "x" :+ Const 1)

-- one function between two integers
ex4 = Lambda "x" 
      (Lambda "y"
       (IfZero (Var "x" :- Var "y") 
        (Var "y") (Var "x")))
--run(compileMain (App(App ex4 (Const 5)) (Const 6)))                   
-- an example that builds a closure with free vars
ex5 = Let "x" (Const 42) (Lambda "y" (Var "x" :+ Var "y"))

ex5b = Let "x" (Const 23) (Lambda "y" (Var "x" :+ Var "y"))

-- a recursive function (factorial)
ex6 = Fix 
      (Lambda "f" 
       (Lambda "n"
        (IfZero (Var "n")
         (Const 1)
         ((App (Var "f") (Var "n" :- Const 1)) :* Var "n")
        )))


-- compute the factorial of 10
ex7 = App ex6 (Const 10)

-- factorial of a negative number (diverges)
ex9 = App ex6  (Const (-1))

-- recursive sum 0^2 + 1^2 + 2^2 + ... + n^2
ex8 = Fix 
      (Lambda "f"
       (Lambda "n"
         (IfZero (Var "n")
          (Const 0)
           ((Var "n" :* Var "n") :+ 
            App (Var "f") (Var "n" :- Const 1)))))


--ex9 = (Fst (Pair (Const 5) (Const 6)))

--ex10 = (Snd (Pair (Const 5) (Const 6)))

append = (Fix 
      (Lambda "f" 
       (Lambda "l"
        (Lambda "n"
          (
           IfZero (MyNull  (Var"l")) 
                  ((Var "n") :$ Empty) 
                  ((MyHead (Var "l")) :$ 
                           (App(App (Var "f") (MyTail (Var "l"))) (Var "n"))
                  ) )
        ))))
exAppend = (App (App append ((Const 0) :$((Const 2) :$ ((Const 1):$ Empty))))(Const 5))

tamanho = (Fix 
      (Lambda "f" 
       (Lambda "l"
         (
          IfZero (MyNull  (Var"l")) (Const 0) ((Const 1) :+ (App (Var "f") (MyTail (Var "l")))) )
         )))  

exTamanho1 =  (App tamanho ((Const 0) :$((Const 2) :$ ((Const 1):$ Empty))))
exTamanho2 = (App tamanho (App (App append ((Const 0) :$((Const 2) :$ ((Const 1):$ Empty))))(Const 5)))

somaPar = (Lambda "x" ((Fst (Var "x")) :+ (Snd (Var "x"))))
par = (Pair (Const 1) (Const 2))
exSomaPar = (App (somaPar) (par))

myzip = (Fix 
      (Lambda "f" 
       (Lambda "l1"
        (Lambda "l2"
          (
           IfZero (MyNull  (Var"l1")) (Empty) (IfZero(MyNull (Var "l2")) (Empty) ((Pair (MyHead (Var"l1"))(MyHead (Var"l2")))) :$ (App(App(Var"f") (MyTail (Var "l1")))(MyTail (Var "l2")) )
        ))))))
exZip = App (App myzip ((((Const 0) :$((Const 2) :$ ((Const 1):$ Empty)))))) (((Const 0) :$((Const 2) :$ ((Const 1):$ Empty))))
exTamanho3 =  (App tamanho (exZip))
--run (compileMain (App (App zip ((((Const 0) :$((Const 2) :$ ((Const 1):$ Empty)))))) (((Const 0) :$((Const 2) :$ ((Const 1):$ Empty))))))
mymap = (Fix 
      (Lambda "f" 
       (Lambda "l"
        (Lambda "func"
          (
           IfZero (MyNull  (Var"l")) (Empty) ((App (Var "func")( MyHead (Var "l"))) :$ (App(App(Var"f") (MyTail (Var "l")))(Var "func"))) 
        )))))

exMap1 = (App (App mymap(exZip))(somaPar))
exMap2 =(App (App mymap(exAppend))(ex3))
exTamanho4 =  (App tamanho (exMap2))
mysum = (Fix 
      (Lambda "f" 
       (Lambda "l"
         (
          IfZero (MyNull  (Var"l")) (Const 0) ((MyHead (Var"l")) :+ (App (Var "f") (MyTail (Var "l")))) )
         ))) 

exSum = (App mysum (exAppend))
exSum2 = (App mysum (exMap1))

-- buggy expressions (type errors)
bug1 = Const 42 :+ Lambda "x" (Var "x")

bug2 = App (Const 42) (Const 1)
