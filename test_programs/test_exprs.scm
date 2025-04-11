(define x 42)
(define y 24)
(define avg-pair (
    (lambda (a b)
        (/ (+ a b) 2))))
(define answer (avg-pair x y))
(display answer)
