(sc-program
 '(
   ;declaration of fact
   (push-label-adress after-decl)
   (jump-to)
   (label factorial)
   
   
   ;recursion limit test
   (push-relative-adress -2)
   (load)
   (push-label-adress after-rec-end)
   (jump-on-non-zero)
   
   ;end recursion
   (push-constant 1)
   (return)
   
   ;continue recursion
   (label after-rec-end)
   (push-relative-adress -2)
   (load)
   (push-constant 1)
   (sub)
   (push-label-adress factorial)
   (call)
   
   (push-relative-adress -2)
   (load)
   (mul)
   (return)
   
   ;after decl
   (label after-decl)
   (push-constant 0)
   (push-label-adress factorial)
   (call)
   (cleanup-after-call 1)
   
   (push-label-adress print-long)
   (call)
   (cleanup-after-call 1)
   ))
   