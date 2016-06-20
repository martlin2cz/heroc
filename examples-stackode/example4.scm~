(sc-program
 '(
   ;init
   (push-label-adress run-them)
   (jump-to)
   (label fibb-1)
   
   ;test for n == 1
   (push-relative-adress -2)
   (load)
   (push-constant 1)
   (sub)
   (push-label-adress return-one)
   (jump-on-zero)
   
   ;test for n == 0
   (push-relative-adress -2)
   (load)
   (push-label-adress return-one)
   (jump-on-zero)
   
   ;invoke fibb(n-2)
   (push-relative-adress -2)
   (load)
   (push-constant 2)
   (sub)
   (push-label-adress fibb-1)
   (call)
   (cleanup-after-call 1)
   
   ;invoke fibb(n-1)
   (push-relative-adress -2)
   (load)
   (push-constant 1)
   (sub)
   (push-label-adress fibb-1)
   (call)
   (cleanup-after-call 1)
   
   ;sum them and return
   (add)
   (return)
   
   ;return 1
   (label return-one)
   (push-constant 1)
   (return)
   
   ;the iterative version
   ;TODO
          
   
   ;run testing ...
   (label run-them)
   (declare-atomic)
   (push-constant 0)
   (push-relative-adress 0)
   (assign)
   
   (label test-loop-start)
   
   ;call fibb
   (push-relative-adress 0)
   (load)
   (push-label-adress fibb-1)
   (call)
   (cleanup-after-call 1)
   ;call print_long
   (push-label-adress print-long)
   (call)
   (cleanup-after-call 1)
   (pop)
   ;call print_nl
   (push-label-adress print-nl)
   (call)
   (cleanup-after-call 0)
   (pop)
   
   ;increment
   (push-relative-adress 0)
   (load)
   (push-constant 1)
   (add)
   (push-relative-adress 0)
   (assign)
   
   ;test loop
   (push-relative-adress 0)
   (load)
   (push-constant 10)
   (sub)
   (push-label-adress test-loop-start)
   (jump-on-non-zero)
   
   ))