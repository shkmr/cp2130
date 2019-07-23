;;;
;;; Test cp2130
;;;

(use gauche.test)

(test-start "cp2130")
(use cp2130)
(test-module 'cp2130)

;; If you don't want `gosh' to exit with nonzero status even if
;; the test fails, pass #f to :exit-on-failure.
(test-end :exit-on-failure #t)




