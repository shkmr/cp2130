;;;
;;; Test cp2130
;;;

(use gauche.test)

(test-start "cp2130")
(use cp2130)
(test-module 'cp2130)

;; The following is a dummy test code.
;; Replace it for your tests.
(test* "test-cp2130" "cp2130 is working"
       (test-cp2130))

(let ((dev (cp2130-open #x10c4 #x87a0)))
  (cp2130-reset-gpio07 dev)
  (sys-sleep 2)
  (cp2130-set-gpio07 dev)
  (cp2130-close dev))

;; If you don't want `gosh' to exit with nonzero status even if
;; the test fails, pass #f to :exit-on-failure.
(test-end :exit-on-failure #t)




