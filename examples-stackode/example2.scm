(sc-program
 '((declare-atomic)
   (push-constant 10)
   (push-relative-address +1)
   (assign)
   
   (label loop-start)
   ;increment
   (push-constant 2)
   (sub)
   
   ; call print
   (duplicate)
   (push-label-adress print-long)
   (call)
   (push-label-adress print-nl)
   (call)
   
   ;TODO pop duplicated call argument
   
   ; jump back
   (duplicate)
   (push-label-adress loop-start)
   (jump-on-non-zero)
   ;TODO pop duplicated call argument
   
   ;afterloop
   (label after-loop)
   
   (push-constant 42)
   (push-label-adress print-long)
   (call)
   
   (push-label-adress print-nl)
   (call)
   
   ;TODO pop duplicated call argument
   
   ))