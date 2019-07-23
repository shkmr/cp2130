;;;
;;; cp2130
;;;

(define-module cp2130
  (use gauche.uvector)
  (use gauche.charconv)
  (export-all))

(select-module cp2130)

;; Loads extension
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

;; EOF


