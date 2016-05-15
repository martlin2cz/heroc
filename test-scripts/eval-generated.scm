(define pow
  (lambda (base exp)
    (cond ((= exp 0) 1)
          ((< exp 0) (/ base (pow base (+ exp 1))))
          ((> exp 0) (* base (pow base (- exp 1)))))))
        
(define error 
  (lambda (x) 
    (display x)
    (newline)))


(define == =)

(define != 
  (lambda (x y)
    (not (= x y))))

(define <<
  (lambda (x y)
    (* x (pow 2 y))))

(define >>
  (lambda (x y)
    (/ x (pow 2 y))))

(define bit-or
  (lambda (x y)
    (display "Not supported")))

(define bit-and
  (lambda (x y)
    (display "Not supported")))

(define bit-xor
  (lambda (x y)
    (display "Not supported")))

(define bit-not
  (lambda (x)
    (display "Not supported")))

(define-syntax post++
  (syntax-rules ()
    ((post++ place)
     (let ((oldval place))
      (set! place (+ place 1))
      oldval))))
     
(define-syntax post--
  (syntax-rules ()
    ((post-- place)
     (let ((oldval place))
      (set! place (- place 1))
      oldval))))

(define-syntax pre++
  (syntax-rules ()
    ((pre++ place)
     (let ((oldval place))
      (set! place (+ place 1))
      place))))

(define-syntax pre--
  (syntax-rules ()
    ((pre-- place)
     (let ((oldval place))
      (set! place (- place 1))
      place))))

(define-syntax for
  (syntax-rules ()
    ((for init cond incr body)
     (begin 
       init
       (while cond (begin 
                     body 
                     incr))))))

(define-syntax while
  (syntax-rules ()
    ((while cond body)
     (if cond
         (begin
           body
           (while cond body))))))

(define-syntax do-while
  (syntax-rules ()
    ((do-while body cond)
     (begin
       body
       (if cond
           (do-while body cond))))))

(define-syntax at-address
  (syntax-rules ()
    ((at-addess place)
     (error "Operator at-address not supported"))))

(define-syntax address-of
  (syntax-rules ()
    ((addess-of place)
     (error "Operator address-of not supported"))))

(define return
  (lambda x-list
    (error "Operator return not supported")))

(define continue
  (lambda ()
    (error "Operator continue not supported")))

(define break
  (lambda ()
    (error "Operator break not supported")))

(define at 
  (lambda (arr index)
    (list-ref arr index)))
