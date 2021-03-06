;; an implementation of heroc library to be used with compiled heroc code
;; Martin Jasek, V/2016
;;

(define print_long
  (lambda (x)
    (display x)))

(define print_char 
  (lambda (x)
    (if (or (<= x 0) (>= x 256))
        (begin
          (display "warning: not a char")
          (newline))
          (display (integer->char x)))
    x))

(define print_nl 
  (lambda ()
    (newline)
    0))
