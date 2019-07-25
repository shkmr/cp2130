;;;
;;; cp2130
;;;

(define-module cp2130
  (use gauche.uvector)
  (use gauche.charconv)
  (export-all))

(select-module cp2130)

(dynamic-load "cp2130")

(define (cp2130otp-get-manufacturing-string dev)
  (let ((buf (make-u8vector 128 0)))
    (cp2130otp-get-manufacturing-string& dev buf)
    (ces-convert buf 'utf-16le)))

(define (cp2130otp-get-product-string dev)
  (let ((buf (make-u8vector 128 0)))
    (cp2130otp-get-product-string& dev buf)
    (ces-convert buf 'utf-16le)))

(define (cp2130otp-get-serial-string dev)
  (let ((buf (make-u8vector 128 0)))
    (cp2130otp-get-serial-string& dev buf)
    (ces-convert buf 'utf-16le)))

(define (cp2130otp-get-usb-config dev)
  (let ((buf (make-u8vector 9 0)))
    (cp2130otp-get-usb-config& dev buf)
    (list (list 'VID (+ (ash (u8vector-ref buf 0) 0)
                        (ash (u8vector-ref buf 1) 8)))
          (list 'PID (+ (ash (u8vector-ref buf 2) 0)
                        (ash (u8vector-ref buf 3) 8)))
          (list 'max-power (* (u8vector-ref buf 4) 2) 'mA)
          (list 'power-mode
                (case (u8vector-ref buf 5)
                  ((0) 'bus-powered-regulator-enabled)
                  ((1) 'self-powered-regulator-disabled)
                  ((2) 'self-powered-regulator-enabled)))
          (list 'major-release (u8vector-ref buf 6))
          (list 'minor-release (u8vector-ref buf 7))
          (list 'transfer-priority
                (case (u8vector-ref buf 8)
                  ((0) 'high-priority-read)
                  ((1) 'high-priority-write))))))

(define (cp2130otp-set-manufacturing-string dev utf8string)
  (cp2130otp-set-manufacturing-string& dev (ces-convert-to <u8vector> utf8string 'utf-8 'utf-16le)))

(define (cp2130otp-set-product-string dev utf8string)
  (cp2130otp-set-product-string& dev (ces-convert-to <u8vector> utf8string 'utf-8 'utf-16le)))

(define (cp2130otp-set-serial-string dev utf8string)
  (cp2130otp-set-serial-string& dev (ces-convert-to <u8vector> utf8string 'utf-8 'utf-16le)))

;; EOF


