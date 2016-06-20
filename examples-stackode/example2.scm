(sc-program
 '(
   ;declare variable
   (declare-atomic)
   (push-constant 10)
   (push-relative-adress 0)
   (assign)
   
   (label loop-start)
   ;increment
   
   (push-relative-adress 0)
   (load)
   (push-constant 2)
   (sub)
   (push-relative-adress 0)
   (assign)
   
   ; call print-long
   (push-relative-adress 0)
   (load)
   (push-label-adress print-long)
   (call)
   (cleanup-after-call 1)
   (pop)
   
   ;;call print-char
   (push-constant 32)
   (push-label-adress print-char)
   (call)
   (cleanup-after-call 1)
   (pop)
   
   ; jump back
   (push-relative-adress 0)
   (load)
   (push-label-adress loop-start)
   (jump-on-non-zero)
   
   ;afterloop
   (label after-loop)
   
   (push-label-adress print-nl)
   (call)
   (cleanup-after-call 0)
   (pop)
   ))