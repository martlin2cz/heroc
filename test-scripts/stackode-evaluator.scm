;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; simple interpreter of heroc stackode
;;


(load "../lib/herocio.scm")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; stack

(define create-stack
  (lambda ()
    (list 'top 'bottom)))

; (define s (create-stack))

(define push
  (lambda (stack x)
    (set-cdr! stack (cons x (cdr stack)))
    stack))

; (push s 10)
; (push s 'x)

(define pop
  (lambda (stack)
    (let ((head (cadr stack)))
      (set-cdr! stack (cddr stack))
      head)))

; (pop s)
; (pop s)

(define stack-top 
  (lambda (stack)
    (- (length stack) 2)))

; (stack-top s)

(define at-stack-absolute
  (lambda (stack index)
    (list-ref stack (- (length stack) index 2))))

; (at-stack-absolute s 0)
; (at-stack-absolute s 1)


(define set-at-stack-absolute
  (lambda (stack index value)
    (let* ((substack (cdr stack))
          (subindex (- (length stack) index 3))
          (replaced (replace-nth substack subindex value)))
    (set-cdr! stack replaced))))

; (set-at-stack-absolute s 1 42)


(define pop-n
  (lambda (stack count)
    (if (<= count 0) 
        stack
        (begin
          (pop stack)
          (pop-n stack (- count 1))))))

; (pop-n s 2)

(define replace-nth
  (lambda (list index value)
    (if (null? list)
        '()
        (let ((this (if (= index 0) value (car list)))
              (rest (cdr list)))
          (cons this (replace-nth rest (- index 1) value))))))
        


;(replace-nth '(1 2 3 4 5 6) 2 'x)
;(replace-nth '(1 2 3 4 5 6) 0 'x)
;(replace-nth '(1 2 3 4 5 6) 99 'x)


;deprecated, everything recomputed to absolutes...
;(define at-stack-relative
;  
;  (lambda (stack index)
;    'XXXX
;    (list-ref stack (+ index 1))))

; (at-stack-relative s 0)
; (at-stack-relative s 1)

(define substract
  (lambda (stack offset n)
    (letrec ((sublist (lambda (list from to)
                     (cond ((> from 0)
                            (sublist (cdr list) (- from 1) to))
                           ((> to 0)
                            (cons (car list) (sublist (cdr list) from (- to 1))))
                           (else '())))))
      (sublist stack (+ offset 1) n))))
                             
; (pop-n s 0 0)
; (pop-n s 1 2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; instruction(s) and program

(define sc-instr?
  (lambda (something)
    (and (list? something) (> (length something) 0))))

; (sc-instr? '(so-something))
; (sc-instr? '())

(define sc-is-label-of?
  (lambda (instruction label)
    (let* ((instr-name (sc-instr-name instruction))
           (instr-args (sc-instr-args instruction)))
      (and (eq? instr-name 'label) (eq? (car instr-args) label)))))

; (sc-is-label-of? '(label foo) 'foo)
; (sc-is-label-of? '(label bar) 'aux)
; (sc-is-label-of? '(push-some-stuff 42) 'foo)

(define sc-instr-name
  (lambda (instruction)
    (car instruction)))

(define sc-instr-args
  (lambda (instruction)
    (cdr instruction)))

(define sc-program 
  (lambda (program)
    (if (list? program)
        program
    (('error 'program-must-be-list-of-instructions program)))))

; (sc-program '((push-constant 1) (push-constant 2) (add)))

(define sc-loop-over-program
  (lambda (program procedure acc)
    (if (null? program)
        acc
        (let* ((instruction (car program))
               (new-acc (procedure instruction acc)))
        (sc-loop-over-program (cdr program) procedure new-acc)))))

; (sc-loop-over-program p (lambda (i a) (display i) (newline)) #f)
; (sc-loop-over-program p (lambda (i a) (+ a 1)) 0)

(define sc-instruction-at
  (lambda (adress program)
    (let* ((finder-proc 
            (lambda (instr acc) 
              (if (number? acc)
                  (if (= acc 0)
                      instr
                      (- acc 1))
                  acc)))
           (found (sc-loop-over-program program finder-proc adress)))
      (if (sc-instr? found)
          found
          'no-such-instruction))))
                  
                  
  

(define sc-address-of-label
  (lambda (label program)
    (let* ((finder-proc 
            (lambda (instr acc) 
              (let ((label (car acc))
                    (addr (cdr acc)))                
                (cond ((not label) acc)
                      ((sc-is-label-of? instr label) (cons #f addr))
                      (else (cons label (+ addr 1)))))))

           (init-acc (cons label 0))
           (found (sc-loop-over-program program finder-proc init-acc))
           (found-label (car found))
           (found-adress (cdr found)))
      (if (not found-label)
          found-adress
          'label-not-found))))

; (sc-address-of-label 'start-loop p) 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; context

(define create-context
  (lambda (program)
    (list program (create-stack) 0 0 0)))

(define program
  (lambda (context)
    (car context)))

(define stack
  (lambda (context)
    (cadr context)))

(define instruction-addr
  (lambda (context)
    (caddr context)))

(define goto-addr
  (lambda (context addr)
    (set-car! (cddr context) addr)))

(define frame-pointer
  (lambda (context)
    (cadddr context)))

(define change-frame-pointer
  (lambda (context pointer)
    (set-car! (cdddr context) pointer)))

(define push-frame 
  (lambda (context)
    (let* ((stack (stack context))
           (current-frame-addr (frame-pointer context))
           (new-frame-addr (stack-top stack)))
      (push stack current-frame-addr)
      (change-frame-pointer context new-frame-addr))))

(define pop-frame
  (lambda (context)
    (let* ((stack (stack context))
           (current-stack-top (stack-top stack))
           (current-frame-addr (frame-pointer context))
           (previous-frame-addr (at-stack-absolute stack current-frame-addr))
           (remove-count (- current-stack-top current-frame-addr)))
      (pop-n stack remove-count)
      (change-frame-pointer context previous-frame-addr))))


; (define c (create-context p))
; (goto-addr c 9)
; (program c)
; (stack c)
; (instruction-addr c)
; (frame-pointer c)
; (change-frame-pointer c 11)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; program runner

(define sc-run
  (lambda (program)
    (let* ((wrapped (sc-wrap-program program))
          (context (create-context wrapped)))
      (sc-run-context context))))

(define sc-run-context
  (lambda (context)
    (let ((instr-addr (instruction-addr context)))
    (if (eq? instr-addr 'end-of-program)
        (stack context)
        (let ((next-instr-addr (sc-run-next-instruction context)))
          (goto-addr context next-instr-addr)
          (sc-run-context context))))))


(define sc-run-next-instruction
  (lambda (context)
    (let* ((instr-addr (instruction-addr context))
           (instruction (sc-instruction-at instr-addr (program context))))
      (sc-run-instruction context instruction))))

(define sc-run-instruction
  (lambda (context instruction)    
    (if (eq? instruction 'instruction-not-found)
        'instruction-not-found
        (let ((next-instr-spec (sc-do-run-instr context instruction)))
          (if (eq? next-instr-spec 'next-instruction)
              (+ (instruction-addr context) 1)
              next-instr-spec)))))

(define sc-do-run-instr
  (lambda (context instruction)
  ;  (display "evaluating ") (display instruction) 
  ;  (display " with stack ") (display (stack context)) 
  ;  (display " and frame pointer ") (display (frame-pointer context)) 
  ;  (newline)
  
    ;DEBUG: 
    ;(read)
    
    (let* ((instr-name (sc-instr-name instruction))
           (instr-proc (sc-find-instr-proc instr-name))
           (instr-args (sc-instr-args instruction)))
           
      (apply instr-proc context instr-args))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; stack encapsulation

(define push-value-and-continue
  (lambda (stack value)
    (push stack value)
    'next-instruction))

(define join-1-and-continue
  (lambda (stack operation)
    (let* ((operand1 (pop stack))
          (value (operation operand1)))
      (push stack value)
      'next-instruction)))

(define join-2-and-continue
  (lambda (stack operation)
    (let* ((operand1 (pop stack))
           (operand2 (pop stack))
           (value (operation operand2 operand1))) ;TODO is order of operands correct?
      (push stack value)
      'next-instruction)))

(define ignore-1-and-continue
  (lambda (stack whatever)
    'next-instruction))

(define just-jump
  (lambda (stack addr)
      addr))

(define jump-if-1
  (lambda (stack predicate)
    (let* ((addr (pop stack))
           (cond (pop stack)))
           (if (predicate cond)
               addr
               'next-instruction))))


(define jump-if-2
  (lambda (stack predicate)
    (let* ((addr (pop stack))
           (cond1 (pop stack))
           (cond2 (pop stack)))
           (if (predicate cond1 cond2)
               addr
               'next-instruction))))

(define set-at-absolute-and-continue
  (lambda (stack addr value)
    (set-at-stack-absolute stack addr value)
    'next-instruction))

(define push-undefineds-and-continue 
  (lambda (stack count)
    (if (= count 0)
    'next-instruction
    (begin
      (push stack 'undefined)
      (push-undefineds-and-continue stack (- count 1))))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; instructions implementation

(define sci-push-constant
  (lambda (context constant)
    (push-value-and-continue (stack context) constant)))

(define sci-end
  (lambda (context)
    ;(display "I'm done.") ;XXX debug
    ;(newline)    
    'end-of-program))

(define sci-label 
  (lambda (context label)
    (ignore-1-and-continue (stack context) label)))

(define sci-comment
  (lambda (context comment)
    (ignore-1-and-continue (stack context) comment)))

(define sci-push-label-adress
  (lambda (context label)
    (push-value-and-continue 
     (stack context) 
     (sc-address-of-label label (program context)))))


(define sci-duplicate
  (lambda (context)
    (let* ((stack (stack context))
          (top (pop stack)))
      (push stack top)
      (push stack top))
    'next-instruction))
    
(define sci-declare-atomic
  (lambda (context)
    (let* ((stack (stack context)))
      (push-undefineds-and-continue stack 1))))
      

(define sci-declare-array
  (lambda (context size)
    (let* ((stack (stack context)))
      (push-undefineds-and-continue stack size))))
    
(define sci-push-relative-adress
  (lambda (context ofset)
    (let* ((stack (stack context))
           (frame-addr (frame-pointer context))
           (addr (+ frame-addr ofset)))
      (push-value-and-continue stack addr))))

(define sci-push-absolute-adress
  (lambda (context addres)
    (let* ((stack (stack context)))
      (push-value-and-continue stack addres))))

(define sci-load
  (lambda (context)
    (let* ((stack (stack context))
           (addr (pop stack))
           (value (at-stack-absolute stack addr)))
      (push-value-and-continue stack value))))
           
(define sci-assign
  (lambda (context) 
    (let* ((stack (stack context))
           (addr (pop stack))
           (value (pop stack)))
      (set-at-absolute-and-continue stack addr value))))

(define sci-pop
  (lambda (context)
    (pop (stack context))
    'next-instruction))

(define sci-jump-on-non-zero
  (lambda (context)
    (jump-if-1 (stack context) 
               (lambda (c) (not (= c 0))))))

(define sci-jump-on-zero
  (lambda (context)
    (jump-if-1 (stack context) 
               (lambda (c) (= c 0)))))

(define sci-jump-to
  (lambda (context label)
    (let* ((stack (stack context))
           (program (program context))
           (addr (sc-address-of-label label program)))
      (just-jump stack addr))))

(define sci-call
  (lambda (context)
    (let* ((stack (stack context))
           (addr (pop stack)))   
      
      (push stack (instruction-addr context))
      (push-frame context)
      
      addr)))
  
(define sci-return
  (lambda (context)
    (let* ((stack (stack context))
          (retval (pop stack)))
      (pop-frame context)
      
      (let* ((call-addr (pop stack))             
             (next-addr (+ call-addr 1)))
        (push stack retval)
        next-addr))))

(define sci-cleanup-after-call
  (lambda (context args-count)
    (let* ((stack (stack context))
          (retval (pop stack)))
      (pop-n stack args-count)
      (push stack retval)
      'next-instruction)))

(define sci-invoke-primitive
  (lambda (context procedure arity)
    (let* ((stack (stack context))
           (arguments (substract stack 2 arity)))
      (push stack (apply procedure arguments))
      'next-instruction)))


(define sci-unary-operation
  (lambda (context operator)
    (join-1-and-continue 
     (stack context)
     (case operator
       ((!) not)
       ((~) bit-not)
       ;TODO
       ))))


(define sci-binary-operation
  (lambda (context operator)
    (join-1-and-continue   
     (stack context)
     (case operator
       ((+) +)
       ((-) -)
       ((*) *)
       ((/) /)
       ((%) mod)
       ;TODO
       ))))

;(define sci-add
;  (lambda (context)
;    (join-2-and-continue (stack context) +)))
;
;(define sci-sub
;  (lambda (context)
;    (join-2-and-continue (stack context) -)))
;
;(define sci-mul
;  (lambda (context)
;    (join-2-and-continue (stack context) *)))
;
;(define sci-div
;  (lambda (context)
;    (join-2-and-continue (stack context) /)))
;
;(define sci-mod
;  (lambda (context)
;    (join-2-and-continue (stack context) mod)))



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; instruction set
(define *sc-instructions*
  (list 
   ;control instructions 
   (list 'comment sci-comment)
   (list 'end sci-end)
   (list 'label sci-label)
   
   ;program flow control instructions
   (list 'call sci-call)
   (list 'return sci-return)
   (list 'cleanup-after-call sci-cleanup-after-call)
   (list 'invoke-primitive sci-invoke-primitive)
   (list 'jump-on-non-zero sci-jump-on-non-zero)
   (list 'jump-on-zero sci-jump-on-zero)
   (list 'jump-to sci-jump-to)
   
   ;data flow control instructions
   (list 'load sci-load)
   (list 'assign sci-assign)
   (list 'declare-atomic sci-declare-atomic)
   (list 'declare-array sci-declare-array)
   (list 'push-constant sci-push-constant)
   (list 'push-label-adress  sci-push-label-adress)
   (list 'push-relative-adress  sci-push-relative-adress)
   (list 'push-absolute-adress  sci-push-absolute-adress)
   (list 'pop sci-pop)
        
   ;arithmetic instructions
   (list 'unary-operation sci-unary-operation)
   (list 'binary-operation sci-binary-operation)
   
   ))



(define sc-find-instr-proc
  (lambda (instruction)
    (let sc-find-instr-rec
      ((name instruction)
       (instructions *sc-instructions*))
      (if (null? instructions)
          (('error 'instruction-not-found name))
          
          (if (eq? name (car (car instructions)))
              (cadar instructions)
              (sc-find-instr-rec name (cdr instructions)))))))
  
; (sc-find-instr-proc 'add)
; (sc-find-instr-proc 'make-world-peace)
           
(define sc-wrap-program
  (lambda (program)
    (append (sc-program 
             `((push-label-adress start-of-program)
               (jump-to)
               
               (label print-long)
               (invoke-primitive ,print_long 1)
               (return)
               
               (label print-char)
               (invoke-primitive ,print_char 1)
               (return)
               
               (label print-nl)
               (invoke-primitive ,print_nl 0)
               (return)
               
               (label start-of-program)))
            program
            (sc-program 
             '((end))))))

; (sc-wrap-program (sc-program '((push-constant 1) (push-constant 2) (add))))
