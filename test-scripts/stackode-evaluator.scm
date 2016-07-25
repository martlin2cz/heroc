;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; simple interpreter of heroc stackode
;;


(load "./lib/herocio.scm")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; stack

(define *stack-size* 1000000)

(define create-stack
  (lambda ()
    (list 'top 'bottom)))

; (define s (create-stack))

(define push
  (lambda (stack x)
    (check-stack-max-size stack)
    (set-cdr! stack (cons x (cdr stack)))
    stack))

; (push s 10)
; (push s 'x)

(define pop
  (lambda (stack)
    (let ((head (cadr stack)))
      (check-stack stack 0)
      (set-cdr! stack (cddr stack))
      head)))

; (pop s)
; (pop s)

(define stack-size
  (lambda (stack)
    (- (length stack) 2)))

; (stack-size s)


(define at-stack-absolute
  (lambda (stack index)
    (check-stack stack (stack-absolute-to-index index))
    (let* ((real-index (stack-absolute-to-index index))
           (ref (- (- (length stack) real-index) 2)))
      (list-ref stack ref))))

; (at-stack-absolute s 0)
; (at-stack-absolute s 1)


(define set-at-stack-absolute
  (lambda (stack index value)
    (check-stack stack (stack-absolute-to-index index))
    (let* ((real-index (stack-absolute-to-index index))
           (substack (cdr stack))
           (subindex (- (- (length stack) real-index) 3))
           (replaced (replace-nth substack subindex value)))
      (set-cdr! stack replaced))))

; (set-at-stack-absolute s 1 42)


(define pop-n
  (lambda (stack count)
    (if (<= count 0) 
        stack
        (begin
          (check-stack stack 0)
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


(define substract
  (lambda (stack offset n)
    (check-stack stack offset)
    (check-stack stack (+ offset n))
    (letrec ((sublist (lambda (list from to)
                     (cond ((> from 0)
                            (sublist (cdr list) (- from 1) to))
                           ((> to 0)
                            (cons (car list) (sublist (cdr list) from (- to 1))))
                           (else '())))))
      (sublist stack (+ offset 1) n))))
                             
; (pop-n s 0 0)
; (pop-n s 1 2)

(define stack-absolute-to-index
  (lambda (index)
    (- *stack-size* (+ index 1))))

(define stack-index-to-absolute
  (lambda (adress)
    (- *stack-size* (+ adress 1))))

(define stack-top-adress
  (lambda (stack)
    (let* ((stack-size (stack-size stack))
           (stack-top-index (- stack-size 1))
           (stack-top-adress (stack-index-to-absolute stack-top-index)))
      stack-top-adress)))
                           
    

;(stack-absolute-to-index 999999)
;(stack-absolute-to-index 999998)
;(stack-absolute-to-index 1000000)
;(stack-absolute-to-index 0)


;(stack-index-to-absolute 0)
;(stack-index-to-absolute 1)
;(stack-absolute-to-index (stack-index-to-absolute 0))
;(stack-absolute-to-index (stack-index-to-absolute 999999))




(define check-stack
  (lambda (stack index)
    (if (or (< index 0) (>= index (- (length stack) 2)))
        (verbose-error "Out of stack" 'index: index 'stack: stack)
        'OK)))

; (check-stack s 2)

(define check-stack-max-size
  (lambda (stack)
    (if (>= (stack-size stack) *stack-size*)
        (verbose-error "Stack overflow, max stack size elapsed" 'max-size: *max-stack-size* 'stack: stack)
        'OK)))

; (check-stack-max-size s)

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
    (verbose-error "program must be list of instructions" 'program program))))

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
          (verbose-error "No such instruction at adress" 'adress adress 'program program 'program-size (length program))))))
                  
                  
  

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
         (verbose-error "Label not found" 'label label 'program program)))))

; (sc-address-of-label 'start-loop p) 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; context

(define create-context
  (lambda (program)
    (list 
     program 
     (create-stack) 
     0 
     *stack-size*)))

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
           (current-frame-addr (frame-pointer context)))
      
      (push stack current-frame-addr)
      (let* ((stack-top-adress (stack-top-adress stack))
             (new-frame-addr stack-top-adress))        
        (change-frame-pointer context new-frame-addr)))))

(define pop-frame
  (lambda (context)
    (let* ((stack (stack context))
           (current-stack-top (stack-top-adress stack))
           (current-frame-addr (frame-pointer context))
           (previous-frame-addr (at-stack-absolute stack current-frame-addr))
           (remove-count (+ (- current-frame-addr current-stack-top) 1)))

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
    (let* ((context (create-context program)))
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
    ;(display "evaluating ") (display instruction) 
    ;(display "\t with stack ") (display (stack context)) 
    ;(display "\t and frame pointer ") (display (frame-pointer context)) 
    ;(newline)
   ; (print-log "evaluating" instruction ;)
   ;            (stack context) (frame-pointer context))
    ;   (read)
    
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
           (value (operation operand2 operand1))) 
      (push stack value)
      'next-instruction)))

(define ignore-1-and-continue
  (lambda (stack whatever)
    'next-instruction))

(define just-jump
  (lambda (stack addr)
      addr))

(define jump-if-1
  (lambda (stack addr predicate)
    (let ((cond (pop stack)))
      (if (predicate cond)
          addr
          'next-instruction))))


(define jump-if-2
  (lambda (stack addr predicate)
    (let ((cond1 (pop stack))
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
    (display "I'm done.") ;XXX debug
    (newline)    
    'end-of-program))

(define sci-label 
  (lambda (context label)
    (ignore-1-and-continue (stack context) label)))

(define sci-comment
  (lambda (context . comments)
    (ignore-1-and-continue (stack context) comments)))

(define sci-push-label-adress
  (lambda (context label)
    (push-value-and-continue 
     (stack context) 
     (sc-address-of-label label (program context)))))
    
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
           (addr (cond ((> ofset 0) (- frame-addr ofset))
                       ((< ofset 0) (- frame-addr (+ ofset 1))) ;skip RIP
                       (else (verbose-error "Unsupported relative adress 0")))))
      
      (push-value-and-continue stack addr))))

(define sci-push-absolute-adress
  (lambda (context adress)
    (let* ((stack (stack context))
           (absolute-adress (stack-index-to-absolute adress)))
      (push-value-and-continue stack absolute-adress))))


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
      (push stack value)
      (set-at-absolute-and-continue stack addr value))))


(define sci-pop
  (lambda (context)
    (let ((stack (stack context)))
      (pop stack))
    'next-instruction))

(define sci-duplicate
  (lambda (context)
    (let* ((stack (stack context))
           (value (pop stack)))
      (push stack value)
      (push stack value))
    'next-instruction))

(define sci-jump-on-non-zero
  (lambda (context label)
    (let* ((stack (stack context))
           (program (program context))
           (addr (sc-address-of-label label program)))
      (jump-if-1 stack addr
                 (lambda (c) (not (= c 0)))))))

(define sci-jump-on-zero
  (lambda (context label)
    (let* ((stack (stack context))
           (program (program context))
           (addr (sc-address-of-label label program)))
      (jump-if-1 stack addr
                 (lambda (c) (= c 0))))))

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

(define sci-invoke-external
  (lambda (context procedure arity)
    (let* ((stack (stack context))
           (arguments (substract stack 2 arity)))
      ;(push stack 
      (apply procedure arguments)
      ;)
      'next-instruction)))

(define sci-unary-operation
  (lambda (context operator)
    (join-1-and-continue 
     (stack context)
     (case operator
       ((op-!) not-of-ints)
       ((op-~) bit-not)
       (else (verbose-error "unknown unary operation" 'operator operator))
       ))))


(define sci-binary-operation
  (lambda (context operator)
    (join-2-and-continue   
     (stack context)
     (case operator
       ((op-+) +)
       ((op--) -)
       ((op-*) *)
       ((op-/) /-of-ints)
       ((op-%) modulo)
       ((op-<) <-of-ints)
       ((op->) >-of-ints)
       ((op-<=) <=-of-ints)
       ((op->=) >=-of-ints)
       ((op-==) =-of-ints)
       ((op-!=) !=-of-ints)
       ((op-<<) bit-shift-left)
       ((op->>) bit-shift-right)
       ((op-bit-or) bit-or)
       ((op-bit-and) bit-and)
       ((op-^) bit-xor)
       ((op-index) +)
       (else (verbose-error "unknown binary operation" 'operator operator))
       ))))

(define /-of-ints
  (lambda (x y)
    (floor (/ x y))))

(define pow
  (lambda (x e)
    (cond ((= e 0) 1)
          ((< e 0)  (/ x (pow x (+ e 1))))
          ((> e 0)  (* x (pow x (- e 1)))))))

;; (pow 4 2)
;; (pow 4 -2)

(define bool-to-int
  (lambda (bool)
    (if bool 1 0)))

(define int-to-bool
  (lambda (int)
    (= int 0)))

(define xor-func
  (lambda (x y)
    (eq? x y)))

(define or-func
  (lambda (x y)
    (or x y)))

(define and-func
  (lambda (x y)
    (and x y)))


;; (xor-func #f #t)
;; (or-func #f #t)
;; (and-func #f #t)

(define int-to-rev-bits
  (lambda (int)
    (if (<= int 0) 
        '()
        (cons (int-to-bool (modulo int 2))
              (int-to-rev-bits (floor (/ int 2)))))))

(define rev-bits-to-int
  (lambda (bits)
    (if (null? bits) 
        0
        (+ (bool-to-int (car bits))
           (* 2 (rev-bits-to-int (cdr bits)))))))

;; (rev-bits-to-int (list 0 0 0 1))
;; (int-to-rev-bits 16)


(define simple-build-list
  (lambda (len item)
    (if (<= len 0)
        '()
        (cons item (simple-build-list (- len 1) item)))))

(define extend-to
  (lambda (l len item)
    (append l (simple-build-list (- len (length l)) item))))

;; (simple-build-list 10 'a)
;; (extend-to '(1 2 3 4) 10 'x)


(define run-bitwise-op ;TODO FIXME: not working properly
   (lambda (op . xs)
    (let* ((bits-xs (map int-to-rev-bits xs))
           (max-len (apply max (map length bits-xs)))
           (bits-xs-ext (map (lambda (x) (extend-to x max-len #f)) bits-xs)))
           
      ;(display bits-x-ext)(newline)(display bits-y-ext)(newline)
      ;(display (map or-func bits-x-ext bits-y-ext))(newline)
    (rev-bits-to-int (apply map op bits-xs-ext)))))

(define bit-or
  (lambda (x y)
   (run-bitwise-op or-func x y)))

(define bit-and
  (lambda (x y)
    (run-bitwise-op and-func x y)))

(define bit-not
  (lambda (x)
    (run-bitwise-op not x))) ;TODO make n-ary...

(define bit-xor
  (lambda (x y)
    (run-bitwise-op xor-func x y)))

(define bit-shift-left
  (lambda (x y)
    (* x (pow 2 y))))

(define bit-shift-right
  (lambda (x y)
    (/ x (pow 2 y))))


(define not-of-ints
  (lambda (x)
    (if (= 0 x) 1 0)))

(define <-of-ints
  (lambda (x y)
    (bool-to-int  (< x y))))
    
(define >-of-ints
  (lambda (x y)
    (bool-to-int  (> x y))))

(define <=-of-ints
  (lambda (x y)
    (bool-to-int  (<= x y))))

(define >=-of-ints
  (lambda (x y)
    (bool-to-int  (>= x y))))

(define =-of-ints
  (lambda (x y)
    (bool-to-int  (= x y))))

(define !=-of-ints
  (lambda (x y)
    (bool-to-int  (not (= x y)))))

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
   (list 'invoke-external sci-invoke-external)
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
   (list 'duplicate sci-duplicate)
        
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
          (verbose-error "instruction not found" 'instruction name)
          
          (if (eq? name (car (car instructions)))
              (cadar instructions)
              (sc-find-instr-rec name (cdr instructions)))))))
  
; (sc-find-instr-proc 'add)
; (sc-find-instr-proc 'make-world-peace)
           

(define verbose-error
  (lambda (cause . info)
    (display "Error: ")
    (apply print-log cause info)
    (throw-error))) ;undefined, will make fail

;(verbose-error "Oops!" 'dont-know 42)

(define print-log 
  (lambda data
    (if (null? data)
        (newline)
        (begin (display "\t")
               (display (car data))
               (newline)
               (apply print-log (cdr data))))))
  
;(print-log "Happened" 'at "16:21:55" 'something)
