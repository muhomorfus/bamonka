(defun set_union1 (set1 set2 result)
	(cond
		((null set2) result)
		((member (car set2) set1) (set_union1 set1 (cdr set2) result))
		(t (set_union1 set1 (cdr set2) (cons (car set2) result)))
	)
)

(defun set_union (set1 set2)
	(set_union1 set1 set2 set1)
)