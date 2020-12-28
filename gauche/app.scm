;;
(use gauche.net)
(use text.html-lite)
(use text.tree)
(use makiki)
(use cp2130)


(define ek-read-adc
  (let ((buf (make-u8vector 3 0)))
    (lambda (ek ain)
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
        -1))))

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


;;
;;    Web App
;;
(define (main args)
  (start-http-server :access-log #f :error-log #t
                     :port 8021
                     :app-data (sys-ctime (sys-time))
                     :startup-callback print-url)
    0)

(define *vendor-id*  #x10c4)
(define *product-id* #x87a0)

(define *ek*  #f)
(define *evc* #f)

(define *potv*  0)
(define *temp*  0)

(define (read-adc)
  (if  *ek*
    (begin
      (set! *potv* (ek-read-adc *ek* 0))
      (set! *temp* (ek-read-adc *ek* 1)))
    (begin
      (set! *potv* #f)
      (set! *temp* #f))))

(define (read-event-counter)
  (if *ek*
    (set! *evc* (cp2130-get-envent-counter-count *ek*))
    (set! *evc* #f)))

(define (D8ON-assert-RESET_ADC)   (if *ek* (cp2130-reset-gpio07)))
(define (D8OFF-negate-RESET_ADC)  (if *ek* (cp2130-set-gpio07)))

(define (print-url server-socks)
  (let ((port (any (^[s] (sockaddr-port (socket-address s))) server-socks)))
    (print #`"Visit http://localhost:,|port|/")
    (flush)))

(define-http-handler "/"
  (with-post-parameters
   (lambda (req app)
     (let ((n 0))
       (respond/ok req
         (html:html
          (html:head (html:title "CP2130EK Demo"))
          (html:body
           ;; -------------------------------------------
           (html:h1 "CP2130EK Demo")
           (html:form
            :action "/" :method "POST"
            (html:p "N:" (html:input :type  "number"
                                     :name  "N"
                                     :value  n
                                     :min    0
                                     :max   16)
                    (html:input :type "submit"
                                :name "submit"
                                :value "-1")
                    (html:input :type "submit"
                                :name "submit"
                                :value "+1")
                    (html:input :type "submit"
                                :name "submit"
                                :value "draw")
                    ))
           ;; -------------------------------------------
           (html:h2 "Request Headers")
           (html:pre
            (map (^[p] (map (^[v] #`",(car p): \",v\"\n")
                            (cdr p)))
                 (request-headers req)))
           ;; -------------------------------------------
           (html:h2 "Request Params")
           (html:pre
            (map (^[p] (map (^[v] #`",(car p): \",v\"\n")
                            (cdr p)))
                 (request-params req)))
           )))))))
;; EOF
