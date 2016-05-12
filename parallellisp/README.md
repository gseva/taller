
## Parallel Lisp

The purpose of this project is to create an interpreter for a subset of *Lisp*
instructions.

Usage:

```bash
make
./tp <<< """(defun cddr (ENV) (cdr (cdr ENV)))
(defun caar (ENV) (car (car ENV)))
(sync)
(print (caar (list (list 1 2 3) 4 5)))
(sync)
(defun condicional (ENV) (if ENV (caar (list ENV 2 3 4 5)) 1234))
(sync)
(print (condicional (list 1 2 3 4)))
(sync)
(print (+ (* 1 2) (* 3 4) 5))
(sync)
(print (* (+ 1 2 3 (- 100 33)) (+ 1 2 3)))
(sync)
(setq a 123)
(sync)
(print a)
"""
```

Every instruction will run in a separate thread, so before printing/assigning
sync function is needed.
