;;
;;
;;     Demo program for cp2130ek (Evaluation Kit)
;;
;;     Usage: gosh -I. test_cp2130ek.scm
;;
;;
;;      CP2130EK
;;
;;      https://www.silabs.com/documents/public/user-guides/CP2130-EK.pdf
;;
;;      Si8902
;;
;;      https://www.silabs.com/documents/public/data-sheets/Si8900-1-2.pdf
;;
;;      Serial EEPROM  25AA02E48-I/SN
;;
;;      http://ww1.microchip.com/downloads/en/DeviceDoc/20002123F.pdf
;;
;;      Temp Sensor   MPC9701AT-E/OT
;;
;;      http://ww1.microchip.com/downloads/en/devicedoc/20001942g.pdf
;;
;;                                       written by skimu@me.com
;;
(use gauche.uvector)
(use cp2130)

(define buf (make-u8vector 3 0))

(define (ek-read-adc ek ain)
  (if (<= 0 ain 2)
    (let ((cnf (logior (ash ain 4)
                       #xc0
                       #x08
                       #x02
                       #x01)))
      (u8vector-set! buf 0 cnf)
      (u8vector-set! buf 1 0)
      (u8vector-set! buf 2 0)
      (cp2130-set-gpio-chip-select ek 0 2)
      (cp2130-write ek buf 1)
      (sys-nanosleep (/ 8e-6 1e-9))
      (cp2130-read ek buf 3)
      (cp2130-set-gpio-chip-select ek 0 0)
      (+ (ash (logand (u8vector-ref buf 1) #x0f)  6)
         (ash (logand (u8vector-ref buf 2) #x7e) -1)))
    -1))

(define (ek-init ek)
  ;;
  ;;  Si8902 (10bit ADC) is on CS0.
  ;;  CPHA=Trailing Edge, CPOL=Active Low
  ;;  SCK should be less than 2MHz.
  ;;
  ;;  7654 3210
  ;;  0011 1011 = 0x3b
  ;;
  (cp2130-set-spi-word ek 0 #x3b)

  ;;  Reset ADC
  (cp2130-reset-gpio07 ek)
  (sys-nanosleep (/ 0.5 1e-9))
  (cp2130-set-gpio07 ek)
  )

(define (print-bar n)
  (cond ((> n 0) (display " ") (print-bar (- n 1)))
        (else    (display "*\n"))))

(define (print-dev-info dev)
  (receive (major minor) (cp2130-get-readonly-version dev)
    (print #`"cp2130 version ,|major|.,|minor|"))
    (print #`"Manufacturer: \",(cp2130otp-get-manufacturing-string dev)\"")
    (print #`"Product: \",(cp2130otp-get-product-string dev)\"")
    (print #`"Serial: \",(cp2130otp-get-serial-string dev)\"")
    (let ((cnf (cp2130otp-get-usb-config dev)))
      (format #t "VID/PID: #x~4,'0x/#x~4,'0x~%"
              (cadr (assq 'VID cnf))
              (cadr (assq 'PID cnf)))
      (print (list-ref cnf 2))
      (print (list-ref cnf 3))
      (print (list-ref cnf 4))
      (print (list-ref cnf 5))
      (print (list-ref cnf 6))
      ))

(define (main args)

  (let ((ek (cp2130-open #x10c4 #x87a0)))

    (define (loop)
      (print-bar (quotient (ek-read-adc ek 0) 10))
      (sys-nanosleep (/ 0.05 1e-9))
      (loop))

    (ek-init ek)
    (print-dev-info ek)

    (loop)

    0))

;; EOF
