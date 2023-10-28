(asdf:oos 'asdf:load-op :FiveAM)
(defpackage :it.bese.FiveAM.example
  (:use :common-lisp
	:it.bese.FiveAM))
(in-package :it.bese.FiveAM.example)

(defun solve (a b c)
	(with-open-file (output "result.txt" :direction :output :if-does-not-exist :create) 
		(or
			(and (equalp a 0) (equalp b 0) (or (format output "no solutions") 0))
			(and (equalp a 0) (or (format output "x = ~f" (/ (- c) (* b))) 1))
			(and (setf d (- (* b b) (* 4 a c))) nil)
			(and (equalp d 0) (or (format output "x = ~f" (/ (- b) (* 2 a))) 1))
			(and (setf sqrtd (sqrt d)) (or (format output "x1 = ~f, x2 = ~f" (/ (+ (- b) sqrtd) (* 2 a)) (/ (- (- b) sqrtd) (* 2 a))) 2))
		)
	)
)

(fiveam:test test_solve
	(fiveam:is (= 1 (solve 1 2 1)))
	(fiveam:is (= 1 (solve 0 2 1)))
	(fiveam:is (= 2 (solve 1 5 1)))
	(fiveam:is (= 2 (solve 1 2 0)))
	(fiveam:is (= 2 (solve 1 1 1)))
)

(fiveam:run!)
