;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; runs nonparametric procedure "main"
;;

(define old-main main)
	
(define main 
	(lambda (params)
	 (old-main)))
