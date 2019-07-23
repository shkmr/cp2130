(use cp2130)


(define (print-dev-info dev)
  (receive (major minor) (cp2130-get-readonly-version dev)
    (print #`"cp2130 version ,|major|.,|minor|"))
    (print #`"Manufacturer: \",(cp2130otp-get-manufacturing-string dev)\"")
    (print #`"Product: \",(cp2130otp-get-product-string dev)\"")
    (print #`"Serial: \",(cp2130otp-get-serial-string dev)\""))


(let ((dev (cp2130-open #x10c4 #x87a0)))
  (print-dev-info dev)
  (sys-sleep 1)
  (cp2130-reset-gpio07 dev)
  (sys-sleep 1)
  (cp2130-set-gpio07 dev)
  (sys-sleep 1)
  (cp2130-reset-gpio07 dev)
  (sys-sleep 1)
  (cp2130-set-gpio07 dev)
  (sys-sleep 1)
  (cp2130-reset-gpio07 dev)
  (sys-sleep 1)
  (cp2130-set-gpio07 dev)
  (cp2130-close dev))

