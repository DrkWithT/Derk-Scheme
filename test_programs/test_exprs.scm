; test_exprs.scm
(define x 42)
(define y 24)
(define (my-average a b)
    (+ (/ a 2)
    (/ b 2)))
(define answer (my-average x y))
(display answer)
