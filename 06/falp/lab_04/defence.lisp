(setq time (go:import 'time))
(setq rand (go:import 'math/rand))
(.Seed rand (.UnixNano (.Now time)))
(print (.Intn rand 100))

(defclass a ())
